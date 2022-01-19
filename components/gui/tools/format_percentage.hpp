#ifndef __GUI_TOOLS_FORMATE_PERCENTAGE_HPP
#define __GUI_TOOLS_FORMATE_PERCENTAGE_HPP

#include <cstddef>
#include <cstdio>

namespace GUI {
  namespace Tools {
    void formatPercentage(char* buffer, const size_t length, const float value, const float max) {
      int i;
      if(value > 100.f || value < -100.f) {
        i = snprintf(buffer, length, "%d%%", (int)value);
      }
      else {
        i = snprintf(buffer, length, "%.2f%%", value);
      }
      buffer[i] = 0;
    }
  };  // namespace Tools
};    // namespace GUI

#endif  // __GUI_TOOLS_FORMATE_PERCENTAGE_HPP
