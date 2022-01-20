#ifndef __GUI_VIEWS_VIEW_HPP
#define __GUI_VIEWS_VIEW_HPP

#include <cstdint>

namespace GUI {
  namespace Views {
    class View {
      virtual void show() = 0;
      virtual void hide() = 0;
    };
  }  // namespace Views
}  // namespace GUI

#endif  // __GUI_VIEWS_VIEW_HPP