//
// Created by ganymed on 06/04/16.
//

#ifndef INTERRUPTS_H
#define INTERRUPTS_H


#include <avr/interrupt.h>
#include <Arduino.h>
#include "util.h"
#include "pinMappings.h"
#include "pinControl.h"


#define globallyEnableInterrupts() sei()
#define globallyDisableInterrupts() cli()


typedef enum InterruptMode {
    LowLevel,
    Change,
    Rising,
    Falling
} InterruptMode;


typedef enum ExternalInterrupt {
    Zero,
    One
} ExternalInterrupt;



typedef struct PinChangeInterruptConfig {
    uint8_t pin;
    InterruptMode mode;
    void (*isrFunction)(void);
    uint8_t lastPinValue;
    unsigned long lastDebounceTime;
} PinChangeInterruptConfig;



void enableExternalInterrupt0(InterruptMode mode, void (*)(void));
void enableExternalInterrupt1(InterruptMode mode, void (*)(void));
// private function
void enableExternalInterrupt(ExternalInterrupt interrupt, InterruptMode mode, void (*isrFunction)(void));
void setExternalInterruptMode(ExternalInterrupt interrupt, InterruptMode mode);

void disableExternalInterrupt0();
void disableExternalInterrupt1();
// private functions
void disableExternalInterrupt(ExternalInterrupt interrupt);

void enablePinChangeInterruptOnRegisterB(uint8_t interruptPin, InterruptMode mode, void (*isrFunction)(void));
void enablePinChangeInterruptOnRegisterC(uint8_t interruptPin, InterruptMode mode, void (*isrFunction)(void));
void enablePinChangeInterruptOnRegisterD(uint8_t interruptPin, InterruptMode mode, void (*isrFunction)(void));
// private function
void enablePinChangeInterrupt(uint8_t pcie, volatile uint8_t* pinChangeMaskRegister, uint8_t interruptPin, void (*isrFunction)(void));


// private functions
void fireExternalInterruptISRIfNoDebounce(void (*isrFunction)(void), unsigned long *lastDebounceTime);
void firePinChangeISRIfNoDebounce(volatile uint8_t* pinRegister, PinChangeInterruptConfig config);

void decreaseCountEnabledInterrupts();


#endif //INTERRUPTS_H
