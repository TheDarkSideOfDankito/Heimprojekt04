//
// Created by ganymed on 30/03/16.
//

#include "shiftRegisterControl.h"
#include "pinMappings.h"
#include "pinControl.h"
#include "util.h"


void setupShiftRegister() {
    setPinAsOutput(&SHIFT_REGISTER_DATA_OUT_DDR, SHIFT_REGISTER_DATA_OUT_DDR_PIN);
    setPinAsOutput(&SHIFT_REGISTER_SHIFT_CLOCK_DDR, SHIFT_REGISTER_SHIFT_CLOCK_DDR_PIN);
    setPinAsOutput(&SHIFT_REGISTER_STORAGE_CLOCK_DDR, SHIFT_REGISTER_STORAGE_CLOCK_DDR_PIN);
}


void shiftBit() {
    setPinToLow(&SHIFT_REGISTER_SHIFT_CLOCK_PORT, SHIFT_REGISTER_SHIFT_CLOCK_PIN);
    setPinToHigh(&SHIFT_REGISTER_SHIFT_CLOCK_PORT, SHIFT_REGISTER_SHIFT_CLOCK_PIN);
}

void moveDataFromShiftRegisterToStorageRegister() {
    setPinToLow(&SHIFT_REGISTER_STORAGE_CLOCK_PORT, SHIFT_REGISTER_STORAGE_CLOCK_PIN);
    setPinToHigh(&SHIFT_REGISTER_STORAGE_CLOCK_PORT, SHIFT_REGISTER_STORAGE_CLOCK_PIN);
}

void writeByteToRegister(uint8_t byte) {
    for(int i = 7; i >= 0; i--) {
        uint8_t bitValue = isBitSet(&byte, i);
        setPinToValue(&SHIFT_REGISTER_DATA_OUT_PORT, SHIFT_REGISTER_DATA_OUT_PIN, bitValue);

        shiftBit();
    }

    moveDataFromShiftRegisterToStorageRegister();
}
