#include <lvgl.h>
#include <unistd.h>

#include <cstdio>
#include <gui/hal/driver.hpp>
#include <gui/views/startup/loading.hpp>

// clang-format off
const char* status_lines[] = {
  "Received WiFi credentials", 
  "Fetching",
  "Loading current data"
  };
// clang-format on

int main(int, char**) {
  GUI::HAL::LVGL()->init();
  GUI::LoadingScreen()->show();

  while(1) {
    for(auto line: status_lines) {
      GUI::LoadingScreen()->status(line);
      usleep(1000 * 1000 * 2);
    }
  }
}