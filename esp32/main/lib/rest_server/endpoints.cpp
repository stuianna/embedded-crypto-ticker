#include "endpoints.hpp"

#include <third_party/ArduinoJson-v6.17.3.h>

using namespace HAL::REST;

// clang-format off
const RestServer::Endpoint registeredEndpoints[] = {
  {
    .uri = "/api/v1/system/information",
    .method = RestServer::RestMethod::GET,
    .handler = REST::Endpoints::systemInformation,
  }
};
// clang-format on

REST::Endpoints::Registered REST::Endpoints::available() {
  return {registeredEndpoints, sizeof(registeredEndpoints) / sizeof(RestServer::Endpoint)};
}

RestServer::Error REST::Endpoints::systemInformation(char* buffer, size_t length) {
  memset(buffer, 0, length);
  StaticJsonDocument<128> doc;
  // TODO actually populate the JSON with system information
  doc["time"] = 12;
  serializeJson(doc, buffer, length);
  return RestServer::Error::HTTP_200;
}