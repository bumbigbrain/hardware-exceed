#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <ESP32Servo.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wifi.h>
#define ULTRASONIC 19
#define SERVO_PIN 13

#include "Ultrasonic.h"

Ultrasonic ultrasonic(ULTRASONIC);

Servo servo01;
void GET_locked_running(void *param);
void Connect_Wifi();
TaskHandle_t TaskLockedRunning = NULL;
int save_id = 1;
int locked;


void PUT_Status(){
  String json;
  DynamicJsonDocument doc(65203);
  doc["safe_id"] = Id;
  doc["flame_alert"] = sensor_flame;
  doc["humid_alert"] = sensor_humid;
  doc["temp_alert"] = sensor_temp;
  doc["ultrasonic_alert"] = 0;
  
  serializeJson(doc,json);

  const String url = "https://ecourse.cpe.ku.ac.th/exceed04/alert";
  HTTPClient http;
  http.begin(url);

  int httpResponseCode = http.PUT(json);
  if (httpResponseCode >= 200 && httpResponseCode < 300) {
    Serial.print("HTTP ");
    Serial.println(httpResponseCode);
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  } 
}



void GET_locked(){
  while(1){
    DynamicJsonDocument doc(150000);
    const String url = "https://ecourse.cpe.ku.ac.th/exceed04/status/1";
    HTTPClient http;
    http.begin(url);
    int httpResponseCode = http.GET();
  
    if (httpResponseCode >= 200 && httpResponseCode < 300){
      Serial.println(httpResponseCode);
      String payload = http.getString();
      //Serial.println(payload);
      deserializeJson(doc,payload);

      //Serial.println(payload);
      locked = doc["locked"];
      Serial.print("locked : ");
      Serial.println(locked);

    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
 
  }
}


void setup(){
  Serial.begin(115200);
  servo01.attach(SERVO_PIN);
  Connect_Wifi();
  xTaskCreatePinnedToCore(GET_locked_running, "GET_locked_running", 10000, NULL, 1, &TaskLockedRunning, 0);
  

}



void loop(){
  // servo01.write(0);
  // delay(1000);
  
  long RangeInCentimeters;
  RangeInCentimeters = ultrasonic.MeasureInCentimeters();
  Serial.println(RangeInCentimeters);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  

 
}


void GET_locked_running(void *param){
  while (1){
    GET_locked();
    
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    
  }
}







void Connect_Wifi()
{
  const char *ssid = "owen_raspi00";
  const char *password = "6443494357";
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("OK! IP=");
  Serial.println(WiFi.localIP());
  Serial.println("----------------------------------");
}