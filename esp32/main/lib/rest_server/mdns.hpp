#ifndef __REST_SERVER_MDNS_HPP
#define __REST_SERVER_MDNS_HPP

#include <cstddef>
#include <cstdint>

namespace HAL {
  /**
   * @brief Namespace for REST HTTP server.
   */
  namespace REST {
    class MDNSService {
     public:
      /**
       * @brief Retrieve singleton instance for the service.
       * @return MDNSService* Singleton instance
       */
      static MDNSService* instance();

      /**
       * @brief Initialise the MDNS service
       * @param hostname The hostname to use. Results to the IP address being resolved to hostname.local
       * @param description The description for the hostname.
       */
      void initialise(const char* hostname, const char* description);

     private:
      // Disabled copy and move constructors and operators.
      MDNSService(const MDNSService&) = delete;
      MDNSService(MDNSService&&) = delete;
      MDNSService& operator=(const MDNSService&) = delete;
      MDNSService& operator=(MDNSService&&) = delete;

     private:
      MDNSService();
    };
    constexpr auto MDNS = MDNSService::instance;
  };    // namespace REST
};      // namespace HAL
#endif  // __REST_SERVER_MDNS_HPP
