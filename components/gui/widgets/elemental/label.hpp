#ifndef __GUI_WIDGETS_LABEL_HPP
#define __GUI_WIDGETS_LABEL_HPP

#include <lvgl.h>

#include <array>

#include "../../themes/theme_factory.hpp"
#include "../../tools/cpl.hpp"
#include "../../tools/format_price.hpp"
#include "../widget.hpp"

namespace GUI {
  namespace Widgets {
    template<size_t _length>
    class Label : public Widget {
     public:
      explicit Label(lv_obj_t* parent) : Widget(parent) {
        _label = lv_label_create(_rootObject, NULL);
        lv_label_set_text(_label, "");
        _draw();
        setColor(currentTheme()->colorTextNormal());
      }
      void update() override { _draw(); };
      void setText(const char* text) {
        lv_label_set_text_static(_label, text);
        lv_obj_align(_label, NULL, LV_ALIGN_CENTER, 0, 0);
      };
      void setColor(lv_color_t color) { lv_style_set_text_color(&_style, LV_STATE_DEFAULT, color); }

     protected:
      void _draw() {
        lv_style_init(&_style);
        auto fontSize = Tools::optimalFontSize(getWidth(), getHeight(), _length);
        lv_style_set_text_font(&_style, LV_STATE_DEFAULT, currentTheme()->font(fontSize));
        lv_obj_add_style(_label, LV_OBJ_PART_MAIN, &_style);
        lv_style_set_border_side(&_style, LV_STATE_DEFAULT, LV_BORDER_SIDE_NONE);
        lv_style_set_bg_color(&_style, LV_STATE_DEFAULT, currentTheme()->colorBackground());
        lv_obj_align(_label, NULL, LV_ALIGN_CENTER, 0, 0);
        containerStyle(&_style, LV_CONT_PART_MAIN);
      }
      lv_obj_t* _label;
      lv_style_t _style;
    };
  };  // namespace Widgets
};    // namespace GUI


#endif  // __GUI_WIDGETS_LABEL_HPP