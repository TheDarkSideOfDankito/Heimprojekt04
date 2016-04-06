//
// Created by ganymed on 30/03/16.
//

#include "util.h"
#include <USBAPI.h>



uint8_t getBitValue(volatile uint8_t* byte, uint8_t bit) {
    return *byte & (1 << bit);
}

bool isBitSet(volatile uint8_t* byte, uint8_t bit) {
    return !!(getBitValue(byte, bit));
}


void sleep_ms_10ms_steps(int ms) {
    while(ms > 0) {
        _delay_ms(10);
        ms -= 10;
    }
}

void sleep_ms_100ms_steps(int ms) {
    while(ms > 0) {
        _delay_ms(100);
        ms -= 100;
    }
}

bool hasTimeElapsed(unsigned long* lastElapsedTime, uint16_t timeToWait) {
    unsigned long currentMillis = millis();

    if((currentMillis - *lastElapsedTime) > timeToWait) {
        *lastElapsedTime = currentMillis;
        return true;
    }

    return false;
}
