/*******************************************************************************/
/**
\file       mscan.c
\brief      MSCAN initialization and CAN frames basic functions
\author     Abraham Tezmol
\version    2.0
\date       27/12/2013
*/

/** CAN Driver definitions  */
#include "mscan.h"

/* Length of received frame */
UINT8 u8RxLength;
/* ID of Rx message in Foreground buffer */
UINT32 u32RxID;

/* pointer to driver global configuration*/
const tMSCAN_config *mscan_cfg;

/* pointer to driver global status structure (dinamically allocated) */
tMSCAN_DeviceStatus *mscan;

/*******************************************************************************/
/**
* \brief    MSCAN configuration
* \author   Abraham Tezmol
* \param    const tMSCAN_config * MSCAN_config, MSCAN driver configuration
* \return   void
*/
void vfnCAN_Init(const tMSCAN_config * MSCAN_config) 
{
    UINT8  u8numberOfDevices;
    UINT8  u8rx_buffer_depth;
    UINT8  u8tx_queue_depth;
    UINT8  device;
    UINT8  MSCANxCTL1_temp;
    UINT32 baudrate;
    
    mscan_cfg = MSCAN_config;
    u8numberOfDevices = mscan_cfg->mscan_devices;
    
    /* Allocate memory for internal status and control structures */
    mscan = (tMSCAN_DeviceStatus *)MemAlloc_NearReserve( u8numberOfDevices * sizeof(tMSCAN_DeviceStatus));
    
    /* go through all device configurations and configure them all based upon their individual settings */
    for (device = 0; device < u8numberOfDevices; device++)
    {
        /* Get individual device buffer settings */
        u8rx_buffer_depth = mscan_cfg->mscan_device_cfg[device].rx_buffer_depth;
        u8tx_queue_depth = mscan_cfg->mscan_device_cfg[device].tx_fifo_depth;
        
        /* Allocate memory for internal rx and tx structures */
        mscan[device].rx_buffer = (tMSCAN_Message *)MemAlloc_NearReserve( u8rx_buffer_depth * sizeof(tMSCAN_Message));
        mscan[device].tx_buffer = (tMSCAN_Message *)MemAlloc_NearReserve( u8tx_queue_depth * sizeof(tMSCAN_Message));
        /* Initialize consumer and producer buffer pointers for rx */
        mscan[device].rx_read  = mscan[device].rx_buffer;
        mscan[device].rx_end   = mscan[device].rx_buffer + u8rx_buffer_depth - 1;
        mscan[device].rx_write = mscan[device].rx_buffer;
        mscan[device].rx_fifo_current_depth = 0;
        /* Initialize consumer and producer buffer pointers for tx */
        mscan[device].tx_read  = mscan[device].tx_buffer;
        mscan[device].tx_end   = mscan[device].tx_buffer + u8tx_queue_depth - 1;
        mscan[device].tx_write = mscan[device].tx_buffer;
        mscan[device].tx_queue_current_depth = 0;
           
        /* MSCAN in initialization mode */
        MSCAN_WRITE_CTL0(device, 0x01);
        
        /* Wait for initialization mode acknowledge */
        while ((MSCAN_READ_CTL1(device) & MSCAN_CTL1_INITAK_MASK) != MSCAN_CTL1_INITAK_MASK)
        {;}

        /* Read settings from CTL1 register */
        MSCANxCTL1_temp  = MSCAN_READ_CTL1(device);
        /* Enable MSCAN module */
        MSCANxCTL1_temp |= MSCAN_CTL1_CANE_MASK;
        /* LoopBack Mode Disabled */
        MSCANxCTL1_temp &= ~MSCAN_CTL1_LOOPB_MASK;                   
        /* Listen only mode Disabled */
        MSCANxCTL1_temp &= ~MSCAN_CTL1_LISTEN_MASK;                  
        /* Clock source is XTAL */
        MSCANxCTL1_temp &= ~MSCAN_CTL1_CLKSRC_MASK;
        /* Write settings back onto CTL1 register */
        MSCAN_WRITE_CTL1(device, MSCANxCTL1_temp);                           
                                        
        /* Configure Baud Rate as per customer settings */
        baudrate =  mscan_cfg->mscan_device_cfg[device].baudrate;
        vfnCAN_BaudRateConfig(device, baudrate);
        
        /* Configure acceptance filters */
        vfnCAN_AcceptanceFiltersInit(&mscan_cfg->mscan_device_cfg[device]);
            
        /* Exit initialization mode request */
        MSCAN_WRITE_CTL0(device, 0x00);
        
        /* Wait for normal mode */
        while ((MSCAN_READ_CTL1(device) & MSCAN_CTL1_INITAK_MASK) == MSCAN_CTL1_INITAK_MASK)
        {;}                                     
        
        /* Clear receiver flags */
        MSCAN_WRITE_RFLG(device, (MSCAN_READ_RFLG(device) | MSCAN_RFLAG_RXF_MASK));

        /* Access to configuration data registers for interrupts */
        INT_CFADDR      = 0xB0; 	/* with vectors from 0xFFB0 to 0xFFBE */
        INT_CFDATA0     = 0x06;     /* CAN Tx Channel 0, S12 Core handled, priority 6 */
        INT_CFDATA1     = 0x06 ; 	/* CAN Rx Channel 0, S12 Core handled, priority 6 */
                        
        /* Enable Full Receive Buffer interrupt */
        MSCAN_WRITE_RIER(device, (MSCAN_READ_RIER(device) | MSCAN_RIER_RXFIE_MASK));  
    }
                         
}

