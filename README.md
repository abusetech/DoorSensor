# DoorSensor

This is a simple sensor that uses an accelerometer and an NRF24l01+ wireless transceiver to monitor the state of a door thumbturn (The small knob that moves the deadbolt). The sensor physically attaches to the deadbolt and uses the accelerometer to deterimine its orientation and therefore the state of the deadbolt.

It is designed to have very low power comsumption to allow the batteries to last a long time.

### Parts

* ATTINY44/84
* ADXL345
* NRF24L01(+) module
* 2x AAA battery holder

You will also need an Arduino board and a second NRF24L01+ to build the receiver.

### Operation

The Arduino runs firmware that allows it to connect to HomeAssistant. It acts as a bridge between the HomeAssistant and the wireless sensor.

### Building

Make a build directory:

`$ mkdir build`

Run `make`

`$ make all`

### Debugging

This project can be run in [simavr](https://github.com/buserror/simavr). To do so, build the binaries and then execute the ELF inside simavr:

`$ simavr -g build/attiny44.elf`

Then connect to simavr using gdb:

`avr-gdb -ex "symbol-file build/attiny44.elf" -ex "target remote localhost:1234"`