#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
//Common.h defines F_CPU, so it must be included before delay.h
#include "common.h"
#include <util/delay.h>
#include "spi.h"
#include "softi2c.h"
#include "nrf24.h"
#include "adxl345.h"
#include "battery.h"

#define NRF_SPI_CS_PORT PORTA
#define NRF_SPI_CE_PORT PORTA
#define NRF_SPI_CS PA1
#define NRF_SPI_CE PA3
//Resolution of STATUS_TX_PERIOD is the same as the WDT timeout setting.
#define STATUS_TX_PERIOD 30

//f = 2400 + RF_CH MHz
#define RF_CHAN (25)
const uint8_t DEST_ADDRESS[5] = {0x7E, 0x7E, 0x7E, 0x7E, 0x7E};

int main (void);
void txStatus(ADXL345&);

volatile uint16_t wdt_timer = 0;
SPI nrf_spi(&NRF_SPI_CS_PORT, NRF_SPI_CS);
SoftI2C i2c;
NRF24 nrf(nrf_spi, &NRF_SPI_CE_PORT, NRF_SPI_CE);
ADXL345 accel(i2c);

//On AVR platforms, this packet will be little-endian 2's complement format
typedef struct __attribute__((packed)) status_packet {
    uint8_t sensorClass;
    uint8_t sensorID;
    uint8_t locked;
    uint16_t battery_mv;
} status_packet_t;

int main (void){
    nrf_spi.init();
    i2c.init();
    //Disable WDT, it may still be enabled after reset
    wdt_disable();
    //Wait ~100ms for periphrials to settle
    _delay_ms(100);
    //Bypass the FIFO, old values overwritten as they come in. 
    accel.fifoCtl(ADXL_FIFO_MODE_BYPASS, 0, 0);
    //Select the measurement rate. 0b0011 = 0.78Hz, 23uA consumption.
    //TODO: Power consumption could be further reduced to ~0.1uA by using standby mode (clearing the measure bit).
    accel.bwRate(0, 0b0011);
    //Enable measurement mode.
    accel.powerCtl(ADXL_POWER_MEASURE, 0);
    //Configure the NRF24L01+ module
    nrf.setDestAddress(DEST_ADDRESS, sizeof(DEST_ADDRESS));
    nrf.setChannel(RF_CHAN);
    //Enable interrupts
    sei();
    //Configure WDT. We need this to bring the chip out of standby.
    wdt_enable(WDTO_8S);
    //WDT triggers an interrupt.
    WDTCSR |= (1 << WDIE);
    //Set the sleep mode
    set_sleep_mode(SLEEP_MODE_STANDBY);
    
    
    while(true){
        //Sleep. Zzzzz.
        sleep_enable();
        sleep_cpu();
        //Clear SE bit
        sleep_disable();
        if (wdt_timer >= STATUS_TX_PERIOD){
            wdt_timer = 0;
            BatteryStatus::init();
            nrf_spi.init(); //USI needs to be re initialized after waking from sleep
            txStatus(accel);
            BatteryStatus::shutdown();
        }
    }
}

void txStatus(ADXL345& accel){
    status_packet_t pkt;
    pkt.sensorClass = 15;
    pkt.sensorID = 103;
    //Read accelerometer status
    adxl_data_t accelData = accel.readFifo();
    //in +/- 2g mode, the axis scaling factor is 4mg / LSB.
    //therefore, we expect a read of ~250 for any axis pointed straight up.
    //if we take any angle > 30 degrees from vertical to be 'open'
    //then 250 * sqrt(3)/2 = 250 * 0.866 = 216.5
    pkt.locked = 1;
    if (accelData.datay < 216){
        pkt.locked = 0;
    }
    pkt.battery_mv = BatteryStatus::measure_mv();
    //transmit
    nrf.transmit(&pkt, sizeof(pkt));
}

ISR(WDT_vect){
    //Clear SE bit
    sleep_disable();
    wdt_timer += 8;
    //Next WDT triggers an interrupt.
    WDTCSR |= (1 << WDIE);
}