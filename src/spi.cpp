#include "spi.hpp"

SPI::SPI(volatile uint8_t * _ss_port, uint8_t _ss_pin){
    ss_pin = _ss_pin;
    ss_port = _ss_port;
    //Initialize the port.    //SCK (PA4)
    DDRA |= 1 << PA4;
    *ss_port &= ~(1 << PA4);
    //DO (PA5)
    DDRA |= 1 << PA5;
    *ss_port &= ~(1 << PA5);
    //SS (CS) [Active low starts transfer]
    DDRA |= (1 << ss_pin);
    *ss_port |= (1 << ss_pin);
    //DI (PA6)
    DDRA &= ~(1 << PA6);
    //Configure SPI mode
    USICR = (1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
}   

void SPI::begin(){
    //Start a transfer by asserting CS low
    *ss_port &= ~(1 << ss_pin);
}

void SPI::end(){
    //Terminate a transfer by asserting CS high
    *ss_port |= 1 << ss_pin;
}

//Based on the example code provided in the ATTINY44A datasheet
uint8_t SPI::transferByte(uint8_t b){
    //Put contents of b into the data register
    USIDR = b;
    //Clear overflow bit
    USISR = (1 << USIOIF);
    //Spin until overflow bit is set
    while (USISR & USIOIF == 0){
        //Original has USICR = (1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC) here
    }
    //Get the byte we received form the data register
    b = USIDR;
    return b;
}

uint8_t SPI::transfer(uint8_t b){
    return SPI::transferByte(b);
}

/// @brief Writes buf to the SPI bus, while copying data read from the bus back to buf
/// @param buf pointer to an array of bytes (or compatible)
/// @param count number of bytes to transfer
void SPI::transfer(uint8_t * buf, uint8_t count){
    uint8_t temp = 0;
    for (int i = 0; i < count; i++){
        temp = transferByte(buf[i]);
        buf[i] = temp;
    }
}

/// @brief Writes `buffer` to the SPI bus
/// @param buffer pointer to an array of bytes (or compatible)
/// @param size number of bytes to transfer
void SPI::write(const uint8_t * buffer, uint8_t size){
    for (int i = 0; i < size; i++){
        transferByte(buffer[i]);
    }   
}