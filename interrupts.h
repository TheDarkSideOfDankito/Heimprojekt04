//
// Created by ganymed on 06/04/16.
//

#ifndef INTERRUPTS_H
#define INTERRUPTS_H


#include <avr/interrupt.h>


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


void enableExternalInterrupt0(InterruptMode mode, void (*)(void));
void enableExternalInterrupt1(InterruptMode mode, void (*)(void));
// private function
void enableExternalInterrupt(ExternalInterrupt interrupt, InterruptMode mode, void (*isrFunction)(void));
void setExternalInterruptMode(ExternalInterrupt interrupt, InterruptMode mode);

void disableExternalInterrupt0();
void disableExternalInterrupt1();
// private functions
void disableExternalInterrupt(ExternalInterrupt interrupt);

void enablePinChangeInterruptOnRegisterC(uint8_t interruptPin, void (*isrFunction)(void));
// private function
void enablePinChangeInterrupt(uint8_t pcie, volatile uint8_t* pinChangeMaskRegister, uint8_t interruptPin, void (*isrFunction)(void));


// private functions
void fireExternalInterruptISRIfNoDebounce(void (*isrFunction)(void), unsigned long *lastDebounceTime);
void firePinChangeISRIfNoDebounce(void (*isrFunction)(void), unsigned long* lastDebounceTime, volatile uint8_t* pinRegister, uint8_t pin, InterruptMode mode);

void decreaseCountEnabledInterrupts();


#endif //INTERRUPTS_H
