/* 
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2006 Christian Walter <wolti@sil.at>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: mbrtu.c,v 1.18 2007/09/12 10:15:56 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbrtu.h"
#include "mbframe.h"

#include "mbcrc.h"
#include "mbport.h"

#if MB_SLAVE_RTU_ENABLED > 0
/* ----------------------- Defines ------------------------------------------*/
#define MB_SER_PDU_SIZE_MIN     4       /*!< Minimum size of a Modbus RTU frame. */
#define MB_SER_PDU_SIZE_MAX     256     /*!< Maximum size of a Modbus RTU frame. */
#define MB_SER_PDU_SIZE_CRC     2       /*!< Size of CRC field in PDU. */
#define MB_SER_PDU_ADDR_OFF     0       /*!< Offset of slave address in Ser-PDU. */
#define MB_SER_PDU_PDU_OFF      1       /*!< Offset of Modbus-PDU in Ser-PDU. */

/* ----------------------- Type definitions ---------------------------------*/
typedef enum
{
    STATE_RX_INIT,              /*!< Receiver is in initial state. */
    STATE_RX_IDLE,              /*!< Receiver is in idle state. */
    STATE_RX_RCV,               /*!< Frame is beeing received. */
    STATE_RX_ERROR              /*!< If the frame is invalid. */
} eMBRcvState;

typedef enum
{
    STATE_TX_IDLE,              /*!< Transmitter is in idle state. */
    STATE_TX_XMIT               /*!< Transmitter is in transfer state. */
} eMBSndState;

/* ----------------------- Static variables ---------------------------------*/
static volatile eMBSndState eSndState;
static volatile eMBRcvState eRcvState;

volatile UCHAR  ucRTUBuf[MB_SER_PDU_SIZE_MAX];

static volatile UCHAR *pucSndBufferCur;
static volatile USHORT usSndBufferCount;

volatile USHORT usRcvBufferPos;

/* ----------------------- Start implementation -----------------------------*/
eMBErrorCode
eMBRTUInit(UCHAR ucSlaveAddress, UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity)
{
    eMBErrorCode    eStatus = MB_ENOERR;

    (void)ucSlaveAddress;
    ENTER_CRITICAL_SECTION();

    /* Modbus RTU uses 8 Databits. */
    if(xMBPortSerialInit(ucPort, ulBaudRate, 8, eParity) != TRUE)
    {
        eStatus = MB_EPORTERR;
    }
    else
    {
        if(xMBPortTimersInit() != TRUE)
        {
            eStatus = MB_EPORTERR;
        }
    }
    EXIT_CRITICAL_SECTION();

    return eStatus;
}

void
eMBRTUStart(void)
{
    ENTER_CRITICAL_SECTION();
    /* Initially the receiver is in the state STATE_RX_INIT. we start
     * the timer and if no character is received within t3.5 we change
     * to STATE_RX_IDLE. This makes sure that we delay startup of the
     * modbus protocol stack until the bus is free.
     */
    eRcvState = STATE_RX_INIT;
    vMBPortSerialEnable(TRUE, FALSE);

    EXIT_CRITICAL_SECTION();
}

void
eMBRTUStop(void)
{
    ENTER_CRITICAL_SECTION();
    vMBPortSerialEnable(FALSE, FALSE);
    EXIT_CRITICAL_SECTION();
}

#include "rtthread.h"
#include <finsh.h>
char modbus_print = 0;
void mp(char val)
{
		modbus_print = val;
}
FINSH_FUNCTION_EXPORT(mp,mp(val))

