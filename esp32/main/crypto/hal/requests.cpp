#include "crypto/hal/requests.hpp"

#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_tls.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static void https_task(void* pvParameters);
static QueueHandle_t getQueue;
static QueueHandle_t responseQueue;

using namespace Crypto::HAL;

Requests Requests::_instance;

// bool HTTPSRequests::_initialised = false;
constexpr const char* LOG_TAG = "Requests";

esp_err_t _http_event_handler(esp_http_client_event_t* evt) {
  static size_t dataPosition = 0;
  switch(evt->event_id) {
    case HTTP_EVENT_ERROR: ESP_LOGI(LOG_TAG, "HTTP_EVENT_ERROR"); break;
    case HTTP_EVENT_ON_CONNECTED: ESP_LOGI(LOG_TAG, "HTTP_EVENT_ON_CONNECTED"); break;
    case HTTP_EVENT_HEADER_SENT: ESP_LOGI(LOG_TAG, "HTTP_EVENT_HEADER_SENT"); break;
    case HTTP_EVENT_ON_HEADER:
      ESP_LOGI(LOG_TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
      dataPosition = 0;
      break;
    case HTTP_EVENT_ON_DATA:
      ESP_LOGI(LOG_TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
      memcpy(static_cast<char*>(evt->user_data) + dataPosition, evt->data, evt->data_len);
      dataPosition += evt->data_len;
      break;
    case HTTP_EVENT_ON_FINISH: ESP_LOGI(LOG_TAG, "HTTP_EVENT_ON_FINISH"); break;
    case HTTP_EVENT_DISCONNECTED:
      ESP_LOGI(LOG_TAG, "HTTP_EVENT_DISCONNECTED");
      int mbedtls_err = 0;
      esp_err_t err = esp_tls_get_and_clear_last_error((esp_tls_error_handle_t)evt->data, &mbedtls_err, NULL);
      if(err != 0) {
        ESP_LOGI(LOG_TAG, "Last esp error code: 0x%x", err);
        ESP_LOGI(LOG_TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
      }
      break;
  }
  return ESP_OK;
}

Requests::Requests() : _initialised(false) {
}


void Requests::init() {
  if(_initialised) {
    return;
  }
  TaskHandle_t httpTaskHandle;
  xTaskCreatePinnedToCore(&https_task, "http_task", 4096 * 2, NULL, 2, &httpTaskHandle, 0);
  getQueue = xQueueCreate(1, sizeof(Requests::GetRequest));
  responseQueue = xQueueCreate(1, sizeof(uint32_t));

  if(!getQueue || !responseQueue || !httpTaskHandle) {
    ESP_LOGE(LOG_TAG, "Failed to allocate memeory for HTTP task, halting.");
    while(1) {};
  }
  _initialised = true;
}

size_t Requests::get(GetRequest& request) {
  init();
  ESP_LOGI(LOG_TAG, "Allocating %d bytes for reqeust", request.length);
  request.data = (char*)pvPortMalloc(sizeof(char) * request.length);

  if(!request.data) {
    ESP_LOGE(LOG_TAG, "Error perform http request. Cannot allocate memory for storage");
    return 0;
  }

  xQueueReset(responseQueue);
  if(xQueueSend(getQueue, (void*)(&request), pdMS_TO_TICKS(request.timeout)) != pdTRUE) {
    ESP_LOGI(LOG_TAG, "Failed to add request to queue.");
    return 0;
  }
  size_t rc = 0;
  bool received = _status(rc, request.timeout);
  ESP_LOGI(LOG_TAG, "Received = %d", received);
  ESP_LOGI(LOG_TAG, "rc = %d", rc);
  return received ? rc : 0;
}

void Requests::cleanup(GetRequest& request) {
  if(request.data) {
    vPortFree(request.data);
  }
}

bool Requests::_status(size_t& httpStatusCode, size_t timeout) {
  if(xQueueReceive(responseQueue, &httpStatusCode, pdMS_TO_TICKS(timeout)) == pdFALSE) {
    return false;
  }
  return true;
}

void https_task(void* pvParameters) {
  Requests::GetRequest currentRequest;

  while(1) {
    if(xQueueReceive(getQueue, &currentRequest, pdMS_TO_TICKS(50)) == pdFALSE) {
      continue;
    }
    ESP_LOGI(LOG_TAG, "HTTPS Request: Host %s, Path %s", currentRequest.host, currentRequest.path);
    memset(currentRequest.data, 0, currentRequest.length);

    esp_http_client_config_t config = {NULL};
    config.host = currentRequest.host;
    config.path = currentRequest.path;
    config.transport_type = HTTP_TRANSPORT_OVER_SSL;
    config.event_handler = _http_event_handler;
    config.user_data = currentRequest.data;

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    uint32_t status = esp_http_client_get_status_code(client);

    if(err == ESP_OK) {
      ESP_LOGI(LOG_TAG, "HTTPS Status = %d, data = %s", status, currentRequest.data);
      xQueueSend(responseQueue, (void*)(&status), pdMS_TO_TICKS(50));
    }
    else {
      ESP_LOGE(LOG_TAG, "Error perform http request %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);
  }
}