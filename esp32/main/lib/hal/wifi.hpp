#ifndef __HAL_WIFI_HPP
#define __HAL_WIFI_HPP

#include <cstddef>

namespace HAL {
  enum WiFiConnectionState {
    CONNECTING,
    CONNECTED,
    LOST_CONNECTION,
    PROVISIONED,
    NOT_PROVISIONED,
    PROVISIONING_STARTED,
    PROVISIONING_RECEVIED_CREDENTIALS,
    PROVISIONING_BAD_CREDENTIALS,
    PROVISIONING_AP_LOST,
    PROVISIONING_SUCCESS,
    PROVISIONING_COMPLETE,
  };

  class Wifi {
   public:
    Wifi(const Wifi&) = delete;
    Wifi(Wifi&&) = delete;
    Wifi& operator=(const Wifi&) = delete;
    Wifi& operator=(Wifi&&) = delete;

    WiFiConnectionState status();
    void connect();
    void disconnect();
    void startProvisioning(const char* ssid, const char* password, const char* popCode);
    void stopProvisioning();
    void resetProvisioningCredentials();
    void getQRCodeData(char* buffer, size_t maxLength, const char* ssid, const char* popCode);

    static Wifi* instance() {
      static Wifi _instance;
      return &_instance;
    }

   private:
    Wifi();
    void _init();
  };
  constexpr auto WiFi = Wifi::instance;
};  // namespace HAL

#endif  // __HAL_WIFI_HPP