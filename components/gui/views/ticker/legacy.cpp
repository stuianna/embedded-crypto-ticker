#include "legacy.hpp"

using namespace GUI::Views;

Legacy::Legacy() :
  _screen(lv_obj_create(NULL, NULL)),
  _empty(lv_obj_create(NULL, NULL)),
  _container(lv_cont_create(_screen, NULL)),
  _banner(_container),
  _chart(_container) {
  auto width = lv_obj_get_width(lv_scr_act());
  auto height = lv_obj_get_height(lv_scr_act());
  lv_obj_set_width(_screen, width);
  lv_obj_set_height(_screen, height);
  lv_obj_set_width(_empty, width);
  lv_obj_set_height(_empty, height);
  lv_cont_set_fit(_container, LV_FIT_PARENT);
  lv_cont_set_layout(_container, LV_LAYOUT_COLUMN_MID);


  lv_style_init(&_innerContainerStyle);
  lv_style_set_pad_left(&_innerContainerStyle, LV_STATE_DEFAULT, 0);
  lv_style_set_pad_right(&_innerContainerStyle, LV_STATE_DEFAULT, 0);
  lv_style_set_pad_top(&_innerContainerStyle, LV_STATE_DEFAULT, 0);
  lv_style_set_pad_bottom(&_innerContainerStyle, LV_STATE_DEFAULT, 0);
  lv_style_set_pad_inner(&_innerContainerStyle, LV_STATE_DEFAULT, 0);
  lv_style_set_margin_all(&_innerContainerStyle, LV_STATE_DEFAULT, 0);
  lv_style_set_border_side(&_innerContainerStyle, LV_STATE_DEFAULT, LV_BORDER_SIDE_NONE);
  lv_style_set_bg_color(&_innerContainerStyle, LV_STATE_DEFAULT, currentTheme()->colorBackground());

  lv_obj_add_style(_container, LV_CONT_PART_MAIN, &_innerContainerStyle);
  lv_obj_add_style(_empty, LV_CONT_PART_MAIN, &_innerContainerStyle);

  _chart.setWidth(width);
  _chart.setPointCount(120);
  _banner.setWidth(width);
  _banner.setHeight(height * 0.4);
  _chart.setHeight(height * 0.6);


  lv_obj_add_style(_screen, LV_CONT_PART_MAIN, &_innerContainerStyle);
}

void Legacy::show() {
  lv_scr_load(_screen);
};

void Legacy::hide() {
  lv_scr_load(_empty);
};

void Legacy::setCurrentQuote(const float quote) {
  _banner.setQuote(quote);
}

void Legacy::clearCurrentQuote() {
  _banner.clearQuote();
}

void Legacy::setDailyDelta(const float percent) {
  _banner.setPercentChange(percent);
}

void Legacy::clearDailyDelta() {
  _banner.clearPercentageChange();
}

void Legacy::setName(const char* text) {
  _banner.setLabel(text);
}

void Legacy::clearName() {
  _banner.clearLabel();
}

void Legacy::setCurrencySymbol(const char symbol) {
  _banner.setCurrencySymbol(symbol);
}

void Legacy::clearCurrencySymbol() {
  _banner.clearCurrencySymbol();
}

void Legacy::setIcon(const lv_img_dsc_t* src) {
  _banner.setIcon(src);
}

void Legacy::clearIcon() {
}

void Legacy::plotValue(const float value) {
  _chart.plot(value);
}

void Legacy::clearPlot() {
  _chart.clear();
}

void Legacy::setPlotRange(const float min, const float max) {
  _chart.setRange(min, max);
}

void Legacy::setPlotPointCount(size_t count) {
  _chart.setPointCount(count);
}