/* Project: ADB-01A
 * File: hardwareConfig.h
 * 
 * PLL output must be between 60M and 120MHz.
 * If the clock is 108MHz (4M*27) and the audio clock is 256 fs, then the audio
 * clock can be 12MHz, BICK = 3MHz with a resulting fs = 46.875Hz.
 */

#include <xc.h>
#include <p33EP512MC806.h>
#include "hardwareConfig.h"

int pinInit()
{
    // Set pin 49 (D1) to output
    PIN_20_IO = 0;
    PIN_21_IO = 0;
    PIN_22_IO = 0;
    PIN_23_IO = 0;
    DAC_PDN_IO = 0;
    DAC_DFS0_IO = 0;
    DAC_DFS1_IO = 0;
    DAC_DIF0_IO = 0;
    DAC_DIF1_IO = 0;
    DAC_DEM_IO = 0;
    
    // Power down
    DAC_PDN = 0;
    
    // 16/24 bit, I2S
    DAC_DIF0 = 1;
    DAC_DIF1 = 1;
    
    // Normal Speed
    DAC_DFS0 = 0;
    DAC_DFS1 = 0;
    
    // De-emphasis filter off
    DAC_DEM = 0;
    
    // Peripheral Pin Select 
    // DAC_MCLK - RP118 - RG6 - REFCLKO
    RPOR13bits.RP118R = 0x31;
    // DAC_BICK - RP87 - RE7 - CSCK
    RPOR6bits.RP87R = 0x0C;
    // DAC_SDTI - RP85 - RE5 - CSDO
    RPOR6bits.RP85R = 0x0B;
    // DAC_LRCK - RP84 - RE4 - COFS
    RPOR5bits.RP84R = 0x0D;
    
    
    PIN_20 = 0;
    PIN_21 = 0;
    PIN_22 = 0;
    PIN_23 = 0;
    
    return 0;
}

/*******************************************************************************
 * OSCILLATOR INITIALIZATION
 * 
 * 
 ******************************************************************************/
int oscillatorInit()
{    
    int i;
    for(i=0; i < 10000; i++)
    {
        Nop();
    }
    
    if(OSCCONbits.COSC == 0x02) return 0;
    
    // Make sure the oscillator PLL is not on
    if(OSCCONbits.COSC != 0x00)
    {
        //Unlock PLL
        __builtin_write_OSCCONH(0x78);
        __builtin_write_OSCCONH(0x9A);
        __builtin_write_OSCCONH(0x00);

        __builtin_write_OSCCONL(OSCCON | 0x46);
        __builtin_write_OSCCONL(OSCCON | 0x57);
        __builtin_write_OSCCONL(OSCCON | 0x01);

        while(OSCCONbits.OSWEN != 0);

        while(OSCCONbits.LOCK != 1)
        {
            if(OSCCONbits.CF == 1){
                OSCCONbits.CF = 0;
                return -1;
            }
        }
    }
    
    CLKDIV = 0x0000;
    PLLFBD = 0x0035;
    OSCTUN = 0x0000;
    
    //Unlock PLL
    __builtin_write_OSCCONH(0x78);
    __builtin_write_OSCCONH(0x9A);
    __builtin_write_OSCCONH(0x02);
    
    __builtin_write_OSCCONL(OSCCON | 0x46);
    __builtin_write_OSCCONL(OSCCON | 0x57);
    __builtin_write_OSCCONL(OSCCON | 0x01);
    
    // No Operations to allow the OSWEN bit to set
    Nop(); Nop(); Nop(); Nop();
    
    while(OSCCONbits.OSWEN != 0);
    
    while(OSCCONbits.LOCK != 1)
    {
        if(OSCCONbits.CF == 1){
            OSCCONbits.CF = 0;
            return -1;
        }
    }
    
    return 0;
}


// **UNFINISHED
void timerInit()
{
    // Timer configuration
    T1CONbits.TON = 0;  // Turn timer off for setup
    T1CONbits.TSIDL = 1;    // Discontinue in idle mode
    T1CONbits.TGATE = 0;    // Gated Time accumulation disabled
    T1CONbits.TCKPS = 0x0;  // 1:1 prescale value
    T1CONbits.TSYNC = 0;   
    T1CONbits.TCS = 0;  // Clock = Fosc/2
    
    // Interrupt
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
    IPC0bits.T1IP = 0x02;
    
    PR1 = 3168; // 6334 @ 101.3375MHz = 8kHz
    
    T1CONbits.TON = 1;  // Turn on timer
}

