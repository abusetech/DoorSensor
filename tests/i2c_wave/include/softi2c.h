#pragma once
#include "common.h"

#define I2C_FAKE_ACK 1

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
        uint8_t writeBytes(uint8_t address, uint8_t bytes[], uint8_t count);
        void dummyTest();
};