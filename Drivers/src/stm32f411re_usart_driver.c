/**
 ******************************************************************************
 * @file           : stm32f411re_usart_driver.c
 * @author         : Octavio Piña
 * @brief          : Driver to USART's
 ******************************************************************************
 */

#include "stm32f411re_usart_driver.h"
#include "stm32f411re_rcc_driver.h"

/*********************************************************************
 * @fn      		  - USART_PeriClockControl
 *
 * @brief             - Enables or disables the peripheral clock for a given USART peripheral
 *
 * @param[in]         - pUSARTx : Base address of the USART peripheral (USART1, USART2, USART6)
 * @param[in]         - EnorDi  : Enable or Disable macro
 *                               ENABLE  - Enables peripheral clock
 *                               DISABLE - Disables peripheral clock
 *
 * @return            - None
 *
 * @Note              - This function configures the corresponding RCC register
 *                      to control the clock of the selected USART peripheral.
 *                      Must be called before initializing or using the USART.
 *
 *********************************************************************/
void USART_PeriClockControl(USARTx_Reg_t* pUSARTx, uint8_t EnorDi)
{
    if (EnorDi)
    {
        if (pUSARTx == USART1)
            USART1_PCKL_EN();
        else if (pUSARTx == USART2)
            USART2_PCKL_EN();
        else if (pUSARTx == USART6)
            USART6_PCKL_EN();
    }
    else
    {
        if (pUSARTx == USART1)
            USART1_PCKL_DI();
        else if (pUSARTx == USART2)
            USART2_PCKL_DI();
        else if (pUSARTx == USART6)
            USART6_PCKL_DI();
    }
}

/*********************************************************************
 * @fn        - USART_Init
 *
 * @brief     - Initializes the specified USART peripheral based on
 *              the configuration provided in the handle
 *
 * @param[in] - pUSARTHandle : Pointer to USART handle structure
 *                             containing peripheral base address
 *                             and configuration settings
 *
 * @return    - None
 *
 * @Note      - This function configures:
 *                - CR1 (mode, word length, parity, oversampling)
 *                - CR2 (stop bits)
 *                - CR3 (hardware flow control)
 *                - BRR (baud rate via USART_SetBaudRate)
 *              It also enables the peripheral clock before configuration.
 *
 *********************************************************************/
