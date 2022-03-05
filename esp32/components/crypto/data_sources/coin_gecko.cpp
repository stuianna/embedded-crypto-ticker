#include "coin_gecko.hpp"

#include <esp_log.h>
#include <freertos/FreeRTOS.h>

#include <utility>

#include "../ArduinoJson-v6.17.3.h"
#include "../hal/requests.hpp"
#include "../tools//json_parsing.hpp"

using namespace Crypto::DataSources;
using namespace Crypto::HAL;
using namespace Crypto::Tools;


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
  request.timeout = 5000;

  size_t rc = Request()->get(request);
  SimplePrice response = _parse_simple_price(request.data, rc, vsCurrency, ids);
  Request()->cleanup(request);
  return response;
}

std::pair<size_t, std::array<float, 288>> CoinGecko::marketChartRange24h(const char* ids, const char* vsCurrency, size_t from, size_t to) {
  const char* pathBase = "/api/v3/coins/";
  const char* currencyDesc = "vs_currency=";
  const char* fromQuery = "&from=";
  const char* toQuery = "&to=";
  char path[196] = {0};

  strcat(path, pathBase);
  strcat(path, ids);
  strcat(path, "/market_chart/range?");
  strcat(path, currencyDesc);
  strcat(path, vsCurrency);
  strcat(path, fromQuery);
  size_t pos = strlen(path);
  snprintf(&path[pos], 196 - pos, "%zu", from);
  strcat(path, toQuery);
  pos = strlen(path);
  snprintf(&path[pos], 196 - pos, "%zu", to);

  Requests::GetRequest request;
  request.host = "api.coingecko.com";
  request.path = path;
  request.length = 45000;
  request.timeout = 10000;

  std::array<float, 288> results;
  size_t rc = Request()->get(request);
  results.fill(0.f);

  if(rc != 200) {
    Request()->cleanup(request);
    return std::make_pair(rc, results);
  }

  size_t priceStart = find_key_value(request.data, "prices");
  if(!priceStart) {
    Request()->cleanup(request);
    return std::make_pair(JSON_PARSING_FAILED, results);
  }
  size_t priceEnd = find_nested_array_end(&request.data[priceStart]);
  if(!priceEnd) {
    Request()->cleanup(request);
    return std::make_pair(JSON_PARSING_FAILED, results);
  }
  request.data[priceStart + priceEnd] = 0;

  DynamicJsonDocument doc(26384);
  DeserializationError error = deserializeJson(doc, &request.data[priceStart]);
  if(error) {
    Request()->cleanup(request);
    return std::make_pair(JSON_PARSING_FAILED, results);
  }

  pos = 0;
  for(JsonVariant value: doc.as<JsonArray>()) {
    results[pos++] = value.as<JsonArray>()[1];
  }
  Request()->cleanup(request);
  return std::make_pair(rc, results);
}