#ifndef __GUI_TOOLS_CPL_HPP
#define __GUI_TOOLS_CPL_HPP

#include <cstddef>
#include <cstdio>

namespace GUI {
  namespace Tools {
    static size_t fontSizeFromWidth(const size_t width, const size_t characterCount) { return 1.9 * (width / characterCount); }
    static size_t fontSizeFromHeight(const size_t height) { return 0.9 * height; }
    static size_t optimalFontSize(const size_t width, const size_t height, const size_t characterCount) {
      auto fromWidth = fontSizeFromWidth(width, characterCount);
      auto fromHeight = fontSizeFromHeight(height);
      if(fromHeight <= fromWidth) {
        return fromHeight;
      }
      return fromWidth;
    }
  };  // namespace Tools
};    // namespace GUI

#endif  // __GUI_TOOLS_CPL_HPP
