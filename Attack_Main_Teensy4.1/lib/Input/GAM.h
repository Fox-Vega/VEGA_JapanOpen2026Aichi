//変更禁止
#pragma once

#include <Arduino.h>


class GAM { //Gyro,Accelero,Magnetoの略
    public:
        void setup(); //初期設定
        void read_azimuth(); //値の更新
        int get_azimuth(); //方位角取得
        void set_zero(); //方向の初期化
        float get_atob(float azimuth); //俯瞰角度に変換
        float get_btoa(float bimuth); //機体角度に変換
        void cal_ctob(float x, float y);
        void cal_btoc(float x, float y);
        float get_x();
        float get_y();

    private:
        int azimuth{};
        int yawtweak{};
        float x;
        float y;
};