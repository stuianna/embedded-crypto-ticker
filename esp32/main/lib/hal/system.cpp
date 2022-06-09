#include "system.hpp"

#include <esp_heap_caps.h>
#include <esp_system.h>
#include <esp_timer.h>

size_t HAL::System::Heap::minimumFree() {
  return esp_get_minimum_free_heap_size();
}

size_t HAL::System::Heap::currentFree() {
  return esp_get_free_heap_size();
}

size_t HAL::System::Heap::largestFreeBlock() {
  return heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
}

void HAL::System::reset() {
  esp_restart();
}

const char* HAL::System::resetReason() {
  auto reason = esp_reset_reason();
  switch(reason) {
    default:
    case ESP_RST_UNKNOWN: return "Unknown";
    case ESP_RST_POWERON: return "Power on";
    case ESP_RST_EXT: return "External pin";
    case ESP_RST_SW: return "Software request";
    case ESP_RST_PANIC: return "Software panic";
    case ESP_RST_INT_WDT: return "Interrupt watchdog";
    case ESP_RST_TASK_WDT: return "Task watchdog";
    case ESP_RST_WDT: return "Undefined watchdog";
    case ESP_RST_DEEPSLEEP: return "Deep sleep";
    case ESP_RST_BROWNOUT: return "Brownout";
    case ESP_RST_SDIO: return "SDIO";
  }
}

int64_t HAL::System::uptime() {
  return esp_timer_get_time() / 1000;
}