#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h> 
#include <stdint.h>
#include "spi.h"
#include "nrf24.h"

#define NRF_SPI_CS_PORT PORTA
#define NRF_SPI_CE_PORT PORTA
#define NRF_SPI_CS PA1
#define NRF_SPI_CE PA3

//f = 2400 + RF_CH MHz
#define RF_CHAN (25)
const uint8_t DEST_ADDRESS[5] PROGMEM = {0x7E, 0x7E, 0x7E, 0x7E, 0x7E};

int main (void);

int main (void){
    SPI nrf_spi(&NRF_SPI_CS_PORT, NRF_SPI_CS);
    NRF24 nrf(nrf_spi, &NRF_SPI_CE_PORT, NRF_SPI_CE);
    //DEST_ADDRESS is stored in program memory to save SRAM, we copy it to a buffer
    //Temporarily to set it in the NRF library. It is enclosed in a closure so that it
    //falls out of scope immediately.
    {
        uint8_t addr[sizeof(DEST_ADDRESS)];
        for (uint8_t i = 0; i < sizeof(DEST_ADDRESS); i++){
            addr[i] = pgm_read_byte(&(DEST_ADDRESS[i]));
        }
        nrf.setDestAddress(addr, sizeof(DEST_ADDRESS));
    }
    nrf.setChannel(RF_CHAN);
    
}