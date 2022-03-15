#include "currency_update.hpp"

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <data_sources/coin_gecko.hpp>
#include <data_sources/sntp.hpp>

#define LOG_TAG "currency_update"

using namespace Tasks;
using namespace Crypto::DataSources;

static void task_currencyUpdate(void* pvParameters) {
  (void)pvParameters;
  while(1) {
    auto fiat = Crypto::getDefinition(Crypto::baseCurrency);
    for(auto i = 0; i < Crypto::currencyCount(); i++) {
      while(CurrencyUpdate()->running() == false) {
        vTaskDelay(pdMS_TO_TICKS(100));
      }
      size_t startTime = Crypto::SNTP()->unixTime();
      auto crypto = &Crypto::Table[i];
      CoinGecko::SimplePrice currentData = CoinGecko().simplePrice(crypto->params.geckoName, fiat.geckoName, false, false, true);
      if(currentData.status == 200) {
        crypto->pricesDB.add(currentData.price);
        crypto->delta24hDB.add(currentData.change24h);
        crypto->latestUpdate = startTime;
      }
      else {
        ESP_LOGE(LOG_TAG, "Response code %d", currentData.status);
      }
      size_t endTime = Crypto::SNTP()->unixTime();
      uint32_t secondsDelta = endTime - startTime;
      uint32_t secondsWait = (CurrencyUpdate()->updatePeriod() / Crypto::currencyCount()) - secondsDelta;
      ESP_LOGI(LOG_TAG, "Seconds to next request %d", secondsWait);
      for(int j = 0; j < secondsWait; j++) {
        vTaskDelay(pdMS_TO_TICKS(1000));
      }
    }
  }
}

CurrencyUpdateTask* CurrencyUpdateTask::instance() {
  static auto _instance = CurrencyUpdateTask();
  return &_instance;
}

void CurrencyUpdateTask::start() {
  _active = true;
}

void CurrencyUpdateTask::stop() {
  _active = false;
}

Crypto::Currency CurrencyUpdateTask::currencyCurrency() {
  return _currentCurrency;
}

void CurrencyUpdateTask::updatePeriod(size_t seconds) {
  _updatePeriod = seconds;
}

size_t CurrencyUpdateTask::updatePeriod() {
  return _updatePeriod;
}

bool CurrencyUpdateTask::running() {
  return _active;
}

CurrencyUpdateTask::CurrencyUpdateTask() : _updatePeriod(300), _currentCurrency(Crypto::Table[0].currency), _active(false) {
  xTaskCreatePinnedToCore(task_currencyUpdate, "Currency Update Task", 4096, NULL, 3, NULL, 0);
}
