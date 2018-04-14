#include <EEPROM.h>
#include "L293D.h"

#define EPPROM_MAX_SIZE 1024

#define DATA_TYPE uint32_t
#define DATA_LENGTH 4 // in byte
#define DATA_END_SIGNAL 0xFF

#define AUTO_RUN_MODE 1
#define SETUP_MODE 0

#define SELECTED_MODE_PIN 8

void writeToEEPROM(uint16_t addr, DATA_TYPE data)
{
  for(int i = 0; i < DATA_LENGTH; i++)
  {
    EEPROM.write(addr+i, (uint8_t)(data>>(i*8)));
    delay(10);
  }
}

DATA_TYPE readFromEEPROM(uint16_t addr)
{
  DATA_TYPE data = 0;
  for(int i = 0; i < DATA_LENGTH; i++)
  {
    data |= ((DATA_TYPE)(EEPROM.read(addr+i)))<<(i*8);
    delay(1);
  }

  return data;
}

void earseEEPROM()
{
  for(int i = 0; i < EPPROM_MAX_SIZE; i++)
  {
    EEPROM.write(i, 0xFF);
    delay(5);
  }
}

volatile DATA_TYPE pulse_count1 = 0;
volatile DATA_TYPE pulse_count2 = 0;

uint8_t running_mode = AUTO_RUN_MODE;
char running_direction = 'H';
uint8_t enable = 0;
uint16_t address_seek = 0;

L293D my_car;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  my_car.begin();
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(SELECTED_MODE_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), pulseCount1, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), pulseCount2, FALLING);

  if(digitalRead(SELECTED_MODE_PIN))
  {
    running_mode = AUTO_RUN_MODE;
    Serial.println("AUTO_RUN_MODE");
  }
  else
  {
    running_mode = SETUP_MODE;
    Serial.println("SETUP_MODE");
    Serial.println("Earsing EEPROM");
    earseEEPROM();
    Serial.println("Earsing Done");
  }
  
  delay(5000);
  while(Serial.available() > 0)
  {
    Serial.read();
  }
  
}

void loop() {
  // put  your main code here, to run repeatedly:
//------------------------------------------------------------------------------------------
  if(running_mode == SETUP_MODE)
  {
    if(Serial.available() > 0)
    {
      char c = Serial.read();
      Serial.println((char)c);
      
      if(c == 'U')
      {
        running_direction = 'U';
        my_car.runForward();

        if(enable == 1)
        {
          EEPROM.write(address_seek, running_direction);
          address_seek++;
        }
      }
      
      else if(c == 'D')
      {
        running_direction = 'D';
        my_car.runBack();

        if(enable == 1)
        {
          EEPROM.write(address_seek, running_direction);
          address_seek++;
        }
      }
      
      else if(c == 'L')
      {
        running_direction = 'L';
        my_car.turnLeft();

        if(enable == 1)
        {
          EEPROM.write(address_seek, running_direction);
          address_seek++;
        }
      }
      
      else if(c == 'R')
      {
        running_direction = 'R';
        my_car.turnRight();

        if(enable == 1)
        {
          EEPROM.write(address_seek, running_direction);
          address_seek++;
        }
      }
      
      else if(c == 'H')
      {
        my_car.hold();

        if(enable == 1)
        {
          switch(running_direction)
          {
            case 'U':
              writeToEEPROM(address_seek, (pulse_count1+pulse_count2)/2);
              Serial.print(address_seek+1);
              Serial.print('\t');
              Serial.println(readFromEEPROM(address_seek));
              address_seek += DATA_LENGTH;
              break;
            case 'D':
              writeToEEPROM(address_seek, (pulse_count1+pulse_count2)/2);
              Serial.print(address_seek+1);
              Serial.print('\t');
              Serial.println(readFromEEPROM(address_seek));
              address_seek += DATA_LENGTH;
              break;
            case 'L':
              writeToEEPROM(address_seek, pulse_count2);
              Serial.print(address_seek+1);
              Serial.print('\t');
              Serial.println(readFromEEPROM(address_seek));
              address_seek += DATA_LENGTH;
              break;
            case 'R':
              writeToEEPROM(address_seek, pulse_count1);
              Serial.print(address_seek+1);
              Serial.print('\t');
              Serial.println(readFromEEPROM(address_seek));
              address_seek += DATA_LENGTH;
              break;
          }
        }

        pulse_count1 = 0;
        pulse_count2 = 0;
      }
      
      else if(c == '1')
      {
        enable = 1;
      }
      
      else if(c == '0')
      {
        enable = 0;
        pulse_count1 = 0;
        pulse_count2 = 0;
      }
    }
  }
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
  if(running_mode == AUTO_RUN_MODE)
  { 
    running_direction = EEPROM.read(address_seek);
    while((running_direction != 'U') && (running_direction != 'D') && (running_direction != 'L') && (running_direction != 'R'))
    {
      digitalWrite(13, 1);
      delay(100);
      digitalWrite(13, 0);
      delay(100);
    }
    address_seek++;
    uint32_t num_of_pulse = readFromEEPROM(address_seek);
    address_seek += DATA_LENGTH;
    Serial.println((char)running_direction);
    Serial.println(num_of_pulse);
    switch(running_direction)
    {
      case 'U':
        enable = 1;
        my_car.runForward();
        while(((pulse_count1+pulse_count2)/2) < num_of_pulse)
        {
          delay(1);
        }
        pulse_count1 = 0;
        pulse_count2 = 0;
        break;
        
      case 'D':
        enable = 1;
        my_car.runBack();
        while(((pulse_count1+pulse_count2)/2) < num_of_pulse)
        {
          delay(1);
        }
        pulse_count1 = 0;
        pulse_count2 = 0;
        break;
        
      case 'L':
        enable = 1;
        my_car.turnLeft();
        while(pulse_count2 < num_of_pulse)
        {
          delay(1);
        }
        pulse_count1 = 0;
        pulse_count2 = 0;
        break;
        
      case 'R':
        enable = 1;
        my_car.turnRight();
        while(pulse_count1 < num_of_pulse)
        {
          delay(1);
        }
        pulse_count1 = 0;
        pulse_count2 = 0;
        break;
  
      default:
        break;
    }

    my_car.hold();
    delay(500);
    enable = 0;
  }
//------------------------------------------------------------------------------------------
}

void pulseCount1()
{
  if(enable == 1)
  {
    delay(50);
    pulse_count1++;
    Serial.println(pulse_count1);
  }
}

void pulseCount2()
{
  if(enable == 1)
  {
    delay(50);
    pulse_count2++;
    Serial.println(pulse_count2);
  }
}

