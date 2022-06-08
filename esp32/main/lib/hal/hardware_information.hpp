#ifndef __HAL_HARDWARE_INFORMATION_HPP
#define __HAL_HARDWARE_INFORMATION_HPP

#include <cstddef>
#include <cstdint>

namespace HAL {
  class _HardwareInformation {
   public:
    static _HardwareInformation* instance();
    const char* sdkVersion() const;
    const char* macAddress() const;
    const char* chip() const;
    size_t chipRevision() const;
    size_t flashSize() const;
    size_t cores() const;
    const char* appVersion() const;
    const char* compileTime() const;
    const char* compileDate() const;
    const char* binarySHA256() const;

   private:
    void update();
    _HardwareInformation();
    _HardwareInformation(const _HardwareInformation&) = delete;
    _HardwareInformation(_HardwareInformation&&) = delete;
    _HardwareInformation& operator=(const _HardwareInformation&) = delete;
    _HardwareInformation& operator=(_HardwareInformation&&) = delete;
    const char* parseESPModel(size_t model);

    const char* _sdkVersion;
    char _macAddress[18];
    const char* _chip;
    size_t _chipRevision;
    size_t _flashSize;
    size_t _cores;
    const char* _appVersion;
    const char* _compileTime;
    const char* _compileDate;
    char _binarySHA256[65];
  };
  constexpr auto HardwareInformation = _HardwareInformation::instance;
}  // namespace HAL

#endif  // __HAL_HARDWARE_INFORMATION_HPP