#include "Bico_STM8_Serial.h"


HardwareSerial::HardwareSerial() {};

void HardwareSerial::begin(unsigned long baud)
{
  UART1_Cmd(ENABLE);
  UART1_Init((uint32_t)baud, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
              UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE); 
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
  enableGlobalInterrupt();
}

void HardwareSerial::setTimeout(unsigned long val)
{
  timeout = val;
}

void HardwareSerial::isr(char c)
{
  rx_buffer_index_t i = (unsigned int)(_rx_buffer_head + 1) % SERIAL_RX_BUFFER_SIZE;
  if (i != _rx_buffer_tail) 
  {
    _rx_buffer[_rx_buffer_head] = c;
    _rx_buffer_head = i;
  }
}

uint8_t HardwareSerial::available()
{
  return ((unsigned int)(SERIAL_RX_BUFFER_SIZE + _rx_buffer_head - _rx_buffer_tail)) % SERIAL_RX_BUFFER_SIZE;
}

uint8_t HardwareSerial::read()
{
  if (_rx_buffer_head == _rx_buffer_tail) 
  {
    return -1;
  } 
  else 
  {
    unsigned char c = _rx_buffer[_rx_buffer_tail];
    _rx_buffer_tail = (rx_buffer_index_t)(_rx_buffer_tail + 1) % SERIAL_RX_BUFFER_SIZE;
    return c;
  }
}

void HardwareSerial::write(char c)
{
  UART1_SendData8(c);
  uint8_t count = 0;
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
  {
    if(count++ > UART1_WAITING_TIMEOUT )
    {
      break;
    }
  }
}

void HardwareSerial::write(uint8_t c)
{
  UART1_SendData8(c);
  uint8_t count = 0;
  while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET)
  {
    if(count++ > UART1_WAITING_TIMEOUT )
    {
      break;
    }
  }
}

void HardwareSerial::println()
{
  print("\r\n");
}

uint8_t HardwareSerial::print(const char* str)
{
  uint8_t i = 0;
  while(str[i] != '\0')
  {
    write(str[i]);
    i++;
  }
  return i;
}

uint8_t HardwareSerial::println(const char* str)
{
  uint8_t origin_str = print(str);
  uint8_t rn_str = print("\r\n");
  return (origin_str + rn_str);
}

uint8_t HardwareSerial::print(char* array, uint8_t length)
{
  for(uint8_t i = 0; i < length; i++)
  {
    write(array[i]);
  }
  
  return length;
}

uint8_t HardwareSerial::println(char* array, uint8_t length)
{
  uint8_t origin_str = print(array, length);
  uint8_t rn_str = print("\r\n");
  return (origin_str + rn_str);
}

void HardwareSerial::print(uint8_t c)
{
  print((long)c);
}

void HardwareSerial::println(uint8_t c)
{
  println((long)c);
}

void HardwareSerial::print(char c)
{
  write(c);
}

void HardwareSerial::println(char c)
{
  write(c);
  println();
}

void HardwareSerial::print(int number)
{
  print((long)number);
}

void HardwareSerial::println(int number)
{
  println((long)number);
}

void HardwareSerial::print(long number)
{
  if(number < 0)
    write('-');
  for(uint8_t i = 0; i < getNumberOfDigit(number); i++)
  {
    uint8_t temp= 48+getDigitInNumber(number, getNumberOfDigit(number)-1 - i);
    write(temp);
  }
}

void HardwareSerial::println(long number)
{
  if(number < 0)
    write('-');
  for(uint8_t i = 0; i < getNumberOfDigit(number); i++)
  {
    uint8_t temp= 48+getDigitInNumber(number, getNumberOfDigit(number)-1 - i);
    write(temp);
  }
  print("\r\n");
}

uint8_t HardwareSerial::peek(void)
{
  if (_rx_buffer_head == _rx_buffer_tail) {
    return -1;
  } else {
    return _rx_buffer[_rx_buffer_tail];
  }
}

HardwareSerial Serial;

#pragma vector = 20  /* Symbol from I/O */ 
__interrupt void UART1_ISR(void)
{
  if(UART1_GetFlagStatus(UART1_FLAG_RXNE) == SET)
  { 
    char c = UART1_ReceiveData8();
    Serial.isr(c);
    
    UART1_ClearFlag(UART1_FLAG_RXNE);
  }
}