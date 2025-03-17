#pragma once
//#define DEBUG

#define F_CPU 4000000UL
#define I2C_PORT PORTB
#define I2C_PIN PINB
#define I2C_DDR DDRB
#define I2C_SDA PB0
#define I2C_SCL PB1
#define I2C_CLOCK_PERIOD 100


#ifdef DEBUG
    //#include <avr/avr_mcu_section.h>
    //AVR_MCU(F_CPU, "attiny44");
#endif

