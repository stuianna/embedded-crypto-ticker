#ifndef __REST_SERVER_SERVER_HPP
#define __REST_SERVER_SERVER_HPP

#include <cstddef>
#include <cstdint>

namespace HAL {
  /**
   * @brief Namespace for REST HTTP server.
   */
  namespace REST {
    class RestServer {
     public:
      /**
       * @brief Defined REST methods which can be processed by the server.
       */
      enum class RestMethod {
        GET,  ///< Rest GET request.
        POST  ///< Rest POST request
      };

      /**
       * @brief HTTP responses which the server can use to respond to the client.
       */
      enum class Error {
        HTTP_200,  ///< No Error
        HTTP_404,  ///< Not found
        HTTP_500   ///< Internal server error
      };

      /**
       * @brief Convenience definitions for URI handlers called when an endpoint is triggered.
       * @details The input buffer, along with the error code is used to populate the server response. An error of HTTP_200 sends either the
       * GET request payload or 'successful' POST request message. Non-HTTP_200 error codes send any error message contained in the buffer
       * when the handler returns.
       * @param buffer The buffer containing JSON payload.
       * @param length The length of the payload.
       * @return Error HTTP status for the request.
       */
      typedef Error (*URIHandler)(char* buffer, size_t length);

      /**
       * @brief An endpoint description which can be registered by the server.
       */
      struct Endpoint {
        const char* uri;     // The URI for the endpoint.
        RestMethod method;   // The method which the endpoint uses
        URIHandler handler;  // The handler called when the endpoint is triggered.
      };

      /**
       * @brief Retrieve the singleton instance of the server.
       * @return RestServer* Singleton instance
       */
      static RestServer* instance();

      /**
       * @brief Start the rest server. Repeated calls are ignored.
       */
      void start();
      /**
       * @brief Stop the reset server. The server can only be restarted with a call to start.
       */
      void stop();

      /**
       * @brief
       * @brief Register an endpoint to be handled by the server.
       * @param endpoint Structure describing the endpoint.
       */
      void registerEndpoint(const Endpoint& endpoint);

      // Disabled copy and move constructors and operators.
      RestServer(const RestServer&) = delete;
      RestServer(RestServer&&) = delete;
      RestServer& operator=(const RestServer&) = delete;
      RestServer& operator=(RestServer&&) = delete;

     private:
      RestServer();
    };
    constexpr auto Server = RestServer::instance;
  };    // namespace REST
};      // namespace HAL
#endif  // __REST_SERVER_SERVER_HPP
