//
// Created by ganymed on 06/04/16.
//

#ifndef FAILUREMEMORY_H
#define FAILUREMEMORY_H

#include <USBAPI.h>


#define FailureMemoryLength 5


typedef enum ErrorCode {
    NoError,
    A,
    B,
    C
} ErrorCode;


void initFailureMemory();

void pushErrorCodeOntoFailureMemory(ErrorCode errorCode);

void printFailureMemory();
// private function
void printFailure(ErrorCode errorCode);


#endif //FAILUREMEMORY_H
