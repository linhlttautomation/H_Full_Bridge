//###########################################################################
//
// FILE:   sdfm_filters_sync_cla_cpu01.c
//
// TITLE:  SDFM Filter sync CLA Example for F2837xD.
//
//! \addtogroup cpu01_example_list
//! <h1> SDFM Filter Sync CLA</h1>
//!
//! In this example, SDFM filter data is read by CLA in Cla1Task1. The
//! SDFM configuration is shown below:
//!  - SDFM1 used in this example
//!  - MODE0 Input control mode selected
//!  - Comparator settings
//!       -    Sinc3 filter selected
//!       -    OSR = 32
//!       -    HLT = 0x7FFF (Higher threshold setting)
//!       -    LLT  = 0x0000(Lower threshold setting)
//!  -  Data filter settings
//!      - All the 4 filter modules enabled
//!      - Sinc3 filter selected
//!      - OSR = 256
//!      - All the 4 filters are synchronized by using MFE
//!       (Master Filter enable bit)
//!      - Filter output represented in 16 bit format
//!      - In order to convert 25 bit Data filter
//!        into 16 bit format user needs to right shift by 9 bits for
//!        Sinc3 filter with OSR = 256
//!  - Interrupt module settings for SDFM filter
//!       -    All the 4 higher threshold comparator interrupts disabled
//!       -    All the 4 lower threshold comparator interrupts disabled
//!       -    All the 4 modulator failure interrupts disabled
//!       -    All the 4 filter will generate interrupt when a new filter data
//!         is available
//!
//! \b External \b Connections \n
//!   - SDFM_PIN_MUX_OPTION1 Connect Sigma-Delta streams to
//!     (SDx-D1, SDx-C1 to SDx-D4,SDx-C4) on GPIO16-GPIO31
//!   - SDFM_PIN_MUX_OPTION2 Connect Sigma-Delta streams to
//!     (SDx-D1, SDx-C1 to SDx-D4,SDx-C4) on GPIO48-GPIO63
//!   - SDFM_PIN_MUX_OPTION3 Connect Sigma-Delta streams to
//!     (SDx-D1, SDx-C1 to SDx-D4,SDx-C4) on GPIO122-GPIO137
//!
//! \b Watch \b Variables \n
//! -  \b Filter1_Result - Output of filter 1
//! -  \b Filter2_Result - Output of filter 2
//! -  \b Filter3_Result - Output of filter 3
//! -  \b Filter4_Result - Output of filter 4
//!
//
//###########################################################################
//
// $Release Date:  $
// $Copyright:
// Copyright (C) 2013-2024 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//###########################################################################

//
// Included Files
//
#include "F28x_Project.h"
#include "cla_sdfm_filter_sync_shared.h"
#include "F2837xD_sdfm_drivers.h"
#include "F2837xD_struct.h"
#include "F2837xD_epwm.h"
#include "F2837xD_adc.h"
#include "setting.h"
#include "variables.h"

//
// Defines
//
#define MAX_SAMPLES               1024
#define SDFM_PIN_MUX_OPTION1      1
#define SDFM_PIN_MUX_OPTION2      2
#define SDFM_PIN_MUX_OPTION3      3
#define WAITSTEP                  asm(" RPT #255 || NOP")

//
// Globals
//
Uint16 gPeripheralNumber;

//
// Function Prototypes
//
// Khai bao cac bien share CPU --> CLA
extern volatile CPU_TO_CLA CpuToCLA;

// Khai bao cac bien share CPU --> CLA
extern volatile CLA_TO_CPU ClaToCPU;

