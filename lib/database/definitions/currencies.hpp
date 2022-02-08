#ifndef __DEFINITIONS_CURRENCIES_HPP
#define __DEFINITIONS_CURRENCIES_HPP

namespace Currencies {
  enum class Crypto {
    BTC,
    ETH,
    LTC,
    DOGE,
  };

  enum class Fiat {
    AUD,
  };

  struct Definition {
    const char* name;
    const char symbol;
  };

  constexpr Definition getDefinition(const Crypto currency) {
    switch(currency) {
      default:
      case Crypto::BTC: return {"Bitcoin", ' '};
      case Crypto::ETH: return {"Ethereum", ' '};
      case Crypto::LTC: return {"Litecoin", ' '};
      case Crypto::DOGE: return {"Dogecoin", ' '};
    }
  };

  constexpr Definition getDefinition(const Fiat currency) {
    switch(currency) {
      default:
      case Fiat::AUD: return {"Australian Dollar", '$'};
    }
  };
};  // namespace Currencies

#endif  // __DEFINITIONS_CURRENCIES_HPP