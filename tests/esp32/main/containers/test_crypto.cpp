#include <doctest.h>

#include <crypto/data_table.hpp>

using namespace Crypto;

TEST_CASE("Get the number of enabled currencies.") {
  // Disable all currencies.
  for(auto& entry: Table) {
    entry.enabled = false;
  }
  CHECK(enabledCurrencyCount() == 0);

  // Enable one currency
  Table[0].enabled = true;
  CHECK(enabledCurrencyCount() == 1);

  // Enable all currencies
  for(auto& entry: Table) {
    entry.enabled = true;
  }
  CHECK(enabledCurrencyCount() == currencyCount());
}

TEST_CASE("Get the index of a selected currency") {
  // This test is not ideal. If the order of currencies in the table changes, then it will fail.
  CHECK(currencyIndex(Currency::BTC) == 0);
  CHECK(currencyIndex(Currency::LTC) == 2);
}