//変更禁止
#pragma once

#include <Arduino.h>


class Cat {
    public:
        void setup(); //初期設定
        bool check_catch(); //検知ステータス取得

    private:
        bool cat = false; //stat
        int ocount = 0; //offcount
        int icount = 0; //oncount
        const int PIN = 41;
        unsigned long start;
};