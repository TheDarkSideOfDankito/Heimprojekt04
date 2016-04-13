// Failure Memory
//
// Hardware Setup is actually quite easy: there isn't any.
// On Arduino Pins 2 (PD2), 3 (PD3) and A0 (PC0) internal Pull-ups are set.
// If one of these Pins is connected to Ground, an Interrupt gets executed.
// Each of these Interrupts adds a different Failure code to Failure Memory,
// which can be seen over Serial Port.


#include "Arduino.h"
#include "interrupts.h"
#include "FailureMemory.h"
#include "util.h"
#include "pinMappings.h"
#include "pinControl.h"


#define OutputPeriodMillis 1000


unsigned long lastOutputTime = 0;


void doSetup();

void doLoop();


int main() {
    doSetup();

    while(1) {
        doLoop();
    }
}


void externalInterrupt0Triggered() {
    pushErrorCodeOntoFailureMemory(A);
}

void externalInterrupt1Triggered() {
    pushErrorCodeOntoFailureMemory(B);
}

void pinChangeInterruptTriggered() {
    pushErrorCodeOntoFailureMemory(C);
}



void initSerialCommunication() {
    init(); // for Arduino functions like Serial.println()
    Serial.begin(9600);
}

void initInterrupts() {
    enableExternalInterrupt0(Falling, externalInterrupt0Triggered);
    enableExternalInterrupt1(Falling, externalInterrupt1Triggered);

    enablePinChangeInterruptOnRegisterC(PIN_CHANGE_INTERRUPT_PIN, Falling, pinChangeInterruptTriggered);

    // turn internal Pull-ups on
    setPinToHigh(&EXTERNAL_INTERRUPT_PORT_REGISTER, EXTERNAL_INTERRUPT_0_PORT_PIN);
    setPinToHigh(&EXTERNAL_INTERRUPT_PORT_REGISTER, EXTERNAL_INTERRUPT_1_PORT_PIN);
    setPinToHigh(&REGISTER_C_PORT_REGISTER, PIN_CHANGE_INTERRUPT_1_PORT_PIN);
}

void doSetup() {
    initSerialCommunication();

    initFailureMemory();
    initInterrupts();
}


void doLoop() {
    if(hasTimeElapsed(&lastOutputTime, OutputPeriodMillis)) {
        printFailureMemory();
    }
}