#include <DHT.h>

#include "ConfigManager.h"
#include "WiFiManagerHandler.h"
#include "MQTT.h"

#define DHTPIN 2
#define DHTPOWERPIN D1
#define BUTTONPIN 0 // Tied to the Flash button.
#define LEDPIN 16   // NodeMCU onboard LED

DHT dht(DHTPIN, DHT22);

ConfigManager *config;
MQTT *mqtt;

void powerUpTemperatureSensor() {
  pinMode(DHTPOWERPIN, OUTPUT);
  digitalWrite(DHTPOWERPIN, HIGH);
}

void powerDownTemperatureSensor() {
  digitalWrite(DHTPOWERPIN, LOW);
  pinMode(DHTPOWERPIN, INPUT_PULLUP);
}

void initSerial()
{
  Serial.begin(9600);
  Serial.setTimeout(2000);

  while (!Serial);
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

void flushMQTTMessagesBeforeDeepSleep() {
  for(int i=0; i<10; i++) {
    mqtt->loop();
    delay(25);
  }
}

void blinkLed(int times) {
  pinMode(LEDPIN, OUTPUT);
  for(int i=0; i<times; i++) {
    delay(250);
    digitalWrite(LEDPIN, HIGH);
    delay(250);
    digitalWrite(LEDPIN, LOW);
  }
}

void shouldResetSettings() {
  pinMode(BUTTONPIN, INPUT_PULLUP);
  if (!digitalRead(BUTTONPIN)) {
    blinkLed(5);
    if (Serial) { Serial.println("[Reset] Resetting Configurations"); }
    WifiManagerHandler wifiManager = WifiManagerHandler(config);
    wifiManager.reset();
    SensorFS::format();
    ESP.reset();
  }
}

void setup()
{
  powerUpTemperatureSensor();
  initSerial();
  Serial.println("Device Started");

  config = new ConfigManager();
  connectToWifi();

  mqtt = new MQTT(config->getMQTTServer(), config->getMQTTPort(), config->getMQTTUsername(), config->getMQTTPassword(), PREFIX_DEFAULT, config->getDeviceID());
}

void loop()
{
  sendSensorMeasurements();
  powerDownTemperatureSensor();
  flushMQTTMessagesBeforeDeepSleep();
  shouldResetSettings();
  ESP.deepSleep(SENSOR_POLLING_US);
}