#include <lvgl.h>

#include <hal/lvgl/lvgl_driver.hpp>
// #define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
#define SDL_MAIN_HANDLED    /*To fix SDL's "undefined reference to WinMain" \
                               issue*/
#include <SDL2/SDL.h>

#include "lv_drivers/display/monitor.h"
#include "lv_drivers/indev/keyboard.h"
#include "lv_drivers/indev/mouse.h"
#include "lv_drivers/indev/mousewheel.h"
#include "lvgl/lvgl.h"

using namespace HAL;

#ifndef LV_TICK_PERIOD_MS
  #define LV_TICK_PERIOD_MS 1
#endif

#ifndef LV_UPDATE_INTERVAL
  #define LV_UPDATE_INTERVAL_MS 10
#endif

#include <thread>

static void hal_init(void);
static int tick_thread(void* data);
static void update_thread();
static void memory_monitor(lv_task_t* param);

lv_disp_buf_t disp_buf1;
lv_color_t buf1_1[LV_HOR_RES_MAX * 120];
lv_disp_drv_t disp_drv;
lv_indev_drv_t mouse_drv;
lv_indev_drv_t keyb_drv;
lv_indev_drv_t enc_drv;

LVGLDriver LVGLDriver::_instance;

LVGLDriver::LVGLDriver() {
}

void LVGLDriver::init() {
  lv_init();

  /* Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
  monitor_init();

  /*Create a display buffer*/
  lv_disp_buf_init(&disp_buf1, buf1_1, NULL, LV_HOR_RES_MAX * 120);

  /*Create a display*/
  lv_disp_drv_init(&disp_drv); /*Basic initialization*/
  disp_drv.buffer = &disp_buf1;
  disp_drv.flush_cb = monitor_flush;
  lv_disp_drv_register(&disp_drv);

  /* Add the mouse as input device
   * Use the 'mouse' driver which reads the PC's mouse*/
  mouse_init();
  lv_indev_drv_init(&mouse_drv); /*Basic initialization*/
  mouse_drv.type = LV_INDEV_TYPE_POINTER;

  /*This function will be called periodically (by the library) to get the mouse position and state*/
  mouse_drv.read_cb = mouse_read;
  // lv_indev_t* mouse_indev = lv_indev_drv_register(&mouse_drv);

  // /*Set a cursor for the mouse*/
  // LV_IMG_DECLARE(mouse_cursor_icon);                        /*Declare the image file.*/
  // lv_obj_t* cursor_obj = lv_img_create(lv_scr_act(), NULL); /*Create an image object for the cursor */
  // lv_img_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
  // lv_indev_set_cursor(mouse_indev, cursor_obj);             /*Connect the image  object to the driver*/

  /* Add the keyboard as input device
   * Use the 'keyboard' driver which reads the PC's keyboard*/
  keyboard_init();
  lv_indev_drv_init(&keyb_drv);
  keyb_drv.type = LV_INDEV_TYPE_KEYPAD;
  keyb_drv.read_cb = keyboard_read;
  lv_indev_drv_register(&keyb_drv);

  /* Add the mouse wheel as input device (encoder type)
   * Use the 'mousewheel' driver which reads the PC's mouse wheel*/
  mousewheel_init();
  lv_indev_drv_init(&enc_drv);
  enc_drv.type = LV_INDEV_TYPE_ENCODER;
  enc_drv.read_cb = mousewheel_read;
  lv_indev_drv_register(&enc_drv);

  SDL_CreateThread(tick_thread, "tick", NULL);

  std::thread ut(update_thread);
  ut.detach();
}

bool LVGLDriver::aquireMutex(size_t timeoutMs) {
  (void)timeoutMs;
  return true;
}

void LVGLDriver::releaseMutex() {
}

void update_thread() {
  while(true) {
    lv_task_handler();
    usleep(5 * 1000);
  }
}

int tick_thread(void* data) {
  (void)data;
  while(1) {
    SDL_Delay(5);
    lv_tick_inc(5);
  }
  return 0;
}