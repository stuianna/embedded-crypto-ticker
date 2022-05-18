#include "button.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// clang-format off
// Force button.h relative position to freeRTOS inclues.
#include <button.h>
// clang-format on

using namespace HAL;

static QueueHandle_t button_events = NULL;

_Button::_Button() {
  button_events = button_init(PIN_BIT(BUTTON_A) | PIN_BIT(BUTTON_B));
}

_Button* _Button::instance() {
  static _Button _instance;
  return &_instance;
}

_Button::Event _Button::getEvent(const uint32_t timeout) {
  button_event_t ev;
  if(xQueueReceive(button_events, &ev, timeout)) {
    switch(ev.event) {
      case BUTTON_DOWN: return {.pin = ev.pin, .type = EventType::DOWN};
      case BUTTON_UP: return {.pin = ev.pin, .type = EventType::UP};
      case BUTTON_HELD: return {.pin = ev.pin, .type = EventType::HELD};
      default: break;
    }
  }
  return {.pin = 0, .type = EventType::NONE};
}