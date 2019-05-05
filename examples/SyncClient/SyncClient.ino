#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#else
#include <ESP31BWiFi.h>
#endif
#include "ESPAsyncTCP.h"
#include "SyncClient.h"

#ifndef STASSID
#define STASSID "**********"
#endif
#ifndef STAPSK
#define STAPSK "************"
#endif
const char* ssid = STASSID2;
const char* password = STAPSK;

void setup(){
  Serial.begin(115200);
  delay(10);
  Serial.println();
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }
  Serial.printf("WiFi Connected!\n");
  Serial.println(WiFi.localIP());
#ifdef ESP8266
  ArduinoOTA.begin();
#endif

  SyncClient client;
  DEBUG_ESP_PRINTF("Start connecting ...\n");
  if(!client.connect("www.google.com", 80)){
    Serial.println("Connect Failed");
    DEBUG_ESP_PRINTF("Connection Failed!\n");
    return;
  }
  DEBUG_ESP_PRINTF("Connection succeeded\n");
  client.setTimeout(2);
  if(client.printf("GET / HTTP/1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n") > 0){
    while(client.connected() && client.available() == 0){
      delay(1);
    }
    DEBUG_ESP_PRINTF("Receive length: %d\n", client.available());
    // uint32_t waitTimeout = millis() + 1000U;
    // while(waitTimeout < millis()) {
    constexpr uint32_t idleTime = 1000U;
    int len;
    uint32_t waitTimeout = millis() + idleTime;
    while(waitTimeout > millis()) {
      while((len=client.available()) > 0)
        Serial.write(client.read());

      if (len == 0) {
        if (client.connected()) {
          waitTimeout = millis() + idleTime;
          delay(1);
        } else {
          break;
        }
      }
    }
    DEBUG_ESP_PRINTF("\nFinished:\n");
    if(client.connected()){
      client.stop();
    }
  } else {
    client.stop();
    Serial.println("Send Failed");
    while(client.connected()) delay(0);
  }
}

void loop(){
#ifdef ESP8266
  ArduinoOTA.handle();
#endif
}
