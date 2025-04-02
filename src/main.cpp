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
void txStatus();

volatile uint16_t wdt_timer = 0;
SPI nrf_spi(&NRF_SPI_CS_PORT, NRF_SPI_CS);
SoftI2C i2c;
NRF24 nrf(nrf_spi, &NRF_SPI_CE_PORT, NRF_SPI_CE);
ADXL345 accel(i2c);

typedef struct __attribute__((packed)) status_packet {
    uint8_t sensorClass;
    uint8_t sensorID;
    uint8_t state;
    uint16_t battery_mv;
} status_packet_t;

int main (void){
    //Disable WDT, it may still be enabled after reset
    wdt_disable();
    //Wait ~100ms for periphrials to settle
    _delay_ms(100);
    nrf_spi.init();
    i2c.init();
    accel.fifoCtl(ADXL_FIFO_MODE_BYPASS, 0, 0b0011);
    nrf.setDestAddress(DEST_ADDRESS, sizeof(DEST_ADDRESS));
    nrf.setChannel(RF_CHAN);
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
        if (wdt_timer >= STATUS_TX_PERIOD){
            wdt_timer = 0;
            nrf_spi.init(); //USI needs to be re initialized after waking from sleep
            txStatus();
        }
    }
}

void txStatus(){
    status_packet_t pkt;
    pkt.sensorClass = 15;
    pkt.sensorID = 103;
    //Read accelerometer status
    pkt.state = 0;
    //Read battery status
    pkt.battery_mv = 3300;
    //transmit
    nrf.transmit(&pkt, sizeof(pkt));
}

ISR(WDT_vect){
    //Clear SE bit
    sleep_disable();
    wdt_timer += 8;
    wdt_reset();
}