void USART_Init(USART_Handle_t* pUSARTHandle)
{
    uint32_t tmp_reg = 0;
    // ENABLE USART PERIPHERAL CLOCK
    USART_PeriClockControl(pUSARTHandle->pUSARTx, ENABLE);

    /******************************** Configuration of CR1 ******************************************/
    // CONFIGURE MODE
    if (pUSARTHandle->USARTConfig.USART_Mode == USART_MODE_ONLY_TX)
    {
        // ONLY TX
        tmp_reg |= (0x1U << USART_CR1_TE);
    }
    else if (pUSARTHandle->USARTConfig.USART_Mode == USART_MODE_ONLY_RX)
    {
        // ONLY RX
        tmp_reg |= (0x1U << USART_CR1_RE);
    }
    else if (pUSARTHandle->USARTConfig.USART_Mode == USART_MODE_TXRX)
    {
        // BOTH TX & RX
        tmp_reg |= (0x1U << USART_CR1_TE) | (0x1U << USART_CR1_RE);
    }

    // CONFIGURE WORD LENGTH
    if (pUSARTHandle->USARTConfig.USART_WordLength == USART_WORDLEN_8BITS)
    {
        // 8 Bits
        tmp_reg &= ~(0x1U << USART_CR1_M);
    }
    else if (pUSARTHandle->USARTConfig.USART_WordLength == USART_WORDLEN_9BITS)
    {
        // 9 Bits
        tmp_reg |= (0x1U << USART_CR1_M);
    }

    // CONFIGURE PARITY CONTROL
    if (pUSARTHandle->USARTConfig.USART_ParityCtrl != USART_PARITY_DISABLE)
    {
        tmp_reg |= (0x01U << USART_CR1_PCE); // ENABLE PARITY
        if (pUSARTHandle->USARTConfig.USART_ParityCtrl == USART_PARITY_EN_EVEN)
        {
            tmp_reg &= ~(0x1U << USART_CR1_PS); // EVEN
        }
        else if (pUSARTHandle->USARTConfig.USART_ParityCtrl == USART_PARITY_EN_ODD)
        {
            tmp_reg |= (0x1U << USART_CR1_PS); // ODD
        }
    }

    // CONFIGURE OVERSAMPLING
    if (pUSARTHandle->USARTConfig.USART_Oversampling == OVERSAMPLING_16)
    {
        // OVERSAMPLING_16 (PRECISION)
        tmp_reg &= ~(0x1U << USART_CR1_OVER8);
    }
    else if (pUSARTHandle->USARTConfig.USART_Oversampling == OVERSAMPLING_8)
    {
        // OVERSAMPLING_8 (HIGH SPEED)
        tmp_reg |= (0x1U << USART_CR1_OVER8);
    }

    pUSARTHandle->pUSARTx->CR1 = tmp_reg;

    /******************************** Configuration of CR2 ******************************************/
    tmp_reg = 0;
    // CONFIGURE STOP bits
    tmp_reg |= (pUSARTHandle->USARTConfig.USART_NoOfStopBits & 0x3U) << USART_CR2_STOP;
    pUSARTHandle->pUSARTx->CR2 = tmp_reg;

    /******************************** Configuration of CR3 ******************************************/

    tmp_reg = 0;
    if (pUSARTHandle->USARTConfig.USART_HW_Flow != USART_HW_FLOW_CTRL_NONE)
    {
        if (pUSARTHandle->USARTConfig.USART_HW_Flow == USART_HW_FLOW_CTRL_CTS)
        {
            tmp_reg |= (1 << USART_CR3_CTSE);
        }
        else if (pUSARTHandle->USARTConfig.USART_HW_Flow == USART_HW_FLOW_CTRL_RTS)
        {
            tmp_reg |= (1 << USART_CR3_RTSE);
        }
        else if (pUSARTHandle->USARTConfig.USART_HW_Flow == USART_HW_FLOW_CTRL_CTS_RTS)
        {
            tmp_reg |= (1 << USART_CR3_CTSE) | (1 << USART_CR3_RTSE);
        }
    }

    pUSARTHandle->pUSARTx->CR3 = tmp_reg;

    /********************** Configuration of BRR (BAUDRATE REGISTER) ****************************/

    // CONFIGURE BAUD RATE
    USART_SetBaudRate(pUSARTHandle->pUSARTx, pUSARTHandle->USARTConfig.USART_BaudRate);

    pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_UE); // ENABLE USART
}

/*********************************************************************
 * @fn        - USART_SetBaudRate
 *
 * @brief     - Sets the baud rate of the specified USART peripheral
 *
 * @param[in] - pUSARTx  : Pointer to USART peripheral (USART1/2/6)
 * @param[in] - BaudRate : Desired baud rate value
 *
 * @return    - None
 *
 * @Note      - Uses the correct peripheral clock (PCLK1/PCLK2) and
 *              current oversampling mode (OVER8) to compute and write
 *              the value into the BRR register.
 *
 *********************************************************************/
void USART_SetBaudRate(USARTx_Reg_t* pUSARTx, uint32_t BaudRate)
{
    uint32_t fClk, userDiv;
    uint32_t mantissa, fraction, tmpReg = 0;

    // SELECT THE CORRECT PERIPHERAL CLOCK BASED ON USART INSTANCE
    if (pUSARTx == USART1 || pUSARTx == USART6)
    {
        fClk = RCC_GetPCLK2Value(); // USART1 & USART6 are on APB2
    }
    else if (pUSARTx == USART2)
    {
        fClk = RCC_GetPCLK1Value(); // USART2 is on APB1
    }

    // CALCULATE SCALED USARTDIV (×100) TO AVOID FLOAT OPERATIONS
    // FORMULA DEPENDS ON OVERSAMPLING MODE
    if (pUSARTx->CR1 & (1 << USART_CR1_OVER8))
    {
        // OVER8 = 1 → oversampling by 8
        // USARTDIV × 100 = (25 × fClk) / (2 × BaudRate)
        userDiv = (25 * fClk) / (2 * BaudRate);
    }
    else
    {
        // OVER8 = 0 → oversampling by 16
        // USARTDIV × 100 = (25 × fClk) / (4 × BaudRate)
        userDiv = (25 * fClk) / (4 * BaudRate);
    }

    // EXTRACT MANTISSA PART (INTEGER PART OF USARTDIV)
    mantissa = userDiv / 100;

    // PLACE MANTISSA IN BRR[15:4]
    tmpReg |= (mantissa << 4);

    // EXTRACT FRACTION PART (DECIMAL PART OF USARTDIV SCALED BY 100)
    fraction = userDiv - (mantissa * 100);

    // CONVERT FRACTION TO BRR FORMAT BASED ON OVERSAMPLING
    if (pUSARTx->CR1 & (1 << USART_CR1_OVER8))
    {
        // OVER8 = 1 → FRACTION REPRESENTED IN 3 BITS (0–7)
        // SCALE: (fraction × 8) / 100 WITH ROUNDING (+50)
        fraction = (((fraction * OVERSAMPLING_8) + 50) / 100) & ((uint8_t)0x07);
    }
    else
    {
        // OVER8 = 0 → FRACTION REPRESENTED IN 4 BITS (0–15)
        // SCALE: (fraction × 16) / 100 WITH ROUNDING (+50)
        fraction = (((fraction * OVERSAMPLING_16) + 50) / 100) & ((uint8_t)0x0F);
    }

    // PLACE FRACTION IN BRR[3:0]
    tmpReg |= fraction;

    // WRITE FINAL VALUE TO BAUD RATE REGISTER
    pUSARTx->BRR = tmpReg;
}

