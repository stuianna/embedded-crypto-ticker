#include "server.hpp"

#include <fcntl.h>
#include <string.h>

#include "endpoints.hpp"
#include "esp_http_server.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "file_system.hpp"
#include "mdns.hpp"

using namespace HAL::REST;
using namespace REST;
#define LOG_TAG "rest server"

#define FILE_PATH_MAX   (ESP_VFS_PATH_MAX + 128)
#define SCRATCH_BUFSIZE (10240)

#define CHECK_FILE_EXTENSION(filename, ext) (strcasecmp(&filename[strlen(filename) - strlen(ext)], ext) == 0)

typedef struct rest_server_context {
  char base_path[ESP_VFS_PATH_MAX + 1];
  char scratch[SCRATCH_BUFSIZE];
} rest_server_context_t;

static httpd_handle_t server = NULL;
static rest_server_context_t* rest_context = NULL;

static esp_err_t set_content_type_from_file(httpd_req_t* req, const char* filepath) {
  const char* type = "text/plain";
  if(CHECK_FILE_EXTENSION(filepath, ".html")) {
    type = "text/html";
  }
  else if(CHECK_FILE_EXTENSION(filepath, ".js")) {
    type = "application/javascript";
  }
  else if(CHECK_FILE_EXTENSION(filepath, ".css")) {
    type = "text/css";
  }
  else if(CHECK_FILE_EXTENSION(filepath, ".png")) {
    type = "image/png";
  }
  else if(CHECK_FILE_EXTENSION(filepath, ".ico")) {
    type = "image/x-icon";
  }
  else if(CHECK_FILE_EXTENSION(filepath, ".svg")) {
    type = "text/xml";
  }
  return httpd_resp_set_type(req, type);
}

// GET request handler for files.
static esp_err_t rest_file_get_handler(httpd_req_t* req) {
  char filepath[FILE_PATH_MAX];

  rest_server_context_t* context = static_cast<rest_server_context_t*>(req->user_ctx);
  strlcpy(filepath, context->base_path, sizeof(filepath));
  if(req->uri[strlen(req->uri) - 1] == '/') {
    strlcat(filepath, "/index.html", sizeof(filepath));
  }
  else {
    strlcat(filepath, req->uri, sizeof(filepath));
  }
  int fd = open(filepath, O_RDONLY, 0);
  if(fd == -1) {
    ESP_LOGE(LOG_TAG, "Failed to open file : %s", filepath);
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }
  set_content_type_from_file(req, filepath);

  char* chunk = context->scratch;
  ssize_t read_bytes;
  do {
    // Read file in chunks into the scratch buffer
    read_bytes = read(fd, chunk, SCRATCH_BUFSIZE);
    if(read_bytes == -1) {
      ESP_LOGE(LOG_TAG, "Failed to read file : %s", filepath);
    }
    else if(read_bytes > 0) {
      // Send the buffer contents as HTTP response chunk
      if(httpd_resp_send_chunk(req, chunk, read_bytes) != ESP_OK) {
        close(fd);
        ESP_LOGE(LOG_TAG, "File sending failed!");
        // Abort sending file
        httpd_resp_sendstr_chunk(req, NULL);
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");
        return ESP_FAIL;
      }
    }
  } while(read_bytes > 0);
  close(fd);
  ESP_LOGI(LOG_TAG, "File sending complete");
  httpd_resp_send_chunk(req, NULL, 0);
  return ESP_OK;
}

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
  HAL::REST::FileSystem()->initialise("/www");
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

  httpd_uri_t file_get_uri = {.uri = "/*", .method = HTTP_GET, .handler = rest_file_get_handler, .user_ctx = rest_context};
  httpd_register_uri_handler(server, &file_get_uri);
}

void HAL::REST::RestServer::stop() {
  httpd_stop(server);
}