/*******************************************************************************/
/**
* \brief    MSCAN Baud rate configuration
* \author   Abraham Tezmol
* \param    enum tMSCAN_Device device
* \param    UINT32 u32Baudrate - baudrate in bps
* \return   void
*/
void vfnCAN_BaudRateConfig(enum tMSCAN_Device device, UINT32 u32Baudrate) 
{
    UINT8 brp;
    UINT8 sjw;
    UINT8 samp;
    UINT8 tseg1;
    UINT8 tseg2;

    switch (u32Baudrate)
    {        
        case (CAN_BAUDRATE_500Kbps):
            /* Prescaler = CANCLK/(Baud rate * time quanta) = 4MHz/(500kHz * 8) = 1         */
            /* TSEG1 + TSEG2 + SYNCH_SEG = 8 time quantas                                   */         
            brp     = 0x00;                 /* Baud rate prescaler = 1      */
            sjw     = 0x01;                 /* SJW = 2 quanta               */      
            samp    = 0x00;                 /* 1 sample per bit             */
            tseg1   = 0x04;                 /* Time Segment 1 = 5 quanta    */
            tseg2   = 0x01;                 /* Time Segment 2 = 2 quanta    */         
            MSCAN_WRITE_BTR0( device, ( (sjw  << 6) | brp ) );
            MSCAN_WRITE_BTR1( device, ( (samp << 7) | (tseg2 << 4) | tseg1 ) );
            break;
            
        case (CAN_BAUDRATE_250Kbps):
        case (CAN_BAUDRATE_1Mbps):
            break;
    }

}

