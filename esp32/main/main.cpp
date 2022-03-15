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
#include <data_sources/coin_gecko.hpp>
#include <data_sources/sntp.hpp>
#include <hal/driver.hpp>
#include <tasks/currency_update.hpp>
#include <views/startup/loading.hpp>
#include <views/startup/provisioning.hpp>
#include <views/ticker/legacy.hpp>

#include "configuration.hpp"
#include "containers/crypto.hpp"
#include "wifi/manager.hpp"
#define LOG_TAG "main"

using namespace Crypto::DataSources;

QueueHandle_t button_events = NULL;


void fetchHistoricalData() {
  GUI::LoadingScreen()->status("Fetching historical prices");
  auto fiat = Crypto::getDefinition(Crypto::baseCurrency);
  for(size_t i = 0; i < Crypto::currencyCount(); i++) {
    auto crypto = &Crypto::Table[i];
    uint32_t currentTimestamp = Crypto::SNTP()->unixTime();
    auto historical =
    CoinGecko().marketChartRange24h(crypto->params.geckoName, fiat.geckoName, currentTimestamp - (60 * 60 * 24), currentTimestamp);

    if(historical.first != 200) {
      GUI::LoadingScreen()->status("Failed fetching data", GUI::Widgets::Severity::BAD);
      while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
      }
    }

    for(auto price: historical.second) {
      if(price != 0.0f) {
        crypto->pricesDB.add(price);
      }
    }

    CoinGecko::SimplePrice currentData = CoinGecko().simplePrice(crypto->params.geckoName, fiat.geckoName, false, false, true);
    if(currentData.status == 200) {
      crypto->pricesDB.add(currentData.price);
      crypto->delta24hDB.add(currentData.change24h);
      crypto->latestUpdate = Crypto::SNTP()->unixTime();
    }
    else {
      ESP_LOGE(LOG_TAG, "Response code %d", currentData.status);
    }
  }
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
  GUI::LoadingScreen()->status("Starting currency update task.");
  Tasks::CurrencyUpdate()->start();
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
  GUI::LegacyScreen()->setPlotPointCount(Crypto::Table[0].pricesDB.length());
  auto fiat = Crypto::getDefinition(Crypto::baseCurrency);

  while(1) {
    for(size_t i = 0; i < Crypto::currencyCount(); i++) {
      auto crypto = &Crypto::Table[i];
      if(!crypto->enabled) {
        continue;
      }
      GUI::LegacyScreen()->hide();
      GUI::LegacyScreen()->clearPlot();
      GUI::LegacyScreen()->setCurrencySymbol(fiat.symbol);
      GUI::LegacyScreen()->setName(crypto->params.name);
      GUI::LegacyScreen()->setIcon(&crypto->icon);
      GUI::LegacyScreen()->setPlotRange(0, 1000);
      GUI::LegacyScreen()->setCurrentQuote(crypto->pricesDB.latest());
      GUI::LegacyScreen()->setDailyDelta(crypto->delta24hDB.latest());
      for(int j = crypto->pricesDB.length() - 1; j >= 0; j--) {
        GUI::LegacyScreen()->plotValue(crypto->pricesDB.at(j));
      }
      GUI::LegacyScreen()->show();
      waitForNextUpdate();
    }
  }
}