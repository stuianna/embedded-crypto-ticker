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
#include <hal/lvgl_driver.hpp>
#include <lib/crypto/coin_gecko.hpp>
#include <lib/gui/views/startup/loading.hpp>
#include <lib/gui/views/startup/provisioning.hpp>
#include <lib/gui/views/ticker/legacy.hpp>
#include <lib/hal/sntp.hpp>
#include <tasks/currency_update.hpp>

#include "configuration.hpp"
#include "containers/crypto.hpp"
#include "wifi/manager.hpp"
#define LOG_TAG "main"

using namespace Crypto;

QueueHandle_t button_events = NULL;


void fetchHistoricalData() {
  GUI::LoadingScreen()->status("Fetching historical prices...");
  for(auto& entry: Crypto::Table) {
    GUI::LoadingScreen()->details(entry.params.name);
    ESP_LOGI(LOG_TAG, "Request historical update of %s", entry.params.name);
    size_t rc = Tasks::CurrencyUpdate()->updateHistorical(entry.currency);
    if(!rc) {
      continue;
    }
    if(rc != 200) {
      char errorLine[24] = {0};
      snprintf(errorLine, sizeof(errorLine), "Error code: %zu", rc);
      GUI::LoadingScreen()->status("Failed fetching historical data", GUI::Widgets::Severity::BAD);
      GUI::LoadingScreen()->details(errorLine);
      while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
      }
    }
  }
}

void initialise() {
  HAL::LVGL()->init();
  GUI::LoadingScreen()->status("Connecting to WIFI..");
  GUI::LoadingScreen()->show();

  if(WIFI::controller()->status() == WIFI::ConnectionState::NOT_PROVISIONED) {
    GUI::LoadingScreen()->status("Device needs provisioning");
    WIFI::controller()->startProvisioning(Provisioning::SSID, NULL, Provisioning::popCode);

    char payload[150] = {0};
    WIFI::controller()->getQRCodeData(payload, sizeof(payload), Provisioning::SSID, Provisioning::popCode);
    vTaskDelay(pdMS_TO_TICKS(3000));
    GUI::LoadingScreen()->hide();

    ESP_LOGI(LOG_TAG, "QR payload: %s", payload);
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
  bool gotNetworkTime = HAL::SNTP()->syncronise();
  if(!gotNetworkTime) {
    GUI::LoadingScreen()->status("Failed SNTP synchronisation", GUI::Widgets::Severity::BAD);
    GUI::LoadingScreen()->details("Check network credentials", GUI::Widgets::Severity::BAD);
    while(1) {
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

bool currencyOutOfDate(const Crypto::Entry* crypto) {
  size_t lastUpdate = HAL::SNTP()->unixTime() - crypto->latestUpdate;
  if(lastUpdate > currencyUpdatePeriodSeconds * 1.5) {
    return true;
  }
  return false;
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

      ESP_LOGI(LOG_TAG, "Switch legacy GUI to currency: %s", crypto->params.name);
      ESP_LOGI(LOG_TAG, "Lastest currency update %d seconds ago", HAL::SNTP()->unixTime() - crypto->latestUpdate);
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
      if(currencyOutOfDate(crypto)) {
        GUI::LegacyScreen()->setWarning(&warning_30);
      }
      else {
        GUI::LegacyScreen()->setWarning(&black_60);
      }
      GUI::LegacyScreen()->show();
      waitForNextUpdate();
    }
  }
}