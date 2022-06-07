#ifndef __HAL_BUTTON_HPP
#define __HAL_BUTTON_HPP

#include <cstdint>

#ifndef BUTTON_A
  #define BUTTON_A 35
#endif  // BUTTON_A

#ifndef BUTTON_B
  #define BUTTON_B 0
#endif  // BUTTON_B

namespace HAL {

  class _Button {
   public:
    enum class EventType {
      UP,    ///< The button went from the pressed to release state.
      DOWN,  ///< The button went from the released to pressed state.
      HELD,  ///< The button was held for more than 3 seconds.
      NONE   ///< No defined button event occurred.
    };

    /**
     * @brief Structure describing a single button event.
     */
    struct Event {
      const uint32_t pin;    ///< The pin on which the event occurred.
      const EventType type;  ///< The type of event which occurred.
    };

    _Button();
    static _Button* instance();
    /**
     * @brief Get a single button event, if it has occurred.
     * @details If no event has occurred, Event::type is set to EventType::None
     * @param timeout The timeout to wait for an event to occur, in ms.
     * @return Event Structure describing the event which occurred.
     */
    Event getEvent(const uint32_t timeout);

    /**
     * @brief Get the current state of a button.
     * @param pin The pin (button) to query.
     * @return EventType Can be one of either DOWN or UP.
     */
    EventType state(const uint32_t pin);

   private:
    _Button(const _Button&) = delete;
    _Button(_Button&&) = delete;
    _Button& operator=(const _Button&) = delete;
    _Button& operator=(_Button&&) = delete;
  };
  constexpr auto Button = _Button::instance;
};  // namespace HAL

#endif  //__HAL_BUTTON_HPP