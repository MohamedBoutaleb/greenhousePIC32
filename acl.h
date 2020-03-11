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
#define ACL_I2C_ADDR 0x1D
#define ACL_OUT_X_MSB 0x01
#define ACL_XYZDATACFG 0x0E
#define ACL_CTRL_REG1 0x2A
#include <p32xxxx.h>


void initACL();
void ACL_ReadGValues(float *rgGVals);
#ifdef __cplusplus
extern "C" {
#endif


#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
