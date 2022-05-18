#ifndef __GUI_VIEWS_VIEW_HPP
#define __GUI_VIEWS_VIEW_HPP

#include <cstdint>
#define LVL_MUTEX_AQUISITION_MSEC 100

namespace GUI {
  namespace Views {
    class View {
     public:
      virtual ~View() = default;
      virtual void show() = 0;
      virtual void hide() = 0;
    };
  }  // namespace Views
}  // namespace GUI

#endif  // __GUI_VIEWS_VIEW_HPP