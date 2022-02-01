#ifndef _CONFIGURATION_HPP
#define _CONFIGURATION_HPP

#include <cstddef>
#include <database/definitions/currencies.hpp>

static const Currencies::Crypto enabledCurrencies[] = {Currencies::Crypto::BTC, Currencies::Crypto::ETH};
static const Currencies::Fiat baseCurrency = Currencies::Fiat::AUD;
constexpr size_t currencyCount() {
  return sizeof(enabledCurrencies) / sizeof(Currencies::Crypto);
}

namespace Provisioning {
  const char* SSID = "PROV_ticker";
  const char* popCode = "abcd1234";
}  // namespace Provisioning

#endif  // _CONFIGURATION_HPP