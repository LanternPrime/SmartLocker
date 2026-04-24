/**
 ******************************************************************************
 * @file           : stm32f411re.c
 * @author         : Octavio Piña
 * @brief          : Core MCU definitions and low-level hardware access for
 *                   STM32F411RE.
 *
 * @details
 * This file provides:
 *  - Base memory addresses for peripherals (AHB1, APB1, APB2)
 *  - Register definition structures (GPIO, RCC, etc.)
 *  - Peripheral base pointers (GPIOA, RCC, ...)
 *  - Low-level macros for direct register manipulation
 *
 * It acts as the hardware abstraction foundation for all peripheral drivers.
 *
 * Design Notes:
 *  - No driver logic should be implemented here.
 *  - This layer must remain independent of higher-level modules (e.g. GPIO driver).
 *  - All peripheral registers are mapped using structures that mirror the
 *    reference manual.
 *
 * Usage:
 *  - Included by peripheral drivers (e.g. GPIO driver)
 *  - Included by application code through a higher-level header (e.g. platform.h)
 *
 * References:
 *  - STM32F411 Reference Manual (RM0383)
 *  - STM32F411 Datasheet
 *
 ******************************************************************************
 */

#include "stm32f411re.h"

void NVIC_IRQConfig(IRQn_Type IRQNum, uint8_t ENoDI)
{
    uint8_t reg_index = IRQNum >> 5;
    uint8_t bit_pos   = IRQNum % 32;

    if (ENoDI)
    {
        *(NVIC_ISER0 + reg_index) |= (1 << bit_pos);
    }
    else
    {
        *(NVIC_ICER0 + reg_index) |= (1 << bit_pos);
    }
}

void NVIC_IRQPriorityConfig(IRQn_Type IRQNum, uint8_t IRQPriority)
{
    uint8_t iprn  = IRQNum >> 2;
    uint8_t index = IRQNum % 4;

    uint8_t msb_shift = (index << 3) + 4;

    *(NVIC_IPR0 + iprn) &= ~(0xFU << msb_shift);
    *(NVIC_IPR0 + iprn) |= (IRQPriority << msb_shift);
}

void Systick_Init(size_t tick_hz)
{
    uint32_t reload_value = (HSI_CLOCK / tick_hz) - 1;

    // Clean the reload Value
    *SYST_RVR &= ~(0x00FFFFFFU);
    // Set the reload value;
    *SYST_RVR |= (reload_value & 0x00FFFFFFU);

    // enable the exception and indicate the clock source
    *SYST_CSR |= (1 << 1); // TICKINT = Enables SysTick Exception
    *SYST_CSR |= (1 << 2); // CLKSOURCE = (processor clock)

    // Enable the counter
    *SYST_CSR |= (1 << 0);
}