void Sdfm_configurePins(Uint16);
void Cla_initMemoryMap(void);
void CLA_initCpu1Cla(void);
void PWM_Init()
{
    EALLOW;

    //EPWM1A Init
    GpioCtrlRegs.GPAGMUX1.bit.GPIO0 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;

    //EPWM1B Init
    GpioCtrlRegs.GPAGMUX1.bit.GPIO1 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;

    //EPWM2A Init
    GpioCtrlRegs.GPAGMUX1.bit.GPIO2 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;

    //EPWM2B Init
    GpioCtrlRegs.GPAGMUX1.bit.GPIO3 = 0;
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;

    EDIS;
}
void PWM(int period, int deadtime)
{
    EPwm1Regs.TBCTL.bit.FREE_SOFT = 3;
    EPwm1Regs.TBCTL.bit.PHSDIR = TB_DOWN;
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm1Regs.TBCTL.bit.SWFSYNC = 0;
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;
    EPwm1Regs.TBCTL.bit.PRDLD = TB_IMMEDIATE;
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;

    EPwm2Regs.TBCTL.bit.FREE_SOFT = 3;
    EPwm2Regs.TBCTL.bit.PHSDIR = TB_DOWN;
    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;
    EPwm2Regs.TBCTL.bit.SWFSYNC = 0;
    EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
    EPwm2Regs.TBCTL.bit.PRDLD = TB_IMMEDIATE;
    EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;

    EPwm1Regs.CMPA.bit.CMPA = period;
    EPwm1Regs.TBPHS.bit.TBPHS = 0;
    EPwm1Regs.TBCTR = 0;
    EPwm1Regs.TBPRD = period;

    EPwm2Regs.CMPA.bit.CMPA = period;
    EPwm2Regs.TBPHS.bit.TBPHS = 0;
    EPwm2Regs.TBCTR = 0;
    EPwm2Regs.TBPRD = period;

    EPwm1Regs.CMPCTL.all = 0x000C;
    EPwm2Regs.CMPCTL.all = 0x000C;

    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm1Regs.AQCTLA.bit.CAD = AQ_SET;
    EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm2Regs.AQCTLA.bit.CAD = AQ_SET;

    EPwm1Regs.DBCTL.all = 0x03CB;
    EPwm1Regs.DBFED.bit.DBFED = deadtime;
    EPwm1Regs.DBRED.bit.DBRED = deadtime;

    EPwm2Regs.DBCTL.all = 0x03CB;
    EPwm2Regs.DBFED.bit.DBFED = deadtime;
    EPwm2Regs.DBRED.bit.DBRED = deadtime;

    EPwm1Regs.ETSEL.bit.SOCAEN = 1;
    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_PRDZERO;
    EPwm1Regs.ETPS.bit.SOCAPRD = ET_2ND;
    EPwm1Regs.ETCLR.bit.SOCA = 1;
    EPwm1Regs.ETPS.bit.SOCACNT = ET_2ND;

    EPwm1Regs.ETSEL.bit.INTEN = 1;
    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_PRDZERO;
    EPwm1Regs.ETPS.bit.INTPRD = ET_2ND;
    EPwm1Regs.ETPS.bit.INTCNT = ET_2ND;
    EPwm1Regs.ETCLR.bit.INT = 1;

    EDIS;
}
void ADCA_Init(void)
{
    Uint16 i;

    EALLOW;

    //
    //write configurations
    //
    AdcaRegs.ADCCTL2.bit.PRESCALE = 6; //set ADCCLK divider to /4
    //AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    // Cấu hình độ phân giải và chế độ tín hiệu cho ADC A
    AdcaRegs.ADCCTL2.bit.RESOLUTION = 0;  // 12-bit resolution
    AdcaRegs.ADCCTL2.bit.SIGNALMODE = 0;  // Single-ended mode

    //
    //Set pulse positions to late
    //
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;

    //
    //power up the ADC
    //
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;

    //
    //delay for > 1ms to allow ADC time to power up
    //
    for(i = 0; i < 1000; i++)
    {
        asm("   RPT#255 || NOP");
    }
    EDIS;

    EALLOW;
    //
    //Select the channels to convert and end of conversion flag ADCA
    //
    // ILoad
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;          //SOC0 will convert pin A0 -> ILoad
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = 19;         //sample window is 20 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = 0x05;     //trigger on ePWM1 SOCA/C

    // VLoad
    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 1;          //SOC1 will convert pin A1 -> VLoad
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = 19;         //sample window is 20 SYSCLK cycles
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = 0x05;     //trigger on ePWM1 SOCA/C

    // Trigger CLA
    AdcaRegs.ADCINTSOCSEL1.all = 0x0000;          // No ADCInterrupt will trigger SOCx
    AdcaRegs.ADCINTSOCSEL2.all = 0x0000;
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 1;      // EOC1 is trigger for ADCINT1
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;        // enable ADC interrupt 1
    AdcaRegs.ADCINTSEL1N2.bit.INT1CONT = 1;     // ADCINT1 pulses are generated whenever an EOC pulse is generated irrespective of whether the flag bit is cleared or not.
                                                // 0 No further ADCINT2 pulses are generated until ADCINT2 flag (in ADCINTFLG register) is cleared by user.
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;      //make sure INT1 flag is cleared
    EDIS;

}
//
// Main
//
int main(void)
{
   Uint16  pinMuxoption;
   Uint16  HLT, LLT;
   int period = 1000;
   int deadtime = 60;

//
// Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the F2837xD_SysCtrl.c file.
//
    InitSysCtrl();

    EALLOW;

    CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;
    CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;

    EDIS;

    PWM_Init();
    PWM(1000, 60);
//
// Clear all __interrupts and initialize PIE vector table:
// Disable CPU __interrupts
//
    DINT;

//
// Initialize PIE control registers to their default state.
// The default state is all PIE __interrupts disabled and flags
// are cleared.
// This function is found in the F2837xD_PieCtrl.c file.
//
    InitPieCtrl();

//
// Disable CPU __interrupts and clear all CPU __interrupt flags:
//
    IER = 0x0000;
    IFR = 0x0000;

//
// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the __interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in F2837xD_SysCtrl.c.
// This function is found in F2837xD_SysCtrl.c.
//
    InitPieVectTable();

//
// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.

    EALLOW;
//   PieVectTable.SD1_INT = &Sdfm1_ISR;
//   PieVectTable.SD2_INT = &Sdfm2_ISR;
    EDIS;

    EALLOW;
//
// Enable CPU INT5 which is connected to SDFM INT
//
    IER |= M_INT11;

//
// Enable SDFM INTn in the PIE: Group 5 __interrupt 9-10
//
    PieCtrlRegs.PIEIER5.bit.INTx9 = 1;    // SDFM1 interrupt enabled
    PieCtrlRegs.PIEIER5.bit.INTx10 = 1;   // SDFM2 interrupt enabled
    EINT;

    pinMuxoption = SDFM_PIN_MUX_OPTION1;

//
// Configure GPIO pins as SDFM pins
//
    Sdfm_configurePins(pinMuxoption);

    EALLOW;
    CPU1_CLA1(ENABLE);            //Enable CPU1.CLA module
    VBUS32_1(CONNECT_TO_CLA1);    //Connect VBUS32_1 (SDFM bus) to CPU1
    EDIS;

//
// Configure the CLA memory spaces
//
    Cla_initMemoryMap();

//
// Configure the CLA task vectors for CPU1
//
    CLA_initCpu1Cla();

    Cla1ForceTask8andWait();
    WAITSTEP;

    EALLOW;
//
// Trigger Source for TASK1 of CLA1 = SDFM1
//
//    DmaClaSrcSelRegs.CLA1TASKSRCSEL1.bit.TASK1=CLA_TRIG_SD1INT;
    DmaClaSrcSelRegs.CLA1TASKSRCSEL1.bit.TASK1 = 1; //ADCINA1
//
// Trigger Source for TASK1 of CLA1 = SDFM2
//
//    DmaClaSrcSelRegs.CLA1TASKSRCSEL1.bit.TASK2=CLA_TRIG_SD2INT;
    DmaClaSrcSelRegs.CLA1TASKSRCSEL1.bit.TASK2 = 1; //ADCINA1
//
// Lock CLA1TASKSRCSEL1 register
//
    DmaClaSrcSelRegs.CLA1TASKSRCSELLOCK.bit.CLA1TASKSRCSEL1 = 1;
    EDIS;

//
// Select SDFM1
//
    gPeripheralNumber = SDFM1;

//
// Input Control Module
//
// Configure Input Control Mode: Modulator Clock rate = Modulator data rate
//
    Sdfm_configureInputCtrl(gPeripheralNumber, FILTER1, MODE_0);
    Sdfm_configureInputCtrl(gPeripheralNumber, FILTER2, MODE_0);
    Sdfm_configureInputCtrl(gPeripheralNumber, FILTER3, MODE_0);
    Sdfm_configureInputCtrl(gPeripheralNumber, FILTER4, MODE_0);

//
// Comparator Module
//
    HLT = 0x7FFF;    //Over value threshold settings
    LLT = 0x0000;    //Under value threshold settings

//
// Configure Comparator module's comparator filter type and comparator's OSR
// value, higher threshold, lower threshold
//
    Sdfm_configureComparator(gPeripheralNumber, FILTER1, SINC3, OSR_32,
                             HLT, LLT);
    Sdfm_configureComparator(gPeripheralNumber, FILTER2, SINC3, OSR_32,
                             HLT, LLT);
    Sdfm_configureComparator(gPeripheralNumber, FILTER3, SINC3, OSR_32,
                             HLT, LLT);
    Sdfm_configureComparator(gPeripheralNumber, FILTER4, SINC3, OSR_32,
                             HLT, LLT);

//
// Data filter Module
//
// Configure Data filter modules filter type, OSR value and
// enable / disable data filter
//
    Sdfm_configureData_filter(gPeripheralNumber, FILTER1, FILTER_ENABLE, SINC3,
                              OSR_256, DATA_16_BIT, SHIFT_9_BITS);
    Sdfm_configureData_filter(gPeripheralNumber, FILTER2, FILTER_ENABLE, SINC3,
                              OSR_256, DATA_16_BIT, SHIFT_9_BITS);
    Sdfm_configureData_filter(gPeripheralNumber, FILTER3, FILTER_ENABLE, SINC3,
                              OSR_256, DATA_16_BIT, SHIFT_9_BITS);
    Sdfm_configureData_filter(gPeripheralNumber, FILTER4, FILTER_ENABLE, SINC3,
                              OSR_256, DATA_16_BIT, SHIFT_9_BITS);

//
// Enable Master filter bit: Unless this bit is set none of the filter modules
// can be enabled. All the filter modules are synchronized when master filter
// bit is enabled after individual filter modules are enabled.
//
    Sdfm_enableMFE(gPeripheralNumber);

//
// PWM11.CMPC, PWM11.CMPD, PWM12.CMPC and PWM12.CMPD signals cannot
// synchronize the filters. This option is not being used in this example.
//
    Sdfm_configureExternalreset(gPeripheralNumber,FILTER_1_EXT_RESET_DISABLE,
                                FILTER_2_EXT_RESET_DISABLE,
                                FILTER_3_EXT_RESET_DISABLE,
                                FILTER_4_EXT_RESET_DISABLE);

//
// Enable interrupts
//
// Following SDFM interrupts can be enabled / disabled using this function.
//  Enable / disable comparator high threshold
//  Enable / disable comparator low threshold
//  Enable / disable modulator clock failure
//  Enable / disable filter acknowledge
//
    Sdfm_configureInterrupt(gPeripheralNumber, FILTER1, IEH_DISABLE,
                            IEL_DISABLE, MFIE_ENABLE, AE_ENABLE);
    Sdfm_configureInterrupt(gPeripheralNumber, FILTER2, IEH_DISABLE,
                            IEL_DISABLE, MFIE_ENABLE, AE_ENABLE);
    Sdfm_configureInterrupt(gPeripheralNumber, FILTER3, IEH_DISABLE,
                            IEL_DISABLE, MFIE_ENABLE, AE_ENABLE);
    Sdfm_configureInterrupt(gPeripheralNumber, FILTER4, IEH_DISABLE,
                            IEL_DISABLE, MFIE_ENABLE, AE_ENABLE);

//
// Enable master interrupt so that any of the filter interrupts can
// trigger by SDFM interrupt to CPU
//
    Sdfm_enableMIE(gPeripheralNumber);

    while(1);
}

