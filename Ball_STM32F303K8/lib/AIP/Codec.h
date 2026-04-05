#pragma once

#include <Arduino.h>


class Codec {
    public:
        void encode(uint8_t *message_write, int start, int end, int keyindex);
        int decode(uint8_t *message_read, int start, int end, int keyindex);
    private:
        int key;
        bool error;
};