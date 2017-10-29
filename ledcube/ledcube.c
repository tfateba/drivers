/**
 *
 * @file    ledcube.c
 *
 * @brief   Led cube source file.
 *
 * @author  Theodore Ateba, tf.ateba@gmail.com
 *
 * @date    03 January 2017
 *
 */

/*==========================================================================*/
/* Includes files.                                                          */
/*==========================================================================*/

/* ChibiOS files. */
#include "hal.h"

/*==========================================================================*/
/* Global variables.                                                        */
/*==========================================================================*/

static uint8_t demoIndex = 0;

/*==========================================================================*/
/* Functions.                                                               */
/*==========================================================================*/

/**
 * @brief   Initialize the pins used for the led-cube.
 */
void ledCubeInit(void) {

  int8_t i;

  for (i = 5; i >= 0; i--) {
    palSetPadMode(IOPORT2, i, PAL_MODE_OUTPUT_PUSHPULL);
  }
  
  for (i = 7; i >= 2; i--) {
    palSetPadMode(IOPORT4, i, PAL_MODE_OUTPUT_PUSHPULL);
  }
}

/**
 * @brief   Set/Clear a gpio pin.
 *
 * @param[in] port    the gpio pin port to control
 * @param[in] pin     the gpio pin to control
 * @param[in] value   the gpio pin value to set
 */
static void padCtrl(volatile avr_gpio_registers_t *port, uint8_t pin,
                    uint8_t value) {

  if (value == 0)
    palClearPad(port, pin);
  else if (value == 1)
    palSetPad(port, pin);
  else
    return;
}

/**
 * @brief   Turn on all the leds on the cube.
 *
 * @param[in] tempo   the time to turn on the cube
 */
static void ledCubeOn(uint16_t tempo) {

  uint8_t i;

  for (i = 5; i >= 0; i--) {
    padCtrl(IOPORT2, i, 1);
  }

  for (i = 7; i >= 2; i--) {
    padCtrl(IOPORT4, i, 1);
  }

  chThdSleepMilliseconds(tempo);
}

/**
 * @brief   Turn off all the led on the cube.
 *
 * @param[in] tempo   the time to turn on the cube
 */
static void ledCubeOff(uint16_t tempo) {

  uint8_t i;

  for (i = 5; i >= 0; i--) {
    padCtrl(IOPORT2, i, 0);
  }

  for (i = 7; i >= 2; i--) {
    padCtrl(IOPORT4, i, 0);
  }

  chThdSleepMilliseconds(tempo);
}

/**
 * @brief   Facto is a function that help to turn on led one at time.
 *
 * @param[in] tempo   the time use between the control of two leds
 */
static void facto(uint16_t tempo) {

  uint8_t i;

  for (i = 2; i <= 7; i++) {
    palSetPad(IOPORT4, i);
    chThdSleepMilliseconds(tempo);
  }

  for (i = 0; i <= 2; i++) {
    palSetPad(IOPORT2, i);
    chThdSleepMilliseconds(tempo);
  }
}

/**
 * @brief   First demo function.
 *
 * @param[in] tempo   the time used in the demo
 */
static void ledCubeDemo1(uint8_t tempo) {

  ledCubeOff(0);
  padCtrl(IOPORT2, 3, 1);
  padCtrl(IOPORT2, 4, 0);
  padCtrl(IOPORT2, 5, 0);
  facto(tempo);

  ledCubeOff(0);
  padCtrl(IOPORT2, 3, 0);
  padCtrl(IOPORT2, 4, 1);
  padCtrl(IOPORT2, 5, 0);
  facto(tempo);

  ledCubeOff(0);
  padCtrl(IOPORT2, 3, 0);
  padCtrl(IOPORT2, 4, 0);
  padCtrl(IOPORT2, 5, 1);
  facto(tempo);
}

/**
 * @brief   Control all leds of an actived layer.
 *
 * @param[in] lsbp   pointer to the line state buffer
 * @param[in] lsbs   line state buffer size
 */
