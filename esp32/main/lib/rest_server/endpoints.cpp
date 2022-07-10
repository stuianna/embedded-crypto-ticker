#include "endpoints.hpp"

#include <third_party/ArduinoJson-v6.17.3.h>

#include <hal/hardware_information.hpp>
#include <hal/sntp.hpp>
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
  StaticJsonDocument<512> doc;
  doc["uptime_ms"] = HAL::System::uptime();
  doc["unix_time"] = HAL::SNTP()->unixTime();
  doc["last_reset_reason"] = HAL::System::resetReason();
  doc["heap_current_free"] = HAL::System::Heap::currentFree();
  doc["heap_minimum_free"] = HAL::System::Heap::minimumFree();
  doc["heap_largest_free_block"] = HAL::System::Heap::largestFreeBlock();
  doc["sdk_version"] = HAL::HardwareInformation()->sdkVersion();
  doc["app_version"] = HAL::HardwareInformation()->appVersion();
  doc["binary_sha256"] = HAL::HardwareInformation()->binarySHA256();
  doc["compile_time"] = HAL::HardwareInformation()->compileTime();
  doc["compile_date"] = HAL::HardwareInformation()->compileDate();
  doc["chip_name"] = HAL::HardwareInformation()->chip();
  doc["chip_revision"] = HAL::HardwareInformation()->chipRevision();
  doc["chip_cores"] = HAL::HardwareInformation()->cores();
  doc["flash_size_bytes"] = HAL::HardwareInformation()->flashSize();
  doc["wifi_mac_address"] = HAL::HardwareInformation()->macAddress();
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
