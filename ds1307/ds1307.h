/**
 *
 * @file     ds1307.h
 *
 * @brief    Real time clock driver hearder file.
 *
 * @author   Theodore Ateba, tf.ateba@gmail.com
 *
 * @date     21 June 2015
 *
 */

#ifndef DS1307_H
#define DS1307_H

/*==========================================================================*/
/* Include Files.                                                           */
/*==========================================================================*/

/* Standard header files. */
#include <stdlib.h>

/* ChibiOS files. */
#include "ch.h"
#include "hal.h"

/*==========================================================================*/
/* Driver macro.                                                            */
/*==========================================================================*/

#define DS1307_MAX_DATA_SIZE  8 /**< Size of the data transmit to the RTC.  */

#define DS1307_ADDRESS      0x68 /**< RTC Address.                          */
#define DS1307_SECONDS_REG  0x00 /**< RTC register containing the seconds.  */

/*==========================================================================*/
/* Driver data structure.                                                   */
/*==========================================================================*/

/**
 * @brief   DS1307 read write data structure.
 */
typedef struct ds1307_data {
  uint8_t   seconds;  /**< RTC seconds.         */
  uint8_t   minutes;  /**< RTC minutes.         */
  uint8_t   hours;    /**< RTC hours.           */
  uint8_t   day;      /**< RTC day.             */
  uint8_t   date;     /**< RTC date.            */
  uint8_t   month;    /**< RTC month.           */
  uint16_t  year;     /**< RTC year.            */
}ds1307_data_t;

typedef struct rtcDriver {
  uint8_t       rxbuf[DS1307_MAX_DATA_SIZE - 1]; /**< RTC reception data.   */
  uint8_t       txbuf[DS1307_MAX_DATA_SIZE];     /**< RTC transmission data.*/
  i2cflags_t    errors;

  ds1307_data_t rtc;
  uint16_t      refYear;  /**< RTC reference year.  */
  uint8_t       dps;
  uint8_t       dtp;
}rtcDriver_t;


//typedef struct ds1307_t rtcDriver;
//rtcDriver_t rtcDriver;
//rtcDriver_t calendar;

/*==========================================================================*/
/* Driver functions prototypes.                                             */
/*==========================================================================*/

uint8_t bcd2Dec(uint8_t val);
uint8_t dec2Bcd(uint8_t val);
void    print(char *p);
void    ds1307InitInterface(void);
void    ds1307PrintClock(rtcDriver_t *rtcp);
void    ds1307GetClock(rtcDriver_t *rtcp);
void    ds1307SetClock(rtcDriver_t *rtcp);

#endif /* DS1307_H */

