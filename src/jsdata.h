#ifndef jsdata_h
#define jsdata_h

#ifdef GLOBAL_VAL_DEF
#define GLOBAL
#else
#define GLOBAL extern
#endif

#include <ArduinoJson.h>
#include <M5Unified.h>

class ledColor{
  public:
    int r;
    int g;
    int b;
};

class ledData{
  public:
    std::vector<ledColor> ledPageData;

};

class jsonData{
  public:
    std::vector<ledData> ledAllData;
    void parseJson(JsonDocument& jsonDocument);

    void setDataArray(JsonVariant& data);
    JsonVariant getDataArray(void);
  private:
//    portMUX_TYPE jsonMutex = portMUX_INITIALIZER_UNLOCKED; // Mutex
    JsonVariant jsonDataArray;



};

GLOBAL jsonData jsData;

#undef GLOBAL
#endif