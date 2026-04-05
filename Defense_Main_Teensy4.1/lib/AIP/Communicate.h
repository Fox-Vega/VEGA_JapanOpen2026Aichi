//変更禁止
#pragma once

#include <Arduino.h>


class Communicate {
    public:
        struct Mode{
            bool cam;
            bool gamreset;
            bool rainbow;
            std::array<bool, 4> switch_;
            byte latency;
            bool buzzer;

            //独自
            byte mode;
            bool usepixel;
        };
        void line_write(int ID);
        void ui_write(int ID);
        void setUI(Mode UI){uiState = UI;};
        void setParam(bool param);
    private:
        Mode uiState;
        bool param = false;
};