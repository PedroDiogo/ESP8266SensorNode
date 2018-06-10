#include "Arduino.h"
#include "WiFiManagerHandler.h"
#include "ConfigManager.h"
#include <WiFiManager.h>

bool shouldSaveConfig = false;

void saveConfigCallback()
{
  if (Serial)
  {
    Serial.println("Should save config");
  }
  shouldSaveConfig = true;
}

WifiManagerHandler::WifiManagerHandler(ConfigManager * config)
{
  _config = config;
}

void WifiManagerHandler::connectAndSetConfigs()
{
  _wifiManager.setTimeout(180);
  _wifiManager.setSaveConfigCallback(saveConfigCallback);
  configureCustomParameters();

  if (!_wifiManager.autoConnect(CONFIGURATION_AP_SSID))
  {
    if (Serial)
    {
      Serial.println("Failed to connect and hit timeout");
    }
    delay(1000);
    ESP.reset();
    delay(5000);
  }

  saveCustomParameters();
}

void WifiManagerHandler::reset() {
  _wifiManager.resetSettings();
}

void WifiManagerHandler::configureCustomParameters()
{
  char port[MQTT_PORT_LENGTH + 1];
  sprintf(port, "%d", _config->getMQTTPort());

  _mqtt_server_param = WiFiManagerParameter(MQTT_SERVER_KEY, "mqtt server", _config->getMQTTServer(), MQTT_SERVER_LENGTH);
  _mqtt_port_param = WiFiManagerParameter(MQTT_PORT_KEY, "mqtt port", port, MQTT_PORT_LENGTH);
  _mqtt_username_param = WiFiManagerParameter(MQTT_USERNAME_KEY, "mqtt username", _config->getMQTTUsername(), MQTT_USERNAME_LENGTH);
  _mqtt_password_param = WiFiManagerParameter(MQTT_PASSWORD_KEY, "mqtt password", _config->getMQTTPassword(), MQTT_PASSWORD_LENGTH);
  _device_id_param = WiFiManagerParameter(DEVICE_ID_KEY, "device ide ntifier", _config->getDeviceID(), DEVICE_ID_LENGTH);

  _wifiManager.addParameter(&_mqtt_server_param);
  _wifiManager.addParameter(&_mqtt_port_param);
  _wifiManager.addParameter(&_mqtt_username_param);
  _wifiManager.addParameter(&_mqtt_password_param);
  _wifiManager.addParameter(&_device_id_param);
}

void WifiManagerHandler::saveCustomParameters()
{
  if (shouldSaveConfig)
  {
    _config->setMQTTServer(_mqtt_server_param.getValue());
    _config->setMQTTPort(atoi(_mqtt_port_param.getValue()));
    _config->setMQTTUsername(_mqtt_username_param.getValue());
    _config->setMQTTPassword(_mqtt_password_param.getValue());
    _config->setDeviceID(_device_id_param.getValue());
    _config->save();
  }
}