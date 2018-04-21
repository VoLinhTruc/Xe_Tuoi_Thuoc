#include "ArduinoStyle.h"
#include "Bico_STM8_Generic.h"

#include "ArduinoJson.h"

//Chuong trinh nhu sau:
//Tao mot chuoi "json" co dang la chuoi Json
//Tao Buffer 100 Byte de phan tich chuoi Json vua tao
//Tao ra doi tuong root dung de truy xuat toi tung doi tuong trong chuoi Json
//Lay gia tri cac doi tuong trong chuoi Json, cu the la c1 c2 va c3
//Tao ra 3 bien c11, c21, c31 co gia tri giong voi doi tuong trong chuoi Json
//Sau do kiem tra, neu c1==c11 thi in ra ky tu '1', != thi in ra ky tu '0', tuong tu voi c2 va c3

//Luu y, thu vien Json nay da duoc Truc chinh sua va test voi kieu so thuc se ko chinh xac
//Vi vay khong nen dung chuoi Json nay voi so thuc
//Gio ta se test voi mang(array)
//Nhu ta thay thi Thu vien Json nay su dung duoc voi mang binh thuong
//Con chuoi (C string) thi Truc cung da test va hoat dong binh thuong
//Chi la kieu so thuc là sai do hoi nay chinh sua o file FloatTrais.hpp

char json[] = "{\"c1\":-123456789,\"c2\":252,\"c3\":[32, 33]}";

StaticJsonBuffer<100> jsonBuffer;

JsonObject& root = jsonBuffer.parseObject(json);

long c1 = root["c1"];
uint8_t c2 = root["c2"];
uint8_t c3 = root["c3"][0];
uint8_t c4 = root["c3"][1];

long c11 = -123456789;
uint8_t c21 = 252;
uint8_t c31 = 32;
uint8_t c41 = 33;

void setup()
{
  pinMode(PB5, OUTPUT);
  Serial.begin(57600);
}

void loop()
{
  if(c1 == c11)
    Serial.write(49); //In ky tu '1'
  else
    Serial.write(48);
  Serial.write('\t');
  
  if(c2 == c21)
    Serial.write(50); //In ky tu '2'
  else
    Serial.write(48);
  Serial.write('\t');
  
  if(c3 == c31)
    Serial.write(51); //In ky tu '3'
  else
    Serial.write(48);
  Serial.write('\t');
  
  if(c4 == c41)
    Serial.write(52); //In ky tu '4'
  else
    Serial.write(48);
  Serial.write('\r');
  Serial.write('\n');
  
  delay_ms(1000);
}
