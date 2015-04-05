/*******************************************************************************/
/**
\file       mscan.h
\brief      Definitions and function prototypes for CAN handling
\author     Abraham Tezmol
\version    0.1
\date       04/07/2007
*/
/*******************************************************************************/

#ifndef __MSCAN        /*prevent duplicated includes*/
#define __MSCAN

/*-- Includes ----------------------------------------------------------------*/

/** Configuration Options */
#include "configuration.h"
/** S12X derivative information */
#include __MCU_DERIVATIVE
/** Variable types */
#include "typedefs.h"
/** MSCAN Configuration */
#include "cnf_mscan.h"
/** Dynamic Memory allocation services */
#include "memory_allocation.h"
/** Interrupt services */
#include  "interrupt.h"


/*-- Types Definitions -------------------------------------------------------*/

typedef struct 
{
    tMSCAN_Message         *rx_buffer;              /**< pointer to software buffer objects allocated for rx */
    tMSCAN_Message         *tx_buffer;              /**< pointer to software queue allocated for tx */
    tMSCAN_Message         *rx_write;               /**< pointer to next available message buffer for rx process */
    tMSCAN_Message         *rx_read;                /**< pointer to next buffer to be read by the upper layer */
    tMSCAN_Message         *rx_end;                 /**< pointer to the end of the tx buffer fifo */
    tMSCAN_Message         *tx_write;               /**< pointer to next available message buffer to store data (producer) */
    tMSCAN_Message         *tx_read;                /**< pointer to next buffer to be transmitted by the CAN controller (consumer) */
    tMSCAN_Message         *tx_end;                 /**< pointer to the end of the tx buffer queue */
    enum tMSCAN_Device	    device;                 /**< Device ID */
    UINT8                   rx_fifo_depth;          /**< number of messages that have been rx and are pending to be read by the uppper layer>*/
    UINT8                   rx_fifo_current_depth;  /**< number of messages that have been rx and are pending to be read by the uppper layer>*/
    UINT8                   tx_queue_depth;         /**< number of messages that have been queued by the upper layer and are pending tx >*/
    UINT8                   tx_queue_current_depth; /**< number of messages that have been queued by the upper layer and are pending tx >*/
    tCallbackFunction       rx_callback;            /*Team3: RX callback triggered when a reception*/
    tCallbackFunction       tx_callback;            /*Team3: TX callback triggered when a Transmission*/
}tMSCAN_DeviceStatus;

/*-- Global Variables --------------------------------------------------------*/


/*-- Defines -----------------------------------------------------------------*/
    

/* Error Flags Definition */
#define NO_ERR 0x00
#define ERR_BUFFER_FULL 0x80 

/*-- Macros ------------------------------------------------------------------*/


#define MSCAN_MAIN_NODE_OFS                     64

#define MSCAN_CTL1_INITAK_MASK                  (UINT8)1
#define MSCAN_CTL1_LISTEN_MASK                  (UINT8)16
#define MSCAN_CTL1_LOOPB_MASK                   (UINT8)32
#define MSCAN_CTL1_CLKSRC_MASK                  (UINT8)64
#define MSCAN_CTL1_CANE_MASK                    (UINT8)128

#define MSCAN_RFLAG_RXF_MASK                    (UINT8)1
#define MSCAN_RIER_RXFIE_MASK                   (UINT8)1
#define MSCAN_TIER_TXEIE0_MASK                  (UINT8)1
#define MSCAN_TIER_TXEIE1_MASK                  (UINT8)2
#define MSCAN_TIER_TXEIE2_MASK                  (UINT8)4
#define MSCAN_IDAC_IDHIT_MASK                   (UINT8)7