/*******************************************************************************/
/**
* \brief    MSCAN acceptance filters configuration
* \author   Abraham Tezmol
* \param    const tMSCAN_DeviceConfig * controller_cfg
* \return   void
*/
void vfnCAN_AcceptanceFiltersInit(const tMSCAN_DeviceConfig * controller_cfg) 
{
    const tMSCAN_RxHWObjectConfig * rx_hwObj_cfg;
    UINT8 *acc_reg;
    UINT8 *mask_reg;
    enum tMSCAN_AccFilterModeCfg filter_cfg;
    enum tMSCAN_Device device;
    tMSCAN_IdType id_type;
    UINT8 idam;
    UINT8 index;
    
    
    filter_cfg = controller_cfg->filter_cfg;
    device = controller_cfg->device;
    rx_hwObj_cfg = controller_cfg->rx_hwObj_cfg; 
    

    switch (filter_cfg)
    {
        case (MSCAN_ACC_FILTERS_8_BIT_MODE):
            /* Eight 8-bit acceptance filters */
            idam = 0x02 << 4;
            MSCAN_WRITE_IDAC(device, idam);
            
            /* Get address of first acceptance register */
            acc_reg = MSCAN_REG_IDAR0 (device);
            /* Get address of first mask register */
            mask_reg = MSCAN_REG_IDMR0(device);
            /* sweep through the first 4 filters */  
            for (index = 0; index < 4; index++)
            {
                id_type = rx_hwObj_cfg->id_type;
                if (id_type == STANDARD_ID)
                {
                    *acc_reg  =  STD_ID_ACC_CODE_HIGH(rx_hwObj_cfg->mssg_id);
                    *mask_reg =  MASK_CODE_ST_ID_HIGH;
                }
                else
                { 
                    *acc_reg  =  EXT_ID_ACC_CODE_IR0(rx_hwObj_cfg->mssg_id);
                    *mask_reg =  MASK_CODE_EXT_ID_IR0;
                }
                /* Point to next acceptance and mask registers */
                acc_reg++;
                mask_reg++;
                /* Similarly, point to next Rx object (acceptance filter) */
                rx_hwObj_cfg++;
            }
            /* Get address of fourth acceptance register */
            acc_reg = MSCAN_REG_IDAR4 (device);
            /* Get address of fourth mask register */
            mask_reg = MSCAN_REG_IDMR4(device);
            /* sweep through the last 4 filters */ 
            for (index = 0; index < 4; index++)
            {
                id_type = rx_hwObj_cfg->id_type;
                if (id_type == STANDARD_ID)
                {
                    *acc_reg  =  STD_ID_ACC_CODE_HIGH(rx_hwObj_cfg->mssg_id);
                    *mask_reg =  MASK_CODE_ST_ID_HIGH;
                }
                else
                { 
                    *acc_reg  =  EXT_ID_ACC_CODE_IR0(rx_hwObj_cfg->mssg_id);
                    *mask_reg =  MASK_CODE_EXT_ID_IR0;
                }
                /* Point to next acceptance and mask registers */
                acc_reg++;
                mask_reg++;
                /* Similarly, point to next Rx object (acceptance filter) */
                rx_hwObj_cfg++;
            }        
            break;
            
        case (MSCAN_ACC_FILTERS_16_BIT_MODE):
            /* Four 16-bit acceptance filters */
            idam = 0x01 << 4;
            MSCAN_WRITE_IDAC(device, idam );                  
    
            /* 16 bit Filter 0 */
            CAN0IDAR0 = STD_ID_ACC_CODE_HIGH(RX_16_BIT_CANID_0);   
            CAN0IDMR0 = MASK_CODE_ST_ID_HIGH;
            CAN0IDAR1 = STD_ID_ACC_CODE_LOW(RX_16_BIT_CANID_0);
            CAN0IDMR1 = MASK_CODE_ST_ID_LOW;
            
            /* 16 bit Filter 1 */
            CAN0IDAR2 = STD_ID_ACC_CODE_HIGH(RX_16_BIT_CANID_1);   
            CAN0IDMR2 = MASK_CODE_ST_ID_HIGH;
            CAN0IDAR3 = STD_ID_ACC_CODE_LOW(RX_16_BIT_CANID_1);
            CAN0IDMR3 = MASK_CODE_ST_ID_LOW;
            
            /* 16 bit Filter 2 */
            CAN0IDAR4 = STD_ID_ACC_CODE_HIGH(RX_16_BIT_CANID_2);   
            CAN0IDMR4 = MASK_CODE_ST_ID_HIGH;
            CAN0IDAR5 = STD_ID_ACC_CODE_LOW(RX_16_BIT_CANID_2);
            CAN0IDMR5 = MASK_CODE_ST_ID_LOW;

            /* 16 bit Filter 3 */    
            CAN0IDAR6 = STD_ID_ACC_CODE_HIGH(RX_16_BIT_CANID_3);   
            CAN0IDMR6 = MASK_CODE_ST_ID_HIGH;
            CAN0IDAR7 = STD_ID_ACC_CODE_LOW(RX_16_BIT_CANID_3);
            CAN0IDMR7 = MASK_CODE_ST_ID_LOW;
            
            break;
            
        case (MSCAN_ACC_FILTERS_32_BIT_MODE):   
            /* Two 32-bit acceptance filters */
            idam = 0x00 << 4;
            MSCAN_WRITE_IDAC(device, idam );              
            
            /* 32 bit Filter 0 */
            CAN0IDAR0 = EXT_ID_ACC_CODE_IR0(RX_32_BIT_CANID_0);   
            CAN0IDMR0 = MASK_CODE_EXT_ID_IR0;
            CAN0IDAR1 = EXT_ID_ACC_CODE_IR1(RX_32_BIT_CANID_0);
            CAN0IDMR1 = MASK_CODE_EXT_ID_IR1;
            CAN0IDAR2 = EXT_ID_ACC_CODE_IR2(RX_32_BIT_CANID_0);   
            CAN0IDMR2 = MASK_CODE_EXT_ID_IR2;
            CAN0IDAR3 = EXT_ID_ACC_CODE_IR3(RX_32_BIT_CANID_0);
            CAN0IDMR3 = MASK_CODE_EXT_ID_IR3;
            
            /* 32 bit Filter 1 */
            CAN0IDAR4 = STD_ID_ACC_CODE_HIGH(CAN_STD_ID_0x0644);   
            CAN0IDMR4 = MASK_CODE_ST_ID_HIGH;
            CAN0IDAR5 = STD_ID_ACC_CODE_LOW(CAN_STD_ID_0x0644);
            CAN0IDMR5 = MASK_CODE_ST_ID_LOW;
            CAN0IDAR6 = STD_ID_ACC_CODE_HIGH(0x0000);   
            CAN0IDMR6 = MASK_CODE_ST_ID_HIGH;
            CAN0IDAR7 = STD_ID_ACC_CODE_HIGH(0x0000);
            CAN0IDMR7 = MASK_CODE_ST_ID_LOW; 
        
            break;
        
        case (MSCAN_ACC_FILTERS_INVALID_MODE):   
        default:
            break;    
    }
}