/*******************************************************************************
 * DCI INITIALIZE
 * 
 * DESCRIPTION:
 * dciInit() sets up the DCI hardware. It does not turn on the module. The BCLK
 * needs to run at a multiple of 256, 384, 512, or 768 fs.
 * 
 * PARAMETERS:
 * [none]
 * 
 * RETURN:
 * [none]
 ******************************************************************************/
void dciInit()
{
    // Disable DCI
    DCICON1bits.DCIEN = 0;
    
    // Disabled in CPU idle
    // Change the data on the low clock, sampled on rising edge
    // CSCKD is an output
    // COFS is an output
    // Transmits 0 on transmit underflow
    // CSDO drives 0's during disables transmit time slots
    // Data transmission begins during the same serial clock as frame sync
    // I2S
    //DCICON1 = 0x2221; //0010 0010 0010 0001
    DCICON1bits.DCISIDL = 0;    // Module continues operation in CPU idle mode
    DCICON1bits.DLOOP = 0;  // Data loopback mode is disabled
    DCICON1bits.CSCKD = 0;  // CSCK is an output pin
    DCICON1bits.CSCKE = 1;  // Data changes on the falling clock
    DCICON1bits.COFSD = 0;  // COFS is an output
    DCICON1bits.UNFM = 0;   // Transmits 0's on underflow
    DCICON1bits.CSDOM = 0;  // Transmits 0's during disabled transmit slots
    DCICON1bits.DJST = 0;   // Data transmission begins one serial clock after frame synch pulse
    DCICON1bits.COFSM = 0x1;    // I2S frame synch mode
    
    // One data word buffered between interrupts
    // Data frame is 2 words
    // Word length is 16 bits
    DCICON2 = 0x002F; // 0000 0000 0010 1111
    DCICON2bits.BLEN = 0x1; // Two data words 
    DCICON2bits.COFSG = 0x1;    // Data frame has 2 words
    DCICON2bits.WS = 0xF;   // Data word is 16 bits
    
    // DCI Clock generator control bits
    // BCG = [Fcy/(2*Fcsck)] - 1
    DCICON3 = 1;
    
    TSCON = 0x0003; // Uses two time slot to transmit
    RSCON = 0x0003;
    
    // Clear DCI IF
    
    // Enable DCI interrupt
    
}

/*******************************************************************************
 * DCI TURN ON
 * 
 * DESCRIPTION:
 * 
 * PARAMETERS:
 * 
 * RETURN:
 * 
 ******************************************************************************/
void dciTurnOn()
{
    // Enable DCI
    DCICON1bits.DCIEN = 1;
}

/*******************************************************************************
 * DCI TURN OFF
 * 
 * DESCRIPTION:
 * 
 * PARAMETERS:
 * 
 * RETURN:
 * 
 ******************************************************************************/
void dciTurnOff()
{
    // Disable DCI
    DCICON1bits.DCIEN = 0;
}

/*******************************************************************************
 * WRITE TO DCI
 * 
 * DESCRIPTION:
 * 
 * PARAMETERS:
 * 
 * RETURN:
 * 
 ******************************************************************************/
void writeToDci()
{
    TXBUF0;
    TXBUF1;
}


/*******************************************************************************
 * TEST DCI
 * 
 * DESCIPTION:
 * testDci() connects the transmit line to the receive line to determine if
 * the hardware is setup correctly.
 * 
 * PARAMETERS:
 * [none]
 * 
 * RETURN:
 * 0 if successful
 * 1 if unsuccessful
 ******************************************************************************/
int testDci()
{
    int retData;
    int sendData = 0xAF05;
    int timeout = 0;
    
    // Activate DLOOP
    DCICON1bits.DLOOP = 1;
    
    // Clear receive buffer
    retData = RXBUF0;
    retData = 0;
    
    Nop();
    Nop();
    
    // Transfer data
    TXBUF0 = sendData;
    TXBUF1 = sendData;
    
    // Wait until data is sent
    while(DCISTATbits.TMPTY == 0) continue;
    
    // Wait until data is received
    while((DCISTATbits.RFUL != 1) && (timeout < 0x0100))
    {
        timeout++;
    }
    
    // Read data
    retData = RXBUF0;
    
    // Detach lines
    DCICON1bits.DLOOP = 0;
    
    if(retData != sendData) return 1;
    if(timeout >= 0x0100) return -1;
    
    return 0;   
}