#include "adxl345.h"

ADXL345::ADXL345(volatile uint8_t * _cs_port, uint8_t _cs_pin) : cs_port(_cs_port), cs_pin(_cs_pin){
    
};