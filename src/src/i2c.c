/* 
 * File:   i2c.c
 * Author: Alienware 13 R2
 *
 * Created on 26 dicembre 2019, 21.34
 */

#include <stdio.h>
#include <stdlib.h>
#include "i2c.h"

void i2c_master_ack(int val){ // sends  ACK = 0 ( slave should send another byte ) // or NACK = 1 (no more bytes requested from slave ) 
    I2C1CONbits.ACKDT = val ; // store ACK/NACK in ACKDT 
    I2C1CONbits.ACKEN = 1; // send ACKDT 
    while ( I2C1CONbits.ACKEN);
}// wait for ACK/NACK to be sent 

void i2cMasterStart() {
    I2C1CONbits.SEN = 1; // send the start bit 
    while( I2C1CONbits.SEN);// wait for the start bit to be sent 
}

void i2cMasterStop() { // send a STOP:
    I2C1CONbits.PEN = 1; // comm i s complete and master relinquishes bus 
    while (I2C1CONbits.PEN);
}// wait for STOP to complete }

void i2cMasterSetup() {
    I2C1CON = 0;            //Clear the content of I2C1CON register
    I2C1BRG = 0x2F; // I2CBRG = [1/(2* Fsck ) ? PGD]* Pblck ? 2 // Fsck i s the freq (100 kHz here ) , PGD = 104 ns 
    I2C1CONbits.ON = 1; // turn on the I2C1 module 
    I2C1CONbits.ACKEN = 1;
}

void i2c_master_restart () { 
    I2C1CONbits.RSEN = 1; // send a restart 
    while ( I2C1CONbits.RSEN); // wait for the restart to clear 
    
}

void i2cMasterWrite(unsigned char slaveAddresse,unsigned char *dataBuffer,
        unsigned char bytesNumber, unsigned char stopBit) { // send a byte to slave
    unsigned char byte;
    i2cMasterStart();
    I2C1TRN = slaveAddresse<<1 ; // if an address , bit0 = 0 for write , 1 for read 
    while ( I2C1STATbits.TRSTAT);// wait for the transmission to f i n i s h i f ( 
    if(I2C1STATbits.ACKSTAT)// optional , useful for debugging { // i f this i s high , slave has not acknowledged 
        putssLCD("failed to receive ACK\r\n"); 
    else{
        for(byte=0;byte<bytesNumber;byte++){
            I2C1TRN=dataBuffer[byte];
            while ( I2C1STATbits.TRSTAT);
        }
    }
    if(stopBit)
        i2cMasterStop();
}

void i2cMasterRead(unsigned char slaveAddress,
        unsigned char *dataBuffer,unsigned char bytesNumber ) { // receive a byte from the slave 
    unsigned char byte=0;
    I2C1CONbits.RSEN=1;
    while(I2C1CONbits.RSEN )
    I2C1TRN=(slaveAddress<<1)+1;
    while(I2C1STATbits.TRSTAT);
    if(!I2C1STATbits.ACKSTAT){
        for(byte=0;byte<bytesNumber; byte++){
            I2C1CONbits.RCEN = 1; // start receiving data 
            if(byte==bytesNumber-1)
                I2C1CONbits.ACKDT = 1;
            else
                I2C1CONbits.ACKDT = 0;
            while(I2C1CONbits.RCEN);
            dataBuffer[byte]=I2C1RCV;
            I2C1CONbits.ACKEN = 1;
            while(I2C1CONbits.ACKEN);
        }
    }
    I2C1CONbits.ACKEN = 1;          //Initiate Acknowledge sequence on SDAx and SCLx pins and transmit ACKDT data bit. Wait for Acknowledge sequence to complete 
    i2cMasterStop();
}