void USART_SendData(USART_Handle_t* pUSARTHandle, uint8_t* pTxBuffer, uint32_t Len)
{
    uint16_t* pData;
    // Loop over until "Len" number of bytes are transferred
    for (size_t i = 0; i < Len; i++)
    {
        // Wait until TXE flag is set in the SR
        while (!USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_TXE_FLAG));

        // Check the USART_WordLength item for 9BIT or 8BIT in a frame
        if (pUSARTHandle->USARTConfig.USART_WordLength == USART_WORDLEN_9BITS)
        {
            pData                     = (uint16_t*)pTxBuffer;
            pUSARTHandle->pUSARTx->DR = (*pData & 0x1FFU);
            if (pUSARTHandle->USARTConfig.USART_ParityCtrl == USART_PARITY_DISABLE)
            {
                // No parity is used in this transfer. so, 9bits of user data will be sent
                pTxBuffer += 2;
            }
            else
            {
                // Parity bit is used in this transfer . so , 8bits of user data will be sent
                pTxBuffer++;
            }
        }
        else if (pUSARTHandle->USARTConfig.USART_WordLength == USART_WORDLEN_8BITS)
        {
            pUSARTHandle->pUSARTx->DR = *pTxBuffer;
            pTxBuffer++;
        }
    }

    // Implement the code to wait till TC flag is set in the SR
    while (!USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_TC_FLAG));
}

USART_States_e USART_SendDataIT(USART_Handle_t* pUSARTHandle, uint8_t* pTxBuffer, uint32_t Len)
{
    if (pUSARTHandle->TxState != USART_BUSY_IN_TX)
    {
        pUSARTHandle->pTxBuffer = pTxBuffer;
        pUSARTHandle->TxLen     = Len;
        pUSARTHandle->TxState   = USART_BUSY_IN_TX;

        // Enable interrupt for TXE
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TXEIE);

        // Enable interrupt for TC
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_TCIE);
        return USART_OK;
    }
    return USART_BUSY;
}

/*********************************************************************
 * @fn        - USART_ReceiveData
 *
 * @brief     - Receives data from the specified USART peripheral
 *              using polling mode
 *
 * @param[in] - pUSARTHandle : Pointer to USART handle structure
 * @param[in] - pRxBuffer    : Pointer to receive buffer
 * @param[in] - Len          : Number of data frames to receive
 *
 * @return    - None
 *
 * @Note      - Waits for RXNE flag and reads data from DR.
 *              Handles 8-bit and 9-bit frames with/without parity.
 *
 *********************************************************************/
