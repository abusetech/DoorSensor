#include "adxl345.h"

ADXL345::ADXL345(SoftI2C& _i2c) : i2c(_i2c){
    
};

adxl_data_t ADXL345::readFifo(){
    //AVR is little endian
    //The data format of the 16 bit registers from the ADXL is big endian
    //ie. DATAx0 is the least significant byte.
    //Bytes are read out in order DATAX0, DATAX1, DATAY0, ..., DATAZ1
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
    uint8_t outBytes[2];
    outBytes[0] = ADXL_FIFO_CTL;
    outBytes[1] = ((fifoMode & 1) << 6) | ((trigger & 1) << 5) | (samples & 0x1F);
    i2c.writeBytes(ADXL_I2C_ADDR, outBytes, 2);
}

void ADXL345::powerCtl(uint8_t flags, uint8_t wakeup){
    uint8_t outBytes[2];
    outBytes[0] = ADXL_POWER_CTL;
    outBytes[1] = (flags & 0b00111100) | (wakeup & 0x03);
    i2c.writeBytes(ADXL_I2C_ADDR, outBytes, 2);
}

void ADXL345::bwRate(uint8_t low_power, uint8_t rate){
    uint8_t outBytes[2];
    outBytes[0] = ADXL_BW_RATE;
    outBytes[1] = ((low_power & 1) << 4) | (rate & 0x0F);
    i2c.writeBytes(ADXL_I2C_ADDR, outBytes, 2);
}