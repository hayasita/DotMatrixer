#ifndef jsdata_h
#define jsdata_h

#ifdef GLOBAL_VAL_DEF
#define GLOBAL
#else
#define GLOBAL extern
#endif

#include <ArduinoJson.h>
#include <M5Unified.h>

/**
 * @brief LED色データ
 * 
 */
class ledColor{
  public:
    int r;  // 赤データ
    int g;  // 緑データ
    int b;  // 青データ
};

/**
 * @brief LED 1ページ分データ
 * 
 */
class ledData{
  public:
    std::vector<ledColor> ledPageData;    // 1ページ分のLEDデータ
};

/**
 * @brief json内部データ管理
 * 
 */
class jsonData{
  public:
    jsonData(void);   // コンストラクタ
    void parseJson(JsonDocument& jsonDocument);
    ledData getPageData(uint8_t page);  // 任意のページのLEDデータ取得
    uint8_t size(void);
    uint8_t empty(void);
  private:
    portMUX_TYPE jsonMutex;
    JsonVariant jsonDataArray;
    std::vector<ledData> ledAllData;      // LEDアニメーションデータ
};

GLOBAL jsonData jsData;   // jsonデータ

#undef GLOBAL
#endif