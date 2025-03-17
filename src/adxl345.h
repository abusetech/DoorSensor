#pragma once
#include "common.h"
#include "spi.h"
#include "softi2c.h"
#include <avr/io.h>
#include <stdint.h>

class ADXL345{
    private:
        volatile uint8_t * cs_port;
        uint8_t cs_pin;
    public:
        ADXL345(volatile uint8_t * _cs_port, uint8_t _cs_pin);

};