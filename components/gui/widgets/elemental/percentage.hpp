#ifndef __GUI_WIDGETS_PERCENTAGE_HPP
#define __GUI_WIDGETS_PERCENTAGE_HPP

#include <lvgl.h>

#include <array>

#include "../../themes/theme_factory.hpp"
#include "../../tools/cpl.hpp"
#include "../../tools/format_percentage.hpp"
#include "label.hpp"

namespace GUI {
  namespace Widgets {
    class Percentage : public Label<8> {
     public:
      explicit Percentage(lv_obj_t* parent) : Label<8>(parent), _styled(true) {}
      void setValue(const float percentage) {
        std::array<char, 8> buffer;
        buffer.fill(' ');
        Tools::formatPercentage(buffer.data(), buffer.size(), percentage, 1000);
        setText(buffer.data());
        _styleText(percentage);
      };
      void styleValue(bool enable) { _styled = enable; }

     private:
      bool _styled;
      void _styleText(const float value) {
        if(!_styled) {
          return;
        }
        if(value >= 0.00f) {
          setColor(currentTheme()->colorTextGood());
        }
        else {
          setColor(currentTheme()->colorTextBad());
        }
      }
    };
  };  // namespace Widgets
};    // namespace GUI


#endif  // __GUI_WIDGETS_PERCENTAGE_HPP