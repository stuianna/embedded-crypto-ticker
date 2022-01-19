#ifndef __GUI_FONTS_FONTS_HPP
#define __GUI_FONTS_FONTS_HPP

#include <lvgl.h>

namespace GUI {
  namespace Fonts {
    enum class Family {
      MONTSERRAT,
    };
    const lv_font_t* get(const Family family, const size_t size) {
      switch(family) {
        default:
        case Family::MONTSERRAT:
          if(size < 16) {
            return &lv_font_montserrat_14;
          }
          else if(size < 18) {
            return &lv_font_montserrat_16;
          }
          else if(size < 20) {
            return &lv_font_montserrat_18;
          }
          else if(size < 22) {
            return &lv_font_montserrat_20;
          }
          else if(size < 24) {
            return &lv_font_montserrat_22;
          }
          else if(size < 26) {
            return &lv_font_montserrat_24;
          }
          else if(size < 36) {
            return &lv_font_montserrat_30;
          }
          else {
            return &lv_font_montserrat_36;
          }
      }
    }
  }  // namespace Fonts
};   // namespace GUI

#endif  // __GUI_FONTS_FONTS_HPP