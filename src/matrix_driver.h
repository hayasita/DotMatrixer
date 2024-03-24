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

#ifdef GLOBAL_VAL_DEF
#define GLOBAL
#else
#define GLOBAL extern
#endif

#include <MD_MAX72xx.h>
#include "jsdata.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW     // 使用するデバイス種類に合わせて設定
#define MAX_DEVICES 4                         // Matrix LED Num

#define CLK_PIN   33  // or SCK
#define DATA_PIN  19  // or MOSI
#define CS_PIN    23  // or SS

#define matrixCol 16
#define matrixRow 8


class MatrixDriverMAX72XX{
  public:
    MatrixDriverMAX72XX(uint8_t col = matrixCol,uint8_t row = matrixRow);
    bool matrixset(std::vector<ledColor>);

  private:
    MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
    uint8_t _col;
    uint8_t _row;
};

#undef GLOBAL
#endif