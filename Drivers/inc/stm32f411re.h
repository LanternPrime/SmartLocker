/**
 ******************************************************************************
 * @file           : stm32f411re.h
 * @author         : Octavio Piña
 * @brief          : Low-level hardware definitions for STM32F411RE MCU.
 *
 * @details
 * This file provides the fundamental hardware abstraction layer by defining:
 *  - Base memory addresses for system buses (AHB1, APB1, APB2)
 *  - Peripheral base addresses (GPIO, RCC, etc.)
 *  - Register definition structures matching the reference manual
 *  - Peripheral access macros (e.g. GPIOA, RCC)
 *
 * This file represents the direct mapping between the STM32F411RE hardware
 * and software, enabling register-level programming without relying on
 * external libraries such as HAL.
 *
 * Design Guidelines:
 *  - This file must remain independent of all drivers.
 *  - No peripheral logic or driver implementation should be added here.
 *  - Only hardware definitions and low-level macros are allowed.
 *
 * Usage:
 *  - Included by peripheral drivers (e.g. GPIO, UART, SPI)
 *  - Can be included directly in application code if needed
 *
 * References:
 *  - STM32F411 Reference Manual (RM0383)
 *  - STM32F411 Datasheet
 *
 ******************************************************************************
 */

#ifndef INC_STM32F4XX_H_
#define INC_STM32F4XX_H_
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define __s_inline static inline
#define __vo       volatile

#define HSI_CLOCK 16000000U

// ARM CORTEX Mx Processor NVIC ISERx Register Addr
#define NVIC_ISER0 ((__vo uint32_t*)0xE000E100)
// ARM CORTEX Mx Processor NVIC ICERx Register Addr
#define NVIC_ICER0 ((__vo uint32_t*)0XE000E180)
// ARM CORTEX Mx Processor NVIC IPRx Register Addr
#define NVIC_IPR0 ((__vo uint32_t*)0xE000E400)
// ARM CORTEX Mx Processor Systick Register Addr
#define SYST_CSR ((__vo uint32_t*)0xE000E010)
#define SYST_RVR ((__vo uint32_t*)0xE000E014)

// AHBx and APBx Bus Peripheral base Addrs

#define PERP_ADDR (0X40000000U)
#define APB1_ADDR (PERP_ADDR)
#define APB2_ADDR (0X40010000U)
#define AHB1_ADDR (0X40020000U)
#define AHB2_ADDR (0X50000000U)

// Base Addr of peripherals which are hanging on AHB1 bus
#define GPIOA_ADDR (AHB1_ADDR + 0x0000)
#define GPIOB_ADDR (AHB1_ADDR + 0x0400)
#define GPIOC_ADDR (AHB1_ADDR + 0x0800)
#define GPIOD_ADDR (AHB1_ADDR + 0x0C00)
#define GPIOE_ADDR (AHB1_ADDR + 0x1000)
#define GPIOH_ADDR (AHB1_ADDR + 0x1C00)
#define RCC_ADDR   (AHB1_ADDR + 0x3800)

// Base Addr of peripherals which are hanging on APB2 bus
#define EXTI_ADDR   (APB2_ADDR + 0x3C00)
#define SYSCFG_ADDR (APB2_ADDR + 0x3800)

// Peripheral Register Definition Structures
// GPIO
typedef struct
{
    __vo uint32_t MODER;   // GPIO port mode register
    __vo uint32_t OTYPER;  // GPIO port output type register
    __vo uint32_t OSPEEDR; // GPIO port output speed register
    __vo uint32_t PUPDR;   // GPIO port pull-up/pull-down register
    __vo uint32_t IDR;     // GPIO port input data register
    __vo uint32_t ODR;     // GPIO port output data register
    __vo uint32_t BSRR;    // GPIO port bit set/reset register
    __vo uint32_t LCKR;    // GPIO port configuration lock register
    __vo uint32_t AFR[2];  // GPIO alternate function low[0] & high[1] register
} GPIOx_Reg_t;

