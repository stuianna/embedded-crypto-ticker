#include "ssl_certificate.hpp"

using namespace HAL::SSLCertificates;

extern const char coin_gecko_root_cert_pem_start[] asm("_binary_coin_gecko_root_cert_pem_start");
extern const char coin_gecko_root_cert_pem_end[] asm("_binary_coin_gecko_root_cert_pem_end");
const size_t coin_gecko_root_cert_length = coin_gecko_root_cert_pem_end - coin_gecko_root_cert_pem_start;

Certificate HAL::SSLCertificates::get(const StoredCertificates certificate) {
  switch(certificate) {
    default:
    case StoredCertificates::COIN_GECKO: return {coin_gecko_root_cert_pem_start, coin_gecko_root_cert_length};
  }
}
