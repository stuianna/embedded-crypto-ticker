#include <lvgl.h>
#include <unistd.h>

#include <cstdio>
#include <hal/lvgl_driver.hpp>
#include <gui/views/startup/loading.hpp>

// clang-format off
const char* status_lines[] = {
  "Received WiFi credentials", 
  "Fetching",
  "Loading current data"
  };

const char* detail_lines[] = {
  "HTTP code 404", 
  "Bitcoin",
  "Feeding goats"
  };
// clang-format on

int main(int, char**) {
  HAL::LVGL()->init();
  GUI::LoadingScreen()->show();

  while(1) {
    for(auto& line: status_lines) {
      GUI::LoadingScreen()->status(line);
      for(auto& details: detail_lines) {
        GUI::LoadingScreen()->details(details);
        usleep(1000 * 1000 * 1);
      }
    }
  }
}