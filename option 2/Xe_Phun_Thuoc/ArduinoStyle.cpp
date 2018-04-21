#include "ArduinoStyle.h"
#include "L293D.h"

#define EXTI2 PC7
#define EXTI3 PD1
#define MA1 PA3
#define MA2 PD4
#define MB2 PD3
#define MB1 PD2
#define SELECTED_MODE_PIN PC5
#define LED_PIN PC6

#define EPPROM_MAX_SIZE 512
#define DATA_TYPE long
#define DATA_LENGTH 4 // in byte
#define STEP_LENGTH (DATA_LENGTH+1)
#define MAX_NUM_OF_JOURNEY (EPPROM_MAX_SIZE/STEP_LENGTH)
//#define DATA_END_SIGNAL 0xFF

#define AUTO_RUN_MODE 1
#define SETUP_MODE 0
#define SET_NUM_OF_JOURNEY 254
#define SELECT_JOURNEY 253


void writeToEEPROM(int addr, DATA_TYPE data);
DATA_TYPE readFromEEPROM(int addr);
void earseEEPROM(int star, int stop);

void pulseCount1();
void pulseCount2();
volatile DATA_TYPE pulse_count1 = 0;
volatile DATA_TYPE pulse_count2 = 0;

uint8_t running_mode = AUTO_RUN_MODE;
char running_direction = 'H';
uint8_t enable = 0;
int address_seek = 0;
uint8_t num_of_journey = 10;
uint8_t journey = 0;
uint8_t num_of_step_per_one_journey = 10;

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

  Serial.println("Waiting...!!!");
  delay(5000);
  while(Serial.available() > 0)
  {
    Serial.read();
  }
  
  
  Serial.println("Which journey? 0 to 9");
  while(!(Serial.available() > 0))
  {
      digitalWrite(LED_PIN, 1);
      delay(200);
      digitalWrite(LED_PIN, 0);
      delay(1000);
  }
  if(Serial.available() > 0)
  {// Cac lenh trong kho nay co the khong can dieu kien if o tren, nhung nen dat trong khoi if de tranh trung ten bien "c"
    uint8_t temp_journey = Serial.read(); 
    if(temp_journey < num_of_journey)
    {
      journey = temp_journey;
      Serial.println(journey);
    }
  }
  address_seek = journey*num_of_step_per_one_journey*STEP_LENGTH;
  
  if(digitalRead(SELECTED_MODE_PIN))
  {
    running_mode = AUTO_RUN_MODE;
    Serial.println("AUTO_RUN_MODE");
  }
  else
  {
    running_mode = SETUP_MODE;
    Serial.println("SETUP_MODE");
    
    Serial.print("Earsing EEPROM from ");
    Serial.print(address_seek);
    Serial.print(" to ");
    Serial.println(((journey+1)*num_of_step_per_one_journey*STEP_LENGTH)-1);
    earseEEPROM(address_seek, (journey+1)*num_of_step_per_one_journey*STEP_LENGTH-1);
    Serial.println("Earsing Done");
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
          if(address_seek < ((journey+1)*num_of_step_per_one_journey*STEP_LENGTH - num_of_step_per_one_journey))
          {
            EEPROM.write(address_seek, running_direction);
            address_seek++;
          }
        }
      }
      
      else if(c == 'D')
      {
        running_direction = 'D';
        my_car.runBack();

        if(enable == 1)
        {
          if(address_seek < ((journey+1)*num_of_step_per_one_journey*STEP_LENGTH - num_of_step_per_one_journey))
          {
            EEPROM.write(address_seek, running_direction);
            address_seek++;
          }
        }
      }
      
      else if(c == 'L')
      {
        running_direction = 'L';
        my_car.turnLeft();

        if(enable == 1)
        {
          if(address_seek < ((journey+1)*num_of_step_per_one_journey*STEP_LENGTH - num_of_step_per_one_journey))
          {
            EEPROM.write(address_seek, running_direction);
            address_seek++;
          }
        }
      }
      
      else if(c == 'R')
      {
        running_direction = 'R';
        my_car.turnRight();

        if(enable == 1)
        {
          if(address_seek < ((journey+1)*num_of_step_per_one_journey*STEP_LENGTH - num_of_step_per_one_journey))
          {
            EEPROM.write(address_seek, running_direction);
            address_seek++;
          }
        }
      }
      
      else if(c == 'H')
      {
        my_car.hold();
        delay(700); //delay de xe dung han

        if(enable == 1)
        {
          switch(running_direction)
          {
            case 'U':
              writeToEEPROM(address_seek, (pulse_count1+pulse_count2)/2);
              Serial.print(address_seek);
              Serial.print('\t');
              Serial.println(readFromEEPROM(address_seek));
              if(address_seek < ((journey+1)*num_of_step_per_one_journey*STEP_LENGTH - num_of_step_per_one_journey))
              {
                address_seek += DATA_LENGTH;
              }
              break;
            case 'D':
              writeToEEPROM(address_seek, (pulse_count1+pulse_count2)/2);
              Serial.print(address_seek);
              Serial.print('\t');
              Serial.println(readFromEEPROM(address_seek));
              if(address_seek < ((journey+1)*num_of_step_per_one_journey*STEP_LENGTH - num_of_step_per_one_journey))
              {
                address_seek += DATA_LENGTH;
              }
              break;
            case 'L':
              writeToEEPROM(address_seek, pulse_count2);
              Serial.print(address_seek);
              Serial.print('\t');
              Serial.println(readFromEEPROM(address_seek));
              if(address_seek < ((journey+1)*num_of_step_per_one_journey*STEP_LENGTH - num_of_step_per_one_journey))
              {
                address_seek += DATA_LENGTH;
              }
              break;
            case 'R':
              writeToEEPROM(address_seek, pulse_count1);
              Serial.print(address_seek);
              Serial.print('\t');
              Serial.println(readFromEEPROM(address_seek));
              if(address_seek < ((journey+1)*num_of_step_per_one_journey*STEP_LENGTH - num_of_step_per_one_journey))
              {
                address_seek += DATA_LENGTH;
              }
              break;
          }
        }

        pulse_count1 = 0;
        pulse_count2 = 0;
        running_direction = 'H';
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

void earseEEPROM(int start, int stop)
{
  for(int i = start; i < stop; i++)
  {
    EEPROM.write(i, 0xFF);
    delay(5);
  }
}


void pulseCount1()
{
  if(enable == 1)
  {
    delay(1);
    if((running_direction == 'U') || (running_direction == 'D') || (running_direction == 'L') || (running_direction == 'R'))
    {
      pulse_count1++;
    }
    Serial.println(pulse_count1);
  }
}

void pulseCount2()
{
  if(enable == 1)
  {
    delay(1);
    if((running_direction == 'U') || (running_direction == 'D') || (running_direction == 'L') || (running_direction == 'R'))
    {
      pulse_count2++;
    }
    Serial.println(pulse_count2);
  }
}

