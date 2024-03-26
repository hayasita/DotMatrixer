/**
 * @file jsdata.cpp
 * @author hayasita04@gmail.com
 * @brief jsonデータ処理
 * @version 0.1
 * @date 2024-03-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "jsdata.h"

jsonData::jsonData(void)
{
  jsonMutex = portMUX_INITIALIZER_UNLOCKED; // Mutex
  return;
}

/**
 * @brief JSONデータのパース
 * 
 * @param jsonDocument 
 */
void jsonData::parseJson(JsonDocument& jsonDocument)
{
   Serial.println("-- parseJson --");
  JsonDocument jsonDoc;

  JsonArray data = jsonDocument["dataArray"];
  Serial.println(data);

  JsonVariant jsondata;
    jsondata = jsonDocument["type"];
    if(!jsondata.isNull()){
      const char* dat = jsondata.as<const char*>();
     Serial.println((char*)dat);
    }

  jsonDataArray = jsonDocument["dataArray"];
  if(!jsonDataArray.isNull()){
    Serial.print("dataArray.size : ");
    Serial.println(jsonDataArray.size());

    portENTER_CRITICAL(&jsonMutex);
    ledAllData.clear();
    for(int i=0;i<jsonDataArray.size();i++){
      ledData PageData;
      Serial.print("datapage.size : ");
      Serial.println(jsonDataArray[i].size());
      for(int j=0; j<jsonDataArray[i].size(); j++){
        ledColor dat;
        dat.r = jsonDataArray[i][j]["r"];
        dat.g = jsonDataArray[i][j]["g"];
        dat.b = jsonDataArray[i][j]["b"];
        PageData.ledPageData.push_back(dat);
      }
      ledAllData.push_back(PageData);
      Serial.println(ledAllData[i].ledPageData[0].r);
    }
    portEXIT_CRITICAL(&jsonMutex);

  }

  return;
}

ledData jsonData::getPageData(uint8_t page)  // 任意のページのLEDデータ取得
{
  ledData tmpdata;
  if((ledAllData.size()-1)>= page){
    portENTER_CRITICAL(&jsonMutex);
    tmpdata = ledAllData[page];
    portEXIT_CRITICAL(&jsonMutex);
  }
  return tmpdata;
}

uint8_t jsonData::size(void)
{
  uint8_t ret;

  portENTER_CRITICAL(&jsonMutex);
  ret = ledAllData.size();
  portEXIT_CRITICAL(&jsonMutex);

  return ret;
}

uint8_t jsonData::empty(void)
{
  uint8_t ret;

  portENTER_CRITICAL(&jsonMutex);
  ret = ledAllData.empty();
  portEXIT_CRITICAL(&jsonMutex);

  return ret;
}

