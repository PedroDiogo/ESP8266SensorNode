#ifndef CONSTANTS_H
#define CONSTANTS_H

#define CONFIG_JSON_FILE "/config.json"

#define MQTT_SERVER_KEY   "mqtt_server"
#define MQTT_PORT_KEY     "mqtt_port"
#define MQTT_USERNAME_KEY "mqtt_username"
#define MQTT_PASSWORD_KEY "mqtt_password"
#define DEVICE_ID_KEY     "device_id"

#define MQTT_SERVER_LENGTH   64
#define MQTT_PORT_LENGTH     5
#define MQTT_USERNAME_LENGTH 32
#define MQTT_PASSWORD_LENGTH 32
#define DEVICE_ID_LENGTH     32
#define PREFIX_LENGTH        32

#define MQTT_SERVER_DEFAULT   "test.mosquitto.org"
#define MQTT_PORT_DEFAULT     1883
#define MQTT_USERNAME_DEFAULT ""
#define MQTT_PASSWORD_DEFAULT ""
#define DEVICE_ID_DEFAULT     "ESP"
#define PREFIX_DEFAULT        "sensors"

#define SENSOR_POLLING_MS 15000

#endif