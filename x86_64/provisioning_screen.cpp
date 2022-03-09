#include <lvgl.h>
#include <unistd.h>

#include <cstdio>
#include <gui/hal/driver.hpp>
#include <gui/views/startup/provisioning.hpp>

int main(int, char**) {
  GUI::HAL::LVGL()->init();
  GUI::ProvisioningScreen()->setQR("{\"ver\":\"v1\",\"name\":\"PROV_ticker\",\"pop\":\"abcd1234\",\"transport\":\"softap\"}");
  GUI::ProvisioningScreen()->show();

  while(1) {
    usleep(1000 * 1000);
  }
}