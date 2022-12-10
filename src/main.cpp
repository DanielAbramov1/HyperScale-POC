#include <Arduino.h>
#include <weight_sensor.h>
#include <serial.h>
#include <globals.h>
#include <event.h>

#define TIMER 500
#define LED_PIN 13
unsigned long loop_time = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("------SUPER-DUPER------");
  event.init_event_values();
  ScaleSetup();
  StartTareSequence(50);
  delay(500);
}

void loop() 
{
  SerialCheck();
  // ReadSensor();
  event_sm_loop();
  
  if( millis() - loop_time > TIMER )
  {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    loop_time = millis();
  }
}