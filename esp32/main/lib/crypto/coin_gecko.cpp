#include "coin_gecko.hpp"

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <third_party/ArduinoJson-v6.17.3.h>

#include <lib/hal/requests.hpp>
#include <lib/hal/ssl_certificate.hpp>
#include <lib/tools/json_parsing.hpp>
#include <utility>

using namespace Crypto;
using namespace HAL;
using namespace Tools;

static const char* TAG = "Coin Gecko";

CoinGecko::CoinGecko() {
  Request()->init();
}

void CoinGecko::_append_boolean(bool value, char* buffer) {
  if(value) {
    strcat(buffer, "true");
  }
  else {
    strcat(buffer, "false");
  }
}

CoinGecko::SimplePrice CoinGecko::_parse_simple_price(char* buffer, size_t httpStatus, const char* vsCurrency, const char* ids) {
  SimplePrice response{0.f, 0.f, 0.f, 0.f, httpStatus};
  if(response.status != 200) {
    return response;
  }

  StaticJsonDocument<192> doc;
  DeserializationError error = deserializeJson(doc, buffer, strlen(buffer));

  if(error) {
    response.status = JSON_PARSING_FAILED;
    return response;
  }

  response.price = doc[ids][vsCurrency];

  char deltaName[32];
  size_t len = strlen(vsCurrency);

  strncpy(deltaName, vsCurrency, 32);
  strncpy(&deltaName[len], "_24h_change", 32 - len);
  response.change24h = doc[ids][deltaName];
  memset(deltaName, 0, 32);

  strncpy(deltaName, vsCurrency, 32);
  strncpy(&deltaName[len], "_24h_vol", 32 - len);
  response.volume24h = doc[ids][deltaName];
  memset(deltaName, 0, 32);

  strncpy(deltaName, vsCurrency, 32);
  strncpy(&deltaName[len], "_market_cap", 32 - len);
  response.marketCap = doc[ids][deltaName];
  return response;
}

CoinGecko::SimplePrice CoinGecko::simplePrice(const char* ids, const char* vsCurrency, bool marketCap, bool volume24h, bool change24h) {
  const char* pathBase = "/api/v3/simple/price?ids=";
  const char* currencyDesc = "&vs_currencies=";
  const char* marketCapQuery = "&include_market_cap=";
  const char* volume24hourQuery = "&include_24hr_volume=";
  const char* delta24hourQuery = "&include_24hr_change=";
  char path[196] = {0};

  strcat(path, pathBase);
  strcat(path, ids);
  strcat(path, currencyDesc);
  strcat(path, vsCurrency);
  strcat(path, marketCapQuery);
  _append_boolean(marketCap, path);
  strcat(path, volume24hourQuery);
  _append_boolean(volume24h, path);
  strcat(path, delta24hourQuery);
  _append_boolean(change24h, path);


  Requests::GetRequest request;
  request.host = "api.coingecko.com";
  request.path = path;
  request.length = 256;
  request.timeout = 10000;
  request.sslCert = SSLCertificates::get(SSLCertificates::StoredCertificates::COIN_GECKO).certificate;
  request.sslCertLength = SSLCertificates::get(SSLCertificates::StoredCertificates::COIN_GECKO).length;

  size_t rc = Request()->get(request);
  SimplePrice response = _parse_simple_price(request.data, rc, vsCurrency, ids);
  Request()->cleanup(request);
  return response;
}

void CoinGecko::_makeMarketChartRequest(char* buffer, size_t length, const char* ids, const char* vsCurrency, size_t from, size_t to) {
  const char* pathBase = "/api/v3/coins/";
  const char* currencyDesc = "vs_currency=";
  const char* fromQuery = "&from=";
  const char* toQuery = "&to=";

  strcat(buffer, pathBase);
  strcat(buffer, ids);
  strcat(buffer, "/market_chart/range?");
  strcat(buffer, currencyDesc);
  strcat(buffer, vsCurrency);
  strcat(buffer, fromQuery);

  size_t pos = strlen(buffer);
  snprintf(&buffer[pos], 196 - pos, "%zu", from);
  strcat(buffer, toQuery);
  pos = strlen(buffer);
  snprintf(&buffer[pos], 196 - pos, "%zu", to);
}

std::pair<size_t, std::array<float, 288>> CoinGecko::marketChartRange24h(const char* ids, const char* vsCurrency, size_t from, size_t to) {
  const size_t divisions = 4;
  std::array<float, 288> results;
  results.fill(0.f);
  size_t resultPos = 0;
  size_t delta = (to - from) / divisions;
  size_t rc;

  Requests::GetRequest request;
  request.host = "api.coingecko.com";
  request.length = 8000;
  request.timeout = 10000;
  request.sslCert = SSLCertificates::get(SSLCertificates::StoredCertificates::COIN_GECKO).certificate;
  request.sslCertLength = SSLCertificates::get(SSLCertificates::StoredCertificates::COIN_GECKO).length;

  for(int i = 0; i < divisions; i++) {
    char path[196] = {0};
    size_t currentFrom = from + (delta * i);
    size_t currentTo = from + (delta * (i + 1));

    _makeMarketChartRequest(path, sizeof(path), ids, vsCurrency, currentFrom, currentTo);
    request.path = path;

    rc = Request()->get(request);
    if(rc != 200) {
      Request()->cleanup(request);
      return std::make_pair(rc, results);
    }
    size_t priceStart = find_key_value(request.data, "prices");
    if(!priceStart) {
      Request()->cleanup(request);
      ESP_LOGE(TAG, "Failed finding JSON key \"prices\"");
      return std::make_pair(JSON_PARSING_FAILED, results);
    }
    size_t priceEnd = find_nested_array_end(&request.data[priceStart]);
    if(!priceEnd) {
      Request()->cleanup(request);
      ESP_LOGE(TAG, "Failed finding end of JSON price array.");
      return std::make_pair(JSON_PARSING_FAILED, results);
    }
    request.data[priceStart + priceEnd] = 0;
    ESP_LOGI(TAG, "Using price array %s", &request.data[priceStart]);

    {
      DynamicJsonDocument doc(6000);
      DeserializationError error = deserializeJson(doc, &request.data[priceStart]);
      if(error) {
        Request()->cleanup(request);
        ESP_LOGE(TAG, "Failed JSON decoding. Error %s", error.c_str());
        return std::make_pair(JSON_PARSING_FAILED, results);
      }

      for(const JsonVariant& value: doc.as<JsonArray>()) {
        results[resultPos++] = value.as<JsonArray>()[1];
      }
    }
    Request()->cleanup(request);
  }
  return std::make_pair(rc, results);
}
