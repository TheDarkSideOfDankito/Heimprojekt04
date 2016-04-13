//
// Created by ganymed on 30/03/16.
//

#include "7SegmentControl.h"
#include "shiftRegisterControl.h"
#include "pinMappings.h"
#include "pinControl.h"
#include "util.h"


void setup7Segment() {
    setupShiftRegister();
}


void show0() {
    writeByteToRegister(0b00111111);
}

void show1() {
    writeByteToRegister(0b00000110);
}

void show2() {
    writeByteToRegister(0b01011011);
}

void show3() {
    writeByteToRegister(0b01001111);
}

void show4() {
    writeByteToRegister(0b01100110);
}

void show5() {
    writeByteToRegister(0b01101101);
}

void show6() {
    writeByteToRegister(0b01111101);
}

void show7() {
    writeByteToRegister(0b00000111);
}

void show8() {
    writeByteToRegister(0b01111111);
}

void show9() {
    writeByteToRegister(0b01101111);
}


void showNumber(uint8_t number) {
    if(number == 0) {
        show0();
    }
    else if(number == 1) {
        show1();
    }
    else if(number == 2) {
        show2();
    }
    else if(number == 3) {
        show3();
    }
    else if(number == 4) {
        show4();
    }
    else if(number == 5) {
        show5();
    }
    else if(number == 6) {
        show6();
    }
    else if(number == 7) {
        show7();
    }
    else if(number == 8) {
        show8();
    }
    else if(number == 9) {
        show9();
    }
}



void showA() {
    writeByteToRegister(0b01110111);
}

void showB() {
    show8();
}

void showC() {
    writeByteToRegister(0b00111001);
}


void test() {
    int delay = 1000;

    show0();
    sleep_ms_100ms_steps(delay);

    show1();
    sleep_ms_100ms_steps(delay);

    show2();
    sleep_ms_100ms_steps(delay);

    show3();
    sleep_ms_100ms_steps(delay);

    show4();
    sleep_ms_100ms_steps(delay);

    show5();
    sleep_ms_100ms_steps(delay);

    show6();
    sleep_ms_100ms_steps(delay);

    show7();
    sleep_ms_100ms_steps(delay);

    show8();
    sleep_ms_100ms_steps(delay);

    show9();
    sleep_ms_100ms_steps(delay);
}


void cascadeLeds(int delay) {
    writeByteToRegister(0b00000001);
    sleep_ms_10ms_steps(delay);

    writeByteToRegister(0b00000010);
    sleep_ms_10ms_steps(delay);

    writeByteToRegister(0b00000100);
    sleep_ms_10ms_steps(delay);

    writeByteToRegister(0b00001000);
    sleep_ms_10ms_steps(delay);

    writeByteToRegister(0b00010000);
    sleep_ms_10ms_steps(delay);

    writeByteToRegister(0b00100000);
    sleep_ms_10ms_steps(delay);

    writeByteToRegister(0b01000000);
    sleep_ms_10ms_steps(delay);

    writeByteToRegister(0b100000000);
    sleep_ms_10ms_steps(delay);
}