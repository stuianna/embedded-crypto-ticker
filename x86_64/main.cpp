#include <lvgl.h>
#include <unistd.h>

#include <cstdio>
#include <hal/lvgl_driver.hpp>

int main(int, char**) {
  HAL::LVGL()->init();
  while(1) {
    usleep(1000 * 100);
  }
}
