/**
 * @file dotserver.cpp
 * @author hayasita04@gmail.com
 * @brief Webサーバ基本操作
 * @version 0.1
 * @date 2024-03-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <SPIFFS.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "dotserver.h"

#include <FastLED.h> // enable FastLED (RGB LED) library.


void handleNotFound(AsyncWebServerRequest *request);  // ハンドル設定無し・要求ファイル取得
void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

#define HTTP_PORT 80
AsyncWebServer server(HTTP_PORT);
AsyncWebSocket ws("/ws");

/**
 * @brief WiFi接続開始
 * 
 */
void startWiFi(void)
{
//  WiFi.begin(ssid, password);
  WiFi.begin();
  Serial.print("WiFi connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println(" connected");

  Serial.print("HTTP Server: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  return;
}

/**
 * @brief webサーバ初期化
 * 
 */
void startWebserver(void)
{
  // WebSocketイベントのハンドラを設定
  ws.onEvent(onWebSocketEvent);

  // WebSocketエンドポイントを登録
  server.addHandler(&ws);

  server.begin();
  Serial.println("HTTP server started");

  return;
}

/**
 * @brief Set the Webhandle object
 * 
 */
void setWebhandle(void){

  server.onNotFound(handleNotFound);

  return;
}

/**
 * @brief Get the Content Type object
 * MIMEタイプを推定
 * 
 * @param filename 
 * @return String 
 */
String getContentType(String filename)
{
  if(filename.endsWith(".html") || filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else return "text/plain";
}

/**
 * @brief 要求されたSPIFSS のファイルをクライアントに転送する
 * 
 * @param request 
 */
void handleNotFound(AsyncWebServerRequest *request)
{
  String path = request->url();
  Serial.println(">> handleFileRead: trying to read " + path);
  // パス指定されたファイルがあればクライアントに送信する
  if (path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  Serial.println("contentType: " + contentType);
  if (SPIFFS.exists(path)) {
    Serial.println("handleFileRead: sending " + path);
    File file = SPIFFS.open(path, "r");
    request->send(SPIFFS, path, String(), false);
    return;
  }
  else {
    Serial.println("handleFileRead: 404 not found");
    request->send(404);
    return;
  }
}

extern CRGB leds[25];   // LEDオブジェクトの配列

void led(int num ,JsonObject data2)
{
  int r = data2["r"];
  int g = data2["g"];
  int b = data2["b"];

  leds[num] = CRGB(r, g, b);
}

void parseJsonAndPrintColors(JsonDocument& jsonDocument) {

   Serial.println("-- parseJsonAndPrintColors --");
  JsonDocument jsonDoc;

  JsonArray data = jsonDocument["dataArray"];
  Serial.println(data);

  JsonVariant jsondata;
    jsondata = jsonDocument["type"];
    if(!jsondata.isNull()){
      const char* dat = jsondata.as<const char*>();
     Serial.println((char*)dat);
    }

  jsondata = jsonDocument["dataArray"];
  if(!jsondata.isNull()){
/*
      serializeJsonPretty(jsondata, Serial);
      Serial.println(); // 改行を追加
      Serial.println(jsondata.size());

      JsonObject data1;
//      serializeJsonPretty(jsondata[0], Serial);
      Serial.println(); // 改行を追加
      data1 = jsondata[0];
      Serial.println("xxx"); // 改行を追加
      Serial.println(data1.size());
//      serializeJsonPretty(data1[0], Serial);
      Serial.println(); // 改行を追加


  // JsonVariant型の変数から配列の2つめの1つめの要素を取得
  JsonObject data2 = jsondata[0][0];

  // RGB値を取得
  int r = data2["r"];
  int g = data2["g"];
  int b = data2["b"];

  // 取得した値をシリアルモニタに出力
  Serial.print("RGB values for the second data array's first element: ");
  Serial.print("R: "); Serial.print(r);
  Serial.print(", G: "); Serial.print(g);
  Serial.print(", B: "); Serial.println(b);

  // JsonVariant型の変数から配列の2つめの1つめの要素を取得
  data2 = jsondata[1][0];

  // RGB値を取得
   r = data2["r"];
   g = data2["g"];
   b = data2["b"];

  // 取得した値をシリアルモニタに出力
  Serial.print("RGB values for the second data array's first element: ");
  Serial.print("R: "); Serial.print(r);
  Serial.print(", G: "); Serial.print(g);
  Serial.print(", B: "); Serial.println(b);
*/
/*
  data2 = jsondata[0][0];
  led(0 ,data2);

  data2 = jsondata[0][1];
  led(1 ,data2);

  data2 = jsondata[0][2];
  led(2 ,data2);

  data2 = jsondata[0][3];
  led(3 ,data2);

  data2 = jsondata[0][4];
  led(4 ,data2);
*/
  JsonObject data2; 
  int i,j,num;
  for(i=0;i<5;i++){
    for(j=0;j<5;j++){
      num = i*16+j;
      data2 = jsondata[0][num];
      led(i*5+j ,data2);
    }
  }

  FastLED.show();

  }

}


      String receivedData;
// WebSocketイベントのハンドラ
void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch(type) {
    case WS_EVT_CONNECT:
      Serial.println("WebSocket client connected");
      receivedData = "";
      break;
    case WS_EVT_DISCONNECT:{
      Serial.println("WebSocket client disconnected");
//      Serial.println(receivedData);

            // ArduinoJsonのためのメモリバッファを定義
      const size_t bufferSize = JSON_OBJECT_SIZE(10); // 10はオブジェクト内のキーの数
      StaticJsonDocument<bufferSize> jsonDocument;

      // 受信したJSONデータをパースする
      DeserializationError error = deserializeJson(jsonDocument, receivedData);
      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }

      parseJsonAndPrintColors(jsonDocument);

      // パースしたデータをシリアルモニタに表示する
    //  serializeJsonPretty(jsonDocument, Serial);
      Serial.println(); // 改行を追加
    }
      break;
    case WS_EVT_DATA:{
      // 受信データを std::string に変換する
      std::string receivedData0(reinterpret_cast<char*>(data), len);
      // 受信データを処理する（ここではシリアルに出力するだけ）
//      Serial.printf("Received data: %s\n", receivedData0.c_str());


      for (size_t i = 0; i < len; i++) {
        receivedData += (char)data[i];
      }
//      Serial.println(receivedData);


      // クライアントにデータを送信する例
      // client->text("Received your message");
    }
      break;
    
    default:
      break;
  }
}