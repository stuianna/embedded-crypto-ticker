#ifndef __GUI_WIDGETS_WIDGET_HPP
#define __GUI_WIDGETS_WIDGET_HPP

#include <lvgl.h>

namespace GUI {
  namespace Widgets {
    enum class Severity {
      GOOD,
      BAD,
      NORMAL,
    };
    class Widget {
     public:
      void setWidth(const size_t width) {
        lv_obj_set_width(_rootObject, width);
        update();
      }
      size_t getWidth() const { return lv_obj_get_width(_rootObject); }
      void setHeight(const size_t height) {
        lv_obj_set_height(_rootObject, height);
        update();
      }
      size_t getHeight() const { return lv_obj_get_height(_rootObject); }
      void setFit(const lv_fit_t horizontal, const lv_fit_t vertical) { lv_cont_set_fit2(_rootObject, horizontal, vertical); }
      void setFit(const lv_fit_t fit) { lv_cont_set_fit(_rootObject, fit); }
      void setLayout(const lv_layout_t layout) { lv_cont_set_layout(_rootObject, layout); }
      void setAlignment(const lv_align_t alignment, size_t xOffset, size_t yOffset) {
        lv_obj_align(_rootObject, NULL, alignment, xOffset, yOffset);
      }
      virtual void update() = 0;

     protected:
      explicit Widget(lv_obj_t* parent) : _rootObject(lv_cont_create(parent, NULL)) {}
      void containerStyle(lv_style_t* style, uint8_t part) { lv_obj_add_style(_rootObject, part, style); }
      lv_color_t _lookupColor(Severity level) const {
        switch(level) {
          case Severity::BAD: return currentTheme()->colorTextBad();
          case Severity::GOOD: return currentTheme()->colorTextGood();
          default:
          case Severity::NORMAL: return currentTheme()->colorTextNormal();
        }
      }
      lv_obj_t* _rootObject;
    };
  };  // namespace Widgets
}  // namespace GUI


#endif  // __GUI_WIDGETS_WIDGET_HPP