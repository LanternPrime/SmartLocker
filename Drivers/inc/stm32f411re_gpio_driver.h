/**
 ******************************************************************************
 * @file           : stm32f411re_gpio_driver.h
 * @author         : Octavio Piña
 * @brief          : Driver to GPIO's
 ******************************************************************************
 */

#include "stm32f411re.h"

#define SET_REG(reg, value, pin_num, n)                   \
    do                                                    \
    {                                                     \
        (reg) &= ~(((1u << (n)) - 1) << (n * (pin_num))); \
        (reg) |= ((value) << ((n) * (pin_num)));          \
    } while (0u)

#define SINGLE_REG 1
#define DOUBLE_REG 2
#define TRIPLE_REG 3
#define QUAD_REG   4

typedef struct GPIO_PinConfig_t
{
    uint8_t GPIO_PinNum;        /*!< Possible Values for @GPIO_PIN_NUM >*/
    uint8_t GPIO_PinMode;       /*!< Possible Values for @GPIO_PIN_MODES >*/
    uint8_t GPIO_PinSpeed;      /*!< Possible Values for @GPIO_PIN_SPEEDS >*/
    uint8_t GPIO_PinPuPdCtlr;   /*!< Possible Values for @GPIO_PIN_PUPD >*/
    uint8_t GPIO_PinOPType;     /*!< Possible Values for @GPIO_PIN_OPT >*/
    uint8_t GPIO_PinAltFunMode; /*!< Possible Values for @GPIO_PIN_MODES >*/
} GPIO_PinConfig_t;

typedef struct GPIO_Handle_t
{
    GPIOx_Reg_t*     pGPIOx; // Holds the base addr of the GPIO port to which pin belongs
    GPIO_PinConfig_t GPIO_PinConfig;
} GPIO_Handle_t;

typedef enum GPIO_Status_t
{
    GPIO_OK    = 0,
    GPIO_ERROR = 1
} GPIO_Status_t;

/*
 * @GPIO_PIN_NUM
 * GPIO Pin Possible Numbers
 * */
#define GPIO_PIN0  0
#define GPIO_PIN1  1
#define GPIO_PIN2  2
#define GPIO_PIN3  3
#define GPIO_PIN4  4
#define GPIO_PIN5  5
#define GPIO_PIN6  6
#define GPIO_PIN7  7
#define GPIO_PIN8  8
#define GPIO_PIN9  9
#define GPIO_PIN10 10
#define GPIO_PIN11 11
#define GPIO_PIN12 12
#define GPIO_PIN13 13
#define GPIO_PIN14 14
#define GPIO_PIN15 15

#define GPIO_PIN_RESET 0
#define GPIO_PIN_SET   1

/*
 * @GPIO_PIN_MODES
 * GPIO Pin Possible Modes
 * */
#define GPIO_MODE_IN     0x00
#define GPIO_MODE_OUT    0x01
#define GPIO_MODE_ALTFUN 0x02
#define GPIO_MODE_ANALOG 0x03

#define GPIO_MODE_IT_FT  4
#define GPIO_MODE_IT_RT  5
#define GPIO_MODE_IT_RFT 6

/*
 * @GPIO_PIN_OPT
 * GPIO pin possible output types
 * */
#define GPIO_OPT_PP 0
#define GPIO_OPT_OD 1

/*
 * @GPIO_PIN_SPEEDS
 * GPIO Pin Possible Speeds
 * */
#define GPIO_SPEED_LOW  0
#define GPIO_SPEED_MED  1
#define GPIO_SPEED_FST  2
#define GPIO_SPEED_VFST 3

/*
 * @GPIO_PIN_PUPD
 * GPIO pin Pull up & Pull Down configuration macros
 * */
#define GPIO_NO_PUPD 0
#define GPIO_PIN_PU  1
#define GPIO_PIN_PD  2

// APIs supported by this driver
// Peripheral Clock Enable
void GPIO_PeriClockEN(GPIOx_Reg_t* pGPIOx, uint8_t ENoDI);

// Init & De-init
void GPIO_Init(GPIO_Handle_t* hgpio);
void GPIO_DeInit(GPIO_Handle_t* hgpio);
void GPIO_DigitalPin(uint8_t mode, GPIOx_Reg_t* port, uint8_t pin);

// Read & Write
uint8_t GPIO_ReadFromInputPin(GPIOx_Reg_t* pGPIOx, uint8_t PinNum);
void    GPIO_WriteToOutputPin(GPIOx_Reg_t* pGPIOx, uint8_t PinNum, uint8_t value);
void    GPIO_ToggleOutputPin(GPIOx_Reg_t* pGPIOx, uint8_t PinNum);

// IRQ Config & Handling
void GPIO_IRQHandling(uint8_t PinNum);
