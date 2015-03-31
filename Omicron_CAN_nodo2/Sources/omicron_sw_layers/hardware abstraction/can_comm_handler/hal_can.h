/*******************************************************************************/
/**
\file       hal_can.h
\brief      Intermediate-layer CAN communication services
\author     Abraham Tezmol
\version    0.1
\date       06/Aug/2007
*/
/*******************************************************************************/

#ifndef _CAN_COMMUNICATIONS        /*prevent duplicated includes*/
#define _CAN_COMMUNICATIONS

/*-- Includes ----------------------------------------------------------------*/
/** Variable types */
#include "typedefs.h"

/*-- Types Definitions -------------------------------------------------------*/


/*-- Defines -----------------------------------------------------------------*/


/*-- Macros ------------------------------------------------------------------*/


/*-- Function Prototypes -----------------------------------------------------*/

/* CAN Periodic frame transmission tasks */
void vfnCAN_Periodic_Tx_Queuing(void);

/* CAN Periorid frame reception tasks */
void vfnCAN_Periodic_Rx(void);

#endif /* _CAN_COMMUNICATIONS */
