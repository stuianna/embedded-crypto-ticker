#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <lvgl.h>
#include <lvgl_helpers.h>

#include "lvgl_driver.hpp"

using namespace HAL;

#ifndef LV_TICK_PERIOD_MS
  #define LV_TICK_PERIOD_MS 1
#endif

#ifndef LV_UPDATE_INTERVAL
  #define LV_UPDATE_INTERVAL_MS 10
#endif

static void task_lv_tick(void* arg);
static void task_GUI(void* pvParameter);

static SemaphoreHandle_t xGuiSemaphore = NULL;

Driver Driver::_instance;

Driver::Driver() {
}

void Driver::init() {
  xTaskCreatePinnedToCore(task_GUI, "GUI", 4096 * 2, NULL, 6, NULL, 1);

  while((xGuiSemaphore == NULL) || (xSemaphoreTake(xGuiSemaphore, portMAX_DELAY) == pdFALSE)) {};
  xSemaphoreGive(xGuiSemaphore);
}

bool Driver::aquireMutex(size_t timeoutMs) {
  TickType_t ticks = timeoutMs ? pdMS_TO_TICKS(timeoutMs) : portMAX_DELAY;
  return xSemaphoreTake(xGuiSemaphore, ticks) == pdTRUE;
}

void Driver::releaseMutex() {
  xSemaphoreGive(xGuiSemaphore);
}

static void task_GUI(void* pvParameter) {
  (void)pvParameter;
  xGuiSemaphore = xSemaphoreCreateMutex();

  // Take semaphore initially while setting up GUI
  xSemaphoreTake(xGuiSemaphore, portMAX_DELAY);

  lv_init();

  // Initialize SPI or I2C bus used by the drivers
  lvgl_driver_init();

  lv_color_t* buf1 = (lv_color_t*)heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
  assert(buf1 != NULL);
  lv_color_t* buf2 = (lv_color_t*)heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
  assert(buf2 != NULL);

  static lv_disp_buf_t disp_buf;
  uint32_t size_in_px = DISP_BUF_SIZE;

  lv_disp_buf_init(&disp_buf, buf1, buf2, size_in_px);

  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.flush_cb = disp_driver_flush;

  disp_drv.buffer = &disp_buf;
  lv_disp_drv_register(&disp_drv);

  const esp_timer_create_args_t periodic_timer_args = {
  .callback = &task_lv_tick,
  .name = "periodic_gui",
  };

  esp_timer_handle_t periodic_timer;
  ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
  ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

  lv_task_handler();
  xSemaphoreGive(xGuiSemaphore);

  while(1) {
    vTaskDelay(pdMS_TO_TICKS(LV_UPDATE_INTERVAL_MS));

    // Try to take semaphore and update GUI
    if(pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
      lv_task_handler();
      xSemaphoreGive(xGuiSemaphore);
    }
  }

  // Shouldn't get here.
  free(buf1);
  free(buf2);
  vTaskDelete(NULL);
}

static void task_lv_tick(void* arg) {
  (void)arg;
  lv_tick_inc(LV_TICK_PERIOD_MS);
}
