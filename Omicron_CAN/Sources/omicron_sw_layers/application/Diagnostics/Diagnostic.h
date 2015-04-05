/*******************************************************************************/
/**
\file       Diagnostic.h
\brief      Diagnostic service implemented for CAN
\author     Team3
\version    0.1
\date       04/April/2015
*/
/*******************************************************************************/

#ifndef __DIAGNOSTIC_H        /*prevent duplicated includes*/
#define __DIAGNOSTIC_H

/*-- Includes ----------------------------------------------------------------*/
/** Variable types */
#include "typedefs.h"

/*-- Types Definitions -------------------------------------------------------*/


/*-- Defines -----------------------------------------------------------------*/


/*-- Macros ------------------------------------------------------------------*/


/*-- Function Prototypes -----------------------------------------------------*/

/* Callback for message ID 0x1abcdef7 */
void vfnCbMsgID_0x1abcdef7(void);

/* Callback for message ID 0x1abcdef8 */
void vfnCbMsgID_0x1abcdef8(void);

#endif /* __DIAGNOSTIC_H */
