#ifndef __GUI_THEME_DARK_HPP
#define __GUI_THEME_DARK_HPP

#include <lvgl.h>

#include "theme.hpp"

namespace GUI {
  class ThemeDark : public Theme {
   public:
    lv_color_t colorBackground() const override { return LV_COLOR_MAKE(0x00, 0x00, 0x00); }
    lv_color_t colorTextNormal() const override { return LV_COLOR_MAKE(0xff, 0xff, 0xff); }
    lv_color_t colorTextShowcase() const override { return LV_COLOR_MAKE(0x83, 0x9c, 0xf7); }
    lv_color_t colorTextGood() const override { return LV_COLOR_MAKE(0x06, 0xd6, 0x0d); }
    lv_color_t colorTextBad() const override { return LV_COLOR_MAKE(0xff, 0x00, 0x00); }
    const lv_font_t* font(size_t size) const override { return Fonts::get(Fonts::Family::MONTSERRAT, size); };
  };
};  // namespace GUI

#endif  // __GUI_THEME_DARK_HPP