#pragma once
#include "common.h"
#include "softi2c.h"
#include <avr/io.h>
#include <stdint.h>

#define ADXL_BW_RATE        (0x2C)
#define ADXL_POWER_CTL      (0x2D)
#define ADXL_DATA_FORMAT    (0x31)
#define ADXL_DATAX0         (0x32)
#define ADXL_FIFO_CTL       (0x38)

//FIFO_CTL bits

#define ADXL_FIFO_MODE_BYPASS  0
#define ADXL_FIFO_MODE_FIFO    1
#define ADXL_FIFO_MODE_STREAM  2
#define ADXL_FIFO_MODE_TIRGGER 3

typedef struct {
    int16_t datax;
    int16_t datay;
    int16_t dataz;
} adxl_data_t;

class ADXL345{
    private:
        SoftI2C& i2c;
    public:
        ADXL345(SoftI2C& i2c);
        adxl_data_t readFifo();
        void fifoCtl(uint8_t fifoMode, uint8_t trigger, uint8_t samples);
};