#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "Switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
#include <ESP8266HTTPClient.h>
const char *ssid =  "meno_wifi";
const char *password =  "heslo_wifi";
boolean connectWifi();
void Relay1On();
void Relay1Off();
boolean wifiConnected = false;
int httpCode = 0;
UpnpBroadcastResponder upnpBroadcastResponder;
Switch *relay1 = NULL;
HTTPClient http;
void setup()
{
  Serial.begin(9600);
  wifiConnected = connectWifi();
  if (wifiConnected) {
    upnpBroadcastResponder.beginUdpMulticast();
    relay1 = new Switch("Relay 1", 80, Relay1On, Relay1Off);
    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*relay1);
  }
}
void loop()
{
  if (wifiConnected) {
    upnpBroadcastResponder.serverLoop();
    relay1->serverLoop();
  }
}

void Relay1On() {
  Serial.println("Zapinam switch 1 na povel od Alexa Dot");
  String url = "http://192.168.1.13:83/dev/sps/io/0f7d5586-039c-3ba2-fffffe345eb0bf31/on";
  http.begin(url);
  http.setAuthorization("admin", "0285");
  httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {

  }
  http.end();
}

void Relay1Off() {
  Serial.println("Vypinam switch 1 na povel od Alexa Dot");
  String url = "http://192.168.1.13:83/dev/sps/io/0f7d5586-039c-3ba2-fffffe345eb0bf31/off";
  http.begin(url);
  http.setAuthorization("admin", "0285");
  httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {

  }
  http.end();
}

boolean connectWifi() {
  boolean state = true;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Pripajanie na wifi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  return state;
}