#include "Bico_STM8_Wire.h"

// Initialize Class Variables //////////////////////////////////////////////////

uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;

uint8_t TwoWire::txAddress = 0;
uint8_t TwoWire::txBuffer[BUFFER_LENGTH];
uint8_t TwoWire::txBufferIndex = 0;
uint8_t TwoWire::txBufferLength = 0;

uint8_t TwoWire::transmitting = 0;
void (*TwoWire::user_onRequest)(void);
void (*TwoWire::user_onReceive)(int);

// Constructors ////////////////////////////////////////////////////////////////

TwoWire::TwoWire()
{
}

// Public Methods //////////////////////////////////////////////////////////////

void TwoWire::begin(void)
{
  begin(MASTER_DEFAULT_ADDRESS);
}

void TwoWire::begin(uint8_t address)
{
  rxBufferIndex = 0;
  rxBufferLength = 0;

  txBufferIndex = 0;
  txBufferLength = 0;

  I2C_Init(I2C_FREQ, (address<<1), I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, I2C_INPUT_FREQ);
}

void TwoWire::begin(int address)
{
  begin((uint8_t)address);
}

void TwoWire::end(void)
{
//xoa BUSY bit neu duong truyen ban, khong the xoa truc tiep bang thanh ghi (tham khao RM0016 Reference manual trang 306/260)
    I2C_SoftwareResetCmd(ENABLE);
    
    //reset cac thanh ghi I2C, ngoai tru 4 thanh ghi ben duoi 
    I2C_DeInit();
    
    //Reset cac thanh ghi con lai cua module I2C
    I2C->SR1 = 0;
    I2C->SR2= 0;
    I2C->SR3 = 0;
    I2C->DR = 0;
    
    pinMode(PB4, OUTPUT);
    pinMode(PB5, OUTPUT);
    digitalWrite(PB4, HIGH);
    digitalWrite(PB5, HIGH);
}

void TwoWire::setClock(uint32_t OutputClockFrequencyHz, I2C_DutyCycle_TypeDef I2C_DutyCycle, uint8_t InputClockFrequencyMHz)
{
  uint16_t result = 0x0004;
  uint16_t tmpval = 0;
  uint8_t tmpccrh = 0;
  
  /* Detect Fast or Standard mode depending on the Output clock frequency selected */
  if (OutputClockFrequencyHz > I2C_MAX_STANDARD_FREQ) /* FAST MODE */
  {
    /* Set F/S bit for fast mode */
    tmpccrh = I2C_CCRH_FS;

    if (I2C_DutyCycle == I2C_DUTYCYCLE_2)
    {
      /* Fast mode speed calculate: Tlow/Thigh = 2 */
      result = (uint16_t) ((InputClockFrequencyMHz * 1000000) / (OutputClockFrequencyHz * 3));
    }
    else /* I2C_DUTYCYCLE_16_9 */
    {
      /* Fast mode speed calculate: Tlow/Thigh = 16/9 */
      result = (uint16_t) ((InputClockFrequencyMHz * 1000000) / (OutputClockFrequencyHz * 25));
      /* Set DUTY bit */
      tmpccrh |= I2C_CCRH_DUTY;
    }

    /* Verify and correct CCR value if below minimum value */
    if (result < (uint16_t)0x01)
    {
      /* Set the minimum allowed value */
      result = (uint16_t)0x0001;
    }

    /* Set Maximum Rise Time: 300ns max in Fast Mode
    = [300ns/(1/InputClockFrequencyMHz.10e6)]+1
    = [(InputClockFrequencyMHz * 3)/10]+1 */
    tmpval = ((InputClockFrequencyMHz * 3) / 10) + 1;
    I2C->TRISER = (uint8_t)tmpval;

  }
  else /* STANDARD MODE */
  {

    /* Calculate standard mode speed */
    result = (uint16_t)((InputClockFrequencyMHz * 1000000) / (OutputClockFrequencyHz << (uint8_t)1));

    /* Verify and correct CCR value if below minimum value */
    if (result < (uint16_t)0x0004)
    {
      /* Set the minimum allowed value */
      result = (uint16_t)0x0004;
    }

    /* Set Maximum Rise Time: 1000ns max in Standard Mode
    = [1000ns/(1/InputClockFrequencyMHz.10e6)]+1
    = InputClockFrequencyMHz+1 */
    I2C->TRISER = (uint8_t)(InputClockFrequencyMHz + (uint8_t)1);

  }

  /* Write CCR with new calculated value */
  I2C->CCRL = (uint8_t)result;
  I2C->CCRH = (uint8_t)((uint8_t)((uint8_t)(result >> 8) & I2C_CCRH_CCR) | tmpccrh);
}

