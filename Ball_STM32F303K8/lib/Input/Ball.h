#pragma once

#include <Arduino.h>

#define BALL_HISTORY_SIZE 45
#define BALL_AZIMUTH_SIZE 10

class BALL {
    public:
        void setup(); //初期設定
        void read(); //センサーの読み取り
        int get_x(); //X座標取得
        int get_y(); //Y座標取得
        int get_value(byte ballNUM); //指定したセンサーの強度を取得（引数は０～１５）

    private:
        int samples = 7; //移動平均計算に使用する個数
        int ball_azix{};//仮座標
        int ball_aziy{};//仮座標
        int ball_magx{};//仮座標
        int ball_magy{};//仮座標
        int ball_x{};
        int ball_y{};
        int ball_x_{};//最終座標（処理済み）
        int ball_y_{};//最終座標（処理済み）
        int ball_hx[BALL_HISTORY_SIZE]{};//履歴
        int ball_hy[BALL_HISTORY_SIZE]{};//履歴
        int ball_value[16]{};//センサー値を保存
        int offset{}; //ずらす量（自動計算＆保存）
        int total_x{};
        int total_y{};

        const float ball_deg[16] = {0, 22.5, 45, 67.5, 90, 112.5, 135, 157.5, 180, 202.5, 225, 247.5, 270, 292.5, 315, 337.5}; //各センサー角度
        const int ball_PIN[16] = {PB1, PB0, PA7, PA4, PA0, PA6, PA5, PA1, PB6, PB7, PB3, PB4, PB5, PA12, PA11, PA8};
};