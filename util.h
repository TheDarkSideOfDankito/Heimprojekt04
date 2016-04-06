//
// Created by ganymed on 30/03/16.
//

#ifndef UTIL_H
#define UTIL_H


#include <stdint.h>
#include <stdbool.h>


uint8_t getBitValue(volatile uint8_t* byte, uint8_t bit);

bool isBitSet(volatile uint8_t *byte, uint8_t bit);

void sleep_ms_10ms_steps(int ms);
void sleep_ms_100ms_steps(int ms);

bool hasTimeElapsed(unsigned long* lastElapsedTime, uint16_t timeToWait);


#endif //HEIMPROJEKT02_UTIL_H
