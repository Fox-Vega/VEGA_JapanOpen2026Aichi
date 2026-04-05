#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>


//color_MODE
//0=none(black)
//1=red
//2=green
//3=blue
//4=yellow
//5=sian
//6=magenta
//7=white
//8=grey
//9=black

class MyPIXEL {
    public:
        void setup(); //初期設定
        void set_brightness(int brightness); //０～２５５明るさを設定（９９９＝規定値）
        void uni(int pixel, int color); //指定のピクセルを点灯する
        void multi(int PIXELNUMstart, int PIXELNUMend, int color); //指定範囲のピクセルを点灯する
        void closest(int azimuth, int color, int range); //指定角度に指定個数のピクセルを光らせる
        void range(int start, int end, int color); //指定した角範囲のピクセルを点灯する
        void rainbow(); //パリピ
        void show(); //ピクセルを表示
        void clear(); //ピクセルを消去
        void set_mode(bool stat); //使用・不使用　の設定

        byte brightness = 50; //最大で255
        byte color_MODE{};
        bool pixel_STAT[24]{};
        bool USE_pixel{};

        enum class COLOR {
            NONE = 0,
            RED = 1,
            GREEN = 2,
            BLUE = 3,
            YELLOW = 4,
            CYAN = 5,
            MAGENTA = 6,
            WHITE = 7,
            GREY = 8,
            BLACK = 9
        };

        void uni(int pixel, COLOR color) { uni(pixel, static_cast<int>(color)); }
        void multi(int PIXELNUMstart, int PIXELNUMend, COLOR color) { multi(PIXELNUMstart, PIXELNUMend, static_cast<int>(color)); }
        void closest(int azimuth, COLOR color, int range) { closest(azimuth, static_cast<int>(color), range); }
        void range(int start, int end, COLOR color) { range(start, end, static_cast<int>(color)); }

    private:
        const int pixel_NUM = 24;
};