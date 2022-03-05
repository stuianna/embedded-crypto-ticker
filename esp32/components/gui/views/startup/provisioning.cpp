#include "provisioning.hpp"

#include "../../hal/driver.hpp"
#include "../../lv_lib_qrcode/lv_qrcode.h"

using namespace GUI::Views;
using namespace GUI::HAL;

Provisioning* Provisioning::instance() {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  static Provisioning _instance;
  LVGL()->releaseMutex();
  return &_instance;
}

Provisioning::~Provisioning() {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  lv_obj_clean(lv_scr_act());
  LVGL()->releaseMutex();
}

Provisioning::Provisioning() :
  _screen(lv_obj_create(NULL, NULL)), _empty(lv_obj_create(NULL, NULL)), _container(lv_cont_create(_screen, NULL)), _qr(_container) {
  auto width = lv_obj_get_width(lv_scr_act());
  auto height = lv_obj_get_height(lv_scr_act());

  lv_obj_set_width(_screen, width);
  lv_obj_set_height(_screen, height);
  lv_obj_set_width(_empty, width);
  lv_obj_set_height(_empty, height);
  lv_obj_set_width(_container, width);
  lv_obj_set_height(_container, height);


  lv_cont_set_layout(_container, LV_LAYOUT_PRETTY_MID);

  lv_style_init(&_innerContainerStyle);
  lv_style_set_pad_all(&_innerContainerStyle, LV_STATE_DEFAULT, 0);
  lv_style_set_margin_all(&_innerContainerStyle, LV_STATE_DEFAULT, 0);
  lv_style_set_border_side(&_innerContainerStyle, LV_STATE_DEFAULT, LV_BORDER_SIDE_NONE);
  lv_style_set_bg_color(&_innerContainerStyle, LV_STATE_DEFAULT, currentTheme()->colorBackground());

  lv_obj_add_style(_container, LV_CONT_PART_MAIN, &_innerContainerStyle);
  lv_obj_add_style(_empty, LV_CONT_PART_MAIN, &_innerContainerStyle);
  lv_obj_add_style(_screen, LV_CONT_PART_MAIN, &_innerContainerStyle);
  _qr.setHeight(height);
  _qr.setWidth(width);
  _qr.setAlignment(LV_ALIGN_CENTER, 0, 0);
}

void Provisioning::show() {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  lv_scr_load(_screen);
  LVGL()->releaseMutex();
};

void Provisioning::hide() {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  lv_scr_load(_empty);
  LVGL()->releaseMutex();
};

void Provisioning::setQR(const char* data) {
  LVGL()->aquireMutex(LVL_MUTEX_AQUISITION_MSEC);
  _qr.data(data);
  LVGL()->releaseMutex();
}