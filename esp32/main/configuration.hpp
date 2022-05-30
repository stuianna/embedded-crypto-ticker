#ifndef _CONFIGURATION_HPP
#define _CONFIGURATION_HPP

#include <cstddef>

const size_t currencyUpdatePeriodSeconds = 300;

namespace Provisioning {
  const char* SSID = "PROV_ticker";
  const char* popCode = "abcd1234";
}  // namespace Provisioning

namespace Configuration {
  const size_t chartMaximumValue = 1000;
  const size_t chartMinimumValue = 1;
};  // namespace Configuration

#endif  // _CONFIGURATION_HPP