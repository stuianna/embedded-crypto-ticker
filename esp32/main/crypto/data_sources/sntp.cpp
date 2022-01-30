#include <esp_log.h>
#include <esp_sntp.h>
#include <sys/time.h>
#include <time.h>

#include <crypto/data_sources/sntp.hpp>
static const char* TAG = "sntp";
using namespace Crypto::DataSources;

static void time_sync_notification_cb(struct timeval* tv) {
  ESP_LOGI(TAG, "Received time synchronization event");
}

SNTP::SNTP() {
}

SNTP* SNTP::instance() {
  static SNTP _instance;
  return &_instance;
}

bool SNTP::syncronise() {
  _initialise_sntp();
  time_t now = 0;
  struct tm timeinfo = {0};
  int retry = 0;
  const int retry_count = 60;

  while(sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
    ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
  if(retry == retry_count) {
    return false;
  }
  time(&now);
  localtime_r(&now, &timeinfo);
  time(&now);
  ESP_LOGI(TAG, "SNTP time synchronised");
  return true;
}

uint32_t SNTP::unixTime() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec;
}

void SNTP::_initialise_sntp() {
  ESP_LOGI(TAG, "Initializing SNTP");
  sntp_setoperatingmode(SNTP_OPMODE_POLL);

  sntp_setservername(0, "pool.ntp.org");
  sntp_set_time_sync_notification_cb(time_sync_notification_cb);
  sntp_init();
  ESP_LOGI(TAG, "List of configured NTP servers:");

  for(uint8_t i = 0; i < SNTP_MAX_SERVERS; ++i) {
    if(sntp_getservername(i)) {
      ESP_LOGI(TAG, "server %d: %s", i, sntp_getservername(i));
    }
    else {
      // we have either IPv4 or IPv6 address, let's print it
      char buff[32];
      ip_addr_t const* ip = sntp_getserver(i);
      if(ipaddr_ntoa_r(ip, buff, 32) != NULL)
        ESP_LOGI(TAG, "server %d: %s", i, buff);
    }
  }
}