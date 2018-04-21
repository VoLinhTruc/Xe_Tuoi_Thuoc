#include "L293D.h"

L293D::L293D()
{
  spd = MAX_SPEED;
  MA1 = 4;
  MA2 = 5;
  MB2 = 6;
  MB1 = 7;
}

L293D::L293D(uint8_t MA1, uint8_t MA2, uint8_t MB1, uint8_t MB2, uint8_t spd)
{
  this->spd = spd;
  this->MA1 = MA1;
  this->MA2 = MA2;
  this->MB2 = MB2;
  this->MB1 = MB1;
}

L293D::L293D(uint8_t MA1, uint8_t MA2, uint8_t MB1, uint8_t MB2)
{
  this->spd = MAX_SPEED;
  this->MA1 = MA1;
  this->MA2 = MA2;
  this->MB2 = MB2;
  this->MB1 = MB1;
}

void L293D::begin()
{
  pinMode(MA1, OUTPUT);
  pinMode(MA2, OUTPUT);
  pinMode(MB2, OUTPUT);
  pinMode(MB1, OUTPUT);
}

//---------------------------------------
void L293D::runForward(uint8_t spd)
{
  digitalWrite(MA1, LOW);
  digitalWrite(MB1, LOW);
  analogWrite(MA2, spd);
  analogWrite(MB2, spd);
}

void L293D::runForward()
{
  runForward(spd);
}
//---------------------------------------
void L293D::runBack(uint8_t spd)
{
  digitalWrite(MA1, HIGH);
  digitalWrite(MB1, HIGH);
  analogWrite(MA2, MAX_SPEED-spd);
  analogWrite(MB2, MAX_SPEED-spd);
}

void L293D::runBack()
{
  runBack(spd);
}
//---------------------------------------
void L293D::turnLeft(uint8_t spd)
{
  digitalWrite(MA1, LOW);
  digitalWrite(MB1, LOW);
  analogWrite(MA2, spd);
  analogWrite(MB2, 0);
}

void L293D::turnLeft()
{
  turnLeft(spd);
}
//---------------------------------------
void L293D::turnRight(uint8_t spd)
{
  digitalWrite(MA1, LOW);
  digitalWrite(MB1, LOW);
  analogWrite(MA2, 0);
  analogWrite(MB2, spd);
}

void L293D::turnRight()
{
  turnRight(spd);
}
//---------------------------------------
void L293D::hold()
{
  digitalWrite(MA1, LOW);
  digitalWrite(MB1, LOW);
  digitalWrite(MA2, LOW);
  digitalWrite(MB2, LOW);
}
//---------------------------------------
void L293D::setSpeed(uint8_t spd)
{
  this->spd = spd;
}

uint8_t L293D::getSpeed()
{
  return spd;
}

