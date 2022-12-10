#include <string.h>
#include <Arduino.h>
#include <serial.h>
#include <weight_sensor.h>

void SerialCheck()
{
  if(Serial.available())
  { 
    String packet = "";
    String input_str = Serial.readStringUntil('\n');
    Serial.print("recieved : "); Serial.println(input_str);

    // few args in the string 
    // int index = input_str.indexOf(',');
    // String arg = input_str.substring(index+1);

    switch(input_str[0])
    {
        case 'T':
            StartTareSequence(20);
            break;
        
        case 2:
            break;
    }
  }
}

