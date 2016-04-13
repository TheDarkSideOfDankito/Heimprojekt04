//
// Actually i wanted to make this 'class' as generically as possible, but being able to set each possible Pin
// for a Pin Change Interrupt would require storing 23 Configs, one for each Pin.
// This would use up a lot of ATMega's SDRAM, which i'd like to avoid.
// So currently it's only possible to configure one Pin of Register B, C and D each for Pin Change Interrupt.
//

#include "interrupts.h"


#define DebounceDelay 50


uint8_t countEnabledInterrupts = 0;

void (*externalInterrupt0ISR)(void) = NULL;
void (*externalInterrupt1ISR)(void) = NULL;

bool doesPinChangeEqualInterruptMode(InterruptMode mode, uint8_t value, uint8_t pinValue);

unsigned long lastDebounceTimeExternalInterrupt0 = 0;
unsigned long lastDebounceTimeExternalInterrupt1 = 0;

PinChangeInterruptConfig unsetPinChangeInterruptConfig; // cannot set a struct to NULL

PinChangeInterruptConfig registerB_PinChangInterruptConfig = unsetPinChangeInterruptConfig;
PinChangeInterruptConfig registerC_PinChangInterruptConfig = unsetPinChangeInterruptConfig;
PinChangeInterruptConfig registerD_PinChangInterruptConfig = unsetPinChangeInterruptConfig;



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
    if(&registerB_PinChangInterruptConfig != &unsetPinChangeInterruptConfig) {
        firePinChangeISRIfNoDebounce(&REGISTER_B_PIN_REGISTER, registerB_PinChangInterruptConfig);
    }
}

ISR(PCINT1_vect) {
    if(&registerC_PinChangInterruptConfig != &unsetPinChangeInterruptConfig) {
        firePinChangeISRIfNoDebounce(&REGISTER_C_PIN_REGISTER, registerC_PinChangInterruptConfig);
    }
}

ISR(PCINT2_vect) {
    if(&registerD_PinChangInterruptConfig != &unsetPinChangeInterruptConfig) {
        firePinChangeISRIfNoDebounce(&REGISTER_D_PIN_REGISTER, registerD_PinChangInterruptConfig);
    }
}


void firePinChangeISRIfNoDebounce(volatile uint8_t* pinRegister, PinChangeInterruptConfig config) {
    if (hasTimeElapsed(&config.lastDebounceTime, DebounceDelay)) {
        uint8_t currentPinValue = *pinRegister & (1 << config.pin);

        if (doesPinChangeEqualInterruptMode(config.mode, currentPinValue, config.lastPinValue)) {
            if(config.isrFunction != NULL) {
                config.isrFunction();
            }
        }

        config.lastPinValue = currentPinValue;
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
        setPinToLow(&EXTERNAL_INTERRUPT_DDR_REGISTER, EXTERNAL_INTERRUPT_0_DDR_PIN); // set INT0 as Input
    }
    else {
        externalInterrupt1ISR = isrFunction;
        EIMSK |= 1 << INT1; // enable External Interrupt 1
        setPinToLow(&EXTERNAL_INTERRUPT_DDR_REGISTER, EXTERNAL_INTERRUPT_1_DDR_PIN); // set INT1 as Input
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


void enablePinChangeInterruptOnRegisterB(uint8_t interruptPin, InterruptMode mode, void (*isrFunction)(void)) {
    enablePinChangeInterrupt(PCIE0, &PCMSK0, interruptPin, isrFunction);

    registerB_PinChangInterruptConfig = {
            interruptPin,
            mode,
            isrFunction,
            1, // Pull-up -> is HIGH per default
            0
    };
}

void enablePinChangeInterruptOnRegisterC(uint8_t interruptPin, InterruptMode mode, void (*isrFunction)(void)) {
    enablePinChangeInterrupt(PCIE1, &PCMSK1, interruptPin, isrFunction);

    registerC_PinChangInterruptConfig = {
            interruptPin,
            mode,
            isrFunction,
            1, // Pull-up -> is HIGH per default
            0
    };
}

void enablePinChangeInterruptOnRegisterD(uint8_t interruptPin, InterruptMode mode, void (*isrFunction)(void)) {
    enablePinChangeInterrupt(PCIE2, &PCMSK2, interruptPin, isrFunction);

    registerD_PinChangInterruptConfig = {
            interruptPin,
            mode,
            isrFunction,
            1, // Pull-up -> is HIGH per default
            0
    };
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

