#ifndef __CRYPTO_CURRENCY_DEFINITIONS_HPP
#define __CRYPTO_CURRENCY_DEFINITIONS_HPP

#include <cstdint>

/**
 * @brief Namespace for Cryptocurrency related attributes and structures.
 */
namespace Crypto {
  /**
   * @brief Structure which can be used to describe either Fiat or Currency enumerations.
   * @details Use getDefinition(X) to retrieve information on a passed currency.
   */
  struct Definition {
    const char* name;       ///< The name of the currency. E.g. Australian Dollar
    const char symbol;      ///< The symbol used by the currency, if it can be represented by AASCI. Otherwise a ' ' is used.
    const char* ticker;     ///< The ticker name for the currency. E.g. Bitcoin has the ticker BTC.
    const char* geckoName;  ///< The name used by the Coin Gecko API.
  };
};  // namespace Crypto

#endif  // __CRYPTO_CURRENCYTO_DEFINITIONS_HPP