//
// Sdfm_configurePins - Configure the SDFM GPIO pins
//
void Sdfm_configurePins(Uint16 sdfmPinOption)
{
    Uint16 pin;

    switch (sdfmPinOption)
    {
        case SDFM_PIN_MUX_OPTION1:
            for(pin=16;pin<=31;pin++)
            {
                GPIO_SetupPinOptions(pin, GPIO_INPUT, GPIO_ASYNC);
                GPIO_SetupPinMux(pin,GPIO_MUX_CPU1,7);
            }
            break;

        case SDFM_PIN_MUX_OPTION2:
            for(pin=48;pin<=63;pin++)
            {
                GPIO_SetupPinOptions(pin, GPIO_INPUT, GPIO_ASYNC);
                GPIO_SetupPinMux(pin,GPIO_MUX_CPU1,7);
            }
            break;

        case SDFM_PIN_MUX_OPTION3:
            for(pin=122;pin<=137;pin++)
            {
                GPIO_SetupPinOptions(pin, GPIO_INPUT, GPIO_ASYNC);
                GPIO_SetupPinMux(pin,GPIO_MUX_CPU1,7);
            }
            break;
    }
}

//
// Cla_initMemoryMap - Initialize Memory map
//
void Cla_initMemoryMap(void)
{
    EALLOW;

    //
    // Initialize and wait for CLA1ToCPUMsgRAM
    //
    MemCfgRegs.MSGxINIT.bit.INIT_CLA1TOCPU = 1;
    while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CLA1TOCPU != 1){};

    //
    // Initialize and wait for CPUToCLA1MsgRAM
    //
    MemCfgRegs.MSGxINIT.bit.INIT_CPUTOCLA1 = 1;
    while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CPUTOCLA1 != 1){};

    //
    // Copy the program and constants from FLASH to RAM before configuring
    // the CLA
    //