void TwoWire::setClock(uint32_t clock)
{
  setClock(clock, I2C_DUTYCYCLE_2, I2C_INPUT_FREQ);
}

// uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop)
// {
//   if (isize > 0) {
//   // send internal address; this mode allows sending a repeated start to access
//   // some devices' internal registers. This function is executed by the hardware
//   // TWI module on other processors (for example Due's TWI_IADR and TWI_MMR registers)

//   beginTransmission(address);

//   // the maximum size of internal address is 3 bytes
//   if (isize > 3){
//     isize = 3;
//   }

//   // write internal register address - most significant byte first
//   while (isize-- > 0)
//     write((uint8_t)(iaddress >> (isize*8)));
//   endTransmission(false);
//   }

//   // clamp to buffer length
//   if(quantity > BUFFER_LENGTH){
//     quantity = BUFFER_LENGTH;
//   }
//   // perform blocking read into buffer
//   uint8_t read = twi_readFrom(address, rxBuffer, quantity, sendStop);
//   // set rx buffer iterator vars
//   rxBufferIndex = 0;
//   rxBufferLength = read;

//   return read;
// }

// uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
// 	return requestFrom((uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
// }

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{ 
  Reset_I2C_During_Master_Transmitting reset = I2C_NOT_RESET;
  uint8_t temp_rxbuffer_index = 0;
  uint8_t count = 0;
  
  if(quantity > BUFFER_LENGTH){
    quantity = BUFFER_LENGTH;
  }
  
  I2C_GenerateSTART(ENABLE);
  while(I2C_GetLastEvent() != I2C_EVENT_MASTER_MODE_SELECT)
  {
#ifdef DEBUG_I2C
    //Moniting hanh dong hien tai cua I2C
    Serial.print("I2C_GenerateSTART\r\n");
#else 
    delay_us(50);
#endif
    if(count++ > I2C_WAITING_TIMEOUT )
    {
      reset = I2C_RESET;
      break;
    }
  }
  
  I2C_Send7bitAddress(address<<1, I2C_DIRECTION_RX);
  count = 0;
  while(I2C_GetLastEvent() != I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)
  {
#ifdef DEBUG_I2C
    //Moniting hanh dong hien tai cua I2C
    Serial.print("I2C_Send7bitAddress\r\n");
#else 
    delay_us(50);
#endif
    if(count++ > I2C_WAITING_TIMEOUT )
    {
      reset = I2C_RESET;
      break;
    }
  }
  
  /* Disable Acknowledgement */
  //I2C_AcknowledgeConfig(I2C_ACK_NONE);

  /* Send STOP Condition */
  //I2C_GenerateSTOP(ENABLE);
  
  for(int i = quantity; i > 0; i--)
  {
    if(i != 3)
    {
      count = 0;
      while ((I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET))
      {
      #ifdef DEBUG_I2C
        //Moniting hanh dong hien tai cua I2C
        Serial.print("I2C_FLAG_TRANSFERFINISHED\r\n");
      #else 
          delay_us(50);
      #endif
        if(count++ > I2C_WAITING_TIMEOUT )
        {
          reset = I2C_RESET;
          break;
        }
      }
      rxBuffer[temp_rxbuffer_index++] = I2C_ReceiveData();
    }
    
    if(i == 3)
    {
      count = 0;
      while ((I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET))
      {
      #ifdef DEBUG_I2C
        //Moniting hanh dong hien tai cua I2C
        Serial.print("I2C_FLAG_TRANSFERFINISHED3\r\n");
      #else 
          delay_us(50);
      #endif
        if(count++ > I2C_WAITING_TIMEOUT )
        {
          reset = I2C_RESET;
          break;
        }
      }
      I2C_AcknowledgeConfig(I2C_ACK_NONE);
      rxBuffer[temp_rxbuffer_index++] = I2C_ReceiveData();
      i--;
      I2C_GenerateSTOP(ENABLE);
      rxBuffer[temp_rxbuffer_index++] = I2C_ReceiveData();
      i--;
 
      count = 0;
      while ((I2C_GetFlagStatus(I2C_FLAG_RXNOTEMPTY) == RESET))
      {
      #ifdef DEBUG_I2C
        //Moniting hanh dong hien tai cua I2C
        Serial.print("I2C_FLAG_RXNOTEMPTY\r\n");
      #else 
          delay_us(50);
      #endif
        if(count++ > I2C_WAITING_TIMEOUT )
        {
          reset = I2C_RESET;
          break;
        }
      }
      rxBuffer[temp_rxbuffer_index++] = I2C_ReceiveData();
      i--;
    }
  }

  Wire.end();
  delay_us(100);
  Wire.begin();
  
  rxBufferIndex = 0;
  rxBufferLength = quantity;
  
  return quantity;
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity/*, (uint8_t)true*/);
}

// uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop)
// {
//   return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
// }

void TwoWire::beginTransmission(uint8_t address)
{
  // indicate that we are transmitting
  transmitting = 1;
  // set address of targeted slave
  txAddress = address;
  // reset tx buffer iterator vars
  txBufferIndex = 0;
  txBufferLength = 0;
}

void TwoWire::beginTransmission(int address)
{
  beginTransmission((uint8_t)address);
}

//
//	Originally, 'endTransmission' was an f(void) function.
//	It has been modified to take one parameter indicating
//	whether or not a STOP should be performed on the bus.
//	Calling endTransmission(false) allows a sketch to 
//	perform a repeated start. 
//
//	WARNING: Nothing in the library keeps track of whether
//	the bus tenure has been properly ended with a STOP. It
//	is very possible to leave the bus in a hung state if
//	no call to endTransmission(true) is made. Some I2C
//	devices will behave oddly if they do not see a STOP.
//
Reset_I2C_During_Master_Transmitting TwoWire::endTransmission(uint8_t sendStop)
{
  
  Reset_I2C_During_Master_Transmitting reset = I2C_NOT_RESET;
  uint8_t count = 0;
  
  //START generation
  I2C_GenerateSTART(ENABLE);
  //wait until "count" reach "TIMER" value
  while(I2C_GetLastEvent() != I2C_EVENT_MASTER_MODE_SELECT)
  {
#ifdef DEBUG_I2C
    //Moniting hanh dong hien tai cua I2C
    Serial.print("I2C_GenerateSTART\r\n");
#else 
    delay_us(50);
#endif
    if(count++ > I2C_WAITING_TIMEOUT )
    {
      reset = I2C_RESET;
      break;
    }
  }

  //Send address and direction
  I2C_Send7bitAddress(txAddress<<1, I2C_DIRECTION_TX);
  count = 0;
  while(I2C_GetLastEvent() != I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)
  {
#ifdef DEBUG_I2C
    //Moniting hanh dong hien tai cua I2C
    Serial.print("I2C_Send7bitAddress\r\n");
#else 
    delay_us(50);
#endif
    if(count++ > I2C_WAITING_TIMEOUT )
    {
      reset = I2C_RESET;
      break;
    }
  }
  
  for(int i = 0; i < txBufferLength; i++)
  {
    //Sent Data
    I2C_SendData(txBuffer[i]);
    count = 0;
    while(I2C_GetLastEvent() != I2C_EVENT_MASTER_BYTE_TRANSMITTED)
    {
#ifdef DEBUG_I2C
      //Moniting hanh dong hien tai cua I2C
      Serial.print("I2C_SendData\r\n");
#else 
    delay_us(50);
#endif
      if(count++ > I2C_WAITING_TIMEOUT )
      {
        //neu "count" vuot qua "TIMER" (het timeout cho 1 hanh dong cua I2C) thi reset=1
        reset = I2C_RESET;
        break;
      }
    }
  }
    
  if(sendStop)
  {
    //Generating Stop after send all data
    I2C_GenerateSTOP(ENABLE);
  }
    
  //Neu co loi xay ra thi reset=1, vi vay ta can reset lai module I2C
  if(reset == I2C_RESET)
  {
    end();
    delay_us(100);
    
#ifdef DEBUG_I2C
    //monitoring I2C register
    Serial.write(I2C->CR1);
    Serial.write(I2C->CR2);
    Serial.write(I2C->FREQR);
    Serial.write(I2C->OARL);
    Serial.write(I2C->OARH);
    Serial.write(I2C->DR);
    Serial.write(I2C->SR1);
    Serial.write(I2C->SR2);
    Serial.write(I2C->SR3);
    Serial.write(I2C->ITR);
    Serial.write(I2C->CCRL);
    Serial.write(I2C->CCRH);
    Serial.write(I2C->TRISER);
#else 
    delay_us(50);
#endif
    
    //Init I2C after reset all I2C registers
    begin();
  }
  return reset;
}

//	This provides backwards compatibility with the original
//	definition, and expected behaviour, of endTransmission
//
Reset_I2C_During_Master_Transmitting TwoWire::endTransmission(void)
{
  return endTransmission(1);
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(uint8_t data)
{
  if(transmitting){
  // in master transmitter mode
    // don't bother if buffer is full
    if(txBufferLength >= BUFFER_LENGTH){
      //setWriteError();
      return 0;
    }
    // put byte in tx buffer
    txBuffer[txBufferIndex] = data;
    ++txBufferIndex;
    // update amount in buffer   
    txBufferLength = txBufferIndex;
  }else{
  // in slave send mode
    // reply to master
    //twi_transmit(&data, 1);
  }
  return 1;
}


size_t TwoWire::write(const char *str)
{
  uint8_t i = 0;
  while(str[i] != '\0')
  {
    write(str[i]);
    i++;
  }
  return i;
}


// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(char *data, size_t quantity)
{
  if(transmitting){
  // in master transmitter mode
    for(size_t i = 0; i < quantity; ++i){
      write(data[i]);
    }
  }else{
  // in slave send mode
    // reply to master
    //twi_transmit(data, quantity);
  }
  return quantity;
}

size_t TwoWire::write(uint8_t *data, uint8_t quantity)
{
  return write((char*)data, quantity);
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::available(void)
{
  return rxBufferLength - rxBufferIndex;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::read(void)
{
  int value = -1;
  
  // get each successive byte on each call
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
  }

  return value;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::peek(void)
{
  int value = -1;
  
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
  }

  return value;
}

void TwoWire::flush(void)
{
  // XXX: to be implemented.
}

// behind the scenes function that is called when data is received
// void TwoWire::onReceiveService(uint8_t* inBytes, int numBytes)
// {
//   // don't bother if user hasn't registered a callback
//   if(!user_onReceive){
//     return;
//   }
//   // don't bother if rx buffer is in use by a master requestFrom() op
//   // i know this drops data, but it allows for slight stupidity
//   // meaning, they may not have read all the master requestFrom() data yet
//   if(rxBufferIndex < rxBufferLength){
//     return;
//   }
//   // copy twi rx buffer into local read buffer
//   // this enables new reads to happen in parallel
//   for(uint8_t i = 0; i < numBytes; ++i){
//     rxBuffer[i] = inBytes[i];    
//   }
//   // set rx iterator vars
//   rxBufferIndex = 0;
//   rxBufferLength = numBytes;
//   // alert user program
//   user_onReceive(numBytes);
// }

// // behind the scenes function that is called when data is requested
// void TwoWire::onRequestService(void)
// {
//   // don't bother if user hasn't registered a callback
//   if(!user_onRequest){
//     return;
//   }
//   // reset tx buffer iterator vars
//   // !!! this will kill any pending pre-master sendTo() activity
//   txBufferIndex = 0;
//   txBufferLength = 0;
//   // alert user program
//   user_onRequest();
// }

// // sets function called on slave write
// void TwoWire::onReceive( void (*function)(int) )
// {
//   user_onReceive = function;
// }

// // sets function called on slave read
// void TwoWire::onRequest( void (*function)(void) )
// {
//   user_onRequest = function;
// }

// Preinstantiate Objects //////////////////////////////////////////////////////

TwoWire Wire = TwoWire();