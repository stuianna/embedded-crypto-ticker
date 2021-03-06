#include "wifi.hpp"

#include <esp_event.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <freertos/event_groups.h>
#include <nvs_flash.h>
#include <wifi_provisioning/manager.h>
#include <wifi_provisioning/scheme_softap.h>

#include "nvs.hpp"

#define TAG                   "WIFI"
#define PROV_QR_VERSION       "v1"
#define PROV_TRANSPORT_SOFTAP "softap"

using namespace HAL;

static WiFiConnectionState _state;

static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
  if(event_base == WIFI_PROV_EVENT) {
    switch(event_id) {
      case WIFI_PROV_START:
        ESP_LOGI(TAG, "WIFI provisioning started");
        _state = WiFiConnectionState::PROVISIONING_STARTED;
        break;
      case WIFI_PROV_CRED_RECV: {
        wifi_sta_config_t* wifi_sta_cfg = (wifi_sta_config_t*)event_data;
        ESP_LOGI(TAG,
                 "Received Wi-Fi credentials"
                 "\n\tSSID     : %s\n\tPassword : %s",
                 (const char*)wifi_sta_cfg->ssid, (const char*)wifi_sta_cfg->password);
        _state = WiFiConnectionState::PROVISIONING_RECEVIED_CREDENTIALS;
        break;
      }
      case WIFI_PROV_CRED_FAIL: {
        wifi_prov_sta_fail_reason_t* reason = (wifi_prov_sta_fail_reason_t*)event_data;
        ESP_LOGE(TAG,
                 "Provisioning failed!\n\tReason : %s"
                 "\n\tPlease reset to factory and retry provisioning",
                 (*reason == WIFI_PROV_STA_AUTH_ERROR) ? "Wi-Fi station authentication failed" : "Wi-Fi access-point not found");
        _state =
        *reason == WIFI_PROV_STA_AUTH_ERROR ? WiFiConnectionState::PROVISIONING_BAD_CREDENTIALS : WiFiConnectionState::PROVISIONING_AP_LOST;
        break;
      }
      case WIFI_PROV_CRED_SUCCESS:
        ESP_LOGI(TAG, "Provisioning successful");
        _state = WiFiConnectionState::PROVISIONING_SUCCESS;
        break;
      case WIFI_PROV_END: _state = WiFiConnectionState::PROVISIONING_COMPLETE; break;
      default: break;
    }
  }
  else if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    esp_wifi_connect();
  }
  else if(event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
    ESP_LOGI(TAG, "Connected with IP Address:" IPSTR, IP2STR(&event->ip_info.ip));
    /* Signal main application to continue execution */
    _state = WiFiConnectionState::CONNECTED;
  }
  else if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
    ESP_LOGI(TAG, "Disconnected. Connecting to the AP again...");
    _state = WiFiConnectionState::LOST_CONNECTION;
    esp_wifi_connect();
  }
}

Wifi::Wifi() {
  _init();
}

void Wifi::_init() {
  NVS()->initialise();

  /* Initialize TCP/IP */
  ESP_ERROR_CHECK(esp_netif_init());

  /* Initialize the event loop */
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  /* Register our event handler for Wi-Fi, IP and Provisioning related events */
  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_PROV_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

  esp_netif_create_default_wifi_sta();
  esp_netif_create_default_wifi_ap();
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));

  wifi_prov_mgr_config_t config = {.scheme = wifi_prov_scheme_softap, .scheme_event_handler = WIFI_PROV_EVENT_HANDLER_NONE};

  ESP_ERROR_CHECK(wifi_prov_mgr_init(config));
  bool provisioned = false;

  ESP_ERROR_CHECK(wifi_prov_mgr_is_provisioned(&provisioned));
  _state = provisioned ? WiFiConnectionState::PROVISIONED : WiFiConnectionState::NOT_PROVISIONED;
}

void Wifi::startProvisioning(const char* ssid, const char* password, const char* pop_code) {
  ESP_LOGI(TAG, "Starting provisioning");
  wifi_prov_security_t security = WIFI_PROV_SECURITY_1;

  ESP_ERROR_CHECK(wifi_prov_mgr_start_provisioning(security, pop_code, ssid, password));
}

void Wifi::stopProvisioning() {
  wifi_prov_mgr_deinit();
}

void Wifi::resetProvisioningCredentials() {
  wifi_prov_mgr_reset_sm_state_on_failure();
}

void Wifi::getQRCodeData(char* buffer, size_t maxLength, const char* ssid, const char* popCode) {
  snprintf(buffer, maxLength,
           "{\"ver\":\"%s\",\"name\":\"%s\""
           ",\"pop\":\"%s\",\"transport\":\"%s\"}",
           PROV_QR_VERSION, ssid, popCode, PROV_TRANSPORT_SOFTAP);
}

WiFiConnectionState Wifi::status() {
  return _state;
}

void Wifi::connect() {
  stopProvisioning();
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_start());
  _state = WiFiConnectionState::CONNECTING;
}

void Wifi::disconnect() {
  esp_wifi_disconnect();
}