#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>

#define RXD1 D7
#define TXD1 D6


class Communicate {
    public:
        void teensy_write(int ID);
    // private:
};