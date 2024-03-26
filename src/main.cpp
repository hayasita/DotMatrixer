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

#include "dotserver.h"
#include <ArduinoJson.h>
#include "jsdata.h"

#include "monitor_real_device.h"
#include "monitor.h"

#include "matrix_driver.h"

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

  // serialMonitor init
    RealMonitorDeviseIo real;
    SerialMonitor serialMonitor(&real);

  MatrixDriverMAX72XX mx;   // MAX7219 Matrix Driver

  // LED setup
  M5ATOMMatrixLED m5mx;

  int count=0;

  while(1){
    timetmp = millis();               // 処理間隔確認用基本時刻

    if(timetmp - ledLasttime >500){   // 500mSecごとに実行
      ledLasttime = timetmp;

      if(!jsData.empty()){
//        Serial.println("jsonData::");
//        Serial.println(jsData.ledAllData.size());

        count++;
        if(count >= jsData.size()){
          count = 0;
        }

        // M5ATOM Matrix LED 
        m5mx.matrixset(jsData.getPageData(count));

        // MAX7219 Matrix Driver
        if(!mx.matrixset(jsData.getPageData(count))){
          Serial.println("Data Error!");
        }

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

    // シリアルモニタ処理
    serialMonitor.exec();

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