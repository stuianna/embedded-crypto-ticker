#ifndef __CRYPTO_COIN_GECKO_HPP
#define __CRYPTO_COIN_GECKO_HPP

#include <lvgl.h>

#include <array>
#include <utility>

namespace Crypto {
  class CoinGecko {
   public:
    CoinGecko();
    enum {
      REQUEST_FAILED = 0,
      JSON_PARSING_FAILED,
    };
    struct SimplePrice {
      float price;
      float marketCap;
      float volume24h;
      float change24h;
      size_t status;
    };
    SimplePrice simplePrice(const char* ids, const char* vsCurrency, bool marketCap, bool volume24h, bool change24h);
    std::pair<size_t, std::array<float, 288>> marketChartRange24h(const char* ids, const char* vsCurrency, size_t from, size_t to);

   private:
    void _append_boolean(bool value, char* buffer);
    SimplePrice _parse_simple_price(char* buffer, size_t httpStatus, const char* vsCurrency, const char* ids);
    void _makeMarketChartRequest(char* buffer, size_t length, const char* ids, const char* vsCurrency, size_t from, size_t to);
  };
};  // namespace Crypto

#endif  // __CRYPTO_COIN_GECKO_HPP