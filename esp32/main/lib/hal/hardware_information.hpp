#ifndef __HAL_HARDWARE_INFORMATION_HPP
#define __HAL_HARDWARE_INFORMATION_HPP

#include <cstddef>
#include <cstdint>

namespace HAL {
  /**
   * @brief Accessors for read-only hardware and software information.
   */
  class _HardwareInformation {
   public:
    /**
     * @brief Get the singleton instance for the class.
     * @return _HardwareInformation*
     */
    static _HardwareInformation* instance();
    /**
     * @brief Get the SDK version used for hardware access.
     * @details For ESP32, this evaluates to the ESP IDF version used.
     * @return const char* SDK version.
     */
    const char* sdkVersion() const;
    /**
     * @brief Get the MAC address for the WIFI station.
     * @return const char* MAC address, formatted as a 48-bit hexidecimal (XX:XX:XX:XX:XX:XX)
     */
    const char* macAddress() const;
    /**
     * @brief Get a description of the chip, or MCU used.
     * @return const char* Chip description.
     */
    const char* chip() const;
    /**
     * @brief Get the chip revision number.
     * @return size_t Revision number.
     */
    size_t chipRevision() const;
    /**
     * @brief Get the size of the flash memory available, in bytes.
     * @details This returns the size of the complete memory bank. The flash memory assigned to the application will almost always be
     * smaller.
     * @return size_t Flash size in bytes.
     */
    size_t flashSize() const;
    /**
     * @brief Get the number of CPU cores available.
     * @return size_t CPU core count.
     */
    size_t cores() const;
    /**
     * @brief Get the application version. Usually, this is equal to the output of git-describe.
     * @return const char* Application version string.
     */
    const char* appVersion() const;
    /**
     * @brief Get the application compile time.
     * @todo Add check for reproducible builds. In which case, undefined should be returned.
     * @return const char* Compile time
     */
    const char* compileTime() const;
    /**
     * @brief Get the application compile date.
     * @todo Add check for reproducible builds. In which case, undefined should be returned.
     * @return const char* Compile date.
     */
    const char* compileDate() const;
    /**
     * @brief Get the application binary's SHA256 sum.
     * @return const char* Application SHA256 as a 64-bit hexidecimal string.
     */
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
  /**
   * @brief Convenience definition for accessing the _HardwareInformation singleton instance.
   */
  constexpr auto HardwareInformation = _HardwareInformation::instance;
}  // namespace HAL

#endif  // __HAL_HARDWARE_INFORMATION_HPP