#ifndef SENSOR_FS
#define SENSOR_FS

#include "ArduinoJson.h"
#include "JsonBundle.h"

class SensorFS
{
public:
  int getJSONFile(const char *filename, JsonBundle * jsonBundle);
  int saveJSONFile(JsonObject &json, const char *filename);
  static void format();
};
#endif