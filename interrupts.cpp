//
// Created by ganymed on 06/04/16.
//

#include <Arduino.h>
#include "interrupts.h"
#include "util.h"
#include "pinMappings.h"


#define DebounceDelay 50


uint8_t countEnabledInterrupts = 0;

void (*externalInterrupt0ISR)(void) = NULL;
void (*externalInterrupt1ISR)(void) = NULL;

void (*pinChangeInterrupt0ISR)(void) = NULL;
void (*pinChangeInterrupt1ISR)(void) = NULL;
void (*pinChangeInterrupt2ISR)(void) = NULL;

bool doesPinChangeEqualInterruptMode(InterruptMode mode, uint8_t value, uint8_t pinValue);

unsigned long lastDebounceTimeExternalInterrupt0 = 0;
unsigned long lastDebounceTimeExternalInterrupt1 = 0;

unsigned long lastDebounceTimePinChangeInterrupt0 = 0;
unsigned long lastDebounceTimePinChangeInterrupt1 = 0;
unsigned long lastDebounceTimePinChangeInterrupt2 = 0;

uint8_t lastPinChangeInterruptPinValue = 1; // Pull-up -> is HIGH per default



ISR(INT0_vect) {
    fireExternalInterruptISRIfNoDebounce(externalInterrupt0ISR, &lastDebounceTimeExternalInterrupt0);
}


ISR(INT1_vect) {
    fireExternalInterruptISRIfNoDebounce(externalInterrupt1ISR, &lastDebounceTimeExternalInterrupt1);
}


void fireExternalInterruptISRIfNoDebounce(void (*isrFunction)(void), unsigned long *lastDebounceTime) {
    if(isrFunction != NULL) {
        if(hasTimeElapsed(lastDebounceTime, DebounceDelay)) {
            isrFunction();
        }
    }
}


ISR(PCINT0_vect) {
    firePinChangeISRIfNoDebounce(pinChangeInterrupt0ISR, &lastDebounceTimePinChangeInterrupt0, &PINB, PCINT0, Falling); // TODO: make generic, no hard coded values
}

ISR(PCINT1_vect) {
    firePinChangeISRIfNoDebounce(pinChangeInterrupt1ISR, &lastDebounceTimePinChangeInterrupt1, &PIN_CHANGE_INTERRUPT_PIN_REGISTER, PIN_CHANGE_INTERRUPT_PIN, Falling); // TODO: make generic, no hard coded values
}

ISR(PCINT2_vect) {
    firePinChangeISRIfNoDebounce(pinChangeInterrupt2ISR, &lastDebounceTimePinChangeInterrupt2, &PIND, PCINT16, Falling); // TODO: make generic, no hard coded values
}


void firePinChangeISRIfNoDebounce(void (*isrFunction)(void), unsigned long* lastDebounceTime, volatile uint8_t* pinRegister, uint8_t pin, InterruptMode mode) {
    if(isrFunction != NULL) {
        if (hasTimeElapsed(lastDebounceTime, DebounceDelay)) {
            uint8_t currentPinValue = *pinRegister & (1 << pin);

            if (currentPinValue != lastPinChangeInterruptPinValue) {
                if (doesPinChangeEqualInterruptMode(mode, currentPinValue, lastPinChangeInterruptPinValue)) {
                    isrFunction();
                }

                lastPinChangeInterruptPinValue = currentPinValue;
            }
        }
    }
}

bool doesPinChangeEqualInterruptMode(InterruptMode mode, uint8_t currentPinValue, uint8_t previousPinValue) {
    if(mode == Rising && currentPinValue > previousPinValue) {
        return true;
    }
    else if(mode == Falling && currentPinValue < previousPinValue) {
        return true;
    }
    else if(mode == Change && currentPinValue != previousPinValue) {
        return true;
    }
    else if(mode == LowLevel && currentPinValue == 0) {
        return true;
    }

    return false;
}


void enableExternalInterrupt0(InterruptMode mode, void (*isrFunction)(void)) {
    enableExternalInterrupt(Zero, mode, isrFunction);
}

void enableExternalInterrupt1(InterruptMode mode, void (*isrFunction)(void)) {
    enableExternalInterrupt(One, mode, isrFunction);
}

void enableExternalInterrupt(ExternalInterrupt interrupt, InterruptMode mode, void (*isrFunction)(void)) {
    if(interrupt == Zero) {
        externalInterrupt0ISR = isrFunction;
        EIMSK |= 1 << INT0; // enable External Interrupt 0
    }
    else {
        externalInterrupt1ISR = isrFunction;
        EIMSK |= 1 << INT1; // enable External Interrupt 1
    }

    setExternalInterruptMode(interrupt, mode);

    globallyEnableInterrupts();

    countEnabledInterrupts++;
}

void setExternalInterruptMode(ExternalInterrupt interrupt, InterruptMode mode) {
    uint8_t bit0 = interrupt == Zero ? ISC00 : ISC10;
    uint8_t bit1 = interrupt == Zero ? ISC01 : ISC11;

    if(mode == LowLevel || mode == Change) {
        EICRA &= ~(1 << bit0);
    }
    else {
        EICRA |= (1 << bit0);
    }

    if(mode == LowLevel || mode == Falling) {
        EICRA &= ~ (1 << bit1);
    }
    else {
        EICRA |= (1 << bit1);
    }
}


void disableExternalInterrupt0() {
    disableExternalInterrupt(Zero);
}

void disableExternalInterrupt1() {
    disableExternalInterrupt(One);
}

void disableExternalInterrupt(ExternalInterrupt interrupt) {
    if(interrupt == Zero) {
        EIMSK &= ~(1 << INT0);
    }
    else {
        EIMSK &= ~(1 << INT1);
    }

    decreaseCountEnabledInterrupts();

}


void enablePinChangeInterruptOnRegisterC(uint8_t interruptPin, void (*isrFunction)(void)) {
    enablePinChangeInterrupt(PCIE1, &PCMSK1, interruptPin, isrFunction);
    pinChangeInterrupt1ISR = isrFunction;
}

void enablePinChangeInterrupt(uint8_t pcie, volatile uint8_t* pinChangeMaskRegister, uint8_t interruptPin, void (*isrFunction)(void)) {
    PCICR |= (1 << pcie);
    *pinChangeMaskRegister |= (1 << interruptPin);

    globallyEnableInterrupts();

    countEnabledInterrupts++;
}

void disablePinChangeInterrupt(uint8_t pcie, volatile uint8_t* pinChangeMaskRegister, uint8_t interruptPin) {
    *pinChangeMaskRegister &= ~(1 << interruptPin);

    if(*pinChangeMaskRegister == 0) { // all Interrupts have been disabled for this Register
        PCICR &= ~(1 << pcie);
    }

    decreaseCountEnabledInterrupts();
}


void decreaseCountEnabledInterrupts() {
    countEnabledInterrupts--;

    if(countEnabledInterrupts == 0) {
        globallyDisableInterrupts();
    }
}

