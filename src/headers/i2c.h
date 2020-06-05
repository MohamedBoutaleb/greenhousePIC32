/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _EXAMPLE_FILE_NAME_H    /* Guard against multiple inclusion */
#define _EXAMPLE_FILE_NAME_H
#include <p32xxxx.h>


void i2c_master_ack(int val);
void i2cMasterStart();
void i2cMasterStop() ;
void i2cMasterSetup();
void i2c_master_restart();
void i2cMasterWrite(unsigned char slaveAddresse,unsigned char *dataBuffer,
        unsigned char bytesNumber, unsigned char stopBit) ;
void i2cMasterRead(unsigned char slaveAddress,
        unsigned char *dataBuffer,unsigned char bytesNumber );

#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
