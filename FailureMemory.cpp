//
// Created by ganymed on 06/04/16.
//

#include "FailureMemory.h"
#include "Arduino.h"


ErrorCode failureMemory[FailureMemoryLength];

ErrorCode lastErrorCode = NoError;

uint8_t countFailures = 0;


void initFailureMemory() {
    for(uint8_t i; i < FailureMemoryLength; i++) {
        failureMemory[i] = NoError;
    }
}


void pushErrorCodeOntoFailureMemory(ErrorCode errorCode) {
    if(errorCode == lastErrorCode) { // do not push the same error code on failure memory two times in a row
        return;
    }

    lastErrorCode = errorCode;

    for(uint8_t i = FailureMemoryLength - 1; i > 0; i--){
        failureMemory[i] = failureMemory[i - 1];
    }

    failureMemory[0] = errorCode;

    countFailures++;
}


void printFailureMemory() {
    Serial.print("Failure Memory contains ");
    Serial.print(countFailures);
    Serial.println(" failure(s):");

    uint8_t countStoredFailures = (countFailures > FailureMemoryLength ? countFailures : FailureMemoryLength);
    for(int i = 0; i < countStoredFailures; i++) {
        printFailure(failureMemory[i]);
    }
}

void printFailure(ErrorCode errorCode) {
    if(errorCode == A) {
        Serial.println("A");
    }
    else if(errorCode == B) {
        Serial.println("B");
    }
    else if(errorCode == C) {
        Serial.println("C");
    }
}