static void lineWrite( uint8_t *lsbp, uint8_t lsbs) {

  uint8_t line = 0, pin;

  for (pin = 2; pin <= 7; pin++) {
    padCtrl(IOPORT4, pin, lsbp[line++]);
    if (line > lsbs)
      return;
  }

  for (pin = 0; pin <= 2; pin++) {
    padCtrl(IOPORT2, pin, lsbp[line++]);
    if (line > lsbs)
      return;
  }
}

/**
 * @brief   Selection of one of the tree layer of the cube.
 * 
 * @param[in] lsbp  pointer to the layer state buffer
 * @param[in] lsbs  layer state buffer size
 */
static void layerWrite(uint8_t *lsbp, uint8_t lsbs) {

  uint8_t layer = 2, pin;

  for (pin = 5; pin >= 3; pin--) {
    padCtrl(IOPORT2, pin, lsbp[layer--]);
    if (layer > lsbs)
      return;
  }
}

/**
 * @brief   Turn off the cube top layer.
 *
 * @param[in] tempo   the time to trun off the top layer leds
 */
static void ledCubeTopOff(uint16_t tempo) {

  palClearPad(IOPORT2, 3);
  chThdSleepMilliseconds(tempo);
}

/**
 * @brief   Turn on the cube top layer.
 * 
 * @param[in] tempo   the time to turn on the top leyer leds
 */
static void ledCubeTopOn(uint16_t tempo) {

  uint8_t plan[3] = {1,0,0}; /* top , midle, Bottom. */
  uint8_t lineState[9] = {1, 1, 1, 1,1, 1, 1, 1, 1};

	layerWrite(plan, 3);
	lineWrite(lineState, 9);
	chThdSleepMilliseconds(tempo);
}

/**
 * @brief   Turn off the midle layer of the cube.
 *
 * @param[in] tempo   the time to turn off the midle layer leds
 */
static void ledCubeMidleOff(uint16_t tempo) {

  palClearPad(IOPORT2, 4);
  chThdSleepMilliseconds(tempo);
}

/**
 * @brief   Trun on the midle layer of the cube.
 *
 * @param[in] tempo   the time to turn on the midle layer leds
 */
