#ifndef __GUI_WIDGETS_PRICE_HPP
#define __GUI_WIDGETS_PRICE_HPP

#include <lvgl.h>

#include <array>

#include "../../themes/theme_factory.hpp"
#include "../../tools/cpl.hpp"
#include "../../tools/format_price.hpp"
#include "label.hpp"

namespace GUI {
  namespace Widgets {
    class Price : public Label<10> {
     public:
      explicit Price(lv_obj_t* parent) : Label<10>(parent) { _symbol = ' '; }

      void setValue(const float price) {
        std::array<char, 10> buffer;
        buffer.fill(' ');
        Tools::formatPrice(buffer.data(), buffer.size(), price, 1000000, _symbol);
        setText(buffer.data());
        setColor(currentTheme()->colorTextNormal());
      };
      void setCurrencySymbol(const char symbol) { _symbol = symbol; }

     private:
      char _symbol;
    };
  };  // namespace Widgets
};    // namespace GUI


#endif  // __GUI_WIDGETS_PRICE_HPP