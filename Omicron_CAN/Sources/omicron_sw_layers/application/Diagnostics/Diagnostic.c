/*******************************************************************************/
/**
\file       Diagnostic.c
\brief      Diagnostic service implemented for CAN
\author     Team3
\version    0.1
\date       04/April/2015
*/
/*******************************************************************************/

#include "Diagnostic.h"
#include "mscan.h"
#include "lib_adc.h"

/* -- Variables ---------------------------------------------------------------*/


/*Global variables to store ADC values*/
UINT16 g16AdcResult_Ch0;
UINT16 g16AdcResult_Ch1; 

extern void vfnStoreAdcResult (AdcDataChannels_t resultOfChannels);

/*******************************************************************************/
/**
* \brief    Callback executed when message ID 0x1abcdef7 is received
* \author   Abraham Tezmol
* \param    void
* \return   void
* \todo     void
* \warning  void
*/
void vfnCbMsgID_0x1abcdef7(void)
{ 
  tMSCAN_Message can_buffer;
  UINT8 data[2];
  /*Read message to unqueued it*/
  vfnCAN_GetRxMessages(MSCAN_A, &can_buffer,1);
  
  /*Store ADC channel0 value*/  
  data[1] =  (UINT8)(g16AdcResult_Ch1 >> 8);
  data[0] =  (UINT8)(g16AdcResult_Ch1 & 0x00FF);  
  
  /*Send message wiht 0x1abcdef5 as ID a read value as two byte data */
  (void)u8CAN_enqueueFrameforTx(MSCAN_A,  RX_32_BIT_CANID_0,2, data, EXTENDED_ID);

}

/*******************************************************************************/
/**
* \brief    Callback executed when message ID 0x1abcdef8 is received
* \author   Abraham Tezmol
* \param    void
* \return   void
* \todo     void
* \warning  void
*/
void vfnCbMsgID_0x1abcdef8(void) 
{
  tMSCAN_Message can_buffer;
  UINT8 data[2];
  /*Read message to unqueued it*/
  vfnCAN_GetRxMessages(MSCAN_A, &can_buffer,1);
  
  /*Store ADC channel1 value*/ 
  data[1] =  (UINT8)(g16AdcResult_Ch0 >> 8);
  data[0] =  (UINT8)(g16AdcResult_Ch0 & 0x00FF);  
  
  /*Send message wiht 0x1abcdef6 as ID a read value as two byte data */
  (void)u8CAN_enqueueFrameforTx(MSCAN_A, RX_32_BIT_CANID_1,2, data, EXTENDED_ID); 
}

void vfnStoreAdcResult (AdcDataChannels_t resultOfChannels){
/*Store Adc result in the global variable to be sent over CAN bus*/
  g16AdcResult_Ch0= resultOfChannels.Channel0;
  g16AdcResult_Ch1= resultOfChannels.Channel1;
}
/*******************************************************************************/