#define MSCAN_WRITE_CTL0(NodeIndex, Value)      (*((&CAN0CTL0)   + (MSCAN_MAIN_NODE_OFS * NodeIndex)) = (UINT8) Value)
#define MSCAN_WRITE_CTL1(NodeIndex, Value)      (*((&CAN0CTL1)   + (MSCAN_MAIN_NODE_OFS * NodeIndex)) = (UINT8) Value)
#define MSCAN_WRITE_RFLG(NodeIndex, Value)      (*((&CAN0RFLG)   + (MSCAN_MAIN_NODE_OFS * NodeIndex)) = (UINT8) Value)
#define MSCAN_WRITE_RIER(NodeIndex, Value)      (*((&CAN0RIER)   + (MSCAN_MAIN_NODE_OFS * NodeIndex)) = (UINT8) Value)
#define MSCAN_WRITE_TBSEL(NodeIndex, Value)     (*((&CAN0TBSEL)  + (MSCAN_MAIN_NODE_OFS * NodeIndex)) = (UINT8) Value)
#define MSCAN_WRITE_TXDLR(NodeIndex, Value)     (*((&CAN0TXDLR)  + (MSCAN_MAIN_NODE_OFS * NodeIndex)) = (UINT8) Value)
#define MSCAN_WRITE_TXTBPR(NodeIndex, Value)    (*((&CAN0TXTBPR) + (MSCAN_MAIN_NODE_OFS * NodeIndex)) = (UINT8) Value)
#define MSCAN_WRITE_TFLG(NodeIndex, Value)      (*((&CAN0TFLG)   + (MSCAN_MAIN_NODE_OFS * NodeIndex)) = (UINT8) Value)
#define MSCAN_WRITE_BTR1(NodeIndex, Value)      (*((&CAN0BTR1)   + (MSCAN_MAIN_NODE_OFS * NodeIndex)) = (UINT8) Value)
#define MSCAN_WRITE_BTR0(NodeIndex, Value)      (*((&CAN0BTR0)   + (MSCAN_MAIN_NODE_OFS * NodeIndex)) = (UINT8) Value)
#define MSCAN_WRITE_IDAC(NodeIndex, Value)      (*((&CAN0IDAC)   + (MSCAN_MAIN_NODE_OFS * NodeIndex)) = (UINT8) Value)
#define MSCAN_WRITE_TIER(NodeIndex, Value)      (*((&CAN0TIER)   + (MSCAN_MAIN_NODE_OFS * NodeIndex)) = (UINT8) Value)


#define MSCAN_READ_CTL0(NodeIndex)              (*((&CAN0CTL0)   + (MSCAN_MAIN_NODE_OFS * NodeIndex)))
#define MSCAN_READ_CTL1(NodeIndex)              (*((&CAN0CTL1)   + (MSCAN_MAIN_NODE_OFS * NodeIndex)))
#define MSCAN_READ_RFLG(NodeIndex)              (*((&CAN0RFLG)   + (MSCAN_MAIN_NODE_OFS * NodeIndex)))
#define MSCAN_READ_RIER(NodeIndex)              (*((&CAN0RIER)   + (MSCAN_MAIN_NODE_OFS * NodeIndex)))
#define MSCAN_READ_TIER(NodeIndex)              (*((&CAN0TIER)   + (MSCAN_MAIN_NODE_OFS * NodeIndex)))
#define MSCAN_READ_RXIDR0(NodeIndex)            (*((&CAN0RXIDR0) + (MSCAN_MAIN_NODE_OFS * NodeIndex)))
#define MSCAN_READ_RXIDR1(NodeIndex)            (*((&CAN0RXIDR1) + (MSCAN_MAIN_NODE_OFS * NodeIndex)))
#define MSCAN_READ_RXIDR2(NodeIndex)            (*((&CAN0RXIDR2) + (MSCAN_MAIN_NODE_OFS * NodeIndex)))
#define MSCAN_READ_RXIDR3(NodeIndex)            (*((&CAN0RXIDR3) + (MSCAN_MAIN_NODE_OFS * NodeIndex)))
#define MSCAN_READ_RXDLR(NodeIndex)             (*((&CAN0RXDLR)  + (MSCAN_MAIN_NODE_OFS * NodeIndex)))
#define MSCAN_READ_IDAC(NodeIndex)              (*((&CAN0IDAC)   + (MSCAN_MAIN_NODE_OFS * NodeIndex)))
#define MSCAN_READ_TFLG(NodeIndex)              (*((&CAN0TFLG)   + (MSCAN_MAIN_NODE_OFS * NodeIndex)))
#define MSCAN_READ_TBSEL(NodeIndex)             (*((&CAN0TBSEL)  + (MSCAN_MAIN_NODE_OFS * NodeIndex)))

#define MSCAN_REG_RXDSR0(NodeIndex)             ((&CAN0RXDSR0)   + (MSCAN_MAIN_NODE_OFS * NodeIndex))
#define MSCAN_REG_TXDSR0(NodeIndex)             ((&CAN0TXDSR0)   + (MSCAN_MAIN_NODE_OFS * NodeIndex))
#define MSCAN_REG_TXIDR0(NodeIndex)             ((&CAN0TXIDR0)   + (MSCAN_MAIN_NODE_OFS * NodeIndex))
#define MSCAN_REG_IDAR0(NodeIndex)              ((&CAN0IDAR0)    + (MSCAN_MAIN_NODE_OFS * NodeIndex))
#define MSCAN_REG_IDAR4(NodeIndex)              ((&CAN0IDAR4)    + (MSCAN_MAIN_NODE_OFS * NodeIndex))
#define MSCAN_REG_IDMR0(NodeIndex)              ((&CAN0IDMR0)    + (MSCAN_MAIN_NODE_OFS * NodeIndex))
#define MSCAN_REG_IDMR4(NodeIndex)              ((&CAN0IDMR4)    + (MSCAN_MAIN_NODE_OFS * NodeIndex))


