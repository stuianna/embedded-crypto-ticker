#ifndef __GUI_TOOLS_FORMAT_PRICE_HPP
#define __GUI_TOOLS_FORMAT_PRICE_HPP

#include <cstddef>
#include <cstdio>

namespace GUI {
  namespace Tools {
    static void formatPrice(char* buffer, const size_t length, const float value, float max, const char symbol) {
      int i;
      if(value < 0.99f) {
        i = snprintf(&buffer[1], 31, "%.4f", value);
        buffer[i + 1] = 0;
      }
      else if(value < 1000.0f) {
        i = snprintf(&buffer[1], 31, "%.2f", value);
        buffer[i + 1] = 0;
      }
      else if(value < max) {
        i = snprintf(&buffer[1], 31, "%d", int(value / 1000));
        buffer[i + 1] = ',';
        if(value < 10000) {
          i += snprintf(&buffer[2 + i], 31, "%06.2f", float(value - (int(value / 1000) * 1000)));
        }
        else {
          i += snprintf(&buffer[2 + i], 31, "%03d", int(value - (int(value / 1000) * 1000)));
        }
        buffer[i + 2] = 0;
      }
      else {
        return;
      }
      buffer[0] = symbol;
    }
  };  // namespace Tools
};    // namespace GUI

#endif  // __GUI_TOOLS_FORMATE_PRICE_HPP
