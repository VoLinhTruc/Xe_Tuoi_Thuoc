#ifndef BICO_STM8_SERIAL_H
#define BICO_STM8_SERIAL_H

#include "stm8s.h"
#include "Algebra_Math.h"

#define UART1_WAITING_TIMEOUT 250
#define SERIAL_RX_BUFFER_SIZE 16
typedef uint8_t rx_buffer_index_t;

extern void enableGlobalInterrupt();

class HardwareSerial
{
protected:
	volatile rx_buffer_index_t _rx_buffer_head;
	volatile rx_buffer_index_t _rx_buffer_tail;
	volatile unsigned char _rx_buffer[SERIAL_RX_BUFFER_SIZE];
	unsigned int timeout;

public:
	HardwareSerial();

	void begin(unsigned long baud);
	void setTimeout(unsigned long second);
    void isr(char c);
	uint8_t available();
	uint8_t read();
	//uint8_t readString();
        void write(char c);
	void write(uint8_t c);
        void println();
	uint8_t print(const char* str);
        uint8_t println(const char* str);
        uint8_t print(char* array, uint8_t length);
        uint8_t println(char* array, uint8_t length);
        void print(char c);
        void println(char c);
        void print(uint8_t c);
        void println(uint8_t c);
        void print(int number);
        void println(int number);
        void print(long number);
        void println(long number);
        //void print(float number, num_of_frac_part);
        //void println(float number, num_of_frac_part);
        //void print(float number);
        //void println(float number);
	//void print(string str);
	//void println(string str);
        uint8_t peek(void);
};

extern HardwareSerial Serial;

#endif