//
// Created by ganymed on 06/04/16.
//

#ifndef FAILUREMEMORY_H
#define FAILUREMEMORY_H

#include <USBAPI.h>
#include "7SegmentControl.h"


#define FailureMemoryLength 5


typedef enum ErrorCode {
    NoFailure,
    A,
    B,
    C
} FailureCode;


void initFailureMemory();

void pushFailureCodeOntoFailureMemory(FailureCode failureCode);

void displayFailureMemory();
// private function
void printFailure(FailureCode failureCode);
void printFailureOn7SegmentDisplay(FailureCode failureCode);


#endif //FAILUREMEMORY_H