static void ledCubeMidleOn(uint16_t tempo) {

  uint8_t plan[3] = {0,1,0}; /* top , midle, Bottom. */
  uint8_t lineState[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

  layerWrite(plan, 3);
  lineWrite(lineState, 9);
  chThdSleepMilliseconds(tempo);
}

/**
 * @brief   Turn off the bottom layer of the cube.
 *
 * @param[in] tempo   the time used to turn off the bottom layer leds
 */
static void ledCubeBottomOff(uint16_t tempo) {

  palClearPad(IOPORT2, 5);
  chThdSleepMilliseconds(tempo);
}

/**
 * @brief   Turn on the bottom layer of the cube.
 *
 * @param[in]	tempo   the time used to turn on the bottom leyer leds
 */
static void ledCubeBottomOn(uint16_t tempo) {

  uint8_t plan[3] = {0,0,1}; /* top , midle, Bottom. */
  uint8_t lineState[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1}; 

  layerWrite(plan, 3);
  lineWrite(lineState, 9);
  chThdSleepMilliseconds(tempo);
}

/**
 * @brief   Blink all leds of the cube.
 *
 * @param[in] tempo   the blinking time
 * @param[in] nbr     the number of blink
 */
static void ledCubeBlink(uint16_t tempo, uint8_t nbr) {

  int8_t i;

  for (i = nbr; i >= 0; i--) {
    ledCubeOn(tempo);
    ledCubeOff(tempo);
  }
}

/**
 * @brief   Make a circular effect.
 *
 * @param[in] tempo   tempo for the rotation effect
 */
static void ledCubeCircularDemo(uint16_t tempo) {

  int8_t i, j;
  uint8_t lineState[8][9] = { {1, 0, 0, 0, 0, 0, 0, 0, 0},
                              {0, 1, 0, 0, 0, 0, 0, 0, 0},
                              {0, 0, 1, 0, 0, 0, 0, 0, 0},
                              {0, 0, 0, 0, 0, 1, 0, 0, 0},
                              {0, 0, 0, 0, 0, 0, 0, 0, 1},
                              {0, 0, 0, 0, 0, 0, 0, 1, 0},
                              {0, 0, 0, 0, 0, 0, 1, 0, 0},
                              {0, 0, 0, 1, 0, 0, 0, 0, 0}};

  for (i = 2; i >= 0; i--) {
    if (i == 0) {
      uint8_t plan[3] = {1,0,0};
      layerWrite(plan, 3);
    }

    if (i == 1) {
      uint8_t plan[3] = {0,1,0};
      layerWrite(plan, 3);
    }

    if (i == 2) {
      uint8_t plan[3] = {0,0,1};
      layerWrite(plan, 3);
    }

    for (j = 0; j <= 7; j++) {
      lineWrite(&(lineState[j][0]), 9);
      chThdSleepMilliseconds(tempo);
    }
  }
}

/**
 * @brief   Turn on the first face of the cube.
 *
 * @param[in] tempo   the time used to trun on the first face of the cube
 */
static void ledCubeFace1On(uint16_t tempo) {

  uint8_t plan[3] = {1,1,1}; /* Bottom, midle, top. */
  uint8_t lineState[9] = {1, 1, 1, 0, 0, 0, 0, 0, 0};

  layerWrite(plan, 3);
  lineWrite(lineState, 9);
  chThdSleepMilliseconds(tempo);
}

/**
 * @brief   Turn on the second face of the cube.
 *
 * @param[in] tempo   the time used to trun on the second face of the cube
 */
static void ledCubeFace2On(uint16_t tempo) {

  uint8_t plan[3] = {1,1,1}; /* Bottom, midle, top. */
  uint8_t lineState[9] = {1, 0, 0, 1, 0, 0, 1, 0, 0};

  layerWrite(plan, 3);
  lineWrite(lineState, 9);
  chThdSleepMilliseconds(tempo);
}

/**
 * @brief   Turn on the third face of the cube.
 *
 * @param[in] tempo   the time used to trun on the third face of the cube
 */
static void ledCubeFace3On(uint16_t tempo) {

  uint8_t plan[3] = {1,1,1}; /* Bottom, midle, top. */
  uint8_t lineState[9] = {0, 0, 0, 0, 0, 0, 1, 1, 1};

  layerWrite(plan, 3);
  lineWrite(lineState, 9);
  chThdSleepMilliseconds(tempo);
}

/**
 * @brief   Turn on the fourth face of the cube.
 *
 * @param[in] tempo   the time used to trun on the fourth face of the cube
 */
static void ledCubeFace4On(uint16_t tempo) {

  uint8_t plan[3] = {1,1,1}; /* Bottom, midle, top. */
  uint8_t lineState[9] = {0, 0, 1, 0, 0, 1, 0, 0, 1};

  layerWrite(plan, 3);
  lineWrite(lineState, 9);
  chThdSleepMilliseconds(tempo);
}

/**
 * @brief   Turn on all the face of the cube.
 *
 * @param[in] tempo   the time used to turn on all the face of the cube
 */
static void ledCubeAllFaceOn(uint16_t tempo) {

  ledCubeFace1On(tempo);
  ledCubeFace2On(tempo);
  ledCubeFace3On(tempo);
  ledCubeFace4On(tempo);
}

/**
 * @brief   Rotation demo, trun on led to make a circular effect.
 *
 * @param[in] tempo   time to turn on the led on the same layer
 */
static void rotation(uint16_t tempo) {
  uint8_t i;
  uint8_t layer[3] = {1,1,1}; /* Bottom, midle, top. */
  uint8_t lineState[4][9] = { {1, 0, 0, 0, 1, 0, 0, 0, 1},
                              {0, 0, 0, 1, 1, 1, 0, 0, 0},
                              {0, 0, 1, 0, 1, 0, 1, 0, 0},
                              {0, 1, 0, 0, 1, 0, 0, 1, 0}};

  layerWrite(layer, 3);

  for (i = 0; i <= 3; i++) {
    lineWrite(&(lineState[i][0]), 9);
    chThdSleepMilliseconds(tempo);
  }
}

/**
 * @brief   This turn on the led from bottom to top.
 *
 * @param[in] tempo   time between the the control of two leds
 * @param[in] nbr     number of time the effect will be run
 */
static void effect(uint16_t tempo, uint8_t nbr) {

  uint8_t j;

  for (j = 0; j <= nbr; j++) {
    ledCubeTopOn(tempo);
    ledCubeMidleOn(tempo);
    ledCubeBottomOn(tempo);
    ledCubeMidleOn(tempo);
  }
}

/**
 * @brief   Turn on all the led with a shadow effect.
 *
 * @param[in] tempo   the time between two led control
 */
static void shadowOn(uint16_t tempo) {

  uint8_t i;

  for (i = 3; i <= 5; i++) {
    palSetPad(IOPORT2, i);
  }

  for (i = 0; i <= 2; i++) {
    padCtrl(IOPORT2, i, 1);
    chThdSleepMilliseconds(tempo);
  }

  for (i = 2; i <= 7; i++) {
    padCtrl(IOPORT4, i, 1);
    chThdSleepMilliseconds(tempo);
  }
}

/**
 * @brief   Turn off all the led with a shadow effect.
 *
 * @param[in] tempo   the time between two led control
 */
static void shadowOff(uint16_t tempo) {

  uint8_t i;

  for (i = 3; i <= 5; i++) {
    palSetPad(IOPORT2, i);
  }

  for (i = 0; i <= 2; i++) {
    padCtrl(IOPORT2, i, 0);
    chThdSleepMilliseconds(tempo);
  }

  for (i = 2; i <= 7; i++) {
    padCtrl(IOPORT4, i, 0);
    chThdSleepMilliseconds(tempo);
  }
}

/**
 * @brief   This demo function call all the define cube functions.
 */
void ledCubeDemo(void) {

  register uint8_t i, nbr = 10;

  ledCubeOff(0);

  if (demoIndex <= 12) {
    for (i = 0; i < nbr; i++) {
      switch (demoIndex) {
        case 0:
          ledCubeOff(50);
          ledCubeOn(50);
        break;

        case 1:
          ledCubeTopOff(50);
          ledCubeTopOn(50);
        break;

        case 2:
          ledCubeMidleOn(50);
          ledCubeMidleOff(50);
        break;

        case 3:
          ledCubeBottomOn(50);
          ledCubeBottomOff(50);
        break;

        case 4:
          ledCubeFace1On(50);
          ledCubeOff(50);
        break;

        case 5:
          ledCubeFace2On(50);
          ledCubeOff(50);
        break;

        case 6:
          ledCubeFace3On(50);
          ledCubeOff(50);
        break;

        case 7:
          ledCubeFace4On(50);
          ledCubeOff(50);
        break;

        case 8:
          ledCubeAllFaceOn(100);
        break;

        case 9:
          ledCubeCircularDemo(50);
        break;

        case 10:
          rotation(100);
        break;

        case 11:
          ledCubeDemo1(50);
        break;

        case 12:
          shadowOn(80);
          shadowOff(80);
        break;
      }
    }
    demoIndex++;
  }
  else if (demoIndex == 13) {
    effect(50, 10);
    demoIndex++;
  }
  else if (demoIndex == 14) {
    ledCubeBlink(50, 5);
    demoIndex++;
  }
  else
    demoIndex = 0;
}
