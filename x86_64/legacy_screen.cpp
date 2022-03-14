#include <lvgl.h>
#include <unistd.h>

#include <configuration.hpp>
#include <containers/crypto.hpp>
#include <cstdio>
#include <cstdlib>
#include <gui/hal/driver.hpp>
#include <gui/views/ticker/legacy.hpp>

using namespace GUI;
using namespace GUI::HAL;

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
      LegacyScreen()->setCurrencySymbol(fiat.symbol);

      LegacyScreen()->show();
      usleep(3 * 1000 * 1000);
      LegacyScreen()->hide();
      LegacyScreen()->clearPlot();
      LegacyScreen()->clearIcon();
    }
  }
}
