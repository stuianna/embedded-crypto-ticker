#include <esp_log.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <crypto/data_sources/coin_gecko.hpp>
#include <crypto/data_sources/sntp.hpp>
#include <gui/hal/driver.hpp>
#include <gui/views/startup/loading.hpp>
#include <gui/views/startup/provisioning.hpp>
#include <gui/views/ticker/legacy.hpp>

#include "wifi/manager.hpp"
#define LOG_TAG "main"

static const char* SSID = "PROV_ticker";
static const char* popCode = "abcd1234";

LV_IMG_DECLARE(btc_icon_60);

using namespace Crypto::DataSources;

void initialise() {
  GUI::HAL::LVGL()->init();
  GUI::LoadingScreen()->status("Connecting to WIFI..");
  GUI::LoadingScreen()->show();

  if(WIFI::controller()->status() == WIFI::ConnectionState::NOT_PROVISIONED) {
    GUI::LoadingScreen()->status("Device needs provisioning");
    WIFI::controller()->startProvisioning(SSID, NULL, popCode);

    char payload[150] = {0};
    WIFI::controller()->getQRCodeData(payload, sizeof(payload), SSID, popCode);
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

  GUI::LoadingScreen()->status("Fetching historical prices");
  uint32_t currentTimestamp = Crypto::SNTP()->unixTime();
  auto historical = CoinGecko().marketChartRange24h("bitcoin", "aud", currentTimestamp - (60 * 60 * 24), currentTimestamp);

  if(historical.first != 200) {
    GUI::LoadingScreen()->status("Failed fetching data", GUI::Widgets::Severity::BAD);
    while(1) {
      vTaskDelay(pdMS_TO_TICKS(2000));
    }
  }

  for(auto price: historical.second) {
    if(price != 0.0f) {
      GUI::LegacyScreen()->plotValue(price);
    }
  }
  auto max = *std::max_element(historical.second.begin(), historical.second.end());
  auto min = *std::min_element(historical.second.begin(), historical.second.end());
  GUI::LegacyScreen()->setPlotRange(min, max);
  GUI::LoadingScreen()->hide();
}

extern "C" void app_main() {
  initialise();

  GUI::LegacyScreen()->setCurrencySymbol('$');
  GUI::LegacyScreen()->setName("Bitcoin");
  GUI::LegacyScreen()->setIcon(&btc_icon_60);
  GUI::LegacyScreen()->show();

  while(1) {
    ESP_LOGI(LOG_TAG, "Current unix time: %d", Crypto::SNTP()->unixTime());
    CoinGecko::SimplePrice currentData = CoinGecko().simplePrice("bitcoin", "aud", false, false, true);
    if(currentData.status == 200) {
      GUI::LegacyScreen()->setCurrentQuote(currentData.price);
      GUI::LegacyScreen()->setDailyDelta(currentData.change24h);
      GUI::LegacyScreen()->plotValue(currentData.price);
    }
    else {
      ESP_LOGE(LOG_TAG, "Response code %d", currentData.status);
    }
    vTaskDelay(pdMS_TO_TICKS(60000));
  }
}