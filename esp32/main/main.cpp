#include <esp_log.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// clang-format off
#include <button.h>
// clang-format on

#include <algorithm>
#include <crypto/data_sources/coin_gecko.hpp>
#include <crypto/data_sources/sntp.hpp>
#include <database/database.hpp>
#include <database/definitions/currencies.hpp>
#include <gui/hal/driver.hpp>
#include <gui/views/startup/loading.hpp>
#include <gui/views/startup/provisioning.hpp>
#include <gui/views/ticker/legacy.hpp>

#include "configuration.hpp"
#include "wifi/manager.hpp"
#define LOG_TAG "main"

using namespace Crypto::DataSources;

LV_IMG_DECLARE(btc_icon_60);
LV_IMG_DECLARE(eth_icon_60);

QueueHandle_t button_events = NULL;

struct {
  Currencies::Definition definition;
  const char* id;
  const char* ticker;
  const lv_img_dsc_t image;
  Database::Database<float, 288> prices;
  Database::Database<float, 1> delta24h;
  size_t latestUpdate;
} cryptoDefinitions[currencyCount()]{{Currencies::getDefinition(Currencies::Crypto::BTC), "bitcoin", "BTC", btc_icon_60, {}, {}, 0},
                                     {Currencies::getDefinition(Currencies::Crypto::ETH), "ethereum", "ETH", eth_icon_60, {}, {}, 0}};

const struct {
  Currencies::Definition definition;
  const char* id;
  const char* ticker;
} fiatDefinition{Currencies::getDefinition(Currencies::Fiat::AUD), "aud", "AUD"};

void task_currencyUpdate(void* pvParameters) {
  (void)pvParameters;
  while(1) {
    for(auto i = 0; i < currencyCount(); i++) {
      size_t startTime = Crypto::SNTP()->unixTime();
      CoinGecko::SimplePrice currentData = CoinGecko().simplePrice(cryptoDefinitions[i].id, fiatDefinition.id, false, false, true);
      if(currentData.status == 200) {
        cryptoDefinitions[i].prices.add(currentData.price);
        cryptoDefinitions[i].delta24h.add(currentData.change24h);
        cryptoDefinitions[i].latestUpdate = startTime;
      }
      else {
        ESP_LOGE(LOG_TAG, "Response code %d", currentData.status);
      }
      size_t endTime = Crypto::SNTP()->unixTime();
      uint32_t secondsDelta = endTime - startTime;
      uint32_t secondsWait = (currencyUpdatePeriodSeconds / currencyCount()) - secondsDelta;
      ESP_LOGI(LOG_TAG, "Seconds to next request %d", secondsWait);
      for(int j = 0; j < secondsWait; j++) {
        vTaskDelay(pdMS_TO_TICKS(1000));
      }
    }
  }
}

void fetchHistoricalData() {
  GUI::LoadingScreen()->status("Fetching historical prices");
  for(size_t i = 0; i < currencyCount(); i++) {
    uint32_t currentTimestamp = Crypto::SNTP()->unixTime();
    auto historical =
    CoinGecko().marketChartRange24h(cryptoDefinitions[i].id, fiatDefinition.id, currentTimestamp - (60 * 60 * 24), currentTimestamp);

    if(historical.first != 200) {
      GUI::LoadingScreen()->status("Failed fetching data", GUI::Widgets::Severity::BAD);
      while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
      }
    }

    for(auto price: historical.second) {
      if(price != 0.0f) {
        cryptoDefinitions[i].prices.add(price);
      }
    }

    CoinGecko::SimplePrice currentData = CoinGecko().simplePrice(cryptoDefinitions[i].id, fiatDefinition.id, false, false, true);
    if(currentData.status == 200) {
      cryptoDefinitions[i].prices.add(currentData.price);
      cryptoDefinitions[i].delta24h.add(currentData.change24h);
      cryptoDefinitions[i].latestUpdate = Crypto::SNTP()->unixTime();
    }
    else {
      ESP_LOGE(LOG_TAG, "Response code %d", currentData.status);
      ESP_LOGE(LOG_TAG, "Response code %d", currentData.status);
    }
  }
}

void startCurrencyUpdate() {
  xTaskCreatePinnedToCore(task_currencyUpdate, "Data Update Task", 4096, NULL, 3, NULL, 0);
}

