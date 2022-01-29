#ifndef __DATA_SOURCES_SNTP_HPP
#define __DATA_SOURCES_SNTP_HPP

#include <lvgl.h>

namespace Crypto {
  namespace DataSources {
    class SNTP {
     public:
      static SNTP* instance();
      void syncronise();
      uint32_t unixTime();

      SNTP(const SNTP&) = delete;
      SNTP(SNTP&&) = delete;

      SNTP& operator=(const SNTP&) = delete;
      SNTP& operator=(SNTP&&) = delete;

     private:
      SNTP();
      void _initialise_sntp();
      void _obtain_time();
    };
  };  // namespace DataSources
  constexpr auto SNTP = DataSources::SNTP::instance;
};  // namespace Crypto

#endif  // __DATA_SOURCES_SNTP_HPP