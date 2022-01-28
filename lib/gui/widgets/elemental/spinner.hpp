#ifndef __GUI_WIDGETS_SPINNER_HPP
#define __GUI_WIDGETS_SPINNER_HPP

#include <lvgl.h>

#include <array>

#include "../../lv_lib_qrcode/lv_qrcode.h"
#include "../../themes/theme_factory.hpp"
#include "../widget.hpp"

namespace GUI {
  namespace Widgets {
    class Spinner : public Widget {
     public:
      explicit Spinner(lv_obj_t* parent) :
        Widget(parent), _spinner(lv_spinner_create(_rootObject, NULL)), _color(currentTheme()->colorTextNormal()) {
        lv_obj_set_style_local_border_color(_rootObject, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, currentTheme()->colorBackground());
        lv_obj_set_style_local_bg_color(_rootObject, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, currentTheme()->colorBackground());
        lv_obj_set_style_local_pad_inner(_rootObject, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

        lv_style_init(&_styleBack);
        lv_style_set_line_color(&_styleBack, LV_STATE_DEFAULT, currentTheme()->colorBackground());
        lv_style_set_line_width(&_styleBack, LV_STATE_DEFAULT, 8);

        lv_style_init(&_styleSpinner);
        lv_style_set_line_color(&_styleSpinner, LV_STATE_DEFAULT, _color);
        lv_style_set_line_width(&_styleSpinner, LV_STATE_DEFAULT, 8);

        lv_obj_add_style(_spinner, LV_SPINNER_PART_BG, &_styleBack);
        lv_obj_add_style(_spinner, LV_SPINNER_PART_INDIC, &_styleSpinner);
      }

      void update() override {
        // lv_obj_align(_spinner, NULL, LV_ALIGN_CENTER, 0, 0);
        // lv_obj_set_size(_spinner, _width, _height);
        const size_t pad = 10;
        size_t width = lv_obj_get_width(_rootObject);
        size_t height = lv_obj_get_height(_rootObject);
        width = width < pad ? width : width - pad;
        height = height < pad ? height : height - pad;
        size_t dimension = width > height ? height : width;

        lv_obj_set_size(_spinner, dimension, dimension);
        lv_obj_align(_spinner, _rootObject, LV_ALIGN_CENTER, 0, 0);
        lv_style_set_line_color(&_styleSpinner, LV_STATE_DEFAULT, _color);
      };

      void colour(const lv_color_t color) {
        _color = color;
        lv_style_set_line_color(&_styleSpinner, LV_STATE_DEFAULT, _color);
      }

     protected:
      lv_obj_t* _spinner;
      lv_style_t _styleBack;
      lv_style_t _styleSpinner;
      lv_color_t _color;
    };
  };  // namespace Widgets
};    // namespace GUI

#endif  // __GUI_WIDGETS_SPINNER_HPP