/* CAN ID definition: */
#define u32CANSTDID(x)          (((x) & 0xFFFFFFFF)<<21)
#define u32CAN_EXT_ID(x)        ( ((x & 0x1FFC0000)<<3) + ((x & 0x0003FFFF)<<1) + 0x00180000)


/* Acceptance Code Definitions */ 
#define STD_ID_ACC_CODE_HIGH(x)     (((x << 5) & 0xFF00) >> 8)
#define STD_ID_ACC_CODE_LOW(x)       ((x << 5) & 0x00FF)


#define EXT_ID_ACC_CODE_IR0(x)      (((x << 3) & 0xFF000000) >> 24)
#define EXT_ID_ACC_CODE_IR1(x)      (((x << 3) & 0x00E00000) >> 16) | (((x << 1) & 0x00070000) >> 16) | (0x18)
#define EXT_ID_ACC_CODE_IR2(x)      (((x << 1) & 0x0000FF00) >> 8)
#define EXT_ID_ACC_CODE_IR3(x)       ((x << 1) & 0x000000FF)


/* Mask Code Definitions */

/* CAN Filtering Mask Definition */
#define MASK_CODE_ST_ID         0x0007
#define MASK_CODE_EXT_ID        0x00180001

#define MASK_CODE_ST_ID_HIGH    ((MASK_CODE_ST_ID  &  0xFF00) >> 8)
#define MASK_CODE_ST_ID_LOW      (MASK_CODE_ST_ID  &  0xFF)

#define MASK_CODE_EXT_ID_IR0    ((MASK_CODE_EXT_ID &  0xFF000000) >> 24)
#define MASK_CODE_EXT_ID_IR1    ((MASK_CODE_EXT_ID &  0x00FF0000) >> 16)
#define MASK_CODE_EXT_ID_IR2    ((MASK_CODE_EXT_ID &  0x0000FF00) >> 8)
#define MASK_CODE_EXT_ID_IR3     (MASK_CODE_EXT_ID &  0x000000FF) 

/*-- Function Prototypes -----------------------------------------------------*/

/* MSCAN configuration */
void vfnCAN_Init(const tMSCAN_config * MSCAN_config);

/* MSCAN Baud rate configuration */
void vfnCAN_BaudRateConfig(enum tMSCAN_Device device, UINT32 u32Baudrate); 

/* MSCAN acceptance filter configuration */
void vfnCAN_AcceptanceFiltersInit(const tMSCAN_DeviceConfig * controller_cfg, UINT8 filters_to_config);

/* CAN frame transmission */
UINT8 u8CAN_enqueueFrameforTx(enum tMSCAN_Device device, UINT32 u32ID, UINT8 u8Length, UINT8 *u8TxData, tMSCAN_IdType id_type);

/* Get current number of CAN messages still pending for TX on queue */
UINT8 u8CAN_TxQueueDepth( enum tMSCAN_Device device);

/* CAN reception handler */
void vfnCAN_Rx_handler(enum tMSCAN_Device device);

/* CAN function that allows to copy new RX messages */
void vfnCAN_GetRxMessages(enum tMSCAN_Device device, tMSCAN_Message *can_buffer, UINT8 u8Messages);

/* CAN transmission handler */
void vfnCAN_Tx_handler(enum tMSCAN_Device device);

/* Poll for new CAN messages on MSCAN software RX circular FIFO */
UINT8 u8CAN_RxFifoDepth( enum tMSCAN_Device device);

/* Read CAN reception ISR */
#pragma CODE_SEG __NEAR_SEG NON_BANKED
    void interrupt vfnCAN_A_RxFrame_Isr(void);
    void interrupt vfnCAN_A_TxFrame_Isr(void);
    void interrupt vfnCAN_B_RxFrame_Isr(void);
    void interrupt vfnCAN_B_TxFrame_Isr(void);
    void interrupt vfnCAN_C_RxFrame_Isr(void);
    void interrupt vfnCAN_C_TxFrame_Isr(void);
    void interrupt vfnCAN_D_RxFrame_Isr(void);
    void interrupt vfnCAN_D_TxFrame_Isr(void);
#pragma CODE_SEG DEFAULT

#endif /* __MSCAN */

/*******************************************************************************/