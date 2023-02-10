#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <ESP32Servo.h>
#define TRIG_PIN 5
#define ECHO_PIN 18
#define SERVO_PIN 13


Servo servo01;
void ServoFunc(void *param);
TaskHandle_t TaskServo;


void setup(){
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  servo01.attach(SERVO_PIN);
  xTaskCreatePinnedToCore(ServoFunc, "ServoFunc", 10000, NULL, 1, &TaskServo, 0);
  
  
}

int duration;
int distance;

void loop(){
  //servo01.write(0);
  //delay(1000);
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration*0.034/2;
  Serial.println(distance);
  vTaskDelay(100/portTICK_PERIOD_MS);
  



}

void ServoFunc(void *param){
  while(1){

    servo01.write(0);
    vTaskDelay(1000/portTICK_PERIOD_MS);
    servo01.write(90);
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}