void initialise() {
  GUI::HAL::LVGL()->init();
  GUI::LoadingScreen()->status("Connecting to WIFI..");
  GUI::LoadingScreen()->show();

  if(WIFI::controller()->status() == WIFI::ConnectionState::NOT_PROVISIONED) {
    GUI::LoadingScreen()->status("Device needs provisioning");
    WIFI::controller()->startProvisioning(Provisioning::SSID, NULL, Provisioning::popCode);

    char payload[150] = {0};
    WIFI::controller()->getQRCodeData(payload, sizeof(payload), Provisioning::SSID, Provisioning::popCode);
    vTaskDelay(pdMS_TO_TICKS(3000));
    GUI::LoadingScreen()->hide();

    GUI::ProvisioningScreen()->setQR(payload);
    GUI::ProvisioningScreen()->show();

    while(WIFI::controller()->status() != WIFI::ConnectionState::PROVISIONING_COMPLETE) {
      switch(WIFI::controller()->status()) {
        case WIFI::ConnectionState::PROVISIONING_STARTED:
        case WIFI::ConnectionState::NOT_PROVISIONED: break;
        case WIFI::ConnectionState::PROVISIONING_RECEVIED_CREDENTIALS:
          GUI::ProvisioningScreen()->hide();
          GUI::LoadingScreen()->show();
          GUI::LoadingScreen()->status("Received WiFi credentials");
          break;
        case WIFI::ConnectionState::PROVISIONING_AP_LOST:
        case WIFI::ConnectionState::LOST_CONNECTION:
          GUI::LoadingScreen()->status("Lost connection, Restarting..", GUI::Widgets::Severity::BAD);
          vTaskDelay(pdMS_TO_TICKS(2000));
          GUI::LoadingScreen()->hide();
          GUI::ProvisioningScreen()->show();
          WIFI::controller()->resetProvisioningCredentials();
          break;
        case WIFI::ConnectionState::PROVISIONING_BAD_CREDENTIALS:
          GUI::LoadingScreen()->status("Bad credentials, Restarting..", GUI::Widgets::Severity::BAD);
          vTaskDelay(pdMS_TO_TICKS(2000));
          GUI::LoadingScreen()->hide();
          GUI::ProvisioningScreen()->show();
          WIFI::controller()->resetProvisioningCredentials();
          break;
        case WIFI::ConnectionState::CONNECTING: GUI::LoadingScreen()->status("Connecting"); break;
        case WIFI::ConnectionState::PROVISIONING_SUCCESS: GUI::LoadingScreen()->status("Device provisioned"); break;
        default: break;
      }
      vTaskDelay(pdMS_TO_TICKS(100));
    }

    WIFI::controller()->stopProvisioning();
  }
  else {
    WIFI::controller()->connect();
    while(WIFI::controller()->status() != WIFI::ConnectionState::CONNECTED) {
      switch(WIFI::controller()->status()) {
        case WIFI::ConnectionState::CONNECTED:
        case WIFI::ConnectionState::CONNECTING: break;
        default:
          char buffer[32] = {0};
          snprintf(buffer, 32, "Unexpected WiFi status: %d", WIFI::controller()->status());
          GUI::LoadingScreen()->status(buffer);
          break;
      }
      vTaskDelay(pdMS_TO_TICKS(100));
    }
  }

  GUI::LoadingScreen()->status("WiFi connected");
  vTaskDelay(pdMS_TO_TICKS(500));
  GUI::LoadingScreen()->status("Synchronising time with SNTP");
  bool gotNetworkTime = Crypto::SNTP()->syncronise();
  if(!gotNetworkTime) {
    while(1) {
      GUI::LoadingScreen()->status("Failed SNTP synchronisation", GUI::Widgets::Severity::BAD);
      vTaskDelay(pdMS_TO_TICKS(2000));
      GUI::LoadingScreen()->status("Check network credentials", GUI::Widgets::Severity::BAD);
      vTaskDelay(pdMS_TO_TICKS(2000));
    }
  }
  fetchHistoricalData();
  startCurrencyUpdate();
  button_events = button_init(PIN_BIT(35) | PIN_BIT(0));
  GUI::LoadingScreen()->hide();
}

void waitForNextUpdate() {
  button_event_t ev;
  for(size_t j = 0; j < (60 * 5); j++) {
    if(xQueueReceive(button_events, &ev, 10)) {
      if((ev.pin == 35) && (ev.event == BUTTON_UP)) {
        return;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

extern "C" void app_main() {
  initialise();
  GUI::LegacyScreen()->setPlotPointCount(cryptoDefinitions[0].prices.length());

  while(1) {
    for(size_t i = 0; i < currencyCount(); i++) {
      GUI::LegacyScreen()->hide();
      GUI::LegacyScreen()->clearPlot();
      GUI::LegacyScreen()->setCurrencySymbol(fiatDefinition.definition.symbol);
      GUI::LegacyScreen()->setName(cryptoDefinitions[i].definition.name);
      GUI::LegacyScreen()->setIcon(&cryptoDefinitions[i].image);
      GUI::LegacyScreen()->setPlotRange(cryptoDefinitions[i].prices.minimum(), cryptoDefinitions[i].prices.maximum());
      GUI::LegacyScreen()->setCurrentQuote(cryptoDefinitions[i].prices.latest());
      GUI::LegacyScreen()->setDailyDelta(cryptoDefinitions[i].delta24h.latest());
      for(int j = cryptoDefinitions[i].prices.length() - 1; j >= 0; j--) {
        GUI::LegacyScreen()->plotValue(cryptoDefinitions[i].prices.at(j));
      }
      GUI::LegacyScreen()->show();
      waitForNextUpdate();
    }
  }
}