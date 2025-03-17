/*
 * This file contains mocks for registers on the MCU as well as mocks for
 * avrlibc functions, which allows us to emulate the function of the
 * software i2c implementation and produce waveforms for debuggging.
 *
 * It is needed because this version of avr-gcc breaks the simavr headers.
 *
 */

#include <stdint.h>
#include <vector>
#include "include/common.h"
#include "include/softi2c.h"
#include <cstdio>

int main(int argc, char * argv[]){
    SoftI2C i2c;
    i2c.dummyTest();
    prettyPrintData();
}