/*******************************************************************************/
/**
* \brief    CAN frame transmission
* \         This functions places a message into the Tx queue ready to be transmitted. No immediate transmission is guaranteed
* \author   Abraham Tezmol
* \param    enum tMSCAN_Device device, u32ID: Identifier, u8Length: Frame size, u8TxData: Data array
* \return   void
*/
UINT8 u8CAN_enqueueFrameforTx(enum tMSCAN_Device device, UINT32 u32ID, UINT8 u8Length, UINT8 *u8TxData, tMSCAN_IdType id_type)
{
    UINT8 return_value;
    UINT8 u8Index;
    
     /* Check for Tx buffer availability. If all queue is full, then return error */
    if ( mscan[device].tx_queue_current_depth >= mscan_cfg->mscan_device_cfg[device].tx_fifo_depth )
    {
        return_value =  ERR_BUFFER_FULL;
    }
    else
    {           
        /* Copy ID to Tx queue */
        mscan[device].tx_write->ID = u32ID;   
        /* Load data to Data Segment Registers */
        for (u8Index=0;u8Index<u8Length;u8Index++) 
        {
            mscan[device].tx_write->Data[u8Index] = u8TxData[u8Index];  
        }
        
        /* Set Data Length Code */
        mscan[device].tx_write->Length = u8Length;
        /* Indicate if message is standard or extended one */
        mscan[device].tx_write->id_type = id_type; 
          
        /* Update queue control variables */  
        mscan[device].tx_queue_current_depth++;
        mscan[device].tx_write++;
        if (mscan[device].tx_write > mscan[device].tx_end)
        {
            mscan[device].tx_write = mscan[device].tx_buffer;
        }
        
        /* Enable mechanism to start transmission */
        MSCAN_WRITE_TIER(device, (MSCAN_READ_TIER(device) | MSCAN_TIER_TXEIE0_MASK | MSCAN_TIER_TXEIE1_MASK | MSCAN_TIER_TXEIE2_MASK));

        /* Indicate that the message was accepted into queue */
        return_value = NO_ERR;
    }
    
    
    return (return_value);
 }

/******************************************************************************************************************/
/**
* \brief    Get current number of CAN messages still pending for TX on queue
* \author   Abraham Tezmol
* \param    enum tMSCAN_Device
* \return   UINT8 - Number of new CAN messages in the reception buffer
*/
UINT8 u8CAN_TxQueueDepth( enum tMSCAN_Device device)
{  
    return ( mscan[device].tx_queue_current_depth );
}


