#ifndef __GUI_THEME_THEMEFACTORY_HPP
#define __GUI_THEME_THEMEFACTORY_HPP

#include <lvgl.h>

#include "dark.hpp"
#include "theme.hpp"

namespace GUI {
  enum class GlobalTheme { DARK };
  class ThemeFactory {
   public:
    static void set(const GlobalTheme theme) { _current = theme; };
    static Theme* current() {
      switch(_current) {
        default:
        case GlobalTheme::DARK: static ThemeDark theme; return &theme;
      }
    }

   private:
    static GlobalTheme _current;
  };
  constexpr auto currentTheme = ThemeFactory::current;
};  // namespace GUI

#endif  // __GUI_THEME_THEMEFACTORY_HPP