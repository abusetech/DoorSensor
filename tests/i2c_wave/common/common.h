#pragma once
#include <stdint.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>

#define I2C_PORT (get_port_info_ptr()->port)
#define I2C_PIN (get_port_info_ptr()->pin)
#define I2C_DDR (get_port_info_ptr()->ddr)
#define PB0 0
#define PB1 1
#define I2C_SDA PB0
#define I2C_SCL PB1
#define I2C_CLOCK_PERIOD 1000

typedef struct _pi {
    uint8_t port;
    uint8_t pin; 
    uint8_t ddr;
    double time;
} PortInfo;

typedef struct _dp
{
    double time;
    uint8_t port;
    uint8_t pin;
    uint8_t ddr;
    uint8_t sda;
    uint8_t scl;
} DataPoint;


DataPoint collectData();
void makeDataPoint();
void _delay_us(double us);
PortInfo * get_port_info_ptr();
void prettyPrintData();
