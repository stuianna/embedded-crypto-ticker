#ifndef __GUI_WIDGETS_CHART_HPP
#define __GUI_WIDGETS_CHART_HPP

#include <lvgl.h>

#include <array>

#include "../../themes/theme_factory.hpp"
#include "../widget.hpp"

namespace GUI {
  namespace Widgets {
    class Chart : public Widget {
     public:
      explicit Chart(lv_obj_t* parent) : Widget(parent) {
        _chart = lv_chart_create(_rootObject, NULL);
        _data = lv_chart_add_series(_chart, currentTheme()->colorTextShowcase());
        lv_chart_set_type(_chart, LV_CHART_TYPE_LINE);

        // Adds faded effect
        lv_obj_set_style_local_bg_opa(_chart, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, LV_OPA_50); /*Max. opa.*/
        lv_obj_set_style_local_bg_grad_dir(_chart, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, LV_GRAD_DIR_VER);
        lv_obj_set_style_local_bg_main_stop(_chart, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 130); /*Max opa on the top*/
        lv_obj_set_style_local_bg_grad_stop(_chart, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 0);   /*Transparent on the bottom*/
        lv_obj_set_style_local_bg_opa(_chart, LV_CHART_PART_BG, LV_STATE_DEFAULT, LV_OPA_100);    /*Transparent on the bottom*/

        // Set the line width of the background seris to 0 to hide the grid.
        lv_obj_set_style_local_line_width(_chart, LV_CHART_PART_SERIES_BG, LV_STATE_DEFAULT, 0);

        // Remove background padding
        lv_obj_set_style_local_border_width(_chart, LV_CHART_PART_BG, LV_STATE_DEFAULT, 0);
        lv_obj_set_style_local_pad_left(_chart, LV_CHART_PART_BG, LV_STATE_DEFAULT, 0);
        lv_obj_set_style_local_pad_right(_chart, LV_CHART_PART_BG, LV_STATE_DEFAULT, 0);
        lv_obj_set_style_local_bg_color(_chart, LV_CHART_PART_BG, LV_STATE_DEFAULT, currentTheme()->colorBackground());

        // Removes the points on the lines
        lv_obj_set_style_local_size(_chart, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 0);
        lv_obj_set_style_local_line_width(_chart, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 2);

        lv_obj_set_style_local_bg_color(_rootObject, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, currentTheme()->colorBackground());
        lv_obj_set_style_local_border_width(_rootObject, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 0);

        _draw();
      }
      void setPointCount(const size_t count) { lv_chart_set_point_count(_chart, count); }
      void setRange(const float min, const float max) { lv_chart_set_range(_chart, min, max); }
      void update() override { _draw(); };
      void plot(const float value) {
        lv_chart_set_next(_chart, _data, value);
        lv_chart_refresh(_chart);
      }

      void clear() {
        lv_chart_clear_series(_chart, _data);
        lv_chart_refresh(_chart);
      }

     protected:
      void _draw() {
        lv_obj_set_width(_chart, getWidth());
        lv_obj_set_height(_chart, getHeight());
      }
      lv_obj_t* _chart;
      lv_chart_series_t* _data;
    };
  };  // namespace Widgets
};    // namespace GUI


#endif  // __GUI_WIDGETS_LABEL_HPP