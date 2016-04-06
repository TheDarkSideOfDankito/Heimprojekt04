//
// Created by ganymed on 30/03/16.
//

#ifndef PINCONTROL_H
#define PINCONTROL_H


#include <stdint.h>
#include <stdbool.h>


void setPinAsOutput(volatile uint8_t* ddr, uint8_t pin);
void setPinAsInput(volatile uint8_t* ddr, uint8_t pin);

void setPinToHigh(volatile uint8_t* port, uint8_t pin);
void setPinToLow(volatile uint8_t* port, uint8_t pin);
void setPinToValue(volatile uint8_t* port, uint8_t pin, uint8_t value);

bool isPinSet(volatile uint8_t *byte, uint8_t bit);


#endif //HEIMPROJEKT02_PINCONTROL_H
