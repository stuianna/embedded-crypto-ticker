#ifndef __HAL_SYSTEM_HPP
#define __HAL_SYSTEM_HPP

#include <cstddef>
#include <cstdint>

namespace HAL {
  /**
   * @brief Namespace for accessing miscellaneous system functions.
   */
  namespace System {
    /**
     * @brief Namespace for accessing information on system heap.
     */
    namespace Heap {
      /**
       * @brief Get the minimum ever available heap. (Heap high water mark.)
       * @return size_t Minimum heap size in bytes.
       */
      size_t minimumFree();
      /**
       * @brief Get the current available heap size, in bytes.
       * @warning The heap may be fragmented such that the number of free bytes is larger than the largest continuous segment.
       * @return size_t Currently available heap, in bytes.
       */
      size_t currentFree();
      /**
       * @brief Get the largest continuous free segment of the heap, in bytes.
       * @details This is the maximum value which could be allocated in a single block
       * @return size_t Largest free block, in bytes.
       */
      size_t largestFreeBlock();
    };  // namespace Heap
    /**
     * @brief Preform a system reset.
     */
    void reset();
    /**
     * @brief Get a short description of the last reset reason.
     * @return const char*
     */
    const char* resetReason();
    /**
     * @brief Get the system uptime, in milliseconds.
     * @return int64_t Time since reset, in milliseconds.
     */
    int64_t uptime();
  };  // namespace System
}  // namespace HAL

#endif  // __HAL_SYSTEM_HPP