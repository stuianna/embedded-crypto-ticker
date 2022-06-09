#include "factory_reset.hpp"

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>

#include <hal/nvs.hpp>
#include <hal/system.hpp>

#define LOG_TAG "factory_reset"

using namespace Tasks;

static void timerCallback(void*) {
  auto state = HAL::Button()->state(FactoryReset()->getPin());
  auto requiredState = FactoryReset()->getFinalButtonState();
  ESP_LOGI(LOG_TAG, "%s.", "Timer callback triggered.");
  ESP_LOGI(LOG_TAG, "%s %s.", "Current button state:", state == HAL::_Button::EventType::UP ? "UP" : "DOWN");
  ESP_LOGI(LOG_TAG, "%s %s.", "Required state", requiredState == HAL::_Button::EventType::UP ? "UP" : "DOWN");
  if(state == requiredState) {
    FactoryReset()->preform();
  }
}

FactoryResetTask* FactoryResetTask::instance() {
  static FactoryResetTask _instance;
  return &_instance;
}

FactoryResetTask::FactoryResetTask() :
  _targetPin(BUTTON_A), _timeout(5000), _trigger(HAL::_Button::EventType::DOWN), _finalState(HAL::_Button::EventType::DOWN) {
}

void FactoryResetTask::startMonitor() {
  auto state = HAL::Button()->state(_targetPin);
  ESP_LOGI(LOG_TAG, "%s %s.", "Got inital button state:", state == HAL::_Button::EventType::UP ? "UP" : "DOWN");

  if(HAL::Button()->state(_targetPin) != _trigger) {
    return;
  }
  ESP_LOGI(LOG_TAG, "Start reset monitor timeout for %d ms.", _timeout);
  auto handle = xTimerCreate("fReset_timer", pdMS_TO_TICKS(_timeout), pdFALSE, 0, timerCallback);
  if(!handle) {
    ESP_LOGE(LOG_TAG, "Failed to allocate memory for factory reset one-shot timer.");
    return;
  }
  auto started = xTimerStart(handle, pdMS_TO_TICKS(1000));
  if(!started) {
    ESP_LOGE(LOG_TAG, "Failed to start timer.");
  }
}

void FactoryResetTask::setTriggerButtonState(const HAL::_Button::EventType event, const uint32_t pin) {
  _trigger = event;
  _targetPin = pin;
}

void FactoryResetTask::setOneShotTimeout(const uint32_t ms) {
  _timeout = ms;
}

void FactoryResetTask::setFinalButtonState(const HAL::_Button::EventType event) {
  _finalState = event;
}

HAL::_Button::EventType FactoryResetTask::getFinalButtonState() const {
  return _finalState;
}

uint32_t FactoryResetTask::getPin() const {
  return _targetPin;
}

void FactoryResetTask::preform() {
  ESP_LOGI(LOG_TAG, "%s.", "Erasing NVS and preforming factory reset.");
  HAL::NVS()->eraseAll();
  HAL::System::reset();
}
