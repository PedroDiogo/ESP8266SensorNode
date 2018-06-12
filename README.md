# ESP8266SensorNode

[![Build Status](https://travis-ci.org/PedroDiogo/ESP8266SensorNode.svg?branch=master)](https://travis-ci.org/PedroDiogo/ESP8266SensorNode)

This is a simple project to read some sensors - Temperature and Humidity at the moment - and send the readings to a MQTT broker.

This project is prepared to be deployed and configured without needing any code changes, through the [WifiManager](https://github.com/tzapu/WiFiManager) interface.

This project can be built using [Platform IO](https://platformio.org/).

## Requirements

* ESP8266 board
* DHT22 Sensor

The DHT22 sensor should be connected to GPIO Pin 2 of the ESP8266 board.

## MQTT 

These MQTT topics will be used to publish the sensor readings. The default polling interval for the sensors is 15 seconds.
* `/sensors/<device id>/temperature`
* `/sensors/<device id>/humidity`
* `/sensors/<device id>/heatindex`

Both temperature readings - `temperature` and `heatindex` - are sent in Celsius, while `humidity` is sent in percentage.

The `<device id>` can be customized via the WiFiManager UI, during the first initialization of the sensor node.

The `sensors` prefix can be changed by changing the `PREFIX_DEFAULT` in `Constants.h`.

## Credits

* [WifiManager](https://github.com/tzapu/WiFiManager)
* [ArduinoJSON](https://arduinojson.org/)
* [PubSubCliennt](https://github.com/knolleary/pubsubclient)
* [Adafruit Unified Sensor Driver](https://github.com/adafruit/Adafruit_Sensor)
* [Adafruit DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library)

## To Do
- [ ] Unit tests
- [x] Add Deep Sleep Support (waiting for a new NodeMCU board to arrive to test this)
- [ ] Support for Pressure Sensor
- [ ] Support for Light Sensor