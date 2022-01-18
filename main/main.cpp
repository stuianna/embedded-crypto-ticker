#include <driver/gpio.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <lvgl.h>
#include <lvgl_helpers.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hal/lvgl/lvgl_driver.hpp>

static void setupGUI();

static lv_obj_t* hello_world_label;
static lv_obj_t* count_label;

extern "C" void app_main() {
  HAL::LVGL()->init();
  HAL::LVGL()->aquireMutex(0);
  setupGUI();
  HAL::LVGL()->releaseMutex();

  int32_t count = 0;
  char count_str[11] = {0};
  while(1) {
    snprintf(count_str, 11, "%d", count++);
    HAL::LVGL()->aquireMutex(0);
    lv_label_set_text(count_label, count_str);
    HAL::LVGL()->releaseMutex();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

static void setupGUI() {
  hello_world_label = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_text(hello_world_label, "Hello world!");
  lv_obj_align(hello_world_label, NULL, LV_ALIGN_CENTER, 0, 0);

  count_label = lv_label_create(lv_scr_act(), NULL);
  lv_obj_align(count_label, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);
}