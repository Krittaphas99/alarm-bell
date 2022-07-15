/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-date-time-ntp-client-server-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <WiFi.h>
#include "time.h"

const char* ssid     = "GTT";
const char* password = "56/17Thai.EE";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 25200;//3600
#define led 18
///active hight a = 1 , b = 0
///active low a = 0 , b = 1
int a = 1;
int b = 0;
void setup(){
  Serial.begin(19200);
  setupAgain:
  Serial.println("check ");
  pinMode(led,OUTPUT);
  digitalWrite(led,b);
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int check_count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    check_count++;
    Serial.print(".");
    if(check_count ==10 )
    {
      Serial.println("check loop");
      goto setupAgain;
      }
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  delay(1000);
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  
}

void loop(){
  digitalWrite(led,b);
  printLocalTime();
  delay(500);
}

struct tm reset(){
  struct tm result;
  //setup();
  getLocalTime(&result);
  return result;
}

boolean settime(int hr,int mins,int sec){
  if((hr==8)&&(mins ==00)&&(sec >= 00 && sec <= 5)) {return true;}
  else if((hr==12)&&(mins ==00)&&(sec >= 00 && sec <= 5)) {return true;}
  else if((hr==13)&&(mins ==00)&&(sec >= 00 && sec <= 5)) {return true;}
  else if((hr==13)&&(mins ==30)&&(sec >= 00 && sec <= 5)) {return true;}
  else if((hr==14)&&(mins ==00)&&(sec >= 00 && sec <= 5)) {return true;}
  else if((hr==17)&&(mins ==30)&&(sec >= 00 && sec <= 5)) {return true;}
  return false;
}
void printLocalTime(){
  struct tm timeinfo;

  String HR , HR12 ,MT ;//SD
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    setup();
    return;
  }
  
  if((timeinfo.tm_hour==23)&& (timeinfo.tm_min==59)&&(timeinfo.tm_sec==00)){ 
   Serial.println(timeinfo.tm_min);
   timeinfo = reset();
   Serial.println("รีเวลา");
  }
 Serial.print(timeinfo.tm_hour);Serial.print(":");
  Serial.print(timeinfo.tm_min);Serial.print(":");
  Serial.println(timeinfo.tm_sec);
  
  if(settime(timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec)){
    Serial.println("เปิดการแจ้งเตือนน");
    digitalWrite(led,a);   //เปิดเสียงเตือน
    delay(10000);
    digitalWrite(led,b);
    Serial.println("ปิดการแจ้งเตือนน");
    }

}
