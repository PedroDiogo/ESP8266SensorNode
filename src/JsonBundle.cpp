#include "JsonBundle.h"

void JsonBundle::parse(const char* json) {
    _jsonVariant = _jsonBuffer.parseObject(json);
}

const JsonObject& JsonBundle::root() const { 
    return _jsonVariant;
}