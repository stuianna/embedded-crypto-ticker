#include <lvgl.h>
#include <unistd.h>

#include <cstdio>
#include <gui/hal/driver.hpp>

int main(int, char**) {
  GUI::HAL::LVGL()->init();
  while(1) {
    usleep(1000 * 100);
  }
}
