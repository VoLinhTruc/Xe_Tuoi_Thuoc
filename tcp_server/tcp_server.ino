#include "L293D.h"
#include <EEPROM.h>

#define EXTI2 2
#define EXTI3 3
#define MA1 4
#define MA2 5
#define MB2 6
#define MB1 7

#define SELECTED_MODE_PIN 8
#define LED_PIN 13

#define EPPROM_MAX_SIZE 512
#define DATA_TYPE long
#define DATA_LENGTH 4 // in byte
//#define DATA_END_SIGNAL 0xFF

#define AUTO_RUN_MODE 1
#define SETUP_MODE 0


void writeToEEPROM(int addr, DATA_TYPE data);
DATA_TYPE readFromEEPROM(int addr);
void earseEEPROM();

void pulseCount1();
void pulseCount2();
volatile DATA_TYPE pulse_count1 = 0;
volatile DATA_TYPE pulse_count2 = 0;

uint8_t running_mode = AUTO_RUN_MODE;
char running_direction = 'H';
uint8_t enable = 0;
int address_seek = 0;

L293D my_car(MA1, MA2, MB1, MB2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  my_car.begin();
  pinMode(EXTI2, INPUT_PULLUP);
  pinMode(EXTI3, INPUT_PULLUP);
  pinMode(SELECTED_MODE_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(EXTI2), pulseCount1, FALLING);
  attachInterrupt(digitalPinToInterrupt(EXTI3), pulseCount2, FALLING);

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
      digitalWrite(LED_PIN, 1);
      delay(100);
      digitalWrite(LED_PIN, 0);
      delay(100);
    }
    address_seek++;
    long num_of_pulse = readFromEEPROM(address_seek);
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
    enable = 0;
    delay(500);
  }
//------------------------------------------------------------------------------------------
}

void writeToEEPROM(int addr, DATA_TYPE data)
{
  for(int i = 0; i < DATA_LENGTH; i++)
  {
    EEPROM.write(addr+i, (uint8_t)(data>>(i*8)));
    delay(10);
  }
}

DATA_TYPE readFromEEPROM(int addr)
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

