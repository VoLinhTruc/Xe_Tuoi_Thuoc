#include "ArduinoStyle.h"
#include "Bico_STM8_Generic.h"

void setup()
{
  clkConfig();
  Serial.begin(115200);
  pinMode(PB5, OUTPUT);
  pinMode(PA3, OUTPUT);
  pinMode(PD3, OUTPUT);
  pinMode(PD4, OUTPUT);
}

void loop()
{
  uint16_t a = analogRead(AIN2);
  analogWrite(PA3, a);
  analogWrite(PD3, 255-a);
  analogWrite(PD4, 127);
  delay_ms(1000);
  digitalWrite(PA3, 1);
  digitalWrite(PD3, 1);
  digitalWrite(PD4, 1);
  delay_ms(500);
  digitalWrite(PA3, 0);
  digitalWrite(PD3, 0);
  digitalWrite(PD4, 0);
  delay_ms(500);
  digitalWrite(PA3, 1);
  digitalWrite(PD3, 1);
  digitalWrite(PD4, 1);
  delay_ms(500);
  digitalWrite(PA3, 0);
  digitalWrite(PD3, 0);
  digitalWrite(PD4, 0);
  delay_ms(500);
  digitalWrite(PA3, 1);
  digitalWrite(PD3, 1);
  digitalWrite(PD4, 1);
  delay_ms(500);
  digitalWrite(PA3, 0);
  digitalWrite(PD3, 0);
  digitalWrite(PD4, 0);
  delay_ms(500);
}
