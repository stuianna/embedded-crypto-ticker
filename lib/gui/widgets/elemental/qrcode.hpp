#ifndef __GUI_WIDGETS_QRCODE_HPP
#define __GUI_WIDGETS_QRCODE_HPP

#include <lvgl.h>

#include <array>

#include "../../lv_lib_qrcode/lv_qrcode.h"
#include "../../themes/theme_factory.hpp"
#include "../widget.hpp"

namespace GUI {
  namespace Widgets {
    template<size_t size>
    class QRCode : public Widget {
     public:
      explicit QRCode(lv_obj_t* parent) :
        Widget(parent),
        _width(0),
        _height(0),
        _qr(lv_qrcode_create(_rootObject, size, currentTheme()->colorBackground(), currentTheme()->colorTextNormal())) {
        lv_obj_set_style_local_border_color(_rootObject, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, currentTheme()->colorBackground());
        lv_obj_set_style_local_bg_color(_rootObject, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, currentTheme()->colorBackground());
      }

      void update() override {
        if(!_width || !_height) {
          return;
        }
        lv_obj_align(_qr, NULL, LV_ALIGN_CENTER, 0, 0);
      };

      void setAlignment(const lv_align_t alignment, size_t xOffset, size_t yOffset) {
        lv_obj_align(_qr, NULL, alignment, xOffset, yOffset);
      }

      void data(const char* data) { lv_qrcode_update(_qr, data, strlen(data)); }

     protected:
      size_t _width;
      size_t _height;
      lv_obj_t* _qr;
      lv_style_t _style;
    };
  };  // namespace Widgets
};    // namespace GUI

#endif  // __GUI_WIDGETS_QRCODE_HPP