/*******************************************************************************/
/**
* \brief    CAN transmission handler.
* \         Should a CAN message has already been transmitted, queue the next one       
* \author   Abraham Tezmol
* \param    enum tMSCAN_Device
* \return   void
*/
void vfnCAN_Tx_handler(enum tMSCAN_Device device)
{
    UINT8 u8Index;
    /* Transmission buffer */
    UINT8 u8TxBuffer = 0;
    
    /* Verify if there exist pending messages in the Tx queue */
    if (mscan[device].tx_queue_current_depth)
    {
       /* Check for Tx buffer availability on all three hw buffers */
        while(!MSCAN_READ_TFLG(device))
        {                  
            /* Wait until one of them goes empty */;
        } 
        
        /* Select lowest empty buffer */
        MSCAN_WRITE_TBSEL(device, MSCAN_READ_TFLG(device));
        /* Backup selected buffer */
        u8TxBuffer = MSCAN_READ_TBSEL(device);         

        /* Load Id to IDR Registers */
        if(mscan[device].tx_read->id_type == STANDARD_ID)
        {
            *((UINT32 *) ((UINT32)(MSCAN_REG_TXIDR0(device))))= u32CANSTDID(mscan[device].tx_read->ID);
        }
        else
        {
            *((UINT32 *) ((UINT32)(MSCAN_REG_TXIDR0(device))))= u32CAN_EXT_ID(mscan[device].tx_read->ID);
        }
        /* Load data to Data Segment Registers */
        for (u8Index=0; u8Index < mscan[device].tx_read->Length; u8Index++) 
        {
            *(MSCAN_REG_TXDSR0(device) + u8Index) = mscan[device].tx_read->Data[u8Index];  
        }

        /* Set Data Length Code */
        MSCAN_WRITE_TXDLR(device, mscan[device].tx_read->Length);
        /* Set Priority, all same priority. Priority shall be resolved only per ID */       
        MSCAN_WRITE_TXTBPR(device, 0);
        /* Start transmission */
        MSCAN_WRITE_TFLG(device, u8TxBuffer);   
        /* Update control variables for tx queue */
        mscan[device].tx_queue_current_depth--;  
        mscan[device].tx_read++;
        if (mscan[device].tx_read > mscan[device].tx_end)
        {
            mscan[device].tx_read = mscan[device].tx_buffer;
        }        
    }
    else
    {
        /* Disable Transmitter Empty interrupt */
        MSCAN_WRITE_TIER(device, (MSCAN_READ_TIER(device) & !(MSCAN_TIER_TXEIE0_MASK | MSCAN_TIER_TXEIE1_MASK | MSCAN_TIER_TXEIE2_MASK)));
    }

}

/******************************************************************************************************************/
/**
* \brief    Poll for new CAN messages on MSCAN software RX circular FIFO
* \author   Abraham Tezmol
* \param    enum tMSCAN_Device
* \return   UINT8 - Number of new CAN messages in the reception buffer
*/
UINT8 u8CAN_RxFifoDepth( enum tMSCAN_Device device)
{  
    return ( mscan[device].rx_fifo_current_depth );
}

/*******************************************************************************/
/**
* \brief    CAN reception handler.
* \         Upon invocation from an ISR, this function copies a new message from the mail box to the Rx FIFO     
* \author   Abraham Tezmol
* \param    enum tMSCAN_Device
* \return   void
*/

void vfnCAN_Rx_handler(enum tMSCAN_Device device)
{       
  /* Index for extracting/storing received data */
    UINT8 u8Index;
    UINT8 MSCAN_RXIDR1_temp;
    
    /* In case there is incomming message in Message Buffer, process it */  
    if ( (MSCAN_READ_RFLG(device) & MSCAN_RFLAG_RXF_MASK) == MSCAN_RFLAG_RXF_MASK)
    {        
        /* Copy id of filter that caused the match */
        mscan[device].rx_write->filter_id = (MSCAN_READ_IDAC(device) & MSCAN_IDAC_IDHIT_MASK);
        /* verify if STD or EXT frame has been received*/
        MSCAN_RXIDR1_temp = MSCAN_READ_RXIDR1(device);
        if ( (MSCAN_RXIDR1_temp & 0x08) == 0x00 )
        {
            
            mscan[device].rx_write->ID = (UINT16)(MSCAN_READ_RXIDR0(device) << 3)+ (MSCAN_RXIDR1_temp >>5);
        }
        else
        {    
            mscan[device].rx_write->ID = ( 
                                            ((UINT32)(MSCAN_READ_RXIDR0(device))<<21) | ((UINT32)(MSCAN_RXIDR1_temp & 0xE0)<< 13)  |
                                            ((UINT32)(MSCAN_RXIDR1_temp & 0x07)<< 15) | ((UINT32)(MSCAN_READ_RXIDR2(device)<< 7))  |
                                            ((UINT32)(MSCAN_READ_RXIDR3(device)>> 1)) 
                                         );
        }
        /* Extract received frame data length */
        mscan[device].rx_write->Length = (MSCAN_READ_RXDLR(device) & 0x0F);
        
        /* Extract received frame data bytes */
        for (u8Index=0; u8Index<mscan[device].rx_write->Length; u8Index++)
        {
           mscan[device].rx_write->Data[u8Index] = *(MSCAN_REG_RXDSR0(device) + u8Index); 
        }
        /* Raise flag to signal that new data is available for current message */
        mscan[device].rx_write->NewMessage = 1;
        /* Update rx buffer depth */
        mscan[device].rx_fifo_current_depth++;
        
        /* Point to next available buffer in Rx FIFO */
        mscan[device].rx_write++;
        if (mscan[device].rx_write > mscan[device].rx_end)
        {
            mscan[device].rx_write = mscan[device].rx_buffer;
        }        
        /* Clear reception flag */
        MSCAN_WRITE_RFLG(device, (MSCAN_READ_RFLG(device) | MSCAN_RFLAG_RXF_MASK));     
    }
}

