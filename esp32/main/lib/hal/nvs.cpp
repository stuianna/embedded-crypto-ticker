#include "nvs.hpp"

#include <nvs_flash.h>

using namespace HAL;

_NVS::Error _NVS::initialise() {
  if(!_initialised) {
    esp_err_t ret = nvs_flash_init();
    if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      // NVS partition was truncated  and needs to be erased
      ESP_ERROR_CHECK(nvs_flash_erase());

      // Retry nvs_flash_init
      ESP_ERROR_CHECK(nvs_flash_init());
    }
  }
  _initialised = true;
  return Error::NONE;
}

_NVS* _NVS::instance() {
  static _NVS _instance;
  return &_instance;
}

_NVS::_NVS() : _initialised(false) {
  initialise();
}