#if defined(_FLASH)
    memcpy((uint32_t *)&Cla1funcsRunStart, (uint32_t *)&Cla1funcsLoadStart,
        (uint32_t)&Cla1funcsLoadSize );
    memcpy((uint32_t *)&Cla1ConstRunStart, (uint32_t *)&Cla1ConstLoadStart,
        (uint32_t)&Cla1ConstLoadSize );
#endif //defined(_FLASH)

    //
    // Select LS1 and LS2 RAM to be the programming space for the CLA
    // Select LS5 to be data RAM for the CLA
    //
    MemCfgRegs.LSxMSEL.bit.MSEL_LS0 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS0 = 0;

    MemCfgRegs.LSxMSEL.bit.MSEL_LS1 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS1 = 1;

    //
    // Filter1 and Filter2 data memory LS0
    //
    MemCfgRegs.LSxMSEL.bit.MSEL_LS2 = 1; //LS2RAM is shared between CPU and CLA
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS2 = 1; // LS2RAM is configured as
                                             // data memory

    //
    // Filter3 and Filter4 data memory LS3
    //
    MemCfgRegs.LSxMSEL.bit.MSEL_LS3 = 1; //LS3RAM is shared between CPU and CLA
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS3 = 1; // LS3RAM is configured as
                                             // data memory
    MemCfgRegs.LSxMSEL.bit.MSEL_LS4 = 1; //LS4RAM is shared between CPU and CLA
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS4 = 1; // LS5RAM is configured as
                                             // program memory
    MemCfgRegs.LSxMSEL.bit.MSEL_LS5 = 1; //LS5RAM is shared between CPU and CLA
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS5 = 1; // LS5RAM is configured as
                                             // program memory

    EDIS;
}

