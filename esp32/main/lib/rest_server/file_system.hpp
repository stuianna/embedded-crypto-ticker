#ifndef __REST_SERVER_FILE_SYSTEM_HPP
#define __REST_SERVER_FILE_SYSTEM_HPP

#include <cstddef>
#include <cstdint>

namespace HAL {
  /**
   * @brief Namespace for REST HTTP server.
   */
  namespace REST {
    class _FileSystem {
     public:
      /**
       * @brief Retrieve singleton instance for the file system.
       * @return _FileSystem* Singleton instance
       */
      static _FileSystem* instance();

      /**
       * @brief Initialise the file system
       * @param mountPoint The mount point for the file system.
       */
      void initialise(const char* mountPoint);

     private:
      // Disabled copy and move constructors and operators.
      _FileSystem(const _FileSystem&) = delete;
      _FileSystem(_FileSystem&&) = delete;
      _FileSystem& operator=(const _FileSystem&) = delete;
      _FileSystem& operator=(_FileSystem&&) = delete;

     private:
      _FileSystem();
    };
    constexpr auto FileSystem = _FileSystem::instance;
  };    // namespace REST
};      // namespace HAL
#endif  // __REST_SERVER_FILE_SYSTEM_HPP
