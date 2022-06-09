#include "hardware_information.hpp"

#include <esp_log.h>
#include <esp_ota_ops.h>
#include <esp_spi_flash.h>
#include <esp_system.h>
#include <string.h>

using namespace HAL;

_HardwareInformation::_HardwareInformation() {
  update();
}

const char* _HardwareInformation::parseESPModel(size_t model) {
  switch(model) {
    case CHIP_ESP32: return "ESP32";
    case CHIP_ESP32S2: return "ESP32-S2";
    case CHIP_ESP32C3: return "ESP32-C3";
    case CHIP_ESP32S3: return "ESP32-S3";
    case CHIP_ESP32H2: return "ESP32-H2";
    default:
    case ESP_CHIP_ID_INVALID: return "Invalid";
  }
}

void _HardwareInformation::update() {
  uint8_t mac[6];
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
  snprintf(_macAddress, sizeof(_macAddress), "%2X:%2X:%2X:%2X:%2X:%2X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

  _sdkVersion = esp_get_idf_version();

  esp_chip_info_t info;
  esp_chip_info(&info);
  _chip = parseESPModel(info.model);
  _chipRevision = info.revision;
  _cores = info.cores;
  _flashSize = spi_flash_get_chip_size();

  const esp_app_desc_t* appDescription = esp_ota_get_app_description();
  _appVersion = appDescription->version;
  _compileDate = appDescription->date;
  _compileTime = appDescription->time;

  for(int i = 0; i < 32; i++) {
    snprintf(&_binarySHA256[i * 2], 3, "%02x", appDescription->app_elf_sha256[i]);
  }
}

const char* _HardwareInformation::sdkVersion() const {
  return _sdkVersion;
}

const char* _HardwareInformation::macAddress() const {
  return _macAddress;
}

const char* _HardwareInformation::chip() const {
  return _chip;
}

size_t _HardwareInformation::chipRevision() const {
  return _chipRevision;
}

size_t _HardwareInformation::flashSize() const {
  return _flashSize;
}

size_t _HardwareInformation::cores() const {
  return _cores;
}

const char* _HardwareInformation::appVersion() const {
  return _appVersion;
}

const char* _HardwareInformation::compileTime() const {
  return _compileTime;
}

const char* _HardwareInformation::compileDate() const {
  return _compileDate;
}

const char* _HardwareInformation::binarySHA256() const {
  return _binarySHA256;
}

_HardwareInformation* _HardwareInformation::instance() {
  static _HardwareInformation _instance;
  return &_instance;
}