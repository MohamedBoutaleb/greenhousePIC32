/* 
 * File:   acl.c
 * Author: Alienware 13 R2
 *
 * Created on 26 dicembre 2019, 21.36
 */

#include <stdio.h>
#include <stdlib.h>
#include "acl.h"
#include "i2c.h"

float fGRangeLSB;


unsigned char ACL_GetRegister(unsigned char bAddress){
    unsigned char bResult;
    i2cMasterWrite(ACL_I2C_ADDR, &bAddress, 1, 0);
    i2cMasterRead(ACL_I2C_ADDR, &bResult, 1);

    return bResult;
}
unsigned char ACL_SetRegister(unsigned char bAddress, unsigned char bValue){
    unsigned char rgVals[2], bResult;
    rgVals[0] = bAddress;       // register address
    rgVals[1] = bValue;         // register value
    
    bResult = i2cMasterWrite(ACL_I2C_ADDR, rgVals, 2, 1);

    return bResult;
}

unsigned char ACL_SetRange(unsigned char bRange){
    unsigned char bResult, bVal;
    bRange &= 3;    // only 2 least significant bits from bRange are used

    
    bVal = ACL_GetRegister(ACL_XYZDATACFG); // get old value of the register
    bVal &= 0xFC;   // mask out the 2 LSBs
    bVal |= bRange; // set the 2 LSBs according to the range value
    bResult = ACL_SetRegister(ACL_XYZDATACFG, bVal);

    // set fGRangeLSB according to the selected range
    unsigned char bValRange = 1<<(bRange + 2);
    fGRangeLSB = ((float)bValRange)/(1<<12);     // the range is divided to the resolution corresponding to number of bits (12)
    return bResult;
}

float ACL_ConvertRawToValueG(unsigned char *rgRawVals)
{
    // Convert the accelerometer value to G's. 
    // With 12 bits measuring over a +/- ng range we can find how to convert by using the equation:
    // Gs = Measurement Value * (G-range/(2^12))

    unsigned short usReading = (((unsigned short)rgRawVals[0]) << 4) + (rgRawVals[1] >> 4);
    // extend sign from bit 12 bits to 16 bits
    if(usReading & (1<<11))
    {
        usReading |= 0xF000;
    }
  // fGRangeLSB is pre-computed in ACL_SetRange
    float fResult = ((float)((short)usReading)) * fGRangeLSB;
    return fResult;
}

void ACL_ReadGValues(float *rgGVals){
    unsigned char rgRawVals[6];
    ACL_ReadRawValues(rgRawVals);
    rgGVals[0] = ACL_ConvertRawToValueG(rgRawVals);
    rgGVals[1] = ACL_ConvertRawToValueG(rgRawVals + 2);
    rgGVals[2] = ACL_ConvertRawToValueG(rgRawVals + 4);
}

void ACL_ReadRawValues(unsigned char *rgRawVals){
    unsigned char bVal = ACL_OUT_X_MSB;
    
    i2cMasterWrite(ACL_I2C_ADDR, &bVal, 1, 0);
    i2cMasterRead(ACL_I2C_ADDR, rgRawVals, 6);
}
void initACL(){
    TRISGbits.TRISG0=1;
    i2cMasterSetup();
    ACL_SetRange(0); // +/- 2g
    ACL_SetRegister(ACL_CTRL_REG1, 1);        // Output data rate at 800Hz, no auto wake, no auto scale adjust, no fast read mode
}