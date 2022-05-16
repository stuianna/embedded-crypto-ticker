#include "data_table.hpp"

using namespace Crypto;

Entry Crypto::Table[currencyCount()]{{Currency::BTC, getDefinition(Currency::BTC), btc_icon_60, {}, {}, 0, true},
                                     {Currency::ETH, getDefinition(Currency::ETH), eth_icon_60, {}, {}, 0, true},
                                     {Currency::LTC, getDefinition(Currency::LTC), ltc_icon_60, {}, {}, 0, true},
                                     {Currency::DOGE, getDefinition(Currency::DOGE), dge_icon_60, {}, {}, 0, true}};

Fiat Crypto::baseCurrency = Fiat::AUD;


size_t Crypto::enabledCurrencyCount() {
  size_t count = 0;
  for(auto& entry: Table) {
    count = entry.enabled ? count + 1 : count;
  }
  return count;
}

size_t Crypto::currencyIndex(Currency currency) {
  for(auto i = 0; i < currencyCount(); i++) {
    if(Table[i].currency == currency) {
      return i;
    }
  }
  return 0;
}