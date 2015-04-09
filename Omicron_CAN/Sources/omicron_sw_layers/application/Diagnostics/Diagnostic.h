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

/*Start Diagnostic DEFINES*/
#define START_DIAGNOSTIC    0x10
#define ACCEPT_SESSION      0x50
#define END_OF_START        0x85


/*Multipacket DEFINES commands */
#define READ_MEM_ADDR       0x23
#define POSITIVE_RSP        0x63
#define MULTIPACKET_CMD     0x11
#define MULTIPACKET_CONT_1  0x21
#define MULTIPACKET_CONT_2  0x22
/*To be received each second*/
#define TESTER_PRESENT      0x3E

/*
244  05 23 00 0c 00 10		Read Memory by Address (0x000c00, 16 bytes)
644  10 11 63 0a 0a 0a 0a 0a    Multi-packet frame, 17 bytes (positive response + 16 bytes total), positive response, 5 first data bytes.                    
644  21 0a 0a 0a 0a 0a 0a 0a    First continuation frame, 7 data bytes     
644  22 0a 0a 0a 0a             Second continuation frame, 5 data bytes

244  01 3e			Tester present (@ 1 second rate to keep Diagnostic Session active)

244  05 23 00 0c 10 10		Read Memory by Address (0x000c10, 16 bytes)
644  10 11 63 0b 0b 0b 0b 0b    Multi-packet frame, 17 bytes (positive response + 16 bytes total), positive response, 5 first data bytes.                    
644  21 0b 0b 0b 0b 0b 0b 0b    First continuation frame, 7 data bytes     
644  22 0b 0b 0b 0b             Second continuation frame, 5 data bytes 

244  01 3e			Tester present (@ 1 second rate to keep Diagnostic Session active)

244  05 23 00 0c 20 10		Read Memory by Address (0x000c20, 16 bytes)
644  10 11 63 0c 0c 0c 0c 0c    Multi-packet frame, 17 bytes (positive response + 16 bytes total), positive response, 5 first data bytes.                    
644  21 0c 0c 0c 0c 0c 0c 0c    First continuation frame, 7 data bytes     
644  22 0c 0c 0c 0c             Second continuation frame, 5 data bytes

244  01 3e			Tester present (@ 1 second rate to keep Diagnostic Session active)

244  05 23 00 0c 30 10		Read Memory by Address (0x000c30, 16 bytes)
644  10 11 63 0d 0d 0d 0d 0d    Multi-packet frame, 17 bytes (positive response + 16 bytes total), positive response, 5 first data bytes.                    
644  21 0d 0d 0d 0d 0d 0d 0d    First continuation frame, 7 data bytes     
644  22 0d 0d 0d 0d             Second continuation frame, 5 data bytes
*/



/*-- Macros ------------------------------------------------------------------*/


/*-- Function Prototypes -----------------------------------------------------*/

/* Callback for message ID 0x1abcdef7 */
void vfnCbMsgID_0x1abcdef7(void);

/* Callback for message ID 0x1abcdef8 */
void vfnCbMsgID_0x1abcdef8(void);
/* Callback for message ID 0x0244 */
void vfnCbMsgID_0x0244(void);
#endif /* __DIAGNOSTIC_H */
