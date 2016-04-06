#include "Arduino.h"
#include "interrupts.h"
#include "FailureMemory.h"
#include "util.h"
#include "pinMappings.h"


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

    enablePinChangeInterruptOnRegisterC(PIN_CHANGE_INTERRUPT_PIN, pinChangeInterruptTriggered);
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