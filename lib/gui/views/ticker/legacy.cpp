#include "legacy.hpp"

#include "../../hal/driver.hpp"

using namespace GUI::Views;
using namespace GUI::HAL;

Legacy* Legacy::instance() {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  static Legacy _instance;
  LVGL()->releaseMutex();
  return &_instance;
}

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
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  lv_scr_load_anim(_screen, LV_SCR_LOAD_ANIM_FADE_ON, 200, 0, false);
  LVGL()->releaseMutex();
};

void Legacy::hide() {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  lv_scr_load(_empty);
  LVGL()->releaseMutex();
};

void Legacy::setCurrentQuote(const float quote) {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  _banner.setQuote(quote);
  LVGL()->releaseMutex();
}

void Legacy::clearCurrentQuote() {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  _banner.clearQuote();
  LVGL()->releaseMutex();
}

void Legacy::setDailyDelta(const float percent) {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  _banner.setPercentChange(percent);
  LVGL()->releaseMutex();
}

void Legacy::clearDailyDelta() {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  _banner.clearPercentageChange();
  LVGL()->releaseMutex();
}

void Legacy::setName(const char* text) {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  _banner.setLabel(text);
  LVGL()->releaseMutex();
}

void Legacy::clearName() {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  _banner.clearLabel();
  LVGL()->releaseMutex();
}

void Legacy::setCurrencySymbol(const char symbol) {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  _banner.setCurrencySymbol(symbol);
  LVGL()->releaseMutex();
}

void Legacy::clearCurrencySymbol() {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  _banner.clearCurrencySymbol();
  LVGL()->releaseMutex();
}

void Legacy::setIcon(const lv_img_dsc_t* src) {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  _banner.setIcon(src);
  LVGL()->releaseMutex();
}

void Legacy::clearIcon() {
}

void Legacy::plotValue(const float value) {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  _chart.plot(value);
  LVGL()->releaseMutex();
}

void Legacy::clearPlot() {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  _chart.clear();
  LVGL()->releaseMutex();
}

void Legacy::setPlotRange(const float min, const float max) {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  _chart.setRange(min, max);
  LVGL()->releaseMutex();
}

void Legacy::setPlotPointCount(size_t count) {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  _chart.setPointCount(count);
  LVGL()->releaseMutex();
}