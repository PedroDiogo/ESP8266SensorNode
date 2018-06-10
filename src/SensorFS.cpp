#include <FS.h>
#include "SensorFS.h"
#include "JsonBundle.h"

int SensorFS::getJSONFile(const char *filename, JsonBundle * jsonBundle)
{
  if (!SPIFFS.begin())
  {
    if (Serial)
    {
      Serial.println("[FS] Couldn't mount filesystem.");
    }
    return -1;
  }

  if (!SPIFFS.exists(filename))
  {
    if (Serial)
    {
      Serial.println("[FS] File doesn't exist.");
    }
    SPIFFS.end();
    return -1;
  }

  File file = SPIFFS.open(filename, "r");
  if (!file)
  {
    if (Serial)
    {
      Serial.println("[FS] Couldn't open file for reading.");
    }
    SPIFFS.end();
    return -1;
  }

  int fileSize = file.size();
  std::unique_ptr<char[]> fileBuffer(new char[fileSize]);
  file.readBytes(fileBuffer.get(), fileSize);

  jsonBundle->parse(fileBuffer.get());
  if (jsonBundle->root().success())
  {
    if (Serial)
    {
      Serial.println("[FS] Successfully read file.");
      jsonBundle->root().prettyPrintTo(Serial);
      Serial.println();
    }
    SPIFFS.end();

    return 0;
  }
  else
  {
    if (Serial)
    {
      Serial.println("[FS] Coudln't parse JSON file.");
    }
    SPIFFS.end();
    return -1;
  }
}

int SensorFS::saveJSONFile(JsonObject &json, const char *filename)
{
  if (!SPIFFS.begin())
  {
    if (Serial)
    {
      Serial.println("[FS] Couldn't mount filesystem.");
    }
    return -1;
  }
  File file = SPIFFS.open(filename, "w");
  if (!file)
  {
    if (Serial)
    {
      Serial.println("[FS] Failed to open config file for writing");
    }
    SPIFFS.end();
    return -1;
  }

  if (Serial)
  {
    Serial.print("[FS] Saving file ");
    Serial.println(filename);
    json.printTo(Serial);
    Serial.println();
  }

  json.printTo(file);
  file.close();
  SPIFFS.end();
  return 0;
}

void SensorFS::format() {
  SPIFFS.format();
}