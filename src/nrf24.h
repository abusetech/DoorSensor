#pragma once

#include <stdint.h>
#include "spi.h"

//Register addresses
#define RF24_CONFIG 0x00
#define RF24_EN_AA 0x01
#define RF24_EN_RXADDR 0x02
#define RF24_SETUP_AW 0x03
#define RF24_SETUP_RETR 0x04
#define RF24_RF_CH 0x05
#define RF24_RF_SETUP 0x06
#define RF24_STATUS 0x07
#define RF24_OBSERVE_TX 0x08
#define RF24_RPD 0x09
#define RF24_RX_ADDR_P0 0x0a
#define RF24_RX_ADDR_P1 0x0b
#define RF24_RX_ADDR_P2 0x0c
#define RF24_RX_ADDR_P3 0x0d
#define RF24_RX_ADDR_P4 0x0e
#define RF24_RX_ADDR_P5 0x0f
#define RF24_TX_ADDR 0x10
#define RF24_RX_PW_P0 0x11
#define RF24_RX_PW_P1 0x12
#define RF24_RX_PW_P2 0x13
#define RF24_RX_PW_3 0x14
#define RF24_RX_PW_P4 0x15
#define RF24_RX_PW_P5 0x16
#define RF24_FIFO_STATUS 0x17
#define RF24_DYNPD 0x1C

//Commands
#define RF24_W_TX_PAYLOAD (0xA0)

#define RF24_EN_CRC 3
#define RF24_PRIM_RX 0


class NRF24
{
private:
    volatile uint8_t *ce_port;
    uint8_t ce_pin;
    SPI &spi;

public:
    NRF24(SPI&, volatile uint8_t * ce_port, uint8_t ce_pin);
    void writeRegister(uint8_t addr, const uint8_t *data, uint8_t size);
    void writeRegister(uint8_t addr, uint8_t data);
    void setDestAddress(const uint8_t * addr, uint8_t size);
    void setChannel(uint8_t offset_mhz);
    void writeTxPayload(const uint8_t *, uint8_t);
    void init();
    void transmit(uint8_t * data, uint8_t size);
};
