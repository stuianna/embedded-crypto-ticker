#ifndef __HAL_LVGL_DRIVER_HPP
#define __HAL_LVGL_DRIVER_HPP

namespace HAL {
  class LVGLDriver {
   public:
    static LVGLDriver* instance() { return &_instance; }
    void init();
    bool aquireMutex(size_t timeoutMs);
    void releaseMutex();

    // Disable copy and move constructors and operators.
    LVGLDriver(const LVGLDriver&) = delete;
    LVGLDriver(LVGLDriver&&) = delete;

    LVGLDriver& operator=(const LVGLDriver&) = delete;
    LVGLDriver& operator=(LVGLDriver&&) = delete;

   private:
    LVGLDriver();
    static LVGLDriver _instance;
  };
  constexpr auto LVGL = LVGLDriver::instance;
};  // namespace HAL

#endif  // __HAL_LVGL_DRIVER_HPP