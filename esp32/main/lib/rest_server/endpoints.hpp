#ifndef __REST_SERVER_ENDPOINTS_HPP
#define __REST_SERVER_ENDPOINTS_HPP

#include <cstddef>
#include <cstdint>

#include "server.hpp"

/**
 * @brief Namespace for REST HTTP server.
 */
namespace REST {
  namespace Endpoints {
    /**
     * @brief Structure for detailing the array of all registered endpoints.
     */
    struct Registered {
      const HAL::REST::RestServer::Endpoint* endpoints;  ///< Pointer to the first element of the registered endpoint array.
      size_t count;                                      ///< The length of the registered endpoint array
    };

    /**
     * @brief Retrieve a structure describing the array of all registered endpoint.
     * @return Registered Structure containing endpoint details.
     */
    Registered available();

    /**
     * @brief System information GET request handler.
     * @param buffer Character buffer to which the get request JSON is populated.
     * @param length Maximum length of the character buffer.
     * @return HAL::REST::RestServer::Error HTTP error code of the response.
     */
    HAL::REST::RestServer::Error systemInformation(char* buffer, size_t length);
  };    // namespace Endpoints
};      // namespace REST
#endif  // __REST_SERVER_ENDPOINTS_HPP