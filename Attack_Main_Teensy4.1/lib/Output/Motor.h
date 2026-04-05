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
        int get_power(); //現在の出力を取得
        void set_motor(bool mode); //モーターの信号ロック　０＝無　１＝有
        void set_difix(int mode); //姿勢制御のモード　０＝無　１＝有　２＝敵

        bool ZONE{};
        int difix_power{};

    private:
        //調整
        const float kp = 2.5; //通常の比例
        const float gkp = 2.5;
        const float kd = 0.06 ; //微分　速度を抑える

        int ZONE_border = 0;
        const int movement_LIMIT = 240; //移動の出力限界
        const int difix_LIMIT = 0; //姿勢制御の単体出力限界
        const int share_LIMIT = 160; //姿勢制御の単体出力限界を超えた時に移動出力を削れる量
        const int pwm_LIMIT = 240; //モーターの出力限界　保護用

        int pwm{};

        //記録用
        int REC_azimuth;
        int REC_power;

        //フラグ
        bool USE_motor = 0;
        int difix_MODE = 0;

        //出力データ
        float motor_power[4]{};
        float motor_power_[4]{};

        //姿勢制御用の記録
        int PREV_azimuth{};
        unsigned long LAST_difix{};

        //PIN情報
        const int motor_DPIN[4] = {4, 6, 10, 12};
        const int motor_PPIN[4] = {3, 5, 9, 11};
        const int motor_azimuth[4] = {45, 135, 225, 315};
};