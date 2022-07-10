#include "mdns.hpp"

#include "esp_log.h"
#include "mdns.h"

#define LOG_TAG "MDNS"

using namespace HAL::REST;

HAL::REST::MDNSService* HAL::REST::MDNSService::instance() {
  static MDNSService _instance;
  return &_instance;
}

void HAL::REST::MDNSService::initialise(const char* hostname, const char* description) {
  ESP_LOGI(LOG_TAG, "%s %s", "Set hostname as ", hostname);
  mdns_hostname_set(hostname);
  mdns_instance_name_set(description);
  mdns_txt_item_t serviceTxtData[] = {{"board", "esp32"}, {"path", "/"}};
  mdns_service_add("ESP32-WebServer", "_http", "_tcp", 80, serviceTxtData, sizeof(serviceTxtData) / sizeof(serviceTxtData[0]));
}

HAL::REST::MDNSService::MDNSService() {
  ESP_LOGI(LOG_TAG, "%s", "Initialise MDNS");
  mdns_init();
}