//変更禁止
#pragma once

#include <Arduino.h>


class MyMOTOR {
    public:
        void setup(); //初期設定
        void run(int movement_azimuth_, int movement_power_, int difix_azimuth_); //（進行方向、出力、方向）
        void free(); //自由回転
        int difix(int target_azimuth); //姿勢制御用（微分先行型PD）
        int get_azimuth(); //現在の進行方向を取得
        int get_intensity(); //現在の出力を取得
        void lock(bool stat); //モーターの信号ロック　０＝無　１＝有
        void set_difix(int stat); //姿勢制御のモード　０＝無　１＝有　２＝敵

    private:
        //調整
        const float kp = 2.5; //通常の比例
        const float dkp = 2.0; //dzone外の比例
        const float dzkp = 1.5; //dzone内の比例
        const float kd = 0.1; //微分　速度を抑える

        const int dzone = 20; //dzoneの範囲（座標）
        const int pwmlimit = 200; //移動の出力限界
        const int difixlimit = 10; //姿勢制御の単体出力限界
        const int sharelimit = 150; //姿勢制御の単体出力限界を超えた時に移動出力を削れる量
        const int redzone = 210; //モーターの出力限界　保護用

        int pwm{};

        //記録用
        int rec_azimuth;
        int rec_power;

        //フラグ
        bool motor_lock = 0;
        int difix_mode = 0;

        //出力データ
        float motor_power[4]{};
        float motor_power_[4]{};

        //姿勢制御用の記録
        int prev_azimuth{};
        unsigned long lastupdate{};

        //PIN情報
        const int motor_DPIN[4] = {4, 6, 10, 12};
        const int motor_PPIN[4] = {3, 5, 9, 11};
        const int motor_degrees[4] = {45, 135, 225, 315};
};
