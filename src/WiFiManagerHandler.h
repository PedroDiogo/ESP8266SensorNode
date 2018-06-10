#ifndef WIFIMANAGERHANDLER_H
#define WIFIMANAGERHANDLER_H

#include <WiFiManager.h>
#include "ConfigManager.h"

#define CONFIGURATION_AP_SSID "HomeSensorsConfig"

class WifiManagerHandler
{
public:
  WifiManagerHandler(ConfigManager *config);
  void connectAndSetConfigs();
  void reset();

private:
  WiFiManagerParameter _mqtt_server_param = NULL;
  WiFiManagerParameter _mqtt_port_param = NULL;
  WiFiManagerParameter _mqtt_username_param = NULL;
  WiFiManagerParameter _mqtt_password_param = NULL;
  WiFiManagerParameter _device_id_param = NULL;

  ConfigManager * _config;
  WiFiManager _wifiManager;

  void configureCustomParameters();
  void saveCustomParameters();
};
#endif