/* Project: ADB-01A 
 * File: main.c
 * Author: Jules Calella
 */

#include <xc.h>
#include <stdint.h>
#include "hardwareConfig.h"

// DSPIC33EP512MC806 Configuration Bit Settings

// 'C' source line config statements

// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GSS = OFF                // General Segment Code-Protect bit (General Segment Code protect is disabled)
#pragma config GSSK = OFF               // General Segment Key bits (General Segment Write Protection and Code Protection is Disabled)

// FOSCSEL
#pragma config FNOSC = PRI              // Initial Oscillator Source Selection bits (Primary Oscillator (XT, HS, EC))
#pragma config IESO = OFF               // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)

// FOSC
#pragma config POSCMD = EC              // Primary Oscillator Mode Select bits (EC (External Clock) Mode)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function bit (OSC2 is general purpose digital I/O pin)
#pragma config IOL1WAY = OFF            // Peripheral pin select configuration (Allow multiple reconfigurations)
#pragma config FCKSM = CSECME           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are enabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler bits (1:32,768)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON              // PLL Lock Wait Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR128           // Power-on Reset Timer Value Select bits (128ms)
#pragma config BOREN = OFF              // Brown-out Reset (BOR) Detection Enable bit (BOR is disabled)
#pragma config ALTI2C1 = OFF            // Alternate I2C pins for I2C1 (SDA1/SCK1 pins are selected as the I/O pins for I2C1)

// FICD
#pragma config ICS = PGD1               // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
#pragma config RSTPRI = PF              // Reset Target Vector Select bit (Device will obtain reset instruction from Primary flash)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

// FAS
#pragma config AWRP = OFF               // Auxiliary Segment Write-protect bit (Aux Flash may be written)
#pragma config APL = OFF                // Auxiliary Segment Code-protect bit (Aux Flash Code protect is disabled)
#pragma config APLK = OFF               // Auxiliary Segment Key bits (Aux Flash Write Protection and Code Protection is Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


//void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
//{
//    
//    IFS0bits.T1IF = 0;
//}


int main(void)
{
    //PTGCONbits.PTGWDT = 0;  // Disable Watchdog timer
    RCONbits.SWDTEN = 0;
    
    int ret;
    
    ret = pinInit();
    PIN_20 = 1;    
    ret = oscillatorInit();
    
    while(ret == -1)
    {
        PIN_20 = ~PIN_20;
        PIN_21 = ~PIN_21;
        PIN_22 = ~PIN_22;
        PIN_23 = ~PIN_23;
        
        ret = oscillatorInit();
    }
    
    PIN_20 = 1;
    PIN_21 = 0;
    PIN_22 = 0;
    PIN_23 = 0;
    
    dciInit();
    dciTurnOn();
    ret = testDci();
    
    if(ret == 1) PIN_21 = 1;        // Returned wrong data
    else if(ret == -1) PIN_22 = 1;  // Timeout
    else if(ret == 0) PIN_23 = 1;   // Worked correctly
    
    // Initialize counter
    int i = 0;
    int j = 0;
    
    // Infinite while loop
    while(1)
    {
        // Increment counter
        i++;
        
        // Determine if enough time has passed
        if(i >= 10000)
        {            
            // Reset counter
            i = 0;
            j++;
            
            if(j > 10)
            {
                j = 0;
                
                // Toggle LED
                PIN_20 = ~PIN_20;
            }
        }
    }
    
    // Return 0 if ever reached
    return 0;
}

