#ifndef __CRYPTO_DATA_TABLE_HPP
#define __CRYPTO_DATA_TABLE_HPP

#include <lvgl.h>

#include <lib/tools/volatile_database.hpp>

#include "crypto_definitions.hpp"
#include "fiat_definitions.hpp"

LV_IMG_DECLARE(btc_icon_60);
LV_IMG_DECLARE(eth_icon_60);
LV_IMG_DECLARE(ltc_icon_60);
LV_IMG_DECLARE(dge_icon_60);
LV_IMG_DECLARE(black_60);
LV_IMG_DECLARE(warning_30);

/**
 * @brief Namespace for Cryptocurrency related attributes and structures.
 */
namespace Crypto {
  /**
   * @brief The currently used base currency. Cryptocurrency quotes are given relative to this.
   */
  extern Fiat baseCurrency;

  /**
   * @brief Lookup table correlating each Currency's definition, databases, configuration and assets.
   */
  struct Entry {
    Currency currency;                      ///< The currency represented by the table's entry.
    Definition params;                      ///< Definition describing the currency's parameters.
    const lv_img_dsc_t icon;                ///< The icon (image) used by the currency.
    VolatileDatabase<float, 288> pricesDB;  // The database used to store historical and current prices (quotes).
    VolatileDatabase<float, 1> delta24hDB;  // The database used to store the change in 24 hour price.
    size_t latestUpdate;                    // Unix time stamp for the latest time the currency was updated.
    bool enabled;                           // Set to true if the currency is enabled.
  };

  /**
   * @brief The main lookuptable for each supported currency
   */
  extern Entry Table[currencyCount()];

  /**
   * @brief Get the number of currencies which are enabled.
   * @details A currency is considered enabled. If its 'enabled' field in Crypto::Table is set to true.
   * @return size_t The number of enabled currencies.
   */
  size_t enabledCurrencyCount();

  /**
   * @brief Get the index of the selected currency in Crypto::Table
   * @param currency The currency to query.
   * @return size_t The position of the currency.
   */
  size_t currencyIndex(Currency currency);
};  // namespace Crypto

#endif