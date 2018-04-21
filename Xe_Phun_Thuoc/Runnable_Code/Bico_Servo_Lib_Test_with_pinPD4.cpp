#include "ArduinoStyle.h"
#include "Bico_STM8_Generic.h"
#include "Bico_STM8_Servo.h"

Servo myservo;

void setup()
{
  myservo.attach(PD4);
  pinMode(PD4, OUTPUT);
  Serial.begin(57600);
}

void loop()
{
  for(uint8_t i = 0; i < 180; i++)
  {
    myservo.write(i);
    delay_ms(5);
  }
  delay_ms(1000);
  
  for(uint8_t i = 180; i > 0; i--)
  {
    myservo.write(i);
    delay_ms(5);
  }
  delay_ms(1000);
  
  
  for(uint8_t i = 0; i < 255; i++)
  {
    analogWrite(PD4, i);
    delay_ms(5);
  }
  
  for(uint8_t i = 255; i > 0; i--)
  {
    analogWrite(PD4, i);
    delay_ms(5);
  }
  
  digitalWrite(PD4, 1);
  delay_ms(500);
  digitalWrite(PD4, 0);
  delay_ms(500);
  digitalWrite(PD4, 1);
  delay_ms(500);
  digitalWrite(PD4, 0);
  delay_ms(500);
  
}
