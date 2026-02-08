//変更禁止
#pragma once

#include <Arduino.h>


class MyBuzzer {
    public:
        void setup(); //初期設定
        void start(int BUZZERnote, int BUZZERduration); //音程、持続時間を指定して音を鳴らす　持続時間を９９９にすると無制限
        void stop(); //停止
        void preset(int BUZZERpresetNUM); //既定の音を鳴らす

    private:
        const bool useBUZZER = 1;
        const byte BUZZER_PIN = 40;
};