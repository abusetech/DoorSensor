#pragma once
#include "common.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>

/*
static inline void _I2C_SDA_LOW();
static inline void _I2C_SDA_HIGH();
static inline void _I2C_SCL_LOW();
static inline void _I2C_SCL_HIGH();
static inline void _I2C_PUMP_BIT(uint8_t b);
*/

class SoftI2C{
    public:
        SoftI2C(){};
        void init();
        void dummyTest();
};