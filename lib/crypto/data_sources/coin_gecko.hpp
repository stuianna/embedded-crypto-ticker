#ifndef __DATA_SOURCES_COIN_GECKO_HPP
#define __DATA_SOURCES_COIN_GECKO_HPP

#include <lvgl.h>

#include <array>
#include <utility>

namespace Crypto {
  namespace DataSources {
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
    };
  };  // namespace DataSources
};    // namespace Crypto

#endif  // __DATA_SOURCES_COIN_GECKO_HPP