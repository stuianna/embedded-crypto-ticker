#include "loading.hpp"

#include "../../hal/driver.hpp"

using namespace GUI::Views;
using namespace GUI::HAL;

Loading* Loading::instance() {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  static Loading _instance;
  LVGL()->releaseMutex();
  return &_instance;
}

Loading::~Loading() {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  lv_obj_clean(lv_scr_act());
  LVGL()->releaseMutex();
}

Loading::Loading() :
  _screen(lv_obj_create(NULL, NULL)),
  _empty(lv_obj_create(NULL, NULL)),
  _container(lv_cont_create(_screen, NULL)),
  _spinner(_container),
  _status(_container),
  _details(_container) {
  auto width = lv_obj_get_width(lv_scr_act());
  auto height = lv_obj_get_height(lv_scr_act());

  lv_obj_set_width(_screen, width);
  lv_obj_set_height(_screen, height);
  lv_obj_set_width(_empty, width);
  lv_obj_set_height(_empty, height);
  lv_obj_set_width(_container, width);
  lv_obj_set_height(_container, height);


  lv_cont_set_layout(_container, LV_LAYOUT_COLUMN_MID);

  lv_style_init(&_innerContainerStyle);
  lv_style_set_pad_inner(&_innerContainerStyle, LV_STATE_DEFAULT, 5);
  lv_style_set_margin_all(&_innerContainerStyle, LV_STATE_DEFAULT, 0);
  lv_style_set_border_side(&_innerContainerStyle, LV_STATE_DEFAULT, LV_BORDER_SIDE_NONE);
  lv_style_set_bg_color(&_innerContainerStyle, LV_STATE_DEFAULT, currentTheme()->colorBackground());

  lv_obj_add_style(_container, LV_CONT_PART_MAIN, &_innerContainerStyle);
  lv_obj_add_style(_empty, LV_CONT_PART_MAIN, &_innerContainerStyle);
  lv_obj_add_style(_screen, LV_CONT_PART_MAIN, &_innerContainerStyle);
  _spinner.setHeight(height / 2.0);
  _spinner.setWidth(width / 2.0);
  _spinner.colour(currentTheme()->colorTextNormal());
  _status.setWidth(width);
  _status.setHeight(16);
  _details.setWidth(width);
  _details.setHeight(16);
}

void Loading::show() {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  lv_scr_load(_screen);
  LVGL()->releaseMutex();
};

void Loading::hide() {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  lv_scr_load(_empty);
  LVGL()->releaseMutex();
};

void Loading::status(const char* text, Widgets::Severity level) {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  _status.setText(text);
  _status.setColor(level);
  LVGL()->releaseMutex();
}

void Loading::details(const char* text, Widgets::Severity level) {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  _details.setText(text);
  _details.setColor(level);
  LVGL()->releaseMutex();
}