#include <DHT.h>

#include "ConfigManager.h"
#include "WiFiManagerHandler.h"
#include "MQTT.h"

#define DHTPIN 2
#define LEDPIN 2
#define BUTTONPIN 0
#define DTHPOLLING 15000

DHT dht(DHTPIN, DHT22);

ConfigManager *config;
MQTT *mqtt;

void initSerial()
{
  Serial.begin(9600);
  Serial.setTimeout(2000);

  while (!Serial)
  {
  }
}

void connectToWifi()
{
  WifiManagerHandler wifiManager = WifiManagerHandler(config);
  wifiManager.connectAndSetConfigs();
}

bool isWifiConnected() {
  return WiFi.status() == WL_CONNECTED;
}
int sendSensorMeasurements()
{
  float humidity = dht.readHumidity();
  float temperatureC = dht.readTemperature();

  if (isnan(humidity) || isnan(temperatureC))
  {
    if (Serial) { Serial.println("[DHT] Failed to read from DHT sensor!"); }
    return -1;
  }

  float heatIndexC = dht.computeHeatIndex(temperatureC, humidity, false);

  mqtt->publish("temperature", temperatureC);
  mqtt->publish("humidity", humidity);
  mqtt->publish("heatindex", heatIndexC);

  char serialOutput[96];
  sprintf(serialOutput, "[DHT] Temperature: %.2fC | Humidity: %.2f%% | Heat Index: %.2fC", temperatureC, humidity, heatIndexC);  
  if (Serial) { Serial.println(serialOutput); }

  return 0;
}

void shouldResetSettings() {
  pinMode(BUTTONPIN, INPUT_PULLUP);
  if (!digitalRead(BUTTONPIN)) {
    if (Serial) { Serial.println("[Reset] Resetting Configurations"); }
    WifiManagerHandler wifiManager = WifiManagerHandler(config);
    wifiManager.reset();
    SensorFS::format();
    ESP.reset();
  }
}

void setup()
{
  initSerial();
  Serial.println("Device Started");

  config = new ConfigManager();
  connectToWifi();

  mqtt = new MQTT(config->getMQTTServer(), config->getMQTTPort(), config->getMQTTUsername(), config->getMQTTPassword(), PREFIX_DEFAULT, config->getDeviceID());
}

int timeSinceLastRead = 0;
void loop()
{
  if (!isWifiConnected()) {
    connectToWifi();
  }
  
  mqtt->loop();

  if (timeSinceLastRead > SENSOR_POLLING_MS)
  {
    sendSensorMeasurements();
    timeSinceLastRead = 0;
  }
  delay(100);
  timeSinceLastRead += 100;
  shouldResetSettings();
}