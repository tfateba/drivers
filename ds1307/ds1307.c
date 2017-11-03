/**
 *
 * @file     ds1307.c
 *
 * @brief    Real time clock driver source file.
 *
 * @author   Theodore Ateba, tf.ateba@gmail.com
 *
 * @date     21 June 2015
 *
 */

/*==========================================================================*/
/* Include files.                                                           */
/*==========================================================================*/

/* Drivers files. */
#include "ds1307.h"
#include "iic.h"

/*==========================================================================*/
/* Configuration structure.                                                 */
/*==========================================================================*/

/**
 * @brief I2C Configuration structure
 */
static const I2CConfig i2cConfig = {
  OPMODE_I2C,         /**< I2C Operation mode.                              */
  400000,             /**< I2C Clock speed.                                 */
  FAST_DUTY_CYCLE_2,  /**< I2C Duty cycle mode.                             */
};

/*==========================================================================*/
/* Driver functions.                                                        */
/*==========================================================================*/

/**
 * @brief   Convert BCD to Decimal.
 *
 * @param[in]   val   value to convert from BCD to Decimal
 * @return      res   converted decimal value
 */
uint8_t bcd2Dec(uint8_t val) {

  uint8_t res = ((val/16*10) + (val % 16));
  return res;
}

/**
 * @brief   Convert Decimal to BCD.
 *
 * @param[in]   val   value to convert from Decimal to BCD
 * @return      res   converted BCD value
 */
uint8_t dec2Bcd(uint8_t val) {

  uint8_t res = ((val/10*16) + (val%10));
  return res;
}

/* TODO: This function must me remove. */

/**
 * @brief   Send data to the Serial Driver 2.
 *
 * @param[in]   p   pointer of data to print
 */
void print(char *dp) {

  while (*dp)
    sdPut(&SD2, *dp++);
}

/* TODO: This function must also be removed. */

/**
 * @brief   Print integer to serial driver 2.
 *
 * @param[in] number    integer to print to the serial console
 */
void printn(int16_t number) {

  char buf[16], *p;

  if (!number)
    sdPut(&SD2, '0');
  else {
    p = buf;
    while (number)
      *p++ = (number % 10) + '0', number /= 10;
    while (p > buf)
      sdPut (&SD2, *--p);
  }
}

/**
 * @brief   Configure the I2C Interface 1 and start the Interface.
 */
 void ds1307InitInterface(void) {
 
  i2cStart(&I2CD1, &i2cConfig);
  palSetPadMode(GPIOB, 8, PAL_MODE_ALTERNATE(4) |
                PAL_STM32_OTYPE_OPENDRAIN); /* SCL. */
  palSetPadMode(GPIOB, 9, PAL_MODE_ALTERNATE(4) |
                PAL_STM32_OTYPE_OPENDRAIN); /* SDA. */
}

/**
 * @brief   Set the clock and the calendar of the RTC.
 *
 * @param[in]   rtcp  pointer to the rtc device
 */
void ds1307SetClock(rtcDriver_t *rtcp) {

  msg_t msg;

  rtcp->txbuf[0] = DS1307_SECONDS_REG;
  rtcp->txbuf[1] = dec2Bcd(rtcp->rtc.seconds);
  rtcp->txbuf[2] = dec2Bcd(rtcp->rtc.minutes);
  rtcp->txbuf[3] = dec2Bcd(rtcp->rtc.hours);
  rtcp->txbuf[4] = dec2Bcd(rtcp->rtc.day);
  rtcp->txbuf[5] = dec2Bcd(rtcp->rtc.date);
  rtcp->txbuf[6] = dec2Bcd(rtcp->rtc.month);
  rtcp->txbuf[7] = dec2Bcd(rtcp->rtc.year - rtcp->refYear);

  msg = i2cWriteRegisters(&I2CD1, DS1307_ADDRESS, rtcp->txbuf,
                          DS1307_MAX_DATA_SIZE);

  if (msg != MSG_OK)
    print("\n\r Error when setting the DS1307 date over the I2C bus.");
  else
    print("\n\r DS1307 was setting succefuly.");
}

/* TODO: This function must be removed from here. */

/**
 * @brief   Print the clock and date read from the DS1307 RTC.
 *
 * @param[in]   calendar structure contening the data to print
 */
void ds1307PrintClock(rtcDriver_t *rtcp) {

  print("\n\r");
  printn(rtcp->rtc.date);
  print("/");
  printn( rtcp->rtc.month);
  print("/");
  printn(rtcp->rtc.year);
  print(" ");

  printn(rtcp->rtc.hours);
  print(":");
  printn(rtcp->rtc.minutes);
  print(":");
  printn(rtcp->rtc.seconds);
}

/**
 * @brief   Get Clock and Calendar.
 *
 * @param[out]  rtcp  pointer to the RTC driver module
 */
void ds1307GetClock(rtcDriver_t *rtcp) {

  msg_t msg;

  rtcp->txbuf[0] = DS1307_SECONDS_REG; /* Register address of the Seconds. */

  msg = i2cReadRegisters(&I2CD1, DS1307_ADDRESS, rtcp->txbuf, rtcp->rxbuf,
                         DS1307_MAX_DATA_SIZE - 1);

  if (msg != MSG_OK) {
      rtcp->errors = i2cGetErrors(&I2CD1);
      print("\n\r I2C transmission error!");
  }
  else {
    rtcp->rtc.seconds  = bcd2Dec(rtcp->rxbuf[0] & 0x7F);
    rtcp->rtc.minutes  = bcd2Dec(rtcp->rxbuf[1]);
    rtcp->rtc.hours    = bcd2Dec(rtcp->rxbuf[2] & 0x3F );
    rtcp->rtc.day      = bcd2Dec(rtcp->rxbuf[3]);
    rtcp->rtc.date     = bcd2Dec(rtcp->rxbuf[4]);
    rtcp->rtc.month    = bcd2Dec(rtcp->rxbuf[5]);
    rtcp->rtc.year     = bcd2Dec(rtcp->rxbuf[6]) + rtcp->refYear;
  }
}

