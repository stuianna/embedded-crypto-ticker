#ifndef __CRYPTO_CRYPTO_DEFINITIONS_HPP
#define __CRYPTO_CRYPTO_DEFINITIONS_HPP

#include <cstdint>

#include "currency_definition.hpp"

/**
 * @brief Namespace for Cryptocurrency related attributes and structures.
 */
namespace Crypto {
  /**
   * @brief Enumeration for all cryptocurrency definitions.
   */
  enum class Currency {
    BTC,   ///< Bitcoin
    ETH,   ///< Ethereum
    LTC,   ///< Litecoin
    DOGE,  ///< Dodgecoin
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
   * @brief Evaluates to the length of availableCurrencies()
   * @return constexpr size_t The length of availableCurrencies()
   */
  constexpr size_t currencyCount() { return sizeof(availableCurrencies) / sizeof(Currency); }

  /**
   * @brief Get the Definition for a given cyrptocurrency
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
};  // namespace Crypto

#endif  // __CRYPTO_CRYPTO_DEFINITIONS_HPP