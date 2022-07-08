#include "endpoints.hpp"

#include <third_party/ArduinoJson-v6.17.3.h>

#include <hal/system.hpp>

using namespace HAL::REST;

// clang-format off
const RestServer::Endpoint registeredEndpoints[] = {
  {
    .uri = "/api/v1/system/information",
    .method = RestServer::RestMethod::GET,
    .handler = REST::Endpoints::systemInformation,
  },
  {
    .uri = "/api/v1/system/reset",
    .method = RestServer::RestMethod::POST,
    .handler = REST::Endpoints::systemReset,
  }
};
// clang-format on

REST::Endpoints::Registered REST::Endpoints::available() {
  return {registeredEndpoints, sizeof(registeredEndpoints) / sizeof(RestServer::Endpoint)};
}


RestServer::Error REST::Endpoints::handleURI(const char* uri, char* buffer, size_t length) {
  auto availableEndpoints = REST::Endpoints::available();
  for(size_t i = 0; i < availableEndpoints.count; i++) {
    auto endpoint = availableEndpoints.endpoints[i];
    if(strcmp(uri, endpoint.uri) == 0) {
      auto error = endpoint.handler(buffer, length);
      return error;
    }
  }
  return RestServer::Error::HTTP_404;
}

RestServer::Error REST::Endpoints::systemInformation(char* buffer, size_t length) {
  memset(buffer, 0, length);
  StaticJsonDocument<128> doc;
  // TODO actually populate the JSON with system information
  doc["time"] = 12;
  serializeJson(doc, buffer, length);
  return RestServer::Error::HTTP_200;
}

RestServer::Error REST::Endpoints::systemReset(char* buffer, size_t length) {
  StaticJsonDocument<64> doc;
  DeserializationError error = deserializeJson(doc, buffer, length);
  if(error) {
    snprintf(buffer, length, "%s %s", "JSON deserialize failed with: ", error.c_str());
    return RestServer::Error::HTTP_500;
  }
  bool reset = doc["reset"];
  if(reset) {
    snprintf(buffer, length, "%s", "System reset requested.");
    HAL::System::reset();
  }
  else {
    snprintf(buffer, length, "%s", "System reset not requested.");
  }
  return RestServer::Error::HTTP_200;
}
