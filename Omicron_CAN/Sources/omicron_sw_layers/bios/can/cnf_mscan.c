/****************************************************************************************************/
/**
\file       cnf_mscan.c
\brief      Configuration of MSCAN bios driver
\author     Abraham Tezmol
\version    1.0
\date       22/12/2013
*/
/****************************************************************************************************/

/*****************************************************************************************************
* Include files
*****************************************************************************************************/

/** Configuration of Emulated EEPROM */
#include    "cnf_mscan.h"

/*****************************************************************************************************
* Definition of  VARIABLEs - 
*****************************************************************************************************/


/*****************************************************************************************************
* Definition of module wide (CONST-) CONSTANTs 
*****************************************************************************************************/

/**< Configuration of MSCAN A RX messages */
/* In case MSCAN_ACC_FILTERS_8_BIT_MODE ->  8 exact reception objects (acceptance filters) shall be configured */
/* In case MSCAN_ACC_FILTERS_16_BIT_MODE -> 4 exact reception objects (acceptance filters) shall be configured */
/* In case MSCAN_ACC_FILTERS_32_BIT_MODE -> 2 exact reception objects (acceptance filters) shall be configured */
const tMSCAN_RxHWObjectConfig MSCAN_A_rx_msg_cfg[] =
{
    {
        (UINT32)CAN_STD_ID_0x00a5,
	    (tCallbackFunction)NULL,
	    STANDARD_ID,
	    MSCAN_ACC_FILTER_0,
    },
    {
        (UINT32)CAN_STD_ID_0x00b6,
	    (tCallbackFunction)NULL,
	    STANDARD_ID,
	    MSCAN_ACC_FILTER_1,
    },
    {
        (UINT32)CAN_STD_ID_0x00c7,
	    (tCallbackFunction)NULL,
	    STANDARD_ID,
	    MSCAN_ACC_FILTER_2,
    },
    {
        (UINT32)CAN_STD_ID_0x0244,
	    (tCallbackFunction)NULL,
	    STANDARD_ID,
	    MSCAN_ACC_FILTER_3,
    },
    {
        (UINT32)CAN_EXT_ID_0x0abcdef7,
	    (tCallbackFunction)NULL,
	    EXTENDED_ID,
	    MSCAN_ACC_FILTER_4,
    },
    {
        (UINT32)CAN_EXT_ID_0x0abcdef8,
	    (tCallbackFunction)NULL,
	    EXTENDED_ID,
	    MSCAN_ACC_FILTER_5,
    },
    {
        (UINT32)CAN_EXT_ID_0x0abcdef9,
	    (tCallbackFunction)NULL,
	    EXTENDED_ID,
	    MSCAN_ACC_FILTER_6,
    },
    {
        (UINT32)CAN_EXT_ID_0x0abcdefa,
	    (tCallbackFunction)NULL,
	    EXTENDED_ID,
	    MSCAN_ACC_FILTER_7,
    },
};

/**< Configuration of MSCAN A TX fifo */
const tMSCAN_TxHwFifoConfig MSCAN_A_tx_msg_cfg =
{
    (tCallbackFunction)NULL,                /**< callback */
};

/**< Configuration of MSCAN A device */
const tMSCAN_DeviceConfig CAN_device_cfg[] =
{
    {
        (UINT32)CAN_BAUDRATE_500Kbps,	    /**< Baudrate */
        &MSCAN_A_rx_msg_cfg[0],             /**< pointer to static Rx hw objects configuration */
        &MSCAN_A_tx_msg_cfg,                /**< pointer to static Tx fifo configuration */   
        MSCAN_A,                            /**< Device ID */
        MSCAN_ACC_FILTERS_8_BIT_MODE,       /**< Rx filter acceptance mode configuration */
        sizeof(MSCAN_A_rx_msg_cfg)/sizeof(MSCAN_A_rx_msg_cfg[0]),    /**< number of rx hw objects being configured */
        CAN_RX_BUFFER_DEPTH,                /**< software buffer depth for all the rx FIFO */
        CAN_TX_QUEUE_DEPTH,                 /**< software queue depth for transmission purposes */
    },
};

/**< Configuration of MSCAN driver  */
const tMSCAN_config MSCAN_cfg[] =
{
    {
        sizeof(CAN_device_cfg)/sizeof(CAN_device_cfg[0]),
        &CAN_device_cfg[0]
    }
};

/*****************************************************************************************************
* Code of module wide private FUNCTIONS
*****************************************************************************************************/


/*****************************************************************************************************
* Code of public FUNCTIONS
*****************************************************************************************************/


/***************************************************************************************************/
