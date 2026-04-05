#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>

#define RXD2 PA3
#define TXD2 PA2


class Communicate {
    public:
        void teensy_write(int ID);
    // private:
};