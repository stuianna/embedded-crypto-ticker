#ifndef __CONTAINERS_CRYPTO_HPP
#define __CONTAINERS_CRYPTO_HPP

#include <lvgl.h>

#include <database.hpp>

LV_IMG_DECLARE(btc_icon_60);
LV_IMG_DECLARE(eth_icon_60);
LV_IMG_DECLARE(ltc_icon_60);
LV_IMG_DECLARE(dge_icon_60);

namespace Crypto {
  /**
   * @brief Enumeration for all cryptocurrency definitions.
   */
  enum class Currency {
    BTC,   ///< Bitcoin
    ETH,   ///< Ethereum
    LTC,   ///< Litecoin
    DOGE,  ///< Bitcoin
  };

  /**
   * @brief Enumeration for all Fiat definitions
   */
  enum class Fiat {
    AUD,  ///< Australian Dollar
  };

  /**
   * @brief All available (enabled) cryptocurrencies.
   */
  static constexpr Currency availableCurrencies[] = {
  Currency::BTC,
  Currency::ETH,
  Currency::LTC,
  Currency::DOGE,
  };

  /**
   * @brief All available (enabled) fiat currencies.
   */
  static constexpr Fiat availableFiat[] = {Fiat::AUD};

  /**
   * @brief Evaluates to the length of #availableCurrencies()
   * @return constexpr size_t The length of #availableCurrencies()
   */
  constexpr size_t currencyCount() { return sizeof(availableCurrencies) / sizeof(Currency); }

  /**
   * @brief Evaluates to the length of #availableFiat()
   * @return constexpr size_t The length of #availableFiat()
   */
  constexpr size_t fiatCount() { return sizeof(availableFiat) / sizeof(Fiat); }

  /**
   * @brief Structure which can be used to describe either #Fiat or #Crypto enumerations.
   * @details Use #getDefinition(X) to retrieve information on a passed currency.
   */
  struct Definition {
    const char* name;       ///< The name of the currency. E.g. Australian Dollar
    const char symbol;      ///< The symbol used by the currency, if it can be represented by AASCI. Otherwise a ' ' is used.
    const char* ticker;     ///< The ticker name for the currency. E.g. Bitcoin has the ticker BTC.
    const char* geckoName;  ///< The name used by the Coin Gecko API.
  };

  /**
   * @brief Get the #Definition for a given cyrptocurrency
   *
   * @param currency The currency to retrieve the definition.
   * @return constexpr Definition
   */
  constexpr Definition getDefinition(const Currency currency) {
    switch(currency) {
      default:
      case Currency::BTC: return {"Bitcoin", ' ', "BTC", "bitcoin"};
      case Currency::ETH: return {"Ethereum", ' ', "ETH", "ethereum"};
      case Currency::LTC: return {"Litecoin", ' ', "LTC", "litecoin"};
      case Currency::DOGE: return {"Dogecoin", ' ', "DOGE", "dogecoin"};
    }
  };

  /**
   * @brief Get the #Definition for a given fiat currency
   *
   * @param currency The currency to retrieve the definition.
   * @return constexpr Definition
   */
  constexpr Definition getDefinition(const Fiat fiat) {
    switch(fiat) {
      default:
      case Fiat::AUD: return {"Australian Dollar", '$', "AUD", "aud"};
    }
  };

  /**
   * @brief The currently used base currency. Cryptocurrency quotes are given relative to this.
   */
  static Fiat baseCurrency = Fiat::AUD;

  /**
   * @brief Lookup table correlating each Currency's definition, databases, configuration and assets.
   */
  struct {
    Currency currency;                        ///< The currency represented by the table's entry.
    Definition params;                        ///< Definition describing the currency's parameters.
    const lv_img_dsc_t icon;                  ///< The icon (image) used by the currency.
    Database::Database<float, 288> pricesDB;  // The database used to store historical and current prices (quotes).
    Database::Database<float, 1> delta24hDB;  // The database used to store the change in 24 hour price.
    size_t latestUpdate;                      // Unix time stamp for the latest time the currency was updated.
    bool enabled;                             // Set to true if the currency is enabled.
  } Table[currencyCount()]{{Currency::BTC, getDefinition(Currency::BTC), btc_icon_60, {}, {}, 0, true},
                           {Currency::ETH, getDefinition(Currency::ETH), eth_icon_60, {}, {}, 0, true},
                           {Currency::LTC, getDefinition(Currency::LTC), ltc_icon_60, {}, {}, 0, true},
                           {Currency::DOGE, getDefinition(Currency::DOGE), dge_icon_60, {}, {}, 0, true}};
};  // namespace Crypto

#endif