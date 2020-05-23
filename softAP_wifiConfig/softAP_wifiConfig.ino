/*
  WiFiAccessPoint.ino creates a WiFi access point and provides a web server on it.

  Steps:
  1. Connect to the access point "yourAp"
  2. Point your web browser to http://<This-AP-IP>/H to turn the LED on or http://<This-AP-IP>/L to turn it off
     (<This-AP-IP> should be replaced with the IP got in terminal/SerilPort, see Note 1)
   OR
   Run raw TCP "GET /H" and "GET /L" on PuTTY terminal with IP address (see Note 1) and 80 as port

  Created for arduino-esp32 on 04 July, 2018
  by Elochukwu Ifediora (fedy0)
*/

#include <AtWiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <FlashStorage.h>
#include <LovyanGFX.hpp>

const char* ssid = "WIO_TERMINAL";
const char* password = "seeedstudio1234";

WiFiServer webserver(80);
static LGFX lcd;
static LGFX_Sprite sprite(&lcd);

typedef struct 
{
  boolean saved;
  char ssid[100];
  char password[100];
} wifi_settings_t;

FlashStorage(flash, wifi_settings_t);


void setup() 
{
  Serial.begin(115200);
  Serial.println();
  lcd.init();
  lcd.setRotation(1);
  lcd.setBrightness(255);
  lcd.setColorDepth(24);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SoftAP Configurator Test");
  
  wifi_settings_t wifi_settings;
  wifi_settings = flash.read();
  if (wifi_settings.saved == false) 
  {
    Serial.println("Empty wifi settings.");
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    webserver.begin();
    Serial.println("Server started");
    setting_mode(&wifi_settings);
    flash.write(wifi_settings);
    Serial.println("Saved!");
    webserver.end();
    WiFi.mode(WIFI_OFF);
  }
  else
  {
    Serial.println("Current settings: ");
    Serial.print("SSID = ");
    Serial.println(wifi_settings.ssid);
    Serial.print("Password = ");
    Serial.println(wifi_settings.password);
    Serial.println();
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_settings.ssid, wifi_settings.password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Connected STA");
  get_icon();
  Serial.println("Done");
}

void loop() 
{
}
