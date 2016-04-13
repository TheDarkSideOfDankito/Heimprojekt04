//
// Created by ganymed on 06/04/16.
//

#include "FailureMemory.h"


FailureCode failureMemory[FailureMemoryLength];

FailureCode lastFailureCode = NoFailure;

uint8_t countFailures = 0;


void initFailureMemory() {
    for(uint8_t i; i < FailureMemoryLength; i++) {
        failureMemory[i] = NoFailure; // initialize failureMemory Array with default values (saying no failure has occurred)
    }

    setup7Segment();
}


void pushFailureCodeOntoFailureMemory(FailureCode failureCode) {
    if(failureCode == lastFailureCode) { // do not push the same failure code on failure memory two times in a row
        return;
    }

    lastFailureCode = failureCode;

    for(uint8_t i = FailureMemoryLength - 1; i > 0; i--){
        failureMemory[i] = failureMemory[i - 1];
    }

    failureMemory[0] = failureCode;

    countFailures++;
}


void displayFailureMemory() {
    Serial.print("Failure Memory contains ");
    Serial.print(countFailures);
    Serial.println(" failure(s):");

    uint8_t countStoredFailures = (countFailures > FailureMemoryLength ? countFailures : FailureMemoryLength);
    for(int i = 0; i < countStoredFailures; i++) {
        printFailure(failureMemory[i]);
    }

    if(lastFailureCode != NoFailure) {
        printFailureOn7SegmentDisplay(lastFailureCode);
    }
}

void printFailure(FailureCode failureCode) {
    if(failureCode == A) {
        Serial.println("A");
    }
    else if(failureCode == B) {
        Serial.println("B");
    }
    else if(failureCode == C) {
        Serial.println("C");
    }
}

void printFailureOn7SegmentDisplay(FailureCode failureCode) {
    if(failureCode == A) {
        showA();
    }
    else if(failureCode == B) {
        showB();
    }
    else if(failureCode == C) {
        showC();
    }
}

