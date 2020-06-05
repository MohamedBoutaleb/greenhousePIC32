/* 
 * File:   lcd.c
 * Author: Alienware 13 R2
 *
 * Created on 24 dicembre 2019, 13.40
 */

#include "lcd.h"

static void delayMs(unsigned t){
    TMR1=0X0;
    while(TMR1<t*157);
}

void initLCD(){
    //PMP initialization
    PMCON = 0x83BF ; // Enable the PMP , long waits
    PMMODE = 0x3FF ; // Master Mode 1
    PMAEN = 0x0001 ; // PMA0 enabled
    // init TMR1
    T1CON = 0x8030 ; // Enabled ,1:256 Fpb , 1 tick ~ 6us
    TMR1 = 0; while (TMR1 <6000) ; // 6000 x 6us = 36 ms
    PMADDR = LCDCMD ; // command register ( ADDR = 0)
    PMDATA = 0x38 ; // 8 -bit interface , 2 lines , 5x7
    delayMs(1) ; // >48 us
    PMDATA = 0x0c ; // ON , no cursor , no blink
    delayMs(1) ; // >48 us
    PMDATA = 0x01 ; // clear display
    delayMs(2) ; // >1.6 ms
    PMDATA = 0x06 ; // increment cursor , no shift
    delayMs(2) ; // >1.6 ms
} // end LCDinit
void newLine(){
    cmdLCD(0x80|0x40);
}
void putsLCD(char *s){
    while (*s)
        putLCD(*s++);
}
void putssLCD(char *s){
    initLCD();
    while (*s)
        putLCD(*s++);
}

char readLCD(int addr){
    int dummy ;
    while (PMMODEbits.BUSY); // wait for PMP available
    PMADDR = addr ; // select the command address
    dummy = PMDATA ; // init read cycle , dummy read
    while (PMMODEbits.BUSY);// wait for PMP to be available
    return (PMDATA) ; // read the status register
} // readLCD

void writeLCD(int addr , char c){
    while(busyLCD()); // check busy flag of LCD
    while(PMMODEbits.BUSY);// wait for PMP available
    delayMs(1);
    PMADDR = addr ;
    PMDATA = c;
} // writeLCD