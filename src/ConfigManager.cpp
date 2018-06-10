#include "Arduino.h"
#include "ConfigManager.h"
#include "ArduinoJson.h"
#include <string.h>

ConfigManager::ConfigManager()
{
  fs = SensorFS();
  if (LoadConfigFromJSONFile() < 0)
  {
    setDefaults();
  }
}

char *ConfigManager::getMQTTServer()
{
  return _mqtt_server;
}

int ConfigManager::getMQTTPort()
{
  return _mqtt_port;
}

char *ConfigManager::getDeviceID()
{
  return _device_id;
}

char *ConfigManager::getMQTTUsername()
{
  return _mqtt_username;
}

char *ConfigManager::getMQTTPassword()
{
  return _mqtt_password;
}

void ConfigManager::setMQTTServer(const char *mqtt_server)
{
  strlcpy(_mqtt_server, mqtt_server, MQTT_SERVER_LENGTH + 1);
}

void ConfigManager::setMQTTPort(int mqtt_port)
{
  _mqtt_port = mqtt_port;
}

void ConfigManager::setMQTTUsername(const char *mqtt_username)
{
  strlcpy(_mqtt_username, mqtt_username, MQTT_USERNAME_LENGTH + 1);
}

void ConfigManager::setMQTTPassword(const char *mqtt_password)
{
  strlcpy(_mqtt_password, mqtt_password, MQTT_PASSWORD_LENGTH + 1);
}

void ConfigManager::setDeviceID(const char *device_id)
{
  strlcpy(_device_id, device_id, DEVICE_ID_LENGTH + 1);
}

void ConfigManager::save()
{
  if (Serial)
  {
    Serial.println("[Configuration] Saving configurations");
  }

  if (fs.saveJSONFile(generateJSONFileFromConfigs(), CONFIG_JSON_FILE) < 0)
  {
    if (Serial)
    {
      Serial.println("[Configuration] Failed to save configurations.");
    }
  }
}

int ConfigManager::LoadConfigFromJSONFile()
{
  JsonBundle jsonBundle;
  if (fs.getJSONFile(CONFIG_JSON_FILE, &jsonBundle) >= 0)
  {
    if (Serial)
    {
      Serial.println("[Configuration] Setting configurations from file.");
    }

    setMQTTServer(jsonBundle.root()[MQTT_SERVER_KEY]);
    setMQTTPort(jsonBundle.root()[MQTT_PORT_KEY]);
    setMQTTUsername(jsonBundle.root()[MQTT_USERNAME_KEY]);
    setMQTTPassword(jsonBundle.root()[MQTT_PASSWORD_KEY]);
    setDeviceID(jsonBundle.root()[DEVICE_ID_KEY]);
    return 0;
  }
  else
  {
    return -1;
  }
}

JsonObject &ConfigManager::generateJSONFileFromConfigs()
{
  if (Serial)
  {
    Serial.println("[Configuration] Generating JSON file");
  }

  DynamicJsonBuffer jsonBuffer;
  JsonObject &json = jsonBuffer.createObject();

  json[MQTT_SERVER_KEY] = getMQTTServer();
  json[MQTT_PORT_KEY] = getMQTTPort();
  json[MQTT_USERNAME_KEY] = getMQTTUsername();
  json[MQTT_PASSWORD_KEY] = getMQTTPassword();
  json[DEVICE_ID_KEY] = getDeviceID();

  return json;
}

void ConfigManager::setDefaults()
{
  if (Serial)
  {
    Serial.println("[Configuration] Setting configurations from defaults.");
  }
  
  setMQTTServer(MQTT_SERVER_DEFAULT);
  setMQTTPort(MQTT_PORT_DEFAULT);
  setMQTTUsername(MQTT_USERNAME_DEFAULT);
  setMQTTPassword(MQTT_PASSWORD_DEFAULT);
  setDeviceID(DEVICE_ID_DEFAULT);
}
