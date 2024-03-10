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

void handleNotFound(AsyncWebServerRequest *request);  // ハンドル設定無し・要求ファイル取得

#define HTTP_PORT 80
AsyncWebServer server(HTTP_PORT);

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
