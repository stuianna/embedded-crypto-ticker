#include "file_system.hpp"

#include "esp_log.h"
#include "esp_spiffs.h"
#include "esp_vfs_fat.h"
#include "esp_vfs_semihost.h"

#define LOG_TAG "file system"

using namespace HAL::REST;

HAL::REST::_FileSystem* HAL::REST::_FileSystem::instance() {
  static _FileSystem _instance;
  return &_instance;
}

void HAL::REST::_FileSystem::initialise(const char* mountPoint) {
  esp_vfs_spiffs_conf_t conf = {.base_path = mountPoint, .partition_label = NULL, .max_files = 5, .format_if_mount_failed = false};
  esp_err_t ret = esp_vfs_spiffs_register(&conf);

  if(ret != ESP_OK) {
    if(ret == ESP_FAIL) {
      ESP_LOGE(LOG_TAG, "Failed to mount or format filesystem");
    }
    else if(ret == ESP_ERR_NOT_FOUND) {
      ESP_LOGE(LOG_TAG, "Failed to find SPIFFS partition");
    }
    else {
      ESP_LOGE(LOG_TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));

      // TODO What now? Handle error case.
    }
  }

  size_t total = 0, used = 0;
  ret = esp_spiffs_info(NULL, &total, &used);
  if(ret != ESP_OK) {
    ESP_LOGE(LOG_TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
  }
  else {
    ESP_LOGI(LOG_TAG, "Partition size: total: %d, used: %d", total, used);
  }
}

HAL::REST::_FileSystem::_FileSystem() {
}