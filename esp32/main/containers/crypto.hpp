#ifndef __CONTAINERS_CRYPTO_HPP
#define __CONTAINERS_CRYPTO_HPP

#include <lvgl.h>

#include <database.hpp>

LV_IMG_DECLARE(btc_icon_60);
LV_IMG_DECLARE(eth_icon_60);
LV_IMG_DECLARE(ltc_icon_60);
LV_IMG_DECLARE(dge_icon_60);

namespace Crypto {
  enum class Currency {
    BTC,
    ETH,
    LTC,
    DOGE,
  };

  enum class Fiat {
    AUD,
  };

  static constexpr Currency availableCurrencies[] = {
  Currency::BTC,
  Currency::ETH,
  Currency::LTC,
  Currency::DOGE,
  };

  static constexpr Fiat availableFiat[] = {Fiat::AUD};

  constexpr size_t currencyCount() { return sizeof(availableCurrencies) / sizeof(Currency); }
  constexpr size_t fiatCount() { return sizeof(availableFiat) / sizeof(Fiat); }

  struct Definition {
    const char* name;
    const char symbol;
    const char* ticker;
    const char* geckoName;
  };

  constexpr Definition getDefinition(const Currency currency) {
    switch(currency) {
      default:
      case Currency::BTC: return {"Bitcoin", ' ', "BTC", "bitcoin"};
      case Currency::ETH: return {"Ethereum", ' ', "ETH", "ethereum"};
      case Currency::LTC: return {"Litecoin", ' ', "LTC", "litecoin"};
      case Currency::DOGE: return {"Dogecoin", ' ', "DOGE", "dogecoin"};
    }
  };

  constexpr Definition getDefinition(const Fiat fiat) {
    switch(fiat) {
      default:
      case Fiat::AUD: return {"Australian Dollar", '$', "AUD", "aud"};
    }
  };

  static Fiat baseCurrency = Fiat::AUD;


  struct {
    Definition coin;
    const lv_img_dsc_t icon;
    Database::Database<float, 288> pricesDB;
    Database::Database<float, 1> delta24hDB;
    size_t latestUpdate;
    bool enabled;
  } Table[currencyCount()]{{getDefinition(Currency::BTC), btc_icon_60, {}, {}, 0, true},
                           {getDefinition(Currency::ETH), eth_icon_60, {}, {}, 0, true},
                           {getDefinition(Currency::LTC), ltc_icon_60, {}, {}, 0, true},
                           {getDefinition(Currency::DOGE), dge_icon_60, {}, {}, 0, true}};
};  // namespace Crypto

#endif