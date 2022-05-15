#ifndef __GUI_VIEWS_STARTUP_LOADING_HPP
#define __GUI_VIEWS_STARTUP_LOADING_HPP

#include <lvgl.h>

#include <cstdint>

#include "../../themes/theme_factory.hpp"
#include "../../widgets/elemental/label.hpp"
#include "../../widgets/elemental/spinner.hpp"
#include "../view.hpp"

namespace GUI {
  namespace Views {
    class Loading : public View {
     public:
      ~Loading();
      void show() override;
      void hide() override;
      void status(const char* text, Widgets::Severity level = Widgets::Severity::NORMAL);
      void details(const char* text, Widgets::Severity level = Widgets::Severity::NORMAL);
      static Loading* instance();

     private:
      Loading();
      lv_obj_t* _screen;
      lv_obj_t* _empty;
      lv_obj_t* _container;
      Widgets::Spinner _spinner;
      Widgets::Label<24> _status;
      Widgets::Label<24> _details;
      lv_style_t _innerContainerStyle;
    };

  }  // namespace Views
  constexpr auto LoadingScreen = Views::Loading::instance;
}  // namespace GUI

#endif  // __GUI_VIEWS_STARTUP_LOADING_HPP