//
// Created by ganymed on 30/03/16.
//

#ifndef SEVENSEGMENTCONTROL_H
#define SEVENSEGMENTCONTROL_H


#include <stdint.h>


void setup7Segment();

void show0();
void show1();
void show2();
void show3();
void show4();
void show5();
void show6();
void show7();
void show8();
void show9();

void showNumber(uint8_t number);


void showA();
void showB();
void showC();


void test(); // TODO: remove again

void cascadeLeds(int delay); // TODO: remove again


#endif //HEIMPROJEKT02_7SEGMENTCONTROL_H
