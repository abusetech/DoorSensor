#include "adxl345.h"

ADXL345::ADXL345(SoftI2C& _i2c) : i2c(_i2c){
    
};

adxl_data_t ADXL345::readFifo(){
    //AVR is little endian
    //The data format of the 16 bit registers from the ADXL is big endian
    //ie. DATAx0 is the least significant byte.
    adxl_data_t out;
    uint8_t buffer[6];
    i2c.readBytes(ADXL_DATAX0, buffer, sizeof(buffer));
    out.datax = buffer[0];
    out.datay = buffer[2];
    out.dataz = buffer[4];
    out.datax |= ((uint16_t)buffer[1]) << 8;
    out.datay |= ((uint16_t)buffer[3]) << 8;
    out.dataz |= ((uint16_t)buffer[5]) << 8;
    return out;
}

//Sets the FIFO_CTL register.
void ADXL345::fifoCtl(uint8_t fifoMode, uint8_t trigger, uint8_t samples){
    uint8_t reg = (fifoMode << 6) | (trigger << 5) | (samples & 0x1F);
    i2c.writeBytes(ADXL_FIFO_CTL, &reg, 1);
}