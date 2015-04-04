/****************************************************************************************************/
/**
\file       cnf_mscan.h
\brief      Configuration of MSCAN bios driver
\author     Abraham Tezmol
\version    1.0
\date       22/12/2013
*/
/****************************************************************************************************/

#ifndef __CNF_MSCAN_H        /*prevent duplicated includes*/ 
#define    __CNF_MSCAN_H    

/*****************************************************************************************************
* Include files
*****************************************************************************************************/

/** Core Modules */
/** Configuration Options */
#include    "configuration.h"
/** S12X derivative information */
#include    __MCU_DERIVATIVE
/** Variable types and common definitions */
#include    "typedefs.h"

/*****************************************************************************************************
* Declaration of module wide TYPES
*****************************************************************************************************/

typedef enum 
{
    MSCAN_A,                            /**< CAN A */ 
    MSCAN_B,                            /**< CAN B */
    MSCAN_C,                            /**< CAN C */
    MSCAN_D,                            /**< CAN D */
    MSCAN_NULL                          /**< No valid device */    
}tMSCAN_Device;

typedef enum
{
    STANDARD_ID,                        /**< Standard Id Type, 11-bit Id */
    EXTENDED_ID                         /**< Extended Id Type, 29-bit Id */
}tMSCAN_IdType;

typedef enum
{
    MSCAN_ACC_FILTER_0,                 /**< MSCAN Acceptance filter 0 */
    MSCAN_ACC_FILTER_1,                 /**< MSCAN Acceptance filter 1 */
    MSCAN_ACC_FILTER_2,                 /**< MSCAN Acceptance filter 2 */
    MSCAN_ACC_FILTER_3,                 /**< MSCAN Acceptance filter 3 */
    MSCAN_ACC_FILTER_4,                 /**< MSCAN Acceptance filter 4 */
    MSCAN_ACC_FILTER_5,                 /**< MSCAN Acceptance filter 5 */
    MSCAN_ACC_FILTER_6,                 /**< MSCAN Acceptance filter 6 */
    MSCAN_ACC_FILTER_7,                 /**< MSCAN Acceptance filter 7 */
}tMSCAN_AccFilterIdType;

typedef enum
{
    MSCAN_ACC_FILTERS_32_BIT_MODE,      /**< 32-bit mode acceptance filters configuration, 2 filters in total */
    MSCAN_ACC_FILTERS_16_BIT_MODE,      /**< 16-bit mode acceptance filters configuration, 4 filters in total */
    MSCAN_ACC_FILTERS_8_BIT_MODE,       /**< 8-bit mode acceptance filters configuration, 8 filters in total */
    MSCAN_ACC_FILTERS_INVALID_MODE      /**< Invalid configuration mode */
}tMSCAN_AccFilterModeCfg;

/****************************************************************************************************/

typedef struct 
{
    UINT32                          ID;             /**< Received message ID */
    UINT8                           filter_id;      /**< identificatin of filter that allowed rx of message */
    UINT8                           Data[8];        /**< Received message data string*/
    UINT8                           Length;         /**< Recieved message number of data bytes */
    UINT8                           NewMessage;     /**< Flag to signal current message has been read by application */
    tMSCAN_IdType                   id_type;        /**< Standard or Extended frame */
} tMSCAN_Message;

typedef struct 
{
    UINT32                          mssg_id;
	tCallbackFunction	            callback;
	tMSCAN_IdType                   id_type;
	tMSCAN_AccFilterIdType          filter_id;
}tMSCAN_RxHWObjectConfig;

typedef struct
{
    tCallbackFunction	            callback;
}tMSCAN_TxHwFifoConfig;

typedef struct 
{
 	UINT32			                baudrate;	    /**< Baudrate */
    const tMSCAN_RxHWObjectConfig  *rx_hwObj_cfg;   /**< pointer to static Rx hw objects configuration */
    const tMSCAN_TxHwFifoConfig    *tx_hw_fifo_cfg; /**< pointer to static Tx fifo configuration */
    enum tMSCAN_Device	            device;         /**< Device ID */
    enum tMSCAN_AccFilterModeCfg    filter_cfg;     /**< Rx filter acceptance mode configuration */
    UINT8                           nr_of_rx_hwOb;  /**< number of rx hw objects being configured */
    UINT8                           rx_buffer_depth;/**< software buffer depth for each rx hw filter */
    UINT8                           tx_fifo_depth; 	/**< software queue depth for transmission purposes */
}tMSCAN_DeviceConfig;

