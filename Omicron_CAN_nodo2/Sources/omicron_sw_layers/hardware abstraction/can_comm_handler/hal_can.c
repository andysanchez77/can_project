/*******************************************************************************/
/**
\file       hal_can.c
\brief      Intermediate - layer CAN communication services
\author     Abraham Tezmol
\version    0.1
\date       06/Aug/2007
*/
/*******************************************************************************/

/** Intermediate layer CAN Communication Functions **/
#include "hal_can.h"
/** Low-level layer CAN Communication Functions **/
#include "mscan.h"
#include "cnf_mscan.h"

/* -- Variables ---------------------------------------------------------------*/

/** Error flag for CAN transmission*/
UINT8 u8ErrorFlag;
/* Number of rx frames that are pending to be read from the low level buffer */
UINT8 u8RxFifoDepth;


tMSCAN_Message aMSCAN_Tx_Buffers[2];
tMSCAN_Message aMSCAN_Rx_Buffers[CAN_RX_BUFFER_DEPTH];


/*******************************************************************************/
/**
* \brief    CAN Periodic Message Tx Queuing. Execute @ 2ms
* \author   Abraham Tezmol
* \param    void
* \return   void
* \todo     void
* \warning  void
*/
void vfnCAN_Periodic_Tx_Queuing(void)
{
    /* Control index for Transmission logic */
    static UINT16 u16Tx_Loop_Counter;
    /* Index for data to be transmitted */
    UINT8 u8Index;
    
    /* Update control variable */
    u16Tx_Loop_Counter++;
    
    /* Queue periodic messages accordingly */
    if(u16Tx_Loop_Counter %(CAN_TX_1_PERIOD) == 0)
    {
        aMSCAN_Tx_Buffers[0].ID = CAN_STD_ID_0x00a5;
        aMSCAN_Tx_Buffers[0].Length = 8;
        for (u8Index = 0; u8Index < aMSCAN_Tx_Buffers[0].Length; u8Index++)
        {
            aMSCAN_Tx_Buffers[0].Data[u8Index] = (UINT8)u16Tx_Loop_Counter;
        }
        u8ErrorFlag = u8CAN_enqueueFrameforTx(MSCAN_A, aMSCAN_Tx_Buffers[0].ID, aMSCAN_Tx_Buffers[0].Length, 
                        aMSCAN_Tx_Buffers[0].Data, STANDARD_ID); 

    }
    /* Queue periodic messages accordingly */
    if(u16Tx_Loop_Counter %(CAN_TX_2_PERIOD) == 0)
    {
        aMSCAN_Tx_Buffers[1].ID = CAN_EXT_ID_0x0abcdef7;
        aMSCAN_Tx_Buffers[1].Length = 8;
        for (u8Index = 0; u8Index < aMSCAN_Tx_Buffers[1].Length; u8Index++)
        {
            aMSCAN_Tx_Buffers[1].Data[u8Index] = (UINT8)u16Tx_Loop_Counter;
        }       
        u8ErrorFlag = u8CAN_enqueueFrameforTx(MSCAN_A, aMSCAN_Tx_Buffers[1].ID, aMSCAN_Tx_Buffers[1].Length, 
                        aMSCAN_Tx_Buffers[1].Data, EXTENDED_ID);
    }
    
    /* Add similar code to enable Tx of other periodic messages */
    
    /* Check if maximum allowed periodicity has been reached, if so then reset control variable */
    if (u16Tx_Loop_Counter == MAX_CAN_PERIODICITY)
    {
        u16Tx_Loop_Counter = 0x00;
    }
}

/*******************************************************************************/
/**
* \brief    CAN Periodic Message Rx . Execute @ 2ms or 10ms
* \author   Abraham Tezmol
* \param    void
* \return   void
* \todo     void
* \warning  void
*/
void vfnCAN_Periodic_Rx(void)
{
    
    u8RxFifoDepth = u8CAN_RxFifoDepth(MSCAN_A);
    
    if (u8RxFifoDepth)
    {
        vfnCAN_GetRxMessages(MSCAN_A, &aMSCAN_Rx_Buffers[0], u8RxFifoDepth);
    }
    
}
/*******************************************************************************/