eMBErrorCode
eMBRTUReceive(UCHAR * pucRcvAddress, UCHAR ** pucFrame, USHORT * pusLength)
{
    eMBErrorCode    eStatus = MB_ENOERR;
		int cnt = 0;
    ENTER_CRITICAL_SECTION();
//    assert_param(usRcvBufferPos < MB_SER_PDU_SIZE_MAX);

    /* Length and CRC check */
    if((usRcvBufferPos >= MB_SER_PDU_SIZE_MIN)
		&& (usRcvBufferPos <= MB_SER_PDU_SIZE_MAX)
    && (usMBCRC16((UCHAR *) ucRTUBuf, usRcvBufferPos) == 0))
    {
        /* Save the address field. All frames are passed to the upper layed
         * and the decision if a frame is used is done there.
         */
        *pucRcvAddress = ucRTUBuf[MB_SER_PDU_ADDR_OFF];

        /* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
         * size of address field and CRC checksum.
         */
        *pusLength = (USHORT)(usRcvBufferPos - MB_SER_PDU_PDU_OFF - MB_SER_PDU_SIZE_CRC);

        /* Return the start of the Modbus PDU to the caller. */
        *pucFrame = (UCHAR *) & ucRTUBuf[MB_SER_PDU_PDU_OFF];
				
				if((modbus_print == 1)||(modbus_print == 3))
				{
					rt_kprintf("Modbus Rx: ");
					cnt = usRcvBufferPos;
					for(cnt = 0;cnt<usRcvBufferPos;cnt++)
					{
							rt_kprintf(" 0x%02X|",ucRTUBuf[cnt]);
					}
					rt_kprintf("\r\n");
				}
				
				eRcvState = STATE_RX_IDLE;
    }
    else
    {
        eStatus = MB_EIO;
    }

    EXIT_CRITICAL_SECTION();
    return eStatus;
}

static void Mb_StartDmaTx(char *buffer,__IO rt_uint16_t _dma_tx_len)
{
	/* Disable the USART TX DMA request */
	USART_DMACmd(MODBUS_USE_USART, USART_DMAReq_Tx, DISABLE);
	/* Disable the DMA  TX Streams */
	DMA_Cmd(MB_TX_DMA, DISABLE);
	//---------------------------------------------------------
	// Clear all falgs of DMA TX Streams
	//---------------------------------------------------------
	DMA_ClearFlag(MB_TX_DMA,
		(DMA_FLAG_FEIF4 | DMA_FLAG_DMEIF4 | DMA_FLAG_TEIF4 |
		DMA_FLAG_HTIF4 | DMA_FLAG_TCIF4)
		);
	/* Set the DMA TX Stream lenth */
	MB_TX_DMA->M0AR = (rt_uint32_t)buffer;
	MB_TX_DMA->NDTR = _dma_tx_len; //DMA_TxBuffer_Size;

	/* Clear the TC bit in the SR register by writing 0 to it */
	USART_ClearFlag(MODBUS_USE_USART, USART_FLAG_TC);
	/* Enable the DMA  TX Streams */
	DMA_Cmd(MB_TX_DMA, ENABLE);
	/* Enable the USART TX DMA request */
	USART_DMACmd(MODBUS_USE_USART, USART_DMAReq_Tx, ENABLE);
}



eMBErrorCode
eMBRTUSend(UCHAR ucSlaveAddress, const UCHAR * pucFrame, USHORT usLength)
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          usCRC16;
		int cnt = 0;
    ENTER_CRITICAL_SECTION();

    /* Check if the receiver is still in idle state. If not we where to
     * slow with processing the received frame and the master sent another
     * frame on the network. We have to abort sending the frame.
     */
    if( eRcvState == STATE_RX_IDLE )
    {
				 /* Activate the transmitter. */
        eSndState = STATE_TX_XMIT;
        vMBPortSerialEnable( FALSE, TRUE );
			
        /* First byte before the Modbus-PDU is the slave address. */
        pucSndBufferCur = (UCHAR *) pucFrame - 1;
        usSndBufferCount = 1;

        /* Now copy the Modbus-PDU into the Modbus-Serial-Line-PDU. */
        pucSndBufferCur[MB_SER_PDU_ADDR_OFF] = ucSlaveAddress;
        usSndBufferCount += usLength;

        /* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
        usCRC16 = usMBCRC16((UCHAR *) pucSndBufferCur, usSndBufferCount);
        ucRTUBuf[usSndBufferCount++] = (UCHAR)(usCRC16 & 0xFF);
        ucRTUBuf[usSndBufferCount++] = (UCHAR)(usCRC16 >> 8);

				if((modbus_print == 2)||(modbus_print == 3))
				{
					rt_kprintf("Modbus Tx: ");
				
					for(cnt = 0;cnt<usSndBufferCount;cnt++)
					{
							rt_kprintf(" 0x%02X|",ucRTUBuf[cnt]);
					}
					rt_kprintf("\r\n");
				}
				
				Mb_StartDmaTx((char *)ucRTUBuf,usSndBufferCount);
    }
    else
    {
        eStatus = MB_EIO;
    }
    EXIT_CRITICAL_SECTION();
    return eStatus;
}

void Mb_DMAIRQ_Exc(void)
{
		xMBPortEventPost( EV_FRAME_SENT );
		
		eSndState = STATE_TX_IDLE;
}

#endif