//
// CLA_initCpu1Cla - Initialize CLA1 task vectors and end of task interrupts
//
void CLA_initCpu1Cla(void)
{
    //
    // Compute all CLA task vectors
    // On Type-1 CLAs the MVECT registers accept full 16-bit task addresses as
    // opposed to offsets used on older Type-0 CLAs
    //
    EALLOW;
    Cla1Regs.MVECT1 = (uint16_t)(&Cla1Task1);
    Cla1Regs.MVECT2 = (uint16_t)(&Cla1Task2);
    Cla1Regs.MVECT3 = (uint16_t)(&Cla1Task3);
    Cla1Regs.MVECT4 = (uint16_t)(&Cla1Task4);
    Cla1Regs.MVECT5 = (uint16_t)(&Cla1Task5);
    Cla1Regs.MVECT6 = (uint16_t)(&Cla1Task6);
    Cla1Regs.MVECT7 = (uint16_t)(&Cla1Task7);
    Cla1Regs.MVECT8 = (uint16_t)(&Cla1Task8);

    //
    // Enable IACK instruction to start a task on CLA in software
    // for all  8 CLA tasks
    //
    asm("   RPT #3 || NOP");
    Cla1Regs.MCTL.bit.IACKE = 1;
    Cla1Regs.MIER.all = 0x0083;

    //
    // Configure the vectors for the end-of-task interrupt for all
    // 8 tasks
    //
    PieVectTable.CLA1_1_INT   = &cla1Isr1;
    PieVectTable.CLA1_2_INT   = &cla1Isr2;
    PieVectTable.CLA1_3_INT   = &cla1Isr3;
    PieVectTable.CLA1_4_INT   = &cla1Isr4;
    PieVectTable.CLA1_5_INT   = &cla1Isr5;
    PieVectTable.CLA1_6_INT   = &cla1Isr6;
    PieVectTable.CLA1_7_INT   = &cla1Isr7;
    PieVectTable.CLA1_8_INT   = &cla1Isr8;

    //
    // Enable CLA interrupts at the group and subgroup levels
    //
    PieCtrlRegs.PIEIER11.all  = 0xFFFF;
    IER |= (M_INT11 );

    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM
    EDIS;
}

