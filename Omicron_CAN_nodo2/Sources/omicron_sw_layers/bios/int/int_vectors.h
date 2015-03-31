/****************************************************************************************************/
/**
\file       int_vectors.h
\brief      Interrupt Vector functions and prototypes
\author     Abraham Tezmol
\version    1.0
\date       31/10/2008
*/
/****************************************************************************************************/

#ifndef __INT_VECTORS_H
#define    __INT_VECTORS_H    

/*****************************************************************************************************
* Include files
*****************************************************************************************************/

/** Core modules */
/** Configuration Options */
#include    "configuration.h"
/** MCU derivative information */
#include    __MCU_DERIVATIVE
/** Variable types and common definitions */
#include    "typedefs.h"

/** Used modules */
/* Interrupt service routines prototypes */
#include    "serial.h"
/** Scheduler function declarations */
#include    "scheduler.h"
/** PLL function declarations */
#include    "pll.h"
/** CAN function declarations */
#include    "mscan.h"

/*****************************************************************************************************
* Definition of module wide VARIABLES
*****************************************************************************************************/

/*****************************************************************************************************
* Declaration of module wide TYPES
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of module wide MACROS / #DEFINE-CONSTANTS 
*****************************************************************************************************/


/*****************************************************************************************************
* Declaration of module wide FUNCTIONS
*****************************************************************************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
void interrupt  vfnDummy_D_Isr( void  );
#pragma CODE_SEG DEFAULT


#endif /*__INT_VECTORS_H */ 
/****************************************************************************************************/ 