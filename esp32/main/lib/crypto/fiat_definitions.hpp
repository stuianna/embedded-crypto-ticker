#ifndef __CRYPTO_FIAT_DEFINITIONS_HPP
#define __CRYPTO_FIAT_DEFINITIONS_HPP

#include <cstdint>

#include "currency_definition.hpp"

/**
 * @brief Namespace for Cryptocurrency related attributes and structures.
 */
namespace Crypto {
  /**
   * @brief Enumeration for all Fiat definitions
   */
  enum class Fiat {
    AUD,  ///< Australian Dollar
  };

  /**
   * @brief All available (enabled) fiat currencies.
   */
  static constexpr Fiat availableFiat[] = {Fiat::AUD};

  /**
   * @brief Evaluates to the length of availableFiat()
   * @return constexpr size_t The length of availableFiat()
   */
  constexpr size_t fiatCount() { return sizeof(availableFiat) / sizeof(Fiat); }

  /**
   * @brief Get the Definition for a given fiat currency
   *
   * @param fiat The fiat to retrieve the definition.
   * @return constexpr Definition
   */
  constexpr Definition getDefinition(const Fiat fiat) {
    switch(fiat) {
      default:
      case Fiat::AUD: return {"Australian Dollar", '$', "AUD", "aud"};
    }
  };
};  // namespace Crypto

#endif  // __CRYPTO_FIAT_DEFINITIONS_HPP