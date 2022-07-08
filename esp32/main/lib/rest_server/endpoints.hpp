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
     * @brief Compare a URI to all registered handlers, and call it if a match is found.
     * @param uri The URI to use for comparison
     * @param buffer The buffer passed to the endpoint handler, on match.
     * @param length The length of the buffer, passed to the endpoint handler, on match.
     * @return Error Error code received from the handler. Returns HTTP_404 if no match was found for the given URI.
     */
    HAL::REST::RestServer::Error handleURI(const char* uri, char* buffer, size_t length);

    /**
     * @brief System information GET request handler.
     * @param buffer Character buffer to which the get request JSON is populated.
     * @param length Maximum length of the character buffer.
     * @return HAL::REST::RestServer::Error HTTP error code of the response.
     */
    HAL::REST::RestServer::Error systemInformation(char* buffer, size_t length);

    /**
     * @brief System reset PORT handler.
     * @details Expected payload:
     * System reset is performed if reset is set to true.
     * {
     *   reset: true
     * }
     * @param buffer Character buffer containing post request
     * @param length Length of the post request.
     * @todo Need to be able to schedule a system reset.
     * @return HAL::REST::RestServer::Error Currently, the handler doesn't return if reset is set to TRUE.
     */
    HAL::REST::RestServer::Error systemReset(char* buffer, size_t length);
  };    // namespace Endpoints
};      // namespace REST
#endif  // __REST_SERVER_ENDPOINTS_HPP