#include "server.hpp"

#include <fcntl.h>
#include <string.h>

#include "endpoints.hpp"
#include "esp_http_server.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "mdns.hpp"

using namespace HAL::REST;
using namespace REST;
#define LOG_TAG "rest server"

#define SCRATCH_BUFSIZE (10240)

typedef struct rest_server_context {
  char base_path[ESP_VFS_PATH_MAX + 1];
  char scratch[SCRATCH_BUFSIZE];
} rest_server_context_t;

static httpd_handle_t server = NULL;
static rest_server_context_t* rest_context = NULL;


static esp_err_t rest_common_get_handler(httpd_req_t* req) {
  httpd_resp_set_type(req, "application/json");
  auto availableEndpoints = Endpoints::available();
  char buffer[1024];  // TODO This 1024 should be defined at compile time.

  for(size_t i = 0; i < availableEndpoints.count; i++) {
    auto endpoint = availableEndpoints.endpoints[i];
    if(strcmp(req->uri, endpoint.uri) == 0) {
      auto error = endpoint.handler(buffer, 1024);
      // TODO  Handle error
    }
  }
  // TODO  Handle not found

  httpd_resp_sendstr(req, static_cast<const char*>(buffer));
  return ESP_OK;
}

// TODO Add handler for common POST style endpoints


HAL::REST::RestServer::RestServer() {
  // TODO MDNS name should be set at compile time.
  MDNS()->initialise("ESP32-ECT", "ESP32 based embedded crypto ticker.");
}

HAL::REST::RestServer* HAL::REST::RestServer::instance() {
  static RestServer _instance;
  return &_instance;
}

void HAL::REST::RestServer::start() {
  rest_context = static_cast<rest_server_context_t*>(calloc(1, sizeof(rest_server_context_t)));
  if(!rest_context) {
    ESP_LOGE(LOG_TAG, "%s", "No memory for rest context.");
    return;
  }
  strlcpy(rest_context->base_path, "/www", sizeof(rest_context->base_path));

  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.uri_match_fn = httpd_uri_match_wildcard;

  ESP_LOGI(LOG_TAG, "%s", "Starting HTTP Server");
  if(httpd_start(&server, &config) != ESP_OK) {
    ESP_LOGE(LOG_TAG, "%s", "Server start failed.");
  }

  auto availableEndpoints = Endpoints::available();
  auto method = HTTP_GET;
  for(size_t i = 0; i < availableEndpoints.count; i++) {
    auto endpoint = availableEndpoints.endpoints[i];
    switch(endpoint.method) {
      default:
      case RestMethod::GET: break;
      case RestMethod::POST: method = HTTP_POST; break;
    }

    // Register the common get handler for the request.
    httpd_uri_t uri = {.uri = endpoint.uri, .method = method, .handler = rest_common_get_handler, .user_ctx = rest_context};
    httpd_register_uri_handler(server, &uri);

    // TODO handle post type endpoints.
  }

  // TODO URI handler for getting web server files
}

void HAL::REST::RestServer::stop() {
  httpd_stop(server);
}