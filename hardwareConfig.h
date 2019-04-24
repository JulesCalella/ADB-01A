/* Project: ADB-01A
 * File:   hardwareConfig.h
 * Author: Jules
 *
 * Created on April 18, 2019, 11:21 PM
 */

#ifndef HARDWARECONFIG_H
#define	HARDWARECONFIG_H

#define PIN_20 LATDbits.LATD1
#define PIN_21 LATDbits.LATD2
#define PIN_22 LATDbits.LATD3
#define PIN_23 LATDbits.LATD4
#define DAC_DFS0 LATGbits.LATG8
#define DAC_DFS1 LATGbits.LATG7
#define DAC_DEM LATEbits.LATE6
#define DAC_DIF0 LATEbits.LATE3
#define DAC_DIF1 LATEbits.LATE2
#define DAC_PDN LATEbits.LATE1

#define PIN_20_IO TRISDbits.TRISD1
#define PIN_21_IO TRISDbits.TRISD2
#define PIN_22_IO TRISDbits.TRISD3
#define PIN_23_IO TRISDbits.TRISD4
#define DAC_DFS0_IO TRISGbits.TRISG8
#define DAC_DFS1_IO TRISGbits.TRISG7
#define DAC_DEM_IO TRISEbits.TRISE6
#define DAC_DIF0_IO TRISEbits.TRISE3
#define DAC_DIF1_IO TRISEbits.TRISE2
#define DAC_PDN_IO TRISEbits.TRISE1
#define DAC_MCLK_IO TRISGbits.TRISG6
#define DAC_BICK_IO TRISEbits.TRISE7
#define DAC_SDTI_IO TRISEbits.TRISE5
#define DAC_LRCK_IO TRISEbits.TRISE4

int pinInit();
int oscillatorInit();
void timerInit();
void dciInit();
void dciTurnOn();
void dciTurnOff();
void writeToDci();
int testDci();

#endif	/* HARDWARECONFIG_H */

