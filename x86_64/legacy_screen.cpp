#include <lvgl.h>
#include <unistd.h>

#include <cstdio>
// #include <gui/tools/cpl.hpp>
// #include <gui/widgets/elemental/image.hpp>
// #include <gui/widgets/elemental/percentage.hpp>
// #include <gui/widgets/elemental/price.hpp>
// #include <gui/widgets/composite/quote_banner.hpp>
// #include <gui/widgets/elemental/chart.hpp>
#include <gui/hal/driver.hpp>
#include <gui/views/startup/loading.hpp>
#include <gui/views/startup/provisioning.hpp>
#include <gui/views/ticker/legacy.hpp>

#define QRCODE_BASE_URL       "https://espressif.github.io/esp-jumpstart/qrcode.html"
#define PROV_QR_VERSION       "v1"
#define PROV_TRANSPORT_SOFTAP "softap"

LV_IMG_DECLARE(btc_icon_60);

static void setupGUI();

// static lv_obj_t* hello_world_label;
// static lv_obj_t* count_label;

// static void setupGUI() {
//   hello_world_label = lv_label_create(lv_scr_act(), NULL);

// }

int main(int, char**) {
  GUI::HAL::LVGL()->init();
  // GUI::HAL::LVGL()->aquireMutex(0);
  // auto view = GUI::LegacyScreen()
  // view.status("I am the status of it", GUI::Widgets::Severity::NORMAL);
  GUI::LegacyScreen()->show();
  GUI::LegacyScreen()->setPlotPointCount(100);
  GUI::LegacyScreen()->setPlotRange(19.9, 20.1);
  // view.plotValue(2);
  // view.plotValue(9);
  // view.plotValue(2);
  // view.plotValue(4);
  // view.plotValue(4);
  // view.plotValue(3);
  // view.plotValue(10);
  // view.plotValue(0);
  // view.plotValue(1);
  // view.plotValue(1);
  // view.setCurrencySymbol('$');
  // view.setDailyDelta(0.32);
  GUI::LegacyScreen()->setName("Bitcoin");
  GUI::LegacyScreen()->setIcon(&btc_icon_60);
  // view.setCurrentQuote(123456);

  // view.show();
  // auto chart = GUI::Widgets::Chart(lv_scr_act());
  // chart.setHeight(135);
  // chart.setWidth(240);
  // chart.setRange(0, 10);
  // chart.plot(2);
  // chart.plot(9);
  // chart.plot(1);
  // chart.plot(8);
  // chart.plot(1);
  // chart.plot(2);
  // chart.plot(0);
  // chart.plot(8);
  // chart.plot(4);
  // chart.plot(10);
  // chart.update();
  // auto banner = GUI::Widgets::QuoteBanner(lv_scr_act());
  // banner.setIcon(&btc_icon_60);
  // banner.setWidth(lv_obj_get_width(lv_scr_act()));
  // banner.setHeight(lv_obj_get_height(lv_scr_act()) * 0.50);
  // banner.setCurrencySymbol('$');
  // banner.setPercentChange(0.32);
  // banner.setLabel("Bitcoin");
  // banner.setQuote(123456);
  // GUI::HAL::LVGL()->releaseMutex();
  // auto image = GUI::Widgets::Image(lv_scr_act());
  // image.setSource(&btc_icon_60);
  // setupGUI();
  // auto price = GUI::Widgets::Percentage(lv_scr_act());
  // price.setLayout(LV_LAYOUT_OFF);
  // price.setHeight(100);
  // price.setWidth(200);
  // price.setFit(LV_FIT_PARENT);
  // price.update();

  // int32_t count = 0;
  // char count_str[11] = {0};

  // char payload[150] = {0};
  // snprintf(payload, sizeof(payload),
  //           "{\"ver\":\"%s\",\"name\":\"%s\""
  //           ",\"pop\":\"%s\",\"transport\":\"%s\"}",
  //           PROV_QR_VERSION, "PROV_ticker", "abcd1234", PROV_TRANSPORT_SOFTAP);


  float value = 19.9;
  while(1) {
    for(int i = 0; i < 100; i++) {
      GUI::LegacyScreen()->setCurrentQuote(value);
      GUI::LegacyScreen()->plotValue(value);
      value += 0.01;
    }
    while(1) {
      usleep(1000 * 100);
    }
    // snprintf(count_str, 11, "%d", count++);
    // GUI::HAL::LVGL()->aquireMutex(0);
    // view.setQR(payload);
    // if (count %2) {

    // }
    // else {
    //   view.hide();

    // }
    // auto image = GUI::Widgets::Image(lv_scr_act());
    // image.setSource(&btc_icon_60);
    // image.setWidth(count);
    // image.setHeight(count);
    // count++;
    // price.setValue(count);
    // count++;
    // lv_label_set_text(count_label, count_str);
    // GUI::HAL::LVGL()->releaseMutex();
  }
}
