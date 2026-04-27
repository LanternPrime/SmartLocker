/**
 ******************************************************************************
 * @file           : stm32f411re_USART_driver.h
 * @author         : Octavio Piña
 * @brief          : Driver to USART's
 ******************************************************************************
 */

#ifndef INC_STM32F411RETX_USART_DRIVER_H_
#define INC_STM32F411RETX_USART_DRIVER_H_

#include "stm32f411re.h"

#define maxWaitCycles   100000
#define USART_MAX_DELAY -1
/*
 * @USART_App_States
 * USART application states
 */
typedef enum
{
    USART_TIMEOUT,
    USART_BUSY,
    USART_OK,
    USART_READY,
    USART_BUSY_IN_RX,
    USART_BUSY_IN_TX
} USART_States_e;

/*
 * @USART_App_Events
 * USART application Events
 */
typedef enum
{
    USART_EVENT_RX_CMPLT,
    USART_EVENT_TX_CMPLT
} USART_Events_e;

/*
 *  Configuration structure for USARTx peripheral
 */
typedef struct
{
    uint8_t USART_Mode;
    /*!< Possible Values for @USART_Mode >*/
    uint32_t USART_BaudRate;
    /*!< Possible Values for @USART_Baud >*/
    uint8_t USART_ParityCtrl;
    /*!< Possible Values for @USART_ParityControl >*/
    uint8_t USART_WordLength;
    /*!< Possible Values for @USART_WordLength >*/
    uint8_t USART_NoOfStopBits;
    /*!< Possible Values for @USART_NoOfStopBits >*/
    uint8_t USART_HW_Flow;
    /*!< Possible Values for @USART_HWFlowControl >*/
    uint8_t USART_Oversampling;
    /*!< Possible Values for @USART_Oversampling >*/
} USART_Config_t;

typedef struct USART_Handle_t
{
    USARTx_Reg_t* pUSARTx;
    /*!< USART base address (register access) */
    USART_Config_t USARTConfig;
    /*!< USART configuration (baud, mode, parity, etc.) */
    USART_States_e TxState;
    /*!< Transmission state (READY / BUSY_IN_TX) */
    USART_States_e RxState;
    /*!< Reception state (READY / BUSY_IN_RX) */
    uint8_t RxLen;
    /*!< Remaining RX frames */
    uint8_t TxLen;
    /*!< Remaining TX frames */
    uint8_t* pRxBuffer;
    /*!< Pointer to RX buffer */
    uint8_t* pTxBuffer;
    /*!< Pointer to TX buffer */
} USART_Handle_t;

/*
 *@USART_Mode
 *Possible options for USART_Mode
 */
#define USART_MODE_ONLY_TX 0
#define USART_MODE_ONLY_RX 1
#define USART_MODE_TXRX    2

/*
 *@USART_Baud
 *Possible options for USART_Baud
 */
#define USART_STD_BAUD_1200   1200
#define USART_STD_BAUD_2400   2400
#define USART_STD_BAUD_9600   9600
#define USART_STD_BAUD_19200  19200
#define USART_STD_BAUD_38400  38400
#define USART_STD_BAUD_57600  57600
#define USART_STD_BAUD_115200 115200
#define USART_STD_BAUD_230400 230400
#define USART_STD_BAUD_460800 460800
#define USART_STD_BAUD_921600 921600
#define USART_STD_BAUD_2M     2000000
#define SUART_STD_BAUD_3M     3000000

/*
 *@USART_ParityControl
 *Possible options for USART_ParityControl
 */
#define USART_PARITY_EN_ODD  2
#define USART_PARITY_EN_EVEN 1
#define USART_PARITY_DISABLE 0

/*
 *@USART_WordLength
 *Possible options for USART_WordLength
 */
#define USART_WORDLEN_8BITS 0
#define USART_WORDLEN_9BITS 1

/*
 *@USART_NoOfStopBits
 *Possible options for USART_NoOfStopBits
 */
#define USART_STOPBITS_1   0x00U
#define USART_STOPBITS_0_5 0x01U
#define USART_STOPBITS_2   0x02U
#define USART_STOPBITS_1_5 0x03U

/*
 *@USART_HWFlowControl
 *Possible options for USART_HWFlowControl
 */
#define USART_HW_FLOW_CTRL_NONE    0
#define USART_HW_FLOW_CTRL_CTS     1
#define USART_HW_FLOW_CTRL_RTS     2
#define USART_HW_FLOW_CTRL_CTS_RTS 3

/*
 *@USART_Oversampling
 *Possible options for USART_Oversampling
 */
#define OVERSAMPLING_8  8
#define OVERSAMPLING_16 16

/*
 *@USART_FLAGS
 *Possible options for USART_FLAGS
 */
#define USART_TXE_FLAG  (1 << USART_SR_TXE)
#define USART_RXNE_FLAG (1 << USART_SR_RXNE)
#define USART_TC_FLAG   (1 << USART_SR_TC)

/******************************************************************************************
 *								APIs supported
 * by this driver For more information about the APIs check the function
 * definitions
 ******************************************************************************************/
// TODO
/*
 * Peripheral Clock Control
 */
void USART_PeriClockControl(USARTx_Reg_t* pUSARTx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void USART_Init(USART_Handle_t* pUSARTHandle);
void USART_DeInit(USARTx_Reg_t* pUSARTx); // TODO

/*
 * Data Send and Receive
 */
void           USART_SendData(USART_Handle_t* pUSARTHandle, uint8_t* pTxBuffer, uint32_t Len);
USART_States_e USART_ReceiveData(USART_Handle_t* pUSARTHandle, uint8_t* pRxBuffer, uint32_t Len, uint8_t timeout);
USART_States_e USART_SendDataIT(USART_Handle_t* pUSARTHandle, uint8_t* pTxBuffer, uint32_t Len);
USART_States_e USART_ReceiveDataIT(USART_Handle_t* pUSARTHandle, uint8_t* pRxBuffer, uint32_t Len);

/*
 * IRQ Configuration and ISR handling
 */
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority); // TODO

void USART_IRQHandling(USART_Handle_t* pHandle);
/*
 * Other Peripheral Control APIs
 */

void USART_SetBaudRate(USARTx_Reg_t* pUSARTx, uint32_t BaudRate);

uint8_t USART_GetFlagStatus(USARTx_Reg_t* pUSARTx, uint32_t FlagName);
void    USARTClearFlag(USARTx_Reg_t* pUSARTx, uint32_t FlagName); // TODO

/*
 * Application callback
 */
void USART_ApplicationEventCallback(USART_Handle_t* pUSARTHandle, uint8_t AppEv);

#endif /*INC_STM32F411RETX_USART_DRIVER_H_*/
