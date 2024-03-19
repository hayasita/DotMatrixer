/**
 * @file main.cpp
 * @author hayasita04@gmail.com
 * @brief DotMatrixer
 * @version 0.1
 * @date 2024-03-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <M5Unified.h>
#include <SPIFFS.h>
#include <FastLED.h> // enable FastLED (RGB LED) library.

#include "dotserver.h"
#include <ArduinoJson.h>
#include "jsdata.h"

// Specify the number of RGB LEDs (25 for M5Atom Matrix).
#define NUM_LEDS 25
// Specify DATA PIN of RGB LEDs.
#define LED_DATA_PIN 27

// global variables (define variables to be used throughout the program)
//uint32_t count;
CRGB leds[NUM_LEDS];
extern portMUX_TYPE jsonMutex; // Mutex


//const char* ssid = "**********";
//const char* password = "**********";

void led(int num ,JsonObject data2)
{
  int r = data2["r"];
  int g = data2["g"];
  int b = data2["b"];

  leds[num] = CRGB(r, g, b);
}

/**
 * @brief デバイス制御タスク
 * 
 * @param Parameters 
 */
void taskDeviceCtrl(void *Parameters){
  unsigned long timetmp;      // millis()tmp
  unsigned long ledLasttime;
  bool ledOnOff = 0;

  JsonVariant jsonData;

  // LED setup
  FastLED.addLeds<WS2811, LED_DATA_PIN, GRB>(leds, NUM_LEDS); // initialize RGB LEDs
  FastLED.setBrightness(10); // set the brightness (more than 20 may break due to heat.)

  int count=0;

  while(1){
    timetmp = millis();               // 処理間隔確認用基本時刻

    if(timetmp - ledLasttime >500){   // 500mSecごとに実行
      ledLasttime = timetmp;

      if(!jsData.ledAllData.empty()){
//        Serial.println("jsonData::");
//        Serial.println(jsData.ledAllData.size());

        portENTER_CRITICAL(&jsonMutex);
        count++;
        if(count >= jsData.ledAllData.size()){
          count = 0;
        }
        int i,j,num;
        for(i=0;i<5;i++){
          for(j=0;j<5;j++){
            num = i*16+j;
            leds[i*5+j] = CRGB(
              jsData.ledAllData[count].ledPageData[num].r,
              jsData.ledAllData[count].ledPageData[num].g,
              jsData.ledAllData[count].ledPageData[num].b
            );
          }
        }
        portEXIT_CRITICAL(&jsonMutex);
        FastLED.show();
      
      }
      else{
        count = 0;
      }

/*      ledLasttime = timetmp;
      if(ledOnOff){
        leds[0] = CRGB::Red; // set LED[0] to red
      }
      else{
        leds[0] = CRGB::Black; // set LED to black
      }
      ledOnOff = !ledOnOff;
      FastLED.show(); // display LEDs
*/
    }

    delay(1);
  }

}

/**
 * @brief setup
 * 
 */
void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);

  Serial.begin(115200);
  while (!Serial);

  // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Core1で関数taskDeviceCtrlをstackサイズ4096,優先順位1で起動
  xTaskCreatePinnedToCore(taskDeviceCtrl, "taskDeviceCtrl", 4096, NULL, 1, NULL, 1);

  // Start WiFi & WebServer
  setWebhandle();       // Webhandle設定
  startWiFi();          // WiFi接続開始
  startWebserver();     // Webサーバ開始

}

/**
 * @brief main loop
 * 
 */
void loop() {
}