#include "softi2c.h"

/*
*
* Pins and port must be defined before this file is included.
* #define I2C_PORT PORTB
* #define I2C_PIN PINB
* #define I2C_DDR DDRB
* #define I2C_SDA PB0
* #define I2C_SCL PB1
* #define I2C_CLOCK_PERIOD 1000
*
*/

//The following one liners *SHOULD* compile to sbi or cbi instructions.
//No need for inline assembly here.

static inline void _I2C_SDA_LOW(){
    I2C_DDR |= (1 << I2C_SDA);
}

static inline void _I2C_SDA_HIGH(){
    I2C_DDR &= ~(1 << I2C_SDA);
};

static inline void _I2C_SCL_LOW(){
    I2C_DDR |= (1 << I2C_SCL);
}

static inline void _I2C_SCL_HIGH(){
    I2C_DDR &= ~(1 << I2C_SCL);
};

//returns 1 or 0 depending on the state of the SDA pin
static inline uint8_t _I2C_READ_SDA(){
    return (I2C_PIN & (1 << I2C_SDA)) >> I2C_SDA;
}

//PUMP_BIT puts a bit onto the bus, starting 1/4 cycle before the rising edge of the clock
//and waiting 1/4 cycle of space (clock low) time.
static void _I2C_PUMP_BIT(uint8_t b){
    _I2C_SCL_LOW();
    if (b>0){
        _I2C_SDA_HIGH();
    }else{
        _I2C_SDA_LOW();
    }
    _delay_us(I2C_CLOCK_PERIOD>>2);
    _I2C_SCL_HIGH();
    _delay_us(I2C_CLOCK_PERIOD>>1);
    _I2C_SCL_LOW();
    _delay_us(I2C_CLOCK_PERIOD>>2);
}

static void _I2C_REP_START(){
    _I2C_SDA_HIGH();
}

//Assumes SCA/SCL HIGH
static void _I2C_START(){
    _I2C_SDA_LOW();
    _delay_us(I2C_CLOCK_PERIOD>>2);
    _I2C_SCL_LOW();
    _delay_us(I2C_CLOCK_PERIOD>>2);
}

static void _I2C_STOP(){
    _I2C_SDA_LOW();
    _delay_us(I2C_CLOCK_PERIOD>>2);
    _I2C_SCL_HIGH();
    //Wait and release SDA
    _delay_us(I2C_CLOCK_PERIOD>>2);
    _I2C_SDA_HIGH();
    _delay_us(I2C_CLOCK_PERIOD>>2);
}

//Wait for ACK after last bit transmitted.
//returns at the END of the clock pulse
static uint8_t _I2C_WAIT_ACK(){
    uint8_t sda;
    _I2C_SDA_HIGH();
    _delay_us(I2C_CLOCK_PERIOD>>2);
    _I2C_SCL_HIGH();
    _delay_us(I2C_CLOCK_PERIOD>>2);
    sda = _I2C_READ_SDA();
    _delay_us(I2C_CLOCK_PERIOD>>2);
    return sda;
}

uint8_t SoftI2C::writeBytes(uint8_t address, uint8_t bytes[], uint8_t count){
    uint8_t ack = 0;
    _I2C_START();
    //Last bit 0 indicates a write
    address = (address << 1);
    for (uint8_t i = 0; i < 8; i++){
        _I2C_PUMP_BIT((address << i) & 0x80);
    }
    //Wait for ACK
    ack = _I2C_WAIT_ACK();
    if(ack){
        //NACK. give up.
        return 1;
    }
    //Wait for the middle of the next space time.
    _I2C_SCL_LOW();
    _delay_us(I2C_CLOCK_PERIOD>>2);
    //Now we can write out the data. Most significant byte first.
    for (uint8_t byte_counter = 0; byte_counter < count; byte_counter++){
        for (uint8_t i = 0; i < 8; i++){
            _I2C_PUMP_BIT((bytes[byte_counter] << i) & 0x80);
        }
        //wait for ack
        ack = _I2C_WAIT_ACK();
        if(ack){
            //NACK. give up.
            return 1;
        }

        //Wait for the middle of the next space time.
        _I2C_SCL_LOW();
        _delay_us(I2C_CLOCK_PERIOD>>2);
            
    }

    //STOP condition. SCL should already be LOW at this point, but we will assert it anyways.
    _I2C_STOP();
    return 0;
}


uint8_t SoftI2C::readBytes(uint8_t address, uint8_t buffer[], uint8_t count){
    uint8_t ack = 0;
    uint8_t data = 0;
    _I2C_START();
    //Last bit 1 indicates a read
    address = (address << 1) | 1;
    for (uint8_t i = 0; i < 8; i++){
        _I2C_PUMP_BIT((address << i) & 0x80);
    }
    //Wait for ACK
    ack = _I2C_WAIT_ACK();
    if(ack){
        //NACK. give up.
        return 1;
    }
    //Wait for the middle of the next space time.
    _I2C_SCL_LOW();
    _delay_us(I2C_CLOCK_PERIOD>>2);
    //Pump in the data. MSB first.
    for (uint8_t byte_count = 0; byte_count < count; byte_count++){
        data = 0;
        for (uint8_t i = 0; i < 8; i++){
            _delay_us(I2C_CLOCK_PERIOD>>2);
            _I2C_SCL_HIGH();
            _delay_us(I2C_CLOCK_PERIOD>>2);
            data = (data << 1) | _I2C_READ_SDA();
            _delay_us(I2C_CLOCK_PERIOD>>2);
            _I2C_SCL_LOW();
            _delay_us(I2C_CLOCK_PERIOD>>2);
        }
        buffer[byte_count] = data;

        //Send ACK / NACK
        //Check if this is the last byte (this form is used because subi/bne compiles to two instructions)
        if (byte_count != count - 1){
            //ACK. Pull SDA low and pulse the clock. Return mid cycle.
            _I2C_SDA_LOW();
            _delay_us(I2C_CLOCK_PERIOD>>2);
            _I2C_SCL_HIGH();
            _delay_us(I2C_CLOCK_PERIOD>>1);
            _I2C_SCL_LOW();
            _delay_us(I2C_CLOCK_PERIOD>>2);
            _I2C_SDA_HIGH();
        }else{
            //NACK.
            _I2C_SDA_HIGH();
            _delay_us(I2C_CLOCK_PERIOD>>2);
            _I2C_SCL_HIGH();
            _delay_us(I2C_CLOCK_PERIOD>>1);
            _I2C_SCL_LOW();
            _delay_us(I2C_CLOCK_PERIOD>>2);
        }
    }
    //We are now done, so we can send a stop condition.
    //SCL is low at this point. Wait the remaining 1/4 period and then release SCL
     _I2C_STOP();
    return 0;
}

void SoftI2C::init(){
    //Both pins set to 0 (low, pull up disabled)
    I2C_PORT &= ~(1 << I2C_SDA);
    I2C_PORT &= ~(1 << I2C_SCL);
    _I2C_SDA_HIGH();
    _I2C_SCL_HIGH();
}

void SoftI2C::dummyTest(){
    uint8_t testBytes[] = {0x1f, 0x55, 0x01};
    uint8_t inBytes[2];
    init();
    _delay_us(500);
    writeBytes(0x1, testBytes, sizeof(testBytes));
    //readBytes(0x1, inBytes, sizeof(inBytes));
}