#include <lvgl.h>
#include <unistd.h>

#include <configuration.hpp>
#include <crypto/data_table.hpp>
#include <cstdio>
#include <cstdlib>
#include <gui/views/ticker/legacy.hpp>
#include <hal/lvgl_driver.hpp>

using namespace GUI;
using namespace HAL;

int main(int, char**) {
  LVGL()->init();

  while(1) {
    auto fiat = Crypto::getDefinition(Crypto::baseCurrency);
    for(auto i = 0; i < Crypto::currencyCount(); i++) {
      auto coin = &Crypto::Table[i];
      LegacyScreen()->setPlotPointCount(100);
      LegacyScreen()->setPlotRange(0, 100);

      for(int j = 0; j < 100; j++) {
        LegacyScreen()->plotValue(rand() % 100);
      }

      LegacyScreen()->setCurrentQuote((rand() % 10000000) / 100);
      LegacyScreen()->setDailyDelta(((rand() % 1000) / 100) - 5);

      LegacyScreen()->setName(coin->params.name);
      LegacyScreen()->setIcon(&coin->icon);
      LegacyScreen()->setWarning(&black_60);
      LegacyScreen()->setCurrencySymbol(fiat.symbol);
      LegacyScreen()->show();

      usleep(1.5 * 1000 * 1000);
      LegacyScreen()->setWarning(&warning_30);

      usleep(1.5 * 1000 * 1000);
      LegacyScreen()->hide();
      LegacyScreen()->clearPlot();
      LegacyScreen()->clearIcon();
    }
  }
}
