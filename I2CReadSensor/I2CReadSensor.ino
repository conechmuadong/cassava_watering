
#include <ArduinoJson.h>
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <SPI.h>         // for communication with Ethernet Shield
#include "TimeLib.h"     // for update/display of time
#include <Ethernet.h>    // for communication with NTP Server via UDP
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <PCF8591.h>      // for I2C communicate with ADC PCF8591
#include <Wire.h>
//#define WIFI_SSID "Trung Tien"
//#define WIFI_PASSWORD "88888888"
#define WIFI_SSID "702E3"
#define WIFI_PASSWORD "9M@9254u"

#define FIREBASE_HOST "testfirebase-aceba-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "MDey8L7qdfvtjKcwFNEnVuoDqhuKkEWHwuleIk9n"

#define PCF8591_ADDRESS 0x48
#define V_REF 3.3

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

FirebaseData firebaseData;
FirebaseData fbdo;
//WiFiClient client
String path = "/";
FirebaseJson json;
String timeNow;
String pathFirebase = "testUser/field2/measured_data/";

PCF8591 pcf8591(PCF8591_ADDRESS);

void connectWifi() {
  Serial.println();
  WiFi.disconnect();
  delay(10);
  Serial.println("Start connect Wifi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void connectFirebase() {
  Serial.println("Start connect Firebase");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  if (Firebase.beginStream(firebaseData, path)) {
    Serial.println("REASON: " + firebaseData.errorReason());
  }
  Serial.println("Connect Firecbase success");
  Serial.println();
}

void setTimeDay() {
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(25200);
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);

  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime);

  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon + 1;
  int currentYear = ptm->tm_year + 1900;

  //Print complete date:
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  Serial.print("Current date: ");
  Serial.println(currentDate);

  timeNow = currentDate + "/" + formattedTime;
  Serial.print("timeNow: ");
  Serial.println(timeNow);
  delay(5000);
}

/*void printFirebase(int channel, int value) {
    Serial.println("printFirebase");
    switch (channel){
      case 0:

    }
    Firebase.setInt(firebaseData, path + time, random(0, 100));
    Firebase.setFloat(firebaseData, "LED", 0);
    Firebase.setFloat(firebaseData, "/do am/1", random(0, 100));
  //  Firebase.setFloat(firebaseData, "/do am/2", 0);
    String dataPath = "/do am/" + String(random(0, 100));
    Firebase.setFloat(firebaseData, dataPath, random(0, 100));
  //  Serial.println("donet");
//    Firebase.setFloat(firebaseData, "/do am/" + random(0, 100), random(0, 100));
  //  Serial.println("I'm awake, but I'm going into deep sleep mode for 5 seconds");
    Serial.println(Firebase.setFloat(firebaseData, "/do am/time/" + timeNow, random(0, 100)));
    Serial.println("printFirebase success");
}
*/
void setDataField(String fieldName, double humidity_channel_1, double humidity_channel_2) {
  Serial.println("Start setDataField " + fieldName);
  String measured_data = fieldName + "/measured_data/" + timeNow;
  Firebase.setDouble(firebaseData, measured_data + "/humidity_channel_1", humidity_channel_1);
  Serial.println(measured_data + "/humidity_channel_1: " + humidity_channel_1);
  Firebase.setDouble(firebaseData, measured_data + "/humidity_channel_2", humidity_channel_2);
  Serial.println(measured_data + "/humidity_channel_2: " + humidity_channel_2);
  Firebase.setString(firebaseData, measured_data + "/timeNow", timeNow);
  Serial.println(measured_data + "/timeNow: " + timeNow);
  Serial.println("End setDataField " + fieldName);
}

void getJsonFirebase() {
  Serial.println("Start getJsonData");
  Serial.println("End getJsonData");
}

void readSensorValue(int* p){
  PCF8591::AnalogInput ai = pcf8591.analogReadAll();
  p[0] = ai.ain0*V_REF/255.0;
  p[1] = ai.ain0*V_REF/255.0;
  p[2] = ai.ain0*V_REF/255.0;
  p[3] = ai.ain0*V_REF/255.0;
} 

void setup() {
  Serial.begin(115200);
  pcf8591.begin();
//  Serial.setTimeout(2000);
  Serial.println();
  connectWifi();
  connectFirebase();
  setTimeDay();
  //setDataField("testUser/field2", random(0, 100), random(0, 100));
//  printFirebase();
//getJsonFirebase();
//  while(!Serial) { }
  // Deep sleep mode for 30 seconds, the ESP8266 wakes up by itself when GPIO 16 (D0 in NodeMCU board) is connected to the RESET pin
//  Serial.println("I'm awake, but I'm going into deep sleep mode for 10 seconds");
 
//  if (Firebase.RTDB.getInt(&firebaseData, "/data")) {
//      if (firebaseData.dataType() == "int") {
//        int intValue = firebaseData.intData();
//        Serial.print("data: ");
//        Serial.println(intValue);
//      }
//  }
//  if (Firebase.RTDB.getString(&firebaseData, "/name")) {
//    if (firebaseData.dataType() == "string") {
//      String stringValue = firebaseData.stringData();
//      Serial.print("name: ");
//      Serial.println(stringValue);
//    }
//  } 
//   if (Firebase.RTDB.getJSON(&firebaseData, "/name")) {
//    if (firebaseData.dataType() == "string") {
//      String stringValue = firebaseData.stringData();
//      Serial.print("name: ");
//      Serial.println(stringValue);
//    }
//  } 
//  ESP.deepSleep(300e6);

  //Serial.println("Sleep Arduino");
  //ESP.deepSleep(15e6);
}

void loop(){
  int redval[4];
  readSensorValue(redval);
  setDataField("field2", redval[0], redval[1]);
  delay(300000);
} 