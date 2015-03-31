/*******************************************************************************/
/**
\file       lib_adc.h
\brief      ADC header file
\author     Miguel Reyes
\version    0.1
\date       03/05/2013
*/
/*******************************************************************************/

#ifndef LIB_ADC_H        /*prevent duplicated includes*/
#define LIB_ADC_H

/*-- Includes ----------------------------------------------------------------*/

/** Variable types and common definitions */
#include "typedefs.h"

/** PLL prototypes and definitions */
#include "lib_pll.h"

/*-- Types Definitions -------------------------------------------------------*/
//MRBC typedef to control the Tx Flow 
typedef struct AdcDataChannels_tag{
UINT16 Channel0;
UINT16 Channel1;
}AdcDataChannels_t; 

/* MRBC typedef of Tx Callback */
typedef void (* ADC_Conv_Callback_t) (AdcDataChannels_t);

/*-- Function Prototypes -----------------------------------------------------*/

/** ADC Initalization */
void vfnADC_Init(ADC_Conv_Callback_t AdcCallback);

/** ADC Start conversion */
void vfnADC_StartConversion(void);

#pragma CODE_SEG __NEAR_SEG NON_BANKED
/** ADC(atd0) conversion complete interrupt function **/
void interrupt vfnADC_Complete_Conversion_Isr (void);
#pragma CODE_SEG DEFAULT


/*******************************************************************************/

#endif /* LIB_ADC_H */