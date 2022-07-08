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


void send_httpd_response(httpd_req_t* req, RestServer::Error httpCode, const char* buffer) {
  switch(httpCode) {
    case RestServer::Error::HTTP_200: httpd_resp_sendstr(req, buffer); break;
    case RestServer::Error::HTTP_404: httpd_resp_send_404(req); break;
    case RestServer::Error::HTTP_500: httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, buffer); break;
  }
}

static esp_err_t rest_common_get_handler(httpd_req_t* req) {
  httpd_resp_set_type(req, "application/json");
  char buffer[1024];  // TODO This 1024 should be defined at compile time.
  auto error = Endpoints::handleURI(req->uri, buffer, 1024);
  send_httpd_response(req, error, static_cast<const char*>(buffer));
  return ESP_OK;
}

static esp_err_t rest_common_post_handler(httpd_req_t* req) {
  int total_len = req->content_len;
  int cur_len = 0;
  char* buffer = (static_cast<rest_server_context_t*>(req->user_ctx))->scratch;
  if(total_len >= SCRATCH_BUFSIZE) {
    // Respond with 500 Internal Server Error
    httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Post content too long");
    return ESP_FAIL;
  }
  while(cur_len < total_len) {
    int received = httpd_req_recv(req, buffer + cur_len, total_len);
    if(received <= 0) {
      // Respond with 500 Internal Server Error
      httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to post control value");
      return ESP_FAIL;
    }
    cur_len += received;
  }
  buffer[total_len] = '\0';

  auto error = Endpoints::handleURI(req->uri, buffer, SCRATCH_BUFSIZE);
  send_httpd_response(req, error, static_cast<const char*>(buffer));
  return ESP_OK;
}

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
  for(size_t i = 0; i < availableEndpoints.count; i++) {
    auto endpoint = availableEndpoints.endpoints[i];
    switch(endpoint.method) {
      default:
      case RestMethod::GET: {
        httpd_uri_t uri = {.uri = endpoint.uri, .method = HTTP_GET, .handler = rest_common_get_handler, .user_ctx = rest_context};
        httpd_register_uri_handler(server, &uri);
      } break;
      case RestMethod::POST: {
        httpd_uri_t uri = {.uri = endpoint.uri, .method = HTTP_POST, .handler = rest_common_post_handler, .user_ctx = rest_context};
        httpd_register_uri_handler(server, &uri);
      } break;
    }
  }

  // TODO URI handler for getting web server files
}

void HAL::REST::RestServer::stop() {
  httpd_stop(server);
}