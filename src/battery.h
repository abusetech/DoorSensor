#pragma once
#include "common.h"
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#define _BATTERY_PRESCALE (0b101)

class BatteryStatus{
    public:
        static void init();
        static uint16_t measure_mv();
        static void shutdown();
};