/*******************************************************************************/
/**
* \brief    Copy Rx messages.
* \         Upon upper layer request, this function copies n CAN messages from the rx FIFO to a buffer on the upper layer       
* \author   Abraham Tezmol
* \param    enum tMSCAN_Device
* \return   void
*/

void vfnCAN_GetRxMessages(enum tMSCAN_Device device, tMSCAN_Message *can_buffer, UINT8 u8Messages)
{       
    /* Index for extracting/storing received data */
    UINT8 u8Index;
    /* Local copy of pointer to upper layer buffer */
    tMSCAN_Message *buffer_ptr;
    
    /* Store local copy of pointer */
    buffer_ptr = can_buffer;

    /* verify that the driver buffer has at least the amount of messages that the upper layer requested */
    if (mscan[device].rx_fifo_current_depth >= u8Messages) 
    {   
        for (u8Index = 0; u8Index < u8Messages; u8Index++)
        {
            /* Copy the content of the driver buffer into the consumer upper layer buffer */
            *buffer_ptr = *mscan[device].rx_read;
            /* Indicate that the current message has been copied */
            mscan[device].rx_read->NewMessage = 0;
            /* update rx fifo current depth indicator */
            mscan[device].rx_fifo_current_depth--;
            /* update pointers */
            buffer_ptr++;
            mscan[device].rx_read++;
            if (mscan[device].rx_read > mscan[device].rx_end)
            {
                mscan[device].rx_read = mscan[device].rx_buffer;
            }
        }
    }
}

/*******************************************************************************/
/**
* \brief    CAN reception and transmission Interrupt Service Routine.       
* \author   Abraham Tezmol
* \param    void
* \return   void
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED

void interrupt vfnCAN_A_RxFrame_Isr(void)
{          
    vfnCAN_Rx_handler(MSCAN_A);
}

void interrupt vfnCAN_A_TxFrame_Isr(void)
{          
    vfnCAN_Tx_handler(MSCAN_A);
}

#pragma CODE_SEG DEFAULT

/*******************************************************************************/
/**
* \brief    CAN reception and transmission Interrupt Service Routine.       
* \author   Abraham Tezmol
* \param    void
* \return   void
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED

void interrupt vfnCAN_B_RxFrame_Isr(void)
{          
    vfnCAN_Rx_handler(MSCAN_B);
}

void interrupt vfnCAN_B_TxFrame_Isr(void)
{          
    vfnCAN_Tx_handler(MSCAN_B);
}

#pragma CODE_SEG DEFAULT

/*******************************************************************************/
/**
* \brief    CAN reception and transmission Interrupt Service Routine.       
* \author   Abraham Tezmol
* \param    void
* \return   void
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED

void interrupt vfnCAN_C_RxFrame_Isr(void)
{          
    vfnCAN_Rx_handler(MSCAN_C);
}

void interrupt vfnCAN_C_TxFrame_Isr(void)
{          
    vfnCAN_Tx_handler(MSCAN_C);
}

#pragma CODE_SEG DEFAULT

/*******************************************************************************/
/**
* \brief    CAN reception and transmission Interrupt Service Routine.       
* \author   Abraham Tezmol
* \param    void
* \return   void
*/
#pragma CODE_SEG __NEAR_SEG NON_BANKED

void interrupt vfnCAN_D_RxFrame_Isr(void)
{          
    vfnCAN_Rx_handler(MSCAN_D);
}

void interrupt vfnCAN_D_TxFrame_Isr(void)
{          
    vfnCAN_Tx_handler(MSCAN_D);
}

#pragma CODE_SEG DEFAULT
/*******************************************************************************/