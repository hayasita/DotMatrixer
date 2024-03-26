/**
 * @file matrix_driver.h
 * @author hayasita04@gmail.com
 * @brief Matrix Driver
 * @version 0.1
 * @date 2024-03-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef matrix_driver_h
#define matrix_driver_h

#include <FastLED.h> // enable FastLED (RGB LED) library.

#ifdef GLOBAL_VAL_DEF
#define GLOBAL
#else
#define GLOBAL extern
#endif

#include <MD_MAX72xx.h>
#include "jsdata.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW     // 使用するデバイス種類に合わせて設定
#define MAX_DEVICES 4                         // Matrix LED Num
#define MATRIX_COL 16
#define MATRIX_ROW 8

#define CLK_PIN   33  // or SCK
#define DATA_PIN  19  // or MOSI
#define CS_PIN    23  // or SS

#define NUM_LEDS 25       // Specify the number of RGB LEDs (25 for M5Atom Matrix).
#define LED_DATA_PIN 27   // Specify DATA PIN of RGB LEDs.
#define M5MATRIX_COL  5   // M5Atom Matrix Led Col
#define M5MATRIX_ROW  5   // M5Atom Matrix Led Row


class M5ATOMMatrixLED{
  public:
    M5ATOMMatrixLED(void);
    bool matrixset(ledData);
  private:
    CRGB leds[NUM_LEDS];
};

class MatrixDriverMAX72XX{
  public:
    MatrixDriverMAX72XX(uint8_t col = MATRIX_COL,uint8_t row = MATRIX_ROW);
    bool matrixset(ledData);

  private:
    MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
    uint8_t _col;
    uint8_t _row;
};

#undef GLOBAL
#endif