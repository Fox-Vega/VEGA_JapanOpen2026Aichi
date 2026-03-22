//変更禁止
#pragma once

#include <Arduino.h>


class Cat {
    public:
        void setup(); //初期設定
        bool fetch(); //検知ステータス取得

        int cat_value{};
        unsigned long st;

    private:
        bool cat{};
        int ocount{};
        int icount{};
        const int cat_PIN = 38;
        unsigned long LAST_cat;
};