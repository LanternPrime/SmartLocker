#include "stm32f411re.h"

#define RCC_HSI_SWS 0
#define RCC_HSE_SWS 0
#define RCC_PLL_SWS 0

uint32_t RCC_GetPCLK1Value(void);
uint32_t RCC_GetPCLK2Value(void);