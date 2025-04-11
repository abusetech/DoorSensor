#include "battery.h"

void BatteryStatus::init(){
    //Clear ADC Power Reduction Bit.
    PRR &= ~(1<<PRADC);
    //Configure the input mode:
    //Single ended mode. no left adjustment. No auto triggering.
    ADCSRB = 0;
    //Configure the ADC clock prescaler:
    //A clock between 50 and 200kHz should be selected.
    //A normal conversion takes 13 clocks. The first conversion after ADEN is set takes 25.
    //ADSP[2:0] = 0b110 will give a prescaler factor of 64 ~= 125kHz at 4MHz system clock
    ADCSRA |= _BATTERY_PRESCALE;
    //At 125 kHz, the first conversion takes ~200us
}

uint16_t BatteryStatus::measure_mv(){
    uint16_t raw = 0;
    //We will use VCC as the reference and measure the internal 1.1v bandgap reference
    //Select the voltage reference by clearing REFS[1:0] in ADMUX and selecting the 
    //internal 1.1v reference as the single ended input. 
    //See p. 150 of the ATTINY44A datasheet.
    ADMUX = 0b100001;
    //Set the ADEN bit to start the clock. Write the interrupt flag to 1 to clear it.
    ADCSRA = (1 << ADEN) | (1 << ADIF) | _BATTERY_PRESCALE;
    //Loop until interrupt flag is set
    while ((ADCSRA & (1<<ADIF)) == 0){}
    ADCSRA &= ~(1 << ADEN);
    raw = (ADCH & 3);
    raw = raw << 8;
    raw |= ADCL;
    // ADC = (1.1/Vcc) * 1024
    // Therefore: Vcc = 1126/ADC
    // Or in millivolts: Vcc = 1126000/ADC
    return 1126000 / raw;
}

void BatteryStatus::shutdown(){
    //Stop the clocks
    ADCSRA &= ~(1 << ADEN);
    //Disable the ADC
    PRR |= (1<<PRADC);
}