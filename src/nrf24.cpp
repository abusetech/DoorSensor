#include "nrf24.h"

NRF24::NRF24(SPI& _spi, volatile uint8_t * port, uint8_t pin) : spi(_spi){
 
    ce_port = port;
    ce_pin = pin;
}

//Should be LSByte to MSByte, so this reverse the data inline?
void NRF24::writeRegister(uint8_t addr, const uint8_t * data, uint8_t size){
    uint8_t wcmd[size+1];
    wcmd[0] = addr | (1 << 5);
    for (int i = 0; i < size; i++){
        wcmd[i + 1] = data[size - 1 - i];
    }
    spi.transfer(wcmd, size+1);
}

void NRF24::writeRegister(uint8_t addr, uint8_t data){
    NRF24::writeRegister(addr, &data, 1);
}

void NRF24::init(){
    //CRC ON
    uint8_t config = (1<<RF24_EN_CRC);
    //1MBps, 0dBm
    uint8_t rf_config = (0x3 << 1);
    spi.begin();
    writeRegister(RF24_CONFIG, config);
    spi.end();
    spi.begin();
    writeRegister(RF24_RF_SETUP, rf_config);
    spi.end();
}

void NRF24::setDestAddress(const uint8_t * addr, uint8_t size){
    spi.transfer(RF24_W_TX_PAYLOAD);
    spi.write(addr, size);
}

/// @brief Sets the channel to transmit or receive on
/// @param off_mhz frequency offset from 2400MHz in 1MHz increments. 2400-2525MHz.
void NRF24::setChannel(uint8_t off_mhz){
    writeRegister(RF24_RF_CH, off_mhz);
}

void NRF24::writeTxPayload(const uint8_t * payload, uint8_t size){
    spi.transfer(RF24_W_TX_PAYLOAD);
    spi.write(payload, size);
}

void NRF24::transmit(uint8_t * data, uint8_t size){
    spi.begin();
    writeRegister(RF24_CONFIG, 1<<RF24_EN_CRC);
    writeTxPayload(data, size);
}
