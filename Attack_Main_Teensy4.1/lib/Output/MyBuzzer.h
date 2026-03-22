//変更禁止
#pragma once

#include <Arduino.h>


class MyBuzzer {
    public:
        void setup(); //初期設定
        void run(int note, int duration); //音程、持続時間を指定して音を鳴らす　持続時間を９９９にすると無制限
        void stop(); //停止
        void preset(int preset_NUM); //既定の音を鳴らす

    private:
        const bool USE_buzzer = 1;
        const int buzzer_PIN = 36;
};