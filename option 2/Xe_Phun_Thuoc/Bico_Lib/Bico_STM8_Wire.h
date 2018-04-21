#include "Bico_STM8_Generic.h"

#ifndef BICO_STM8_WIRE_H
#define BICO_STM8_WIRE_H

//---------------------------------------------------------------------------
//#define DEBUG_I2C

#define I2C_WAITING_TIMEOUT 10 // in us 

#ifndef I2C_FREQ
#define I2C_FREQ 100000 // in Hz
#endif
#define I2C_INPUT_FREQ 4 //in MHz

#define MASTER_DEFAULT_ADDRESS 7 // thich thi de so 7 thoi, lam gi nhau nao

typedef uint16_t size_t;

typedef enum
{
  I2C_RESET = (uint8_t)1,
  I2C_NOT_RESET = (uint8_t)0
} Reset_I2C_During_Master_Transmitting;
//----------------------------------------------------------------------------

#define BUFFER_LENGTH 32

// WIRE_HAS_END means Wire has end()
#define WIRE_HAS_END 1

class TwoWire
{
  private:
    static uint8_t rxBuffer[];
    static uint8_t rxBufferIndex;
    static uint8_t rxBufferLength;

    static uint8_t txAddress;
    static uint8_t txBuffer[];
    static uint8_t txBufferIndex;
    static uint8_t txBufferLength;

    static uint8_t transmitting;
    static void (*user_onRequest)(void);
    static void (*user_onReceive)(int);
    static void onRequestService(void);
    static void onReceiveService(uint8_t*, int);
  public:
    TwoWire();
    void begin();
    void begin(uint8_t);
    void begin(int);
    void end();
    void setClock(uint32_t OutputClockFrequencyHz, I2C_DutyCycle_TypeDef I2C_DutyCycle, uint8_t InputClockFrequencyMHz); //chua test
    void setClock(uint32_t); //chua test
    void beginTransmission(uint8_t);
    void beginTransmission(int);
    Reset_I2C_During_Master_Transmitting endTransmission(void);
    Reset_I2C_During_Master_Transmitting endTransmission(uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
	uint8_t requestFrom(uint8_t, uint8_t, uint32_t, uint8_t, uint8_t);
    uint8_t requestFrom(int, int);
    uint8_t requestFrom(int, int, int);
    virtual size_t write(uint8_t);
    virtual size_t write(const char *str);
    virtual size_t write(char *data, size_t quantity);
    virtual size_t write(uint8_t *data, uint8_t quantity);
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
    void onReceive( void (*)(int) );
    void onRequest( void (*)(void) );

    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
};

extern TwoWire Wire;

#endif