#pragma once
#include <avr/io.h>
#include <stdint.h>

class SPI {  
    private:
        uint8_t ss_pin;
        volatile uint8_t * ss_port;
    public:
        SPI(volatile uint8_t * ss_port, uint8_t ss_pin);
        void begin();
        void end();
        uint8_t transfer(uint8_t b);
        uint8_t transferByte(uint8_t b);
        void transfer(uint8_t * buffer, uint8_t size);
        void write(const uint8_t * buffer, uint8_t size);
};