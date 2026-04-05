//変更禁止
#pragma once

#include <Arduino.h>


class Cat {
    public:
        void setup(); //初期設定
        bool check_catch();

    private:
        bool cat = false; //検出ステータス
        int ocount = 0;
        int icount = 0;
        const int PIN = 38;
        unsigned long start;
};