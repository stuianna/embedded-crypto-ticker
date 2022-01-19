#ifndef __GUI_THEME_THEME_HPP
#define __GUI_THEME_THEME_HPP

#include <lvgl.h>

#include "../fonts/fonts.hpp"

namespace GUI {
  class Theme {
   public:
    ~Theme() = default;
    virtual lv_color_t colorBackground() const = 0;
    virtual lv_color_t colorTextNormal() const = 0;
    virtual lv_color_t colorTextShowcase() const = 0;
    virtual lv_color_t colorTextGood() const = 0;
    virtual lv_color_t colorTextBad() const = 0;
    virtual const lv_font_t* font(size_t size) const = 0;
  };
};  // namespace GUI

#endif  // __GUI_THEME_THEME_HPP