#include "stm32f411re_rcc_driver.h"

uint16_t AHB_PreScalerTable[8] = {2, 4, 8, 16, 64, 128, 256, 512};
uint8_t  APB_PreScalerTable[4] = {2, 4, 8, 16};

uint32_t RCC_GetPCLK1Value(void)
{
    uint32_t sysclk, tmp;
    uint8_t  clksrc = (RCC->CFGR >> 2 & 0x3);
    uint16_t AHB_prescaler, APB1_prescaler;

    if (clksrc == RCC_HSI_SWS)
    {
        sysclk = HSI_CLOCK;
    }
    else if (clksrc == RCC_HSE_SWS)
    {
        sysclk = HSE_CLOCK;
    }
    // else
    // {
    //     PLL CLK SOURCE
    // }

    // ABH PRESCALER
    tmp = (RCC->CFGR >> 4 & 0xF);
    if (tmp < 0x8U)
    {
        AHB_prescaler = 1;
    }
    else
    {
        AHB_prescaler = AHB_PreScalerTable[(tmp % 8)];
    }

    //  APB Low speed prescaler (APB1)
    tmp = (RCC->CFGR >> 10 & 0x7);
    if (tmp < 0x4U)
    {
        APB1_prescaler = 1;
    }
    else
    {
        APB1_prescaler = APB_PreScalerTable[(tmp % 4)];
    }

    return (uint32_t)(sysclk / (AHB_prescaler * APB1_prescaler));
}

uint32_t RCC_GetPCLK2Value(void)
{
    uint32_t sysclk, tmp;
    uint8_t  clksrc = (RCC->CFGR >> 2 & 0x3);
    uint16_t AHB_prescaler, APB2_prescaler;

    if (clksrc == RCC_HSI_SWS)
    {
        sysclk = HSI_CLOCK;
    }
    else if (clksrc == RCC_HSE_SWS)
    {
        sysclk = HSE_CLOCK;
    }
    // else
    // {
    //     PLL CLK SOURCE
    // }

    // ABH PRESCALER
    tmp = (RCC->CFGR >> 4 & 0xF);
    if (tmp < 0x8U)
    {
        AHB_prescaler = 1;
    }
    else
    {
        AHB_prescaler = AHB_PreScalerTable[(tmp % 8)];
    }

    //  APB Low speed prescaler (APB1)
    tmp = (RCC->CFGR >> 13 & 0x7);
    if (tmp < 0x4U)
    {
        APB2_prescaler = 1;
    }
    else
    {
        APB2_prescaler = APB_PreScalerTable[(tmp % 4)];
    }

    return (uint32_t)(sysclk / (AHB_prescaler * APB2_prescaler));
}