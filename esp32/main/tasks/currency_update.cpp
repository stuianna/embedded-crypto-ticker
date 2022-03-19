#include "currency_update.hpp"

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <data_sources/coin_gecko.hpp>
#include <data_sources/sntp.hpp>

#define LOG_TAG "currency_update"

using namespace Tasks;
using namespace Crypto;
using namespace Crypto::DataSources;

static void delaySeconds(size_t seconds) {
  for(int i = 0; i < seconds; i++) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

static size_t singleUpdate(Crypto::Entry& crypto, const Crypto::Definition fiat) {
  CoinGecko::SimplePrice currentData = CoinGecko().simplePrice(crypto.params.geckoName, fiat.geckoName, false, false, true);
  if(currentData.status == 200) {
    crypto.pricesDB.add(currentData.price);
    crypto.delta24hDB.add(currentData.change24h);
    crypto.latestUpdate = Crypto::SNTP()->unixTime();
  }
  else {
    ESP_LOGW(LOG_TAG, "Response code %d received", currentData.status);
  }
  return currentData.status;
}

static size_t updateCurrency(Crypto::Entry& crypto, const Crypto::Definition& fiat) {
  const uint8_t maxAttempts = 3;
  const uint8_t secondsWaitOnRetry = 5;
  uint8_t attempt = 0;
  size_t httpStatus;

  do {
    ESP_LOGI(LOG_TAG, "Update currency %s, attempt %d", crypto.params.name, attempt);
    httpStatus = singleUpdate(crypto, fiat);
    if(httpStatus == 200) {
      break;
    }
    delaySeconds(secondsWaitOnRetry);
  } while(++attempt < maxAttempts);

  if(attempt == maxAttempts) {
    ESP_LOGE(LOG_TAG, "Failed to update currency %s after %d attempts.", crypto.params.name, maxAttempts);
  }
  return httpStatus;
}

static size_t secondsToNextUpdate(const size_t startSeconds) {
  const size_t endSeconds = Crypto::SNTP()->unixTime();
  uint32_t secondsDelta = endSeconds - startSeconds;
  int32_t secondsWait = (CurrencyUpdate()->updatePeriod() / Crypto::enabledCurrencyCount()) - secondsDelta;
  return secondsWait < 0 ? 0 : secondsWait;
}

static bool currencyNeedsUpdating(const Crypto::Entry& crypto) {
  while(CurrencyUpdate()->running() == false) {
    vTaskDelay(pdMS_TO_TICKS(100));
  }
  return crypto.enabled;
}

template<size_t n>
static void updatePriceDatabase(std::array<float, n> source, Crypto::Entry& crypto) {
  for(const auto& price: source) {
    if(price != 0.0f) {
      crypto.pricesDB.add(price);
    }
  }
}

static size_t fetchHistoricalData(Crypto::Entry& crypto, const Crypto::Definition& fiat) {
  const uint8_t maxAttempts = 3;
  const uint8_t secondsWaitOnRetry = 5;
  const char* cryptoGeckoName = crypto.params.geckoName;
  const char* fiatGeckoName = fiat.geckoName;
  size_t secondsPerDay = 60 * 60 * 24;
  size_t httpStatus;
  uint8_t attempt = 0;

  do {
    ESP_LOGI(LOG_TAG, "Historical currency update for %s, attempt %d", crypto.params.name, attempt);
    uint32_t nowSeconds = Crypto::SNTP()->unixTime();
    size_t fromSeconds = nowSeconds - secondsPerDay;

    auto historical = CoinGecko().marketChartRange24h(cryptoGeckoName, fiatGeckoName, fromSeconds, nowSeconds);
    httpStatus = historical.first;

    if(httpStatus == 200) {
      updatePriceDatabase<historical.second.size()>(historical.second, crypto);
      return httpStatus;
    }
    else {
      ESP_LOGW(LOG_TAG, "Response code from historical update %d", httpStatus);
      delaySeconds(attempt != maxAttempts ? secondsWaitOnRetry : 0);
    }
  } while(++attempt < maxAttempts);
  ESP_LOGE(LOG_TAG, "Failed to update historical data for %s after %d attempts.", crypto.params.name, maxAttempts);
  return httpStatus;
}

static void task_currencyUpdate(void* pvParameters) {
  (void)pvParameters;
  auto fiat = Crypto::getDefinition(Crypto::baseCurrency);

  while(1) {
    for(auto i = 0; i < Crypto::currencyCount(); i++) {
      auto& crypto = Crypto::Table[i];
      if(!currencyNeedsUpdating(crypto)) {
        continue;
      }

      size_t startSeconds = Crypto::SNTP()->unixTime();
      updateCurrency(crypto, fiat);
      size_t secondsWait = secondsToNextUpdate(startSeconds);

      ESP_LOGI(LOG_TAG, "Seconds to next request %d", secondsWait);
      delaySeconds(secondsWait);
    }
  }
}

CurrencyUpdateTask* CurrencyUpdateTask::instance() {
  static auto _instance = CurrencyUpdateTask();
  return &_instance;
}

size_t CurrencyUpdateTask::updateHistorical(Crypto::Currency currency) {
  auto fiat = Crypto::getDefinition(Crypto::baseCurrency);
  auto index = Crypto::currencyIndex(currency);
  auto& crypto = Crypto::Table[index];

  if(!crypto.enabled) {
    return 0;
  }

  ESP_LOGI(LOG_TAG, "Fetch historical for currency %s, index %d", crypto.params.name, index);
  size_t httpStatus = fetchHistoricalData(crypto, fiat);
  if(httpStatus != 200) {
    return httpStatus;
  }
  httpStatus = updateCurrency(crypto, fiat);
  return httpStatus;
}

void CurrencyUpdateTask::start() {
  _active = true;
}

void CurrencyUpdateTask::stop() {
  _active = false;
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

CurrencyUpdateTask::CurrencyUpdateTask() : _updatePeriod(300), _active(false) {
  xTaskCreatePinnedToCore(task_currencyUpdate, "Currency Update Task", 4096, NULL, 3, NULL, 0);
}