USART_States_e USART_ReceiveData(USART_Handle_t* pUSARTHandle, uint8_t* pRxBuffer, uint32_t Len, uint8_t timeout)
{
    uint32_t count;
    // LOOP UNTIL "Len" NUMBER OF DATA FRAMES ARE RECEIVED
    for (size_t i = 0; i < Len; i++)
    {
        count = 0;
        // WAIT UNTIL RXNE (RECEIVE DATA REGISTER NOT EMPTY) FLAG IS SET
        // THIS INDICATES NEW DATA HAS ARRIVED IN DR
        while (!USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_RXNE_FLAG))
        {
            if (count++ > timeout && timeout != USART_MAX_DELAY)
                return USART_TIMEOUT;
        }

        // CHECK WORD LENGTH CONFIGURATION (8-BIT OR 9-BIT FRAME)
        if (pUSARTHandle->USARTConfig.USART_WordLength == USART_WORDLEN_8BITS)
        {
            // 8-BIT FRAME
            if (pUSARTHandle->USARTConfig.USART_ParityCtrl != USART_PARITY_DISABLE)
            {
                // PARITY ENABLED:
                // MSB IS USED FOR PARITY, SO ONLY 7 BITS ARE VALID USER DATA
                *pRxBuffer = pUSARTHandle->pUSARTx->DR & 0x7F;
                // MOVE BUFFER POINTER BY 1 BYTE
                pRxBuffer++;
            }
            else
            {
                // NO PARITY:
                // FULL 8 BITS ARE USER DATA
                *pRxBuffer = pUSARTHandle->pUSARTx->DR;
                // MOVE BUFFER POINTER BY 1 BYTE
                pRxBuffer++;
            }
        }
        else if (pUSARTHandle->USARTConfig.USART_WordLength == USART_WORDLEN_9BITS)
        {
            // 9-BIT FRAME
            if (pUSARTHandle->USARTConfig.USART_ParityCtrl != USART_PARITY_DISABLE)
            {
                // PARITY ENABLED:
                // ONLY 8 BITS ARE USER DATA (1 BIT USED FOR PARITY)
                *pRxBuffer = pUSARTHandle->pUSARTx->DR;
                pRxBuffer++;
            }
            else
            {
                // NO PARITY:
                // FULL 9 BITS ARE USER DATA → STORE AS 16-BIT VALUE
                *(uint16_t*)pRxBuffer = pUSARTHandle->pUSARTx->DR & (uint16_t)0x1FF;

                // ADVANCE POINTER BY 2 BYTES (SINCE 9-BIT DATA STORED IN 16-BIT)
                pRxBuffer += 2;
            }
        }
    }
    return USART_OK;
}

USART_States_e USART_ReceiveDataIT(USART_Handle_t* pUSARTHandle, uint8_t* pRxBuffer, uint32_t Len)
{

    if (pUSARTHandle->RxState != USART_BUSY_IN_RX)
    {
        pUSARTHandle->pRxBuffer = pRxBuffer;
        pUSARTHandle->RxLen     = Len;
        pUSARTHandle->RxState   = USART_BUSY_IN_RX;

        // Implement the code to enable interrupt for RXNE
        pUSARTHandle->pUSARTx->CR1 |= (1 << USART_CR1_RXNEIE);
        return USART_OK;
    }
    return USART_BUSY;
}

uint8_t USART_GetFlagStatus(USARTx_Reg_t* pUSARTx, uint32_t FlagName)
{
    if (pUSARTx->SR & FlagName)
    {
        return FLAG_SET;
    }

    return FLAG_RESET;
}

