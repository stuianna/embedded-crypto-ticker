#ifndef __HAL_SSL_CERTIFICATE_HPP
#define __HAL_SSL_CERTIFICATE_HPP

#include <cstddef>

namespace HAL {
  namespace SSLCertificates {
    /**
     * @brief Available SSL certificates stored.
     */
    enum class StoredCertificates {
      COIN_GECKO,  ///< api.coingecko.com
    };

    /**
     * @brief Certificate raw bytes and length.
     */
    struct Certificate {
      const char* certificate;  ///< Raw certificate contents.
      size_t length;            ///< Length of the certificate.
    };

    /**
     * @brief Fetch a specific SSL certificate definition.
     * @param certificate One of the available stored SSL certificates.
     * @return Certificate The certificate corresponding to the definition.
     */
    Certificate get(const StoredCertificates certificate);
  };  // namespace SSLCertificates
};    // namespace HAL

#endif