/*******************************************************************************/
/**
\file       lib_adc.c
\brief      Analog to Digital Converter Functions
\author     Miguel Reyes
\version    0.1
\date       03/05/2013
*/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/

/** Core modules */
/** Configuration Options */
#include "configuration.h"
/* S12X derivative information */      
#include __MCU_DERIVATIVE   
/** Variable types and common definitions */
#include "typedefs.h"

/** Own headers */
/* SCI routines prototypes */ 
#include "lib_adc.h"

/** Used modules */
/** PLL prototypes and definitions */
#include "pll.h"



#define HAL_ADC_NUMBER_OF_CHANNELS 2u   /* This definition must be between 1-16    */
#define HAL_ADC_CHANNEL_0          (0)
#define HAL_ADC_CHANNEL_1          (1)


UINT16 ADC_SAMPLES_AVG[HAL_ADC_NUMBER_OF_CHANNELS];


/*******************************************************************************/

/*Callback called when ADC conversions are done*/
ADC_Conv_Callback_t pAdcCallback_conversion;

/*******************************************************************************/
/**
* \brief    ADC initialization
* \author   Miguel Reyes @MR 
* \param    ADC_Conv_Callback_t AdcCallback: callback called when conversions are done
* \return   void
*/

void vfnADC_Init(ADC_Conv_Callback_t AdcCallback)
{

        ATD0CTL0=0x01; //@MR: Select channel AN1 for multi sampling.
        ATD0CTL1=0x0F; //@MR: Resolution bits to 8bit.
        ATD0CTL2=0x42; //@MR: Enable fast flag & sequence complete interrupt enable.
        ATD0CTL3=0x93; //@MR: Right justification in the ATDDRx, FRZ= 0b11, SCx=2 conversions.
        ATD0CTL4=0xE1; //@MR: Configure sample time to 24, ADC clock to 5 MHz.
        ATD0CTL5=0x10; //@MR: Multichannel sample mode and 8 bit of resolution.
        
        ATD0DIEN= 0x03; //@MR: Enable channel 0 and channel 1
        
    pAdcCallback_conversion = AdcCallback; //@MR assign callback
}

/*******************************************************************************/
/**
* \brief    Start a new conversion sequence.
* \author   Miguel Reyes @MR
* \param    void
* \return   void
*/
void vfnADC_StartConversion(void){
    
    ATD0CTL5=0x10;

}

/*******************************************************************************/
/**
* \brief    Get ADC Results function.
* \author   Miguel Reyes @MR
* \param    void
* \return   void
*/
 /*
UINT16 u16_ADC_Handler_GetResult(UINT8 adc_channel){
  UINT16 return_val;  
  if (HAL_ADC_NUMBER_OF_CHANNELS <   adc_channel ) 
  {
    return_val = 0;
  } 
  else
  {
    return_val =  ADC_SAMPLES_AVG[adc_channel];
  }
  return (return_val);
}  */

/*******************************************************************************/
/**
* \brief    ADC ISR, read byte into input data buffer
* \author   Miguel Reyes @MR
* \param    void
* \return   void
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
void interrupt vfnADC_Complete_Conversion_Isr(void)
{

//@MR structure where the conversions will be stored
AdcDataChannels_t mDataChannels;

if (ATD0STAT0_SCF==1) {
   /*Check that both conversions were calculated*/
   if (ATD0STAT2&0x03){
      /*After reading ATD0Rx, flags will be cleared due to AFFC=1*/
      mDataChannels.Channel0=ATD0DR0;
      mDataChannels.Channel1=ATD0DR1;
   }
}

//@MR Call callbacks with conversions
  pAdcCallback_conversion(mDataChannels);

}
#pragma CODE_SEG DEFAULT