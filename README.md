# ELT3281: Microprocessor and Embedded System
## Description
Environmental control system in the greenhouse. A system can be developed for smart agriculture.

## List tasks
* Read data from DHT11 and show on LCD
* Show Idea temperature and humidity on LCD
* Automatic set pump's value
* Manual pump value adjustment by potentiometer

## Hardware
1. PIC16F877A
2. DHT11 sensor
3. LCD LM016L
4. Mini pump 5VDC (in simulator, it is DC motor)
5. Potentiometer
6. Button

## Structure
Main code file is ```main.c``` and Proteus simulation is on ```VXL``` folder.

## Import .hex code to PIC on Proteus
1. Double click on PIC
2. At ```Program File```, find hex file at path: ```..\dist\default\production\VXL.X.production.hex``` and choose it.
3. Click ```OK```
4. Run the simulation