// RCC
typedef struct
{
    __vo uint32_t CR;
    __vo uint32_t PLLCFGR;
    __vo uint32_t CFGR;
    __vo uint32_t CIR;
    __vo uint32_t AHB1RSTR;
    __vo uint32_t AHB2RSTR;
    __vo uint32_t RESERVED0[2];
    __vo uint32_t APB1RSTR;
    __vo uint32_t APB2RSTR;
    __vo uint32_t RESERVED1[2];
    __vo uint32_t AHB1ENR;
    __vo uint32_t AHB2ENR;
    __vo uint32_t RESERVED2[2];
    __vo uint32_t APB1ENR;
    __vo uint32_t APB2ENR;
    __vo uint32_t RESERVED3[2];
    __vo uint32_t AHB1LPENR;
    __vo uint32_t AHB2LPENR;
    __vo uint32_t RESERVED4[2];
    __vo uint32_t APB1LPENR;
    __vo uint32_t APB2LPENR;
    __vo uint32_t RESERVED5[2];
    __vo uint32_t BDCR;
    __vo uint32_t CSR;
    __vo uint32_t RESERVED6[2];
    __vo uint32_t SSCGR;
    __vo uint32_t PLLI2SCFGR;
    __vo uint32_t RESERVED7;
    __vo uint32_t DCKCFGR;

} RCC_Reg_t;

// EXTI
typedef struct
{
    uint32_t IMR;
    uint32_t EMR;
    uint32_t RTSR;
    uint32_t FTSR;
    uint32_t SWIER;
    uint32_t PR;
} EXTI_Reg_t;

// SYSCFG
typedef struct
{
    uint32_t MEMRMP;
    uint32_t PMC;
    uint32_t EXTICR[4];
    uint32_t CMPCR;
} SYSCFG_Reg_t;

#define GPIOA ((GPIOx_Reg_t*)GPIOA_ADDR)
#define GPIOB ((GPIOx_Reg_t*)GPIOB_ADDR)
#define GPIOC ((GPIOx_Reg_t*)GPIOC_ADDR)
#define GPIOD ((GPIOx_Reg_t*)GPIOD_ADDR)
#define GPIOE ((GPIOx_Reg_t*)GPIOE_ADDR)
#define GPIOH ((GPIOx_Reg_t*)GPIOH_ADDR)

#define RCC ((RCC_Reg_t*)RCC_ADDR)

#define EXTI   ((EXTI_Reg_t*)EXTI_ADDR)
#define SYSCFG ((SYSCFG_Reg_t*)SYSCFG_ADDR)

// CLK EN Macros for GPIOx Peripherals

#define Px_PCLK_EN(port) (RCC->AHB1ENR |= (1 << (port)))
#define Px_PCLK_DI(port) (RCC->AHB1ENR &= ~(1 << (port)))

// CLK EN Macros for SYSCFG Peripheral

#define SYSCFG_PCKL_EN() (RCC->APB2ENR |= (1 << 14))

// IRQ (Interrupt Request) Numbers of STM32F411RETx MCU
typedef enum
{
    /******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
    SysTick_IRQn = -1, /*!< 15 Cortex-M4 System Tick Interrupt                                */
    /******  STM32 specific Interrupt Numbers **********************************************************************/
    EXTI0_IRQn     = 6,  /*!< EXTI Line0 Interrupt                                              */
    EXTI1_IRQn     = 7,  /*!< EXTI Line1 Interrupt                                              */
    EXTI2_IRQn     = 8,  /*!< EXTI Line2 Interrupt                                              */
    EXTI3_IRQn     = 9,  /*!< EXTI Line3 Interrupt                                              */
    EXTI4_IRQn     = 10, /*!< EXTI Line4 Interrupt                                              */
    EXTI9_5_IRQn   = 23, /*!< External Line[9:5] Interrupts                                     */
    EXTI15_10_IRQn = 40, /*!< External Line[15:10] Interrupts                                   */
} IRQn_Type;
/******************************************************************************************
 *Bit position definitions of GPIO peripheral
 ******************************************************************************************/

// Generics
#define ENABLE     1
#define DISABLE    0
#define HIGH       1
#define LOW        0
#define SET        ENABLE
#define RESET      DISABLE
#define FLAG_RESET RESET
#define FLAG_SET   SET

void NVIC_IRQConfig(IRQn_Type IRQNum, uint8_t ENoDI);
void NVIC_IRQPriorityConfig(IRQn_Type IRQNum, uint8_t IRQPriority);
void Systick_Init(size_t tick_hz);
#endif /* INC_STM32F4XX_H_ */
