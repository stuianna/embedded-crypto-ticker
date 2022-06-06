#ifndef __HAL_NVS_HPP
#define __HAL_NVS_HPP

#include <cstddef>
#include <cstdint>

namespace HAL {
  /**
   * @brief Singleton object for interacting with non-volatile storage (NVS).
   */
  class _NVS {
   public:
    /**
     * @brief Possible error conditions for NVS.
     */
    enum class Error {
      NONE  ///< No error occurred.
    };

    /**
     * @brief Fetch the singleton instance.
     * @return _NVS* Pointer to singleton object.
     */
    static _NVS* instance();

    /**
     * @brief Erase all segments of the NVS.
     * @return Error Any error which occurred when erasing NVS.
     */
    Error eraseAll();

    /**
     * @brief Initialise the NVS.
     * @details NVS is automatically initialised when the object is first called through instance.
     * @details This call exists as a convenience. Repeat call are ignored internally.
     * @return Error Any error which occurred when initialising NVS.
     */
    Error initialise();

    // Disable copy and move constructors and operators.
    _NVS(const _NVS&) = delete;
    _NVS(_NVS&&) = delete;
    _NVS& operator=(const _NVS&) = delete;
    _NVS& operator=(_NVS&&) = delete;

   private:
    _NVS();
    static _NVS _instance;
    bool _initialised;
  };
  constexpr auto NVS = _NVS::instance;
}  // namespace HAL

#endif  // __HAL_NVS_HPP