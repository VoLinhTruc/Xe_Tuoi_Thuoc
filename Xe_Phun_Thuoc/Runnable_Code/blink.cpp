#define FALSE 0
#define TRUE 1

#include "stm8s.h"

#define LED_GPIO_PORTC  (GPIOB)
#define LED_GPIO_PINC7  (GPIO_PIN_5)
#define LED_GPIO_PORTD  (GPIOB)
#define LED_GPIO_PIND2  (GPIO_PIN_6)

void Delay (uint16_t nCount);

bool a = FALSE;

int main()
{
  GPIO_Init(LED_GPIO_PORTC, (GPIO_Pin_TypeDef)LED_GPIO_PINC7, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(LED_GPIO_PORTD, (GPIO_Pin_TypeDef)LED_GPIO_PIND2, GPIO_MODE_OUT_PP_LOW_FAST);

  while (1)
  {
    /* Toggles LEDs */
    GPIO_WriteReverse(LED_GPIO_PORTC, (GPIO_Pin_TypeDef)LED_GPIO_PINC7);
       Delay(0x7FFF);
        GPIO_WriteReverse(LED_GPIO_PORTD, (GPIO_Pin_TypeDef)LED_GPIO_PIND2);
           Delay(0x7FFF);
  }
  return 0;
}

void Delay(uint16_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}




#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
