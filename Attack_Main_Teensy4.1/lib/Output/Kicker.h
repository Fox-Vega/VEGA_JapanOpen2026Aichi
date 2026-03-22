//変更禁止
#pragma once

#include <Arduino.h>


class Kicker {
    public:
        void setup(); //初期設定
        void run(bool kick); //毎ループ呼び出し必須　使用するときは引数に１を入れて呼び出す
        void arm(); //安全のために待機中は毎ループ呼び出し

    private:
        const byte charge_PIN = 41;
        const byte discharge_PIN = 40;

        unsigned long LAST_kick{};
        unsigned long LAST_arm{};
        int MODE{}; //0=NO 1=kicking 2=arming
};