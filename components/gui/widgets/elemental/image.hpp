#ifndef __GUI_WIDGETS_IMAGE_HPP
#define __GUI_WIDGETS_IMAGE_HPP

#include <lvgl.h>

#include <array>

#include "../../themes/theme_factory.hpp"
#include "../widget.hpp"

namespace GUI {
  namespace Widgets {
    class Image : public Widget {
     public:
      explicit Image(lv_obj_t* parent) : Widget(parent), _width(0), _height(0) {
        _image = lv_img_create(_rootObject, NULL);
        lv_obj_set_style_local_border_width(_image, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);
        lv_obj_set_style_local_border_color(_image, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, currentTheme()->colorBackground());
        lv_obj_set_style_local_border_color(_rootObject, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, currentTheme()->colorBackground());
        lv_obj_set_style_local_bg_color(_rootObject, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, currentTheme()->colorBackground());
      }

      void update() override {
        if(!_width || !_height) {
          return;
        }

        float percentOfWidth = getWidth() / static_cast<float>(_width);
        float percentOfHeight = getHeight() / static_cast<float>(_height);
        float smallestDimension = percentOfWidth <= percentOfHeight ? percentOfWidth : percentOfHeight;
        lv_img_set_zoom(_image, 250 * smallestDimension);
        lv_obj_align(_image, NULL, LV_ALIGN_CENTER, 0, 0);
      };

      void setSource(const lv_img_dsc_t* src) {
        lv_img_set_src(_image, src);
        _width = src->header.w;
        _height = src->header.h;
        update();
      }

     protected:
      lv_obj_t* _image;
      lv_style_t _style;
      size_t _width;
      size_t _height;
    };
  };  // namespace Widgets
};    // namespace GUI

#endif  // __GUI_WIDGETS_IMAGE_HPP