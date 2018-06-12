#ifndef MQTT_H
#define MQTT_H

#include "Constants.h"
#include <PubSubClient.h>

class MQTT
{
public:
  MQTT(const char *mqtt_server, int mqtt_port, const char *mqtt_username, const char *mqtt_password, const char *prefix, const char *device_id);
  bool loop();
  bool publish(const char *topic, float value);

private:
  char _mqtt_username[MQTT_USERNAME_LENGTH + 1];
  char _mqtt_password[MQTT_PASSWORD_LENGTH + 1];

  char _prefix[PREFIX_LENGTH];
  char _device_id[DEVICE_ID_LENGTH];

  PubSubClient _client;

  void reconnect();
};
#endif