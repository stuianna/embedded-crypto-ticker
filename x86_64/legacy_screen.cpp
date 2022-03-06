#include <lvgl.h>
#include <unistd.h>

#include <configuration.hpp>
#include <cstdio>
#include <cstdlib>
#include <gui/hal/driver.hpp>
#include <gui/views/ticker/legacy.hpp>

using namespace GUI;
using namespace GUI::HAL;

LV_IMG_DECLARE(btc_icon_60);
LV_IMG_DECLARE(eth_icon_60);
LV_IMG_DECLARE(ltc_icon_60);
LV_IMG_DECLARE(dge_icon_60);

const struct {
  Currencies::Crypto currency;
  const lv_img_dsc_t image;
} definitions[currencyCount()]{
{Currencies::Crypto::BTC, btc_icon_60},
{Currencies::Crypto::ETH, eth_icon_60},
{Currencies::Crypto::LTC, ltc_icon_60},
{Currencies::Crypto::DOGE, dge_icon_60},
};

int main(int, char**) {
  LVGL()->init();

  while(1) {
    for(auto def: definitions) {
      LegacyScreen()->setPlotPointCount(100);
      LegacyScreen()->setPlotRange(0, 100);

      for(int i = 0; i < 100; i++) {
        LegacyScreen()->plotValue(rand() % 100);
      }

      LegacyScreen()->setCurrentQuote((rand() % 10000000) / 100);
      LegacyScreen()->setDailyDelta(((rand() % 1000) / 100) - 5);

      LegacyScreen()->setName(Currencies::getDefinition(def.currency).name);
      LegacyScreen()->setIcon(&def.image);
      LegacyScreen()->setCurrencySymbol(Currencies::getDefinition(baseCurrency).symbol);

      LegacyScreen()->show();
      usleep(3 * 1000 * 1000);
      LegacyScreen()->hide();
      LegacyScreen()->clearPlot();
      LegacyScreen()->clearIcon();
    }
  }
}
