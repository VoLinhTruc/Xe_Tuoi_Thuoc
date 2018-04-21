#ifndef L293D_H
#define L293D_H

#include "Bico_STM8_Generic.h"

#define MAX_SPEED 225
#define MIN_SPEED 0

class L293D
{
  public:
    L293D();
    L293D(uint8_t MA1, uint8_t MA2, uint8_t MB1, uint8_t MB2, uint8_t spd);
    L293D(uint8_t MA1, uint8_t MA2, uint8_t MB1, uint8_t MB2);
    void begin();
    void runForward();
    void runForward(uint8_t spd);
    void runBack();
    void runBack(uint8_t spd);
    void turnLeft();
    void turnLeft(uint8_t spd);
    void turnRight();
    void turnRight(uint8_t spd);
    void hold();

    uint8_t getSpeed();
    void setSpeed(uint8_t spd);

 private:
    uint8_t spd;
    uint8_t MA1;
    uint8_t MA2;
    uint8_t MB2;
    uint8_t MB1;
    
};

#endif
