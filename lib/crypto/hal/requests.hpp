#ifndef __CRYPTO_HAL_REQUESTS_HPP
#define __CRYPTO_HAL_REQUESTS_HPP

#include <cstddef>
#include <cstdint>

namespace Crypto {
  namespace HAL {
    class Requests {
     public:
      struct GetRequest {
        const char* host;
        const char* path;
        char* data;
        size_t length;
        size_t timeout;
      };

      static Requests* instance() { return &_instance; }
      void init();
      size_t get(GetRequest& request);

      // Disable copy and move constructors and operators.
      Requests(const Requests&) = delete;
      Requests(Requests&&) = delete;

      Requests& operator=(const Requests&) = delete;
      Requests& operator=(Requests&&) = delete;

     private:
      Requests();
      static Requests _instance;
      bool _status(size_t& httpStatusCode, size_t timeout);
      bool _initialised;
    };
    constexpr auto Request = Requests::instance;
  }  // namespace HAL
};   // namespace Crypto

#endif  // __CRYPTO_HAL_REQUESTS_HPP