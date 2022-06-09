#ifndef __HAL_SYSTEM_HPP
#define __HAL_SYSTEM_HPP

#include <cstddef>
#include <cstdint>

namespace HAL {
  namespace System {
    namespace Heap {
      size_t minimumFree();
      size_t currentFree();
      size_t largestFreeBlock();
    };  // namespace Heap

    void reset();
    const char* resetReason();
    int64_t uptime();
  };  // namespace System
}  // namespace HAL

#endif  // __HAL_SYSTEM_HPP