/****************************************************************************************************/
/**
  \mainpage
  \n 
  \brief        Main application (main module)
  \author       Abraham Tezmol Otero, M.S.E.E
  \project      Omicron 
  \version      1.0
  \date         26/Jun/2009
   
  Program compiled with CW for HCS12(X) v5.0, tested on DEMO9S12XEP100 board
*/
/****************************************************************************************************/

/** Main application common definitions */
#include    "main.h"

/*****************************************************************************************************
* Definition of module wide VARIABLEs 
*****************************************************************************************************/

/****************************************************************************************************
* Declaration of module wide FUNCTIONs 
*****************************************************************************************************/

/*****************************************************************************************************
* Definition of global wide MACROs / #DEFINE-CONSTANTs
*****************************************************************************************************/

/*****************************************************************************************************
* Declaration of module wide TYPEs 
*****************************************************************************************************/

/******************************************************************************************************
* Definition of module wide VARIABLEs 
******************************************************************************************************/
UINT16 u16MemStatus;
/******************************************************************************************************
* Code of module wide FUNCTIONS
******************************************************************************************************/

void vfnBackgroundSubsystemTasks(void);

/*~~~~~~~ Main Code ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void main(void) 
{       
    /* Disable interrupts */
    INTERRUPT_DISABLE(); 
    /* XGATE initialization */
    vfn_XGATE_CPU_init();
    /* Initialize Interrupt Module */
    vfnInterrupt_Init(ASW_WO_BOOT);          
    /* PLL initialization to the desired target frequency */
    vfnPLL_Init();
    
    /* Data Port A initialization */
    PORTA = 0x00;           
    /* Data Direction Register Setup */
    DDRA =  0x0F; 
    
    /* Dynamic Memory Allocation initialization */
    vfnMemAlloc_NearInit(&NearMemAlloc_config[0]);
    vfnMemAlloc_Init(&DynamicMemAlloc_config[0]);
    /* Microcontroller Input/Output pins initialization */
    vfnInputs_Outputs_Init();
    /* Initialize CAN Communications */
    vfnCAN_Init(&MSCAN_cfg[0]);
    /* Initialize Task Scheduler */ 
    vfnScheduler_Init();
     /* Arm COP watchdog */
    vfnCOPWatchdog_Init();
    /* Start execution of task scheduler */
    vfnScheduler_Start();
    /* Once all initializations are complete, enable interrupts */
    INTERRUPT_ENABLE();     
    
    /*-- Loop through all the periodic tasks from Task Scheduler --*/
    for(;;)
    {
        /* Perform all scheduled tasks */
        vfnTask_Scheduler();
        /* Perform all background susbystem control tasks */
        vfnBackgroundSubsystemTasks();
                   
    }
}
/*~~~~~~~ End of Main Code ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/* Background tasks related to Subsystem control */
void vfnBackgroundSubsystemTasks(void) 
{;}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/