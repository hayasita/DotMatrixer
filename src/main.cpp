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
#include <FastLED.h> // enable FastLED (RGB LED) library.
#include <WebServer.h>

// Specify the number of RGB LEDs (25 for M5Atom Matrix).
#define NUM_LEDS 25
// Specify DATA PIN of RGB LEDs.
#define LED_DATA_PIN 27

// global variables (define variables to be used throughout the program)
//uint32_t count;
CRGB leds[NUM_LEDS];

WiFiServer server(80);

//const char* ssid = "**********";
//const char* password = "**********";

/**
 * @brief デバイス制御タスク
 * 
 * @param Parameters 
 */
void taskDeviceCtrl(void *Parameters){
  unsigned long timetmp;      // millis()tmp
  unsigned long ledLasttime;
  bool ledOnOff = 0;

  while(1){
    timetmp = millis();               // 処理間隔確認用基本時刻
    if(timetmp - ledLasttime >500){   // 500mSecごとに実行
      ledLasttime = timetmp;
      if(ledOnOff){
        leds[0] = CRGB::Red; // set LED[0] to red
      }
      else{
        leds[0] = CRGB::Black; // set LED to black
      }
      ledOnOff = !ledOnOff;
      FastLED.show(); // display LEDs
    }

    delay(1);
  }

}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  auto cfg = M5.config(); // assign a structure for initializing M5Stack
  // If config is to be set, set it here
  // Example.
  // cfg.external_spk = true;

  M5.begin(cfg); // initialize M5 device, Display is also initialized

  FastLED.addLeds<WS2811, LED_DATA_PIN, GRB>(leds, NUM_LEDS); // initialize RGB LEDs
  FastLED.setBrightness(10); // set the brightness (more than 20 may break due to heat.)

//  WiFi.begin(ssid, password);
  WiFi.begin();
  Serial.print("WiFi connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println(" connected");

  server.begin();

  Serial.print("HTTP Server: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");


  // Core1で関数taskDeviceCtrlをstackサイズ4096,優先順位1で起動
  xTaskCreatePinnedToCore(taskDeviceCtrl, "taskDeviceCtrl", 4096, NULL, 1, NULL, 1);

}

void loop() {
}