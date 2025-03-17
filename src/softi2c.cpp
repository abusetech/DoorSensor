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

static inline uint8_t _I2C_READ_SDA(){
    return (I2C_PIN & (1 << I2C_SDA)) >> I2C_SDA;
}

static inline void _I2C_PUMP_BIT(uint8_t b){
    _I2C_SCL_LOW();
    if (b){
        _I2C_SDA_HIGH();
    }else{
        _I2C_SDA_LOW();
    }
    //Clock low period
    _delay_us(I2C_CLOCK_PERIOD>>1);
    //Clock high period
    _I2C_SCL_HIGH();
    //A I2C_CLOCK_PERIOD/2 microsecond delay should follow for correct timing:
    _delay_us(I2C_CLOCK_PERIOD>>1);
}

//returns 0 if the command was ACKnowledged, non-zero otherwise
static uint8_t _I2C_SEND_BYTE(uint8_t b){
    //Shift bits out, MSB first
    for(uint8_t i = 0; i < 8; i++){
        _I2C_PUMP_BIT((b << i) & 0x80);
    }
    //Now wait for ACK.
    //Release SDA and bring the SCL low.
    _I2C_SDA_HIGH();
    _I2C_SCL_LOW();
    //Wait for 1/2 cycle
    _delay_us(I2C_CLOCK_PERIOD>>1);
    //Bring the clock high again
    _I2C_SCL_HIGH();
    //Wait 1/4 cycle to let the receiver settle
    _delay_us(I2C_CLOCK_PERIOD>>2);
    //Read the ACK bit from SDA
    b = _I2C_READ_SDA();
    //Wait the remaining cycle time
    _delay_us(I2C_CLOCK_PERIOD>>2);
    //Return the clock to the LOW state
    _I2C_SCL_LOW();
    return b;
}

//Assumes SCA/SCL HIGH
static void _I2C_START(){
    _I2C_SDA_LOW();
    _delay_us(I2C_CLOCK_PERIOD>>2);
    _I2C_SCL_LOW();
    _delay_us((I2C_CLOCK_PERIOD>>1) + (I2C_CLOCK_PERIOD>>2));
}

static void _I2C_STOP(){
    _I2C_SCL_HIGH();
    _delay_us(I2C_CLOCK_PERIOD>>2);
    _I2C_SDA_HIGH();
    _delay_us((I2C_CLOCK_PERIOD>>1) + (I2C_CLOCK_PERIOD>>2));
}


void SoftI2C::init(){
    //Both pins set to 0 (low, pull up disabled)
    I2C_PORT &= ~(1 << I2C_SDA);
    I2C_PORT &= ~(1 << I2C_SCL);
    _I2C_SDA_HIGH();
    _I2C_SCL_HIGH();
}

void SoftI2C::dummyTest(){
    init();
    _delay_us(0);
    _I2C_START();
    _I2C_SEND_BYTE(0x73);
    _I2C_STOP();
    _delay_us(0);
}