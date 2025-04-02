#include "common.h"

//uint8_t PORTB, PINB, DDRB = 0;
//double time_us = 0.0;

static PortInfo * _pi = NULL;
static std::vector<DataPoint> * dataPoints = NULL;

DataPoint collectData(){
    DataPoint dp;
    dp.time = get_port_info_ptr()->time;
    dp.port = get_port_info_ptr()->port;
    dp.pin = get_port_info_ptr()->pin;
    dp.ddr = get_port_info_ptr()->ddr;
    //if PINx is set to one, this is an output and SDA *should* be low, unless PORTx is set to 1 (should never be the case)
    dp.sda = ( (~(dp.ddr >> I2C_SDA)) & 1 );
    dp.scl = ( (~(dp.ddr >> I2C_SCL)) & 1 );
    return dp;
}

void makeDataPoint(){
    DataPoint dp = collectData();
    if (dataPoints == NULL){
        dataPoints = new std::vector<DataPoint>;
    }
    dataPoints->push_back(dp);
}

void prettyPrintData(){
    printf("Time (us), PORTx, PINx, DDRx, SDA, SCL\n");
    for (int i = 0; i < dataPoints->size(); i++){
        DataPoint dp = (*dataPoints)[i];
        printf("%f,%u,%u,%u,%u,%u\n", dp.time, dp.port, dp.pin, dp.ddr, dp.sda, dp.scl);
    }
}

// Every time this function is called, store a datapoint.
void _delay_us(double us)
{
    _pi->time++;
    makeDataPoint();
    _pi->time += us - 2;
    makeDataPoint();
    _pi->time++;
}

PortInfo * get_port_info_ptr(){
    if (_pi == NULL){
        _pi = (PortInfo*)malloc(sizeof(_pi));
        memset(_pi, 0, sizeof(_pi));
    }
    return _pi;
}