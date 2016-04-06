//
// Created by ganymed on 30/03/16.
//

#include "pinControl.h"
#include "util.h"


void setPinAsOutput(volatile uint8_t* ddr, uint8_t pin) {
    *ddr |= (1 << pin);
}

void setPinAsInput(volatile uint8_t* ddr, uint8_t pin) {
    *ddr &= ~(1 << pin);
}


void setPinToHigh(volatile uint8_t* port, uint8_t pin) {
    *port |= (1 << pin);
}

void setPinToLow(volatile uint8_t* port, uint8_t pin) {
    *port &= ~(1 << pin);
}

void setPinToValue(volatile uint8_t* port, uint8_t pin, uint8_t value) {
    if(value == 0) {
        setPinToLow(port, pin);
    }
    else {
        setPinToHigh(port, pin);
    }
}


bool isPinSet(volatile uint8_t* port, uint8_t pin) {
    return isBitSet(port, pin);
}
