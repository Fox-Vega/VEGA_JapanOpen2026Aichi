//変更禁止
#pragma once

#include <Arduino.h>


class MyMOTOR {
    public:
        void setup(); //初期設定
        void run(int movement_azimuth, int power_, int dir_azimuth); //（進行方向、出力、方向）
        void free(); //自由回転
        int difix(int target_azimuth); //姿勢制御用（微分先行型PD）
        int get_azimuth(); //現在の進行方向を取得
        int get_intensity(); //現在の出力を取得
        void lock(bool stat); //モーターの信号ロック　０＝無　１＝有
        void stabilization(int stat); //姿勢制御の有無　０＝無　１＝有　２＝敵

    private:
        //調整
        const float kp = 2.5; //比例　感度を上げる
        const float dkp = 2.0; //ゴールを向く（dzone外の比例）
        const float dzkp = 1.5; //ゴールを向く（dzoneの比例（dzoneに微分無し））
        const float kd = 0.1; //微分　速度を抑える
        const int dzone = 20; //dzone 範囲
        const int pwmlimit = 200; //移動の出力リミット
        const int difixlimit = 10; //姿勢制御の単体出力リミット
        const int sharelimit = 150; //共有出力　姿勢制御単体出力リミットを超えた時に移動出力を削れる量

        int max_power{};


        int azimuth;
        int intensity;
        bool motor_lock = 0;
        int motor_stabilization = 1;
        float motor_power[4]{};
        float motor_power_[4]{};
        float scale{};

        int prev_azimuth{};
        unsigned long lastupdate{};

        const int motor_DPIN[4] = {4, 6, 10, 12};
        const int motor_PPIN[4] = {3, 5, 9, 11};
        const int motor_degrees[4] = {45, 135, 225, 315};
};
