#ifndef __WIFI_MANAGER_HPP
#define __WIFI_MANAGER_HPP

namespace WIFI {
  enum ConnectionState {
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

  class Manager {
   public:
    Manager(const Manager&) = delete;
    Manager(Manager&&) = delete;
    Manager& operator=(const Manager&) = delete;
    Manager& operator=(Manager&&) = delete;

    ConnectionState status();
    void connect();
    void disconnect();
    void startProvisioning(const char* ssid, const char* password, const char* popCode);
    void stopProvisioning();

    static Manager* instance() {
      static Manager _instance;
      return &_instance;
    }

   private:
    Manager();
    void _init();
  };
  constexpr auto controller = Manager::instance;
};  // namespace WIFI

#endif  // __WIFI_MANAGER_HPP