typedef struct 
{
    UINT8                           mscan_devices;
    const tMSCAN_DeviceConfig *   	mscan_device_cfg;
}tMSCAN_config;

/*****************************************************************************************************
* Definition of module wide MACROS / #DEFINE-CONSTANTS 
*****************************************************************************************************/
/* CAN Baudrate valid definitions: */
#define CAN_BAUDRATE_250Kbps        250000          /**< 250Kbps */
#define CAN_BAUDRATE_500Kbps        500000          /**< 500Kbps */
#define CAN_BAUDRATE_1Mbps          1000000         /**< 1 Mbps  */

/* CAN ID definition: */ 
#define CAN_STD_ID_0x00a5           0x00a5          /* Periodic Rx functional message */
#define CAN_STD_ID_0x00b6           0x00b6          /* Periodic Rx functional message */
#define CAN_STD_ID_0x00c7           0x00c7          /* On-demmand Rx functional message, so far unused */
#define CAN_STD_ID_0x0244           0x0244          /* On-demmand Rx diagnostic request frame */
#define CAN_EXT_ID_0x0abcdef7       0x0abcdef7      /* Periodic extended ID Rx functional message */
#define CAN_EXT_ID_0x0abcdef8       0x0abcdef8      /* Periodic extended ID Rx functional message */
#define CAN_EXT_ID_0x0abcdef9       0x0abcdef9      /* Periodic extended ID Rx functional message */
#define CAN_EXT_ID_0x0abcdefa       0x0abcdefa      /* Periodic extended ID Rx functional message */

#define CAN_STD_ID_0x01a5           0x01a5          /* Periodic Tx functional message */
#define CAN_STD_ID_0x01b6           0x01b6          /* Periodic Tx functional message */
#define CAN_STD_ID_0x01c7           0x01c7          /* On-demmand Tx functional message, so far unused */
#define CAN_STD_ID_0x0644           0x0644          /* On-demmand Tx diagnostic response frame */
#define CAN_EXT_ID_0x1abcdef7       0x1abcdef7      /* Periodic extended ID Tx functional message */
#define CAN_EXT_ID_0x1abcdef8       0x1abcdef8      /* Periodic extended ID Tx functional message */
#define CAN_EXT_ID_0x1abcdef9       0x1abcdef9      /* Periodic extended ID Tx functional message */
#define CAN_EXT_ID_0x1abcdefa       0x1abcdefa      /* Periodic extended ID Tx functional message */

/* Rx CAN IDs for 16-bit acceptance filters and masks */
#define RX_16_BIT_CANID_0           0x01a5
#define RX_16_BIT_CANID_1           0x01b6
#define RX_16_BIT_CANID_2           0x00c7
#define RX_16_BIT_CANID_3           0x0244
  

/* Rx CAN IDs for 32-bit acceptance filters and masks */
#define RX_32_BIT_CANID_0           0x1abcdef5
#define RX_32_BIT_CANID_1           0x1abcdef6

/*Team 3: Rx Masks */
#define RX_32_BIT_CANID_2           0x1abcdef7
#define RX_32_BIT_CANID_3           0x1abcdef8

#define CAN_RX_BUFFER_DEPTH         32              /* Software buffer depth for all rx filters per device */
#define CAN_TX_QUEUE_DEPTH          16              /* Software queue depth for transmission purposes */


/* CAN periodic frames periodicity */
#define CAN_TX_1_PERIOD             (UINT16)(20/2)  /* Message periodicity in loop cycles  */
#define CAN_TX_2_PERIOD             (UINT16)(20/2)  /* Message periodicity in loop cycles  */
#define MAX_CAN_PERIODICITY         (UINT16)(1000/2)    /* Maximum periodicity allowed 10000ms/2ms loops  */

/*****************************************************************************************************
* Declaration of public VARIABLEs / Constants 
*****************************************************************************************************/

extern const tMSCAN_config MSCAN_cfg[];

/*****************************************************************************************************
* Declaration of module wide FUNCTIONS
*****************************************************************************************************/


#endif /* __CNF_MSCAN_H */ 