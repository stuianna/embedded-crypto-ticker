#ifndef __HAL_LVGL_DRIVER_HPP
#define __HAL_LVGL_DRIVER_HPP

#include <cstdint>

namespace HAL {
  class Driver {
   public:
    static Driver* instance() { return &_instance; }
    void init();
    bool aquireMutex(size_t timeoutMs);
    void releaseMutex();

    // Disable copy and move constructors and operators.
    Driver(const Driver&) = delete;
    Driver(Driver&&) = delete;

    Driver& operator=(const Driver&) = delete;
    Driver& operator=(Driver&&) = delete;

   private:
    Driver();
    static Driver _instance;
  };
  constexpr auto LVGL = Driver::instance;
}  // namespace HAL

#endif  // __HAL_LVGL_DRIVER_HPP