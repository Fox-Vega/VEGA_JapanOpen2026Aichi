//変更禁止
#pragma once

#include <Arduino.h>


class GAM { //Gyro,Accelero,Magnetoの略
    public:
        void setup(); //初期設定
        void read_azimuth(); // 方位角の読み取り　値の更新
        int get_azimuth(); //方位角取得　値の更新されない
        void dir_reset(); //方向の初期化
        float bimuth_conv(float azimuth); //俯瞰角度に変換
        float bimuth_rev(float bimuth); //機体角度に変換
        void bird_conv(float x, float y);
        void bird_rev(float x, float y);
        float get_x();
        float get_y();

    private:
        int azimuth{};
        int yawtweak{};
        float n_x; //何かにつかってる
        float n_y; //何かにつかってる
};