void USART_IRQHandling(USART_Handle_t* pHandle)
{
    __vo uint32_t flag_set, interrupt_enabled;
    /************************** IRQ HANDLING FOR TC *****************************/
    flag_set          = pHandle->pUSARTx->SR & (1 << USART_SR_TC);
    interrupt_enabled = pHandle->pUSARTx->CR1 & (1 << USART_CR1_TCIE);
    if (flag_set && interrupt_enabled)
    {
        // this interrupt is because of TXE
        if (pHandle->TxState == USART_BUSY_IN_TX)
        {
            // Check the TxLen . If it is zero then close the data transmission
            if (!pHandle->TxLen)
            {
                if (pHandle->TxLen <= 0)
                {
                    pHandle->pTxBuffer = NULL;
                    pHandle->TxLen     = 0;
                    pHandle->TxState   = USART_READY;

                    // DISABLE TCIE FLAG
                    pHandle->pUSARTx->CR1 &= ~(1 << USART_CR1_TCIE);
                    // DISABLE TC FLAG
                    pHandle->pUSARTx->SR &= ~(1 << USART_SR_TC);
                    USART_ApplicationEventCallback(pHandle, USART_EVENT_TX_CMPLT);
                }
            }
        }
    }

    /************************** IRQ HANDLING FOR TXE *****************************/
    flag_set          = pHandle->pUSARTx->SR & (1 << USART_SR_TXE);
    interrupt_enabled = pHandle->pUSARTx->CR1 & (1 << USART_CR1_TXEIE);
    if (flag_set && interrupt_enabled)
    {
        // this interrupt is because of TXE
        if (pHandle->TxState == USART_BUSY_IN_TX)
        {
            if (pHandle->TxLen > 0)
            {
                // Check the USART_WordLength item for 9BIT or 8BIT in a frame
                if (pHandle->USARTConfig.USART_WordLength == USART_WORDLEN_9BITS)
                {
                    pHandle->pUSARTx->DR = *(uint16_t*)pHandle->pTxBuffer & 0x1FFU;
                    if (pHandle->USARTConfig.USART_ParityCtrl == USART_PARITY_DISABLE)
                    {
                        // PARITY DISABLED
                        // ADVANCE POINTER BY 2 BYTES (SINCE 9-BIT DATA STORED IN 16-BIT)
                        pHandle->pTxBuffer += 2;
                    }
                    else
                    {
                        // PARITY ENABLED:
                        // MOVE BUFFER POINTER BY 1 BYTE
                        pHandle->pTxBuffer++;
                    }
                }
                else if (pHandle->USARTConfig.USART_WordLength == USART_WORDLEN_8BITS)
                {
                    pHandle->pUSARTx->DR = *pHandle->pTxBuffer;
                    // MOVE BUFFER POINTER BY 1 BYTE
                    pHandle->pTxBuffer++;
                }
                // ADVANCE TO THE NEXT DATA FRAME
                pHandle->TxLen--;
            }
            // Check the TxLen . If it is zero then close the data transmission
            if (!pHandle->TxLen)
            {
                // DISABLE TXEIE FLAG
                pHandle->pUSARTx->CR1 &= ~(1 << USART_CR1_TXEIE);
            }
        }
    }

    /************************** IRQ HANDLING FOR RXNE *****************************/
    flag_set          = pHandle->pUSARTx->SR & (1 << USART_SR_RXNE);
    interrupt_enabled = pHandle->pUSARTx->CR1 & (1 << USART_CR1_RXNEIE);
    if (flag_set && interrupt_enabled)
    {
        // this interrupt is because of RNXE
        if (pHandle->RxState == USART_BUSY_IN_RX)
        {
            if (pHandle->RxLen > 0)
            {
                // CHECK WORD LENGTH CONFIGURATION (8-BIT OR 9-BIT FRAME)
                if (pHandle->USARTConfig.USART_WordLength == USART_WORDLEN_8BITS)
                {
                    // 8-BIT FRAME
                    if (pHandle->USARTConfig.USART_ParityCtrl != USART_PARITY_DISABLE)
                    {
                        // PARITY ENABLED:
                        // MSB IS USED FOR PARITY, SO ONLY 7 BITS ARE VALID USER DATA
                        *(pHandle->pRxBuffer) = pHandle->pUSARTx->DR & 0x7F;
                    }
                    else
                    {
                        // NO PARITY:
                        // FULL 8 BITS ARE USER DATA
                        *(pHandle->pRxBuffer) = pHandle->pUSARTx->DR;
                    }
                    // MOVE BUFFER POINTER BY 1 BYTE
                    (pHandle->pRxBuffer)++;
                }
                else if (pHandle->USARTConfig.USART_WordLength == USART_WORDLEN_9BITS)
                {
                    // 9-BIT FRAME
                    if (pHandle->USARTConfig.USART_ParityCtrl != USART_PARITY_DISABLE)
                    {
                        // PARITY ENABLED:
                        // ONLY 8 BITS ARE USER DATA (1 BIT USED FOR PARITY)
                        *(pHandle->pRxBuffer) = pHandle->pUSARTx->DR & 0xFF;
                        (pHandle->pRxBuffer)++;
                    }
                    else
                    {
                        // NO PARITY:
                        // FULL 9 BITS ARE USER DATA → STORE AS 16-BIT VALUE
                        *((uint16_t*)pHandle->pRxBuffer) = pHandle->pUSARTx->DR & (uint16_t)0x1FF;

                        // ADVANCE POINTER BY 2 BYTES (SINCE 9-BIT DATA STORED IN 16-BIT)
                        (pHandle->pRxBuffer) += 2;
                    }
                }
                // ADVANCE TO THE NEXT DATA FRAME
                pHandle->RxLen--;
            }

            if (!pHandle->RxLen)
            {
                // DISABLE RXNEIE FLAG
                pHandle->pUSARTx->CR1 &= ~(1 << USART_CR1_RXNEIE);
                pHandle->RxState = USART_READY;
                USART_ApplicationEventCallback(pHandle, USART_EVENT_RX_CMPLT);
            }
        }
    }
}

void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
    NVIC_IRQConfig(IRQNumber, EnorDi);
}

/*********************************************************************
 * @fn      		  - USART_ApplicationEventCallback
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -

 */
__weak void USART_ApplicationEventCallback(USART_Handle_t* pUSARTHandle, uint8_t event)
{
}
