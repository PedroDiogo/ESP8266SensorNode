#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "ArduinoJson.h"
#include "SensorFS.h"
#include "Constants.h"

class ConfigManager
{
public:
  ConfigManager();
  void save();

  char *getMQTTServer();
  int getMQTTPort();
  char *getMQTTUsername();
  char *getMQTTPassword();
  char *getDeviceID();

  void setMQTTServer(const char *mqtt_server);
  void setMQTTPort(int mqtt_port);
  void setMQTTUsername(const char *mqtt_username);
  void setMQTTPassword(const char *mqtt_password);
  void setDeviceID(const char *device_id);

private:
  char _mqtt_server[MQTT_SERVER_LENGTH + 1];
  int _mqtt_port;
  char _mqtt_username[MQTT_USERNAME_LENGTH];
  char _mqtt_password[MQTT_PASSWORD_LENGTH];
  char _device_id[DEVICE_ID_LENGTH + 1];

  SensorFS fs;

  void setDefaults();
  int LoadConfigFromJSONFile();
  JsonObject &generateJSONFileFromConfigs();
};
#endif