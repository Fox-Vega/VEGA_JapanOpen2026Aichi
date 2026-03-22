//変更禁止
#pragma once

#include <Arduino.h>


class Communicate {
    public:
        void line_read(byte* message);
        void line_write(int ID);
        void ui_write(int ID);

    private:
};