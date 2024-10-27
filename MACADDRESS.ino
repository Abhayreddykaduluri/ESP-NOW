//ESP32 MAC Address Finder (used to find mac address of the receiver)
#include <WiFi.h>
#include "esp_wifi.h"
//Add your wifi network's credentials here
const char* ssid     = "YOUR SSID";
const char* password = "YOUR PASSWORD";
void setup()
{
    Serial.begin(115200);
    delay(1000);
    WiFi.mode(WIFI_STA);
    //Display the ESP32's default Mac Address
    Serial.print("The ESP32's default Mac Address is:  ");
    Serial.println(WiFi.macAddress());
    uint8_t newMacAddress[] = {0xA0, 0xB0, 0xC0, 0xD0, 0xE0, 0xF0};     
    esp_err_t macAddressChangeResult = esp_wifi_set_mac(WIFI_IF_STA, &newMacAddress[0]);
    if (macAddressChangeResult == ESP_OK) {
      Serial.println("Mac Address");
      Serial.print("This ESP32's Mac Address is:  ");
      Serial.println(WiFi.macAddress());
      Serial.println("");
      //Connect to wifi using the new Mac Address
      connectToWifi();
    } else {
      Serial.print("Mac change not changed. Check this value of esp_err_t: ");
      Serial.println(macAddressChangeResult);
    }
}
void connectToWifi() {
  Serial.print("Connecting to wifi network ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to wifi network ");
  Serial.print(ssid);
  Serial.println("!");
  Serial.print("ESP32's local IP address: ");
  Serial.println(WiFi.localIP());
}
void loop(){}
