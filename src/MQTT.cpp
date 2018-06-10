#include "MQTT.h"
#include <ESP8266WiFi.h>

WiFiClient espClient;

MQTT::MQTT(const char *mqtt_server, int mqtt_port, const char *mqtt_username, const char *mqtt_password, const char *prefix, const char *device_id)
{
  _client = PubSubClient(espClient);
  _client.setServer(mqtt_server, mqtt_port);

  strlcpy(_mqtt_username, mqtt_username, MQTT_USERNAME_LENGTH);
  strlcpy(_mqtt_password, mqtt_password, MQTT_PASSWORD_LENGTH);
  strlcpy(_prefix, prefix, PREFIX_LENGTH);
  strlcpy(_device_id, device_id, DEVICE_ID_LENGTH);
}
void MQTT::loop()
{
  _client.loop();
}

void MQTT::publish(const char *topic, float value)
{
  reconnect();

  if (_client.connected()) {
    char fullTopic[64];
    sprintf(fullTopic, "%s/%s/%s", _prefix, _device_id, topic);

    char message[16];
    sprintf(message, "%.2f", value);

    _client.publish(fullTopic, message);
  } else {
    if (Serial) { Serial.println("[MQTT] Couldn't publish message."); }
  }
}

void MQTT::reconnect()
{
  int retries = 5;
  while (!_client.connected() && retries > 0)
  {
    if (Serial)
    {
      Serial.println("[MQTT] Attempting MQTT connection...");
    }

    char clientId[32];
    sprintf(clientId, "ESP8266Client-%lX", random(millis()));

    if (_client.connect(clientId, _mqtt_username, _mqtt_password))
    {
      if (Serial)
      {
        Serial.print("[MQTT] Connected with ClientID=");
        Serial.println(clientId);
      }
    }
    else
    {
      if (Serial)
      {
        Serial.print("[MQTT] Failed to connect. RC=");
        Serial.print(_client.state());
        Serial.println("Trying again in 5 seconds.");
      }
      delay(5000);
      retries--;
    }
  }
}