//
// cla1Isr1 - CLA1 ISR 1
//
interrupt void cla1Isr1 ()
{
     //asm(" ESTOP0");
     PieCtrlRegs.PIEACK.all = M_INT11;
}

//
// cla1Isr1 - CLA1 ISR 2
//
interrupt void cla1Isr2 ()
{
     asm(" ESTOP0");
     PieCtrlRegs.PIEACK.all = M_INT11;
}

//
// cla1Isr1 - CLA1 ISR 3
//
interrupt void cla1Isr3 ()
{
    asm(" ESTOP0");
}

//
// cla1Isr1 - CLA1 ISR 4
//
interrupt void cla1Isr4 ()
{
    asm(" ESTOP0");
}

//
// cla1Isr1 - CLA1 ISR 5
//
interrupt void cla1Isr5 ()
{
    asm(" ESTOP0");
}

//
// cla1Isr1 - CLA1 ISR 6
//
interrupt void cla1Isr6 ()
{
    asm(" ESTOP0");
}

//
// cla1Isr1 - CLA1 ISR 7
//
interrupt void cla1Isr7 ()
{
    asm(" ESTOP0");
}

//
// cla1Isr1 - CLA1 ISR 8
//
interrupt void cla1Isr8 ()
{
    // asm(" ESTOP0");
    PieCtrlRegs.PIEACK.all = M_INT11;
}

//
// End of file
//
