# DoorSensor

This is a simple sensor that uses an accelerometer and an NRF24l01+ wireless transceiver to monitor the state of a door thumbturn (The small knob that moves the deadbolt). The sensor physically attaches to the deadbold and uses the accelerometer to deterimine its orientation and therefore the state of the deadbolt.

It is designed to have very low power comsumption to allow the batteries to last a long time.

### Parts

* ATTINY44/84
* BMA180
* NRF24L01(+) module
* 2x AAA battery holder

You will also need an Arduino board, a second NRF24L01+, and some means of connecting the two devices.

### Operation

The Arduino runs firmware that allows it to connect to HomeAssistant. It acts as a bridge between the HomeAssistant and the wireless sensor.