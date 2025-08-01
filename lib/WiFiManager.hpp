#ifndef _WIFI_MANAGER_HPP_
#define _WIFI_MANAGER_HPP_

#include <WiFi.h>
#include <WString.h>

namespace CONFIG {
    extern const char* SSID;
    extern const char* PASSWORD;
    extern const char* APIENDPOINT;
}

class WiFiManager {
public:
    static bool connect();
    static bool isConnected();
    static String getLocalIP();
};

#endif // _WIFI_MANAGER_HPP_


