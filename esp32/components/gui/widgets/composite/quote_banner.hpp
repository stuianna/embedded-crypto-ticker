#ifndef __GUI_WIDGETS_COMPOSITE_QUOTE_BANNER_HPP
#define __GUI_WIDGETS_COMPOSITE_QUOTE_BANNER_HPP

#include <lvgl.h>

#include "../elemental/image.hpp"
#include "../elemental/label.hpp"
#include "../elemental/percentage.hpp"
#include "../elemental/price.hpp"
#include "../widget.hpp"

namespace GUI {
  namespace Widgets {
    class QuoteBanner : public Widget {
     public:
      explicit QuoteBanner(lv_obj_t* parent) :
        Widget(parent),
        _columnContainer(lv_cont_create(_rootObject, NULL)),
        _icon(_columnContainer),
        _rowContainer(lv_cont_create(_columnContainer, NULL)),
        _label(_rowContainer),
        _textContainer(lv_cont_create(_rowContainer, NULL)),
        _price(_textContainer),
        _priceDelta(_textContainer) {
        _draw();
      }

      void update() override { _draw(); }
      void setIcon(const lv_img_dsc_t* src) { _icon.setSource(src); }
      void setQuote(float currentPrice) { _price.setValue(currentPrice); }
      void clearQuote() { _price.setText(""); }
      void setPercentChange(float percent) { _priceDelta.setValue(percent); }
      void clearPercentageChange() { _priceDelta.setText(""); }
      void setLabel(const char* text) { _label.setText(text); }
      void clearLabel() { _label.setText(""); }
      void setCurrencySymbol(char symbol) { _price.setCurrencySymbol(symbol); }
      void clearCurrencySymbol() { _price.setCurrencySymbol(' '); }

     private:
      void _draw() {
        size_t iconWidth = getWidth() * 0.20;
        size_t infoWidth = getWidth() - iconWidth;
        size_t iconHeight = getHeight();
        size_t labelHeight = iconHeight * 0.45;
        size_t quoteHeight = getHeight() - labelHeight;
        size_t priceWidth = infoWidth * 0.7;
        size_t deltaWidth = infoWidth * 0.25;

        lv_cont_set_fit(_columnContainer, LV_FIT_PARENT);
        lv_cont_set_layout(_columnContainer, LV_LAYOUT_ROW_TOP);

        _icon.setWidth(iconWidth);
        _icon.setHeight(iconHeight);

        lv_cont_set_layout(_rowContainer, LV_LAYOUT_COLUMN_LEFT);
        lv_obj_set_width(_rowContainer, infoWidth);
        lv_obj_set_height(_rowContainer, iconHeight);

        _label.setWidth(infoWidth);
        _label.setHeight(labelHeight);
        _label.setColor(currentTheme()->colorTextShowcase());
        _label.setAlignment(LV_ALIGN_IN_BOTTOM_LEFT);

        lv_cont_set_layout(_textContainer, LV_LAYOUT_ROW_BOTTOM);
        lv_obj_set_width(_textContainer, infoWidth);
        lv_obj_set_height(_textContainer, quoteHeight);

        _price.setHeight(quoteHeight);
        _price.setWidth(priceWidth);
        _price.setAlignment(LV_ALIGN_IN_BOTTOM_LEFT);
        _priceDelta.setHeight(quoteHeight);
        _priceDelta.setWidth(deltaWidth);
        _priceDelta.setAlignment(LV_ALIGN_IN_BOTTOM_RIGHT);


        lv_style_init(&_innerContainerStyle);
        lv_style_set_pad_left(&_innerContainerStyle, LV_STATE_DEFAULT, 0);
        lv_style_set_pad_right(&_innerContainerStyle, LV_STATE_DEFAULT, 0);
        lv_style_set_pad_top(&_innerContainerStyle, LV_STATE_DEFAULT, 0);
        lv_style_set_pad_bottom(&_innerContainerStyle, LV_STATE_DEFAULT, 0);
        lv_style_set_pad_inner(&_innerContainerStyle, LV_STATE_DEFAULT, 0);
        lv_style_set_margin_all(&_innerContainerStyle, LV_STATE_DEFAULT, 0);
        lv_style_set_border_side(&_innerContainerStyle, LV_STATE_DEFAULT, LV_BORDER_SIDE_NONE);
        lv_style_set_bg_color(&_innerContainerStyle, LV_STATE_DEFAULT, currentTheme()->colorBackground());

        lv_obj_add_style(_rootObject, LV_CONT_PART_MAIN, &_innerContainerStyle);
        lv_obj_add_style(_columnContainer, LV_CONT_PART_MAIN, &_innerContainerStyle);
        lv_obj_add_style(_rowContainer, LV_CONT_PART_MAIN, &_innerContainerStyle);
        lv_obj_add_style(_textContainer, LV_CONT_PART_MAIN, &_innerContainerStyle);

        lv_obj_set_style_local_pad_inner(_columnContainer, LV_CHART_PART_BG, LV_STATE_DEFAULT, 4);
      };

      lv_obj_t* _columnContainer;
      Image _icon;
      lv_obj_t* _rowContainer;
      Label<10> _label;
      lv_obj_t* _textContainer;
      Price _price;
      Percentage _priceDelta;
      lv_style_t _innerContainerStyle;
    };
  };  // namespace Widgets
}  // namespace GUI


#endif  // __GUI_WIDGETS_COMPOSITE_QUOTE_BANNER_HPP