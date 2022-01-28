#ifndef __GUI_VIEWS_STARTUP_PROV_HPP
#define __GUI_VIEWS_STARTUP_PROV_HPP

#include <lvgl.h>

#include <cstdint>

#include "../../themes/theme_factory.hpp"
#include "../../widgets/elemental/qrcode.hpp"
#include "../view.hpp"

namespace GUI {
  namespace Views {
    class Provisioning : public View {
     public:
      ~Provisioning();
      void show() override;
      void hide() override;
      void setQR(const char* data);
      static Provisioning* instance();

     private:
      Provisioning();
      lv_obj_t* _screen;
      lv_obj_t* _empty;
      lv_obj_t* _container;
      Widgets::QRCode<120> _qr;
      lv_style_t _innerContainerStyle;
    };

  }  // namespace Views
  constexpr auto ProvisioningScreen = Views::Provisioning::instance;
}  // namespace GUI

#endif  // __GUI_VIEWS_STARTUP_PROV_HPP