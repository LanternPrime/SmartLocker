/**
 ******************************************************************************
 * @file           : stm32f411re_gpio_driver.c
 * @author         : Octavio Piña
 * @brief          : Driver to GPIO's
 ******************************************************************************
 */
#include "stm32f411re_gpio_driver.h"

__s_inline uint8_t GPIO_PortToIndex(GPIOx_Reg_t* port)
{
    if (port == GPIOA) return 0U;
    if (port == GPIOB) return 1U;
    if (port == GPIOC) return 2U;
    if (port == GPIOD) return 3U;
    if (port == GPIOE) return 4U;
    if (port == GPIOH) return 7U;

    return 0xFFU;
}

void GPIO_PeriClockControl(GPIOx_Reg_t* pGPIOx, uint8_t ENoDI)
{
    if (ENoDI == ENABLE)
        Px_PCLK_EN(GPIO_PortToIndex(pGPIOx));
    else
        Px_PCLK_DI(GPIO_PortToIndex(pGPIOx));
}

void GPIO_Init(GPIO_Handle_t* hgpio)
{
    // ENABLE GPIO PERIPHERAL CLOCK
    GPIO_PeriClockControl(hgpio->pGPIOx, ENABLE);

    // CONFIGURE THE MODE
    if (hgpio->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
    {
        SET_REG(hgpio->pGPIOx->MODER, hgpio->GPIO_PinConfig.GPIO_PinMode, hgpio->GPIO_PinConfig.GPIO_PinNum, DOUBLE_REG);
    }
    else
    {
        SET_REG(hgpio->pGPIOx->MODER, GPIO_MODE_IN, hgpio->GPIO_PinConfig.GPIO_PinNum, DOUBLE_REG);

        // Interrupt Mode
        if (hgpio->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
        {
            // 1.Configure The FSTR (FALLING TRIGGER)
            EXTI->FTSR |= (1 << hgpio->GPIO_PinConfig.GPIO_PinNum);
            EXTI->RTSR &= ~(1 << hgpio->GPIO_PinConfig.GPIO_PinNum);
        }
        if (hgpio->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
        {
            // 1.Configure The RSTR (RISING TRIGGER)
            EXTI->RTSR |= (1 << hgpio->GPIO_PinConfig.GPIO_PinNum);
            EXTI->FTSR &= ~(1 << hgpio->GPIO_PinConfig.GPIO_PinNum);
        }
        else if (hgpio->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
        {
            // 1.Configure Both the FSTR & RSTR
            EXTI->RTSR |= (1 << hgpio->GPIO_PinConfig.GPIO_PinNum);
            EXTI->FTSR |= (1 << hgpio->GPIO_PinConfig.GPIO_PinNum);
        }
        // 2. Configure the GPIO port in SYSCFG_EXTICR
        uint8_t exti_r   = hgpio->GPIO_PinConfig.GPIO_PinNum >> 2;
        uint8_t exti_s   = hgpio->GPIO_PinConfig.GPIO_PinNum % 4;
        uint8_t portcode = GPIO_PortToIndex(hgpio->pGPIOx);
        SYSCFG_PCKL_EN();

        SET_REG(SYSCFG->EXTICR[exti_r], portcode, exti_s, QUAD_REG);

        // 3.Enable the EXTI Interrupt Delivery using IMR (INTERRUPT MASK REGISTER)
        EXTI->IMR |= (1 << hgpio->GPIO_PinConfig.GPIO_PinNum);

        GPIO_IRQConfig(hgpio);
    }

    // CONFIGURE SPEED
    SET_REG(hgpio->pGPIOx->OSPEEDR, hgpio->GPIO_PinConfig.GPIO_PinSpeed, hgpio->GPIO_PinConfig.GPIO_PinNum, DOUBLE_REG);

    // CONFIGURE PUPD
    SET_REG(hgpio->pGPIOx->PUPDR, hgpio->GPIO_PinConfig.GPIO_PinPuPdCtlr, hgpio->GPIO_PinConfig.GPIO_PinNum, DOUBLE_REG);

    // CONFIGURE OTYPER
    SET_REG(hgpio->pGPIOx->OTYPER, hgpio->GPIO_PinConfig.GPIO_PinOPType, hgpio->GPIO_PinConfig.GPIO_PinNum, SINGLE_REG);

    // CONFIGURE ALT FUNC
    if (hgpio->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFUN)
    {
        uint32_t reg_index = hgpio->GPIO_PinConfig.GPIO_PinNum / 8;
        uint32_t bit_pos   = hgpio->GPIO_PinConfig.GPIO_PinNum % 8;
        SET_REG(hgpio->pGPIOx->AFR[reg_index], hgpio->GPIO_PinConfig.GPIO_PinAltFunMode, bit_pos, QUAD_REG);
    }
}

uint8_t GPIO_ReadFromInputPin(GPIOx_Reg_t* pGPIOx, uint8_t PinNum)
{
    uint8_t input = (uint8_t)((pGPIOx->IDR >> PinNum) & 0x01);
    return input;
}

void GPIO_WriteToOutputPin(GPIOx_Reg_t* pGPIOx, uint8_t PinNum, uint8_t value)
{
    SET_REG(pGPIOx->ODR, value, PinNum, SINGLE_REG);
}

void GPIO_ToggleOutputPin(GPIOx_Reg_t* pGPIOx, uint8_t PinNum)
{
    pGPIOx->ODR ^= (1 << PinNum);
}

void GPIO_DigitalPin(uint8_t mode, GPIOx_Reg_t* port, uint8_t pin)
{
    GPIO_Handle_t hGPIO;

    hGPIO.pGPIOx                          = port;
    hGPIO.GPIO_PinConfig.GPIO_PinNum      = pin;
    hGPIO.GPIO_PinConfig.GPIO_PinMode     = mode;
    hGPIO.GPIO_PinConfig.GPIO_PinPuPdCtlr = GPIO_NO_PUPD;
    hGPIO.GPIO_PinConfig.GPIO_PinOPType   = GPIO_OPT_PP;
    hGPIO.GPIO_PinConfig.GPIO_PinSpeed    = GPIO_SPEED_FST;

    GPIO_Init(&hGPIO);
}

void GPIO_IRQConfig(GPIO_Handle_t* hgpio)
{
    uint8_t pin = hgpio->GPIO_PinConfig.GPIO_PinNum;

    // CONFIGURE EXTI
    if (pin <= 4)
        NVIC_IRQConfig(EXTI0_IRQn + pin, ENABLE);
    else if (pin <= 9)
        NVIC_IRQConfig(EXTI9_5_IRQn, ENABLE);
    else
        NVIC_IRQConfig(EXTI15_10_IRQn, ENABLE);
}

void GPIO_IRQHandling(uint8_t pinNum)
{
    if (EXTI->PR & (1 << pinNum))
        EXTI->PR |= (1 << pinNum); // clear pending flag

    GPIO_ApplicationEventCallback(pinNum);
}

__weak void USART_ApplicationEventCallback(uint8_t pinNum) {}