#include <esp_log.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gui/hal/driver.hpp>
#include <gui/views/ticker/legacy.hpp>


LV_IMG_DECLARE(btc_icon_60);

int32_t ranint(int32_t min, int32_t max) {
  uint32_t n = esp_random();
  float scaled = (max - min) * (n / float(0xFFFFFFFF));
  return scaled + min;
}

extern "C" void app_main() {
  GUI::HAL::LVGL()->init();
  auto view = GUI::Views::Legacy();
  GUI::HAL::LVGL()->aquireMutex(100);
  view.setPlotRange(0, 100);
  view.setCurrencySymbol('$');
  view.setName("Bitcoin");
  view.setIcon(&btc_icon_60);
  view.show();
  GUI::HAL::LVGL()->releaseMutex();

  while(1) {
    GUI::HAL::LVGL()->aquireMutex(200);
    view.plotValue(ranint(0, 100));
    view.setDailyDelta(ranint(-10000, 10000) / 100.);
    view.setCurrentQuote(ranint(0, 1000000) / 100.);
    GUI::HAL::LVGL()->releaseMutex();
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}