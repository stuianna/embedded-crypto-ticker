#ifndef __HAL_SNTP_HPP
#define __HAL_SNTP_HPP

#include <cstdint>

namespace HAL {
  class _SNTP {
   public:
    static _SNTP* instance();
    bool syncronise();
    uint32_t unixTime();

    _SNTP(const _SNTP&) = delete;
    _SNTP(_SNTP&&) = delete;

    _SNTP& operator=(const _SNTP&) = delete;
    _SNTP& operator=(_SNTP&&) = delete;

   private:
    _SNTP();
    void _initialise_sntp();
    void _obtain_time();
  };
  constexpr auto SNTP = _SNTP::instance;
};  // namespace HAL

#endif  // __HAL_SNTP_HPP