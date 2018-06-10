#ifndef JSONBUNDLE_H
#define JSONBUNDLE_H

#include "ArduinoJson.h"

struct JsonBundle {
  public:
    void parse(const char* json);
    const JsonObject& root() const;

  private:
    DynamicJsonBuffer _jsonBuffer;   
    JsonVariant _jsonVariant;
};
#endif