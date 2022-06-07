#ifndef __TASKS_FACTORY_RESET_HPP
#define __TASKS_FACTORY_RESET_HPP

#include <cstddef>
#include <cstdint>
#include <lib/hal/button.hpp>

namespace Tasks {
  /**
   * @brief Singleton to handle factory reset.
   * @details Factory reset task check for the target button state on commencement. If in the correct state, as one shot timer checks for a
   * pressed state after a set period of time. If a press is detected on timeout, the device preforms a factory reset.
   */
  class FactoryResetTask {
   public:
    ~FactoryResetTask() = default;
    /**
     * @brief Retrieve the singleton instance for the class.
     * @return FactoryResetTask*
     */
    static FactoryResetTask* instance();

    /**
     * @brief Start the factory reset task.
     * @details Check current button state and start one-shot timer if required. If the button state doesn't match the initial trigger, this
     * call does nothing. The default requirements should be considered undefined.
     */
    void startMonitor();

    /**
     * @brief Set the required button state needed to trigger the start of the factory reset countdown.
     * @warning This function has no effect if used after start()
     * @param event The event required to trigger the factory reset one-shot timer.
     * @param pin  The pin on which the even should occur.
     */
    void setTriggerButtonState(const HAL::_Button::EventType event, const uint32_t pin);

    /**
     * @brief Set the one-shot timeout at which the button state is check a second time.
     * @param ms Timeout is milliseconds.
     */
    void setOneShotTimeout(const uint32_t ms);

    /**
     * @brief Set the final state to which to match the button.
     * @details If the button matches this state at the expiry of the one shot timeout, a factory reset is triggered.
     * @param event The event required.
     */
    void setFinalButtonState(const HAL::_Button::EventType event);

    /**
     * @brief Get the final required button state.
     * @return The required state.
     */
    HAL::_Button::EventType getFinalButtonState() const;

    /**
     * @brief Get the target pin used.
     * @return pin.
     */
    uint32_t getPin() const;

    /**
     * @brief Conduct a factory reset.
     * @details The devices NVS (non-volatile storage) is erased, and the device is reset. This is automatically called if the final button
     * state matches at the end of the timeout period. Can be used separately from startMonitor task.
     */
    static void preform();


   private:
    FactoryResetTask();
    size_t _targetPin;
    size_t _timeout;
    HAL::_Button::EventType _trigger;
    HAL::_Button::EventType _finalState;

    // Disable copy and move assignment operators and constructors.
    FactoryResetTask(const FactoryResetTask&) = default;
    FactoryResetTask(FactoryResetTask&&) = default;
    FactoryResetTask& operator=(const FactoryResetTask&) = default;
    FactoryResetTask operator=(FactoryResetTask&&) = delete;
  };
  constexpr auto FactoryReset = FactoryResetTask::instance;
};  // namespace Tasks

#endif  //  __TASKS_FACTORY_RESET_HPP