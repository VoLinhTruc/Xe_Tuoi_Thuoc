#include "Bico_STM8_Exti.h"

void (*EXIT_C7)();
void (*EXIT_D1)();

EXTI_Port_TypeDef digitalPinToInterrupt(uint8_t pin)
{
    if(IS_BETWEEN(C, D))
    {
      return EXTI_PORT_GPIOC;
    }
    
    else if(IS_BETWEEN(D, E))
    {
      return EXTI_PORT_GPIOD;
    }
}

void attachInterrupt(EXTI_Port_TypeDef port, void (*ISR)(), uint8_t in_mode)
{
    EXTI_Sensitivity_TypeDef mode;
    switch(in_mode)
    {
    case LOW:
      mode = EXTI_SENSITIVITY_FALL_LOW;
      break;
    case CHANGE:
      mode = EXTI_SENSITIVITY_RISE_FALL;
      break;
    case RISING:
      mode = EXTI_SENSITIVITY_RISE_ONLY;
      break;
    case FALLING:
      mode = EXTI_SENSITIVITY_FALL_ONLY;
      break;
    }
    
    disableInterrupts();
    // muon thuc hien ham EXTI_SetExtIntSensitivity() thi ta can disableGlobalInterrput truoc, sau khi EXTI_SetExtIntSensitivity() xong thi ta enableGlobalInterrput
    EXTI_SetExtIntSensitivity(port, mode);
    enableGlobalInterrupt();
    
    if(port == EXTI_PORT_GPIOC)
    {
      EXIT_C7 = ISR;
      pinMode(PC7, GPIO_MODE_IN_PU_IT); //Cai dat pin la INPUT sau khi da thuc hien enableGlobalInterrupt(), neu thuc hien truoc enableGlobalInterrupt() se gay ra hau qua nhu sau: Khi chip chua khoi dong ma tin hieu muc 0 tac dong vao pin thi chip se bi treo, con khi chip chua khoi dong ma tin hieu muc 1 tac dong vao pin thi khong sao
    }
    if(port == EXTI_PORT_GPIOD)
    {
      EXIT_D1 = ISR;
      pinMode(PD1, GPIO_MODE_IN_PU_IT); //Cai dat pin la INPUT sau khi da thuc hien enableGlobalInterrupt(), neu thuc hien truoc enableGlobalInterrupt() se gay ra hau qua nhu sau: Khi chip chua khoi dong ma tin hieu muc 0 tac dong vao pin thi chip se bi treo, con khi chip chua khoi dong ma tin hieu muc 1 tac dong vao pin thi khong sao
    }
}



#pragma vector = 7  /* Symbol from I/O */ 
__interrupt void EXIT_PORTC_ISR(void)
{
  if(digitalRead(PC7) == LOW)
  {
    EXIT_C7();
  }
}

#pragma vector = 8  /* Symbol from I/O */ 
__interrupt void EXIT_PORTD_ISR(void)
{
  if(digitalRead(PD1) == LOW)
  {
    EXIT_D1();
  }
}