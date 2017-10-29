/**
 *
 * @file    iic.h
 *
 * @brief   I2C driver header file.
 *
 * @author  Theodore Ateba, tf.ateba@gmail.com
 *
 * @date    05 Jully 2016
 *
 */

#ifndef IIC_H
#define IIC_H

/*==========================================================================*/
/* Include files.                                                           */
/*==========================================================================*/

/* ChibiOS files. */
#include <hal.h>

/*==========================================================================*/
/* Functions prototypes.                                                    */ 
/*==========================================================================*/

msg_t i2cReadRegister(I2CDriver *i2cp, uint8_t sad, uint8_t *reg,
                      uint8_t *rxbuf);
msg_t i2cReadRegisters( I2CDriver *i2cp, uint8_t sad, uint8_t *reg,
                        uint8_t *rxbuf, uint8_t lenght);
msg_t i2cWriteRegisters(I2CDriver *i2cp, uint8_t sad, uint8_t *txbuf,
                        uint8_t lenght);

#endif /* IIC_H */

