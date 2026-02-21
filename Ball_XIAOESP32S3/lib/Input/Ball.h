#pragma once

#include <Arduino.h>

#define BALL_HISTORY_SIZE 42

class BALL {
    public:
        void setup(); //初期設定
        void read(); //センサーの読み取り
        int get_x(); //X座標取得
        int get_y(); //Y座標取得
        int get_value(byte ballNUM); //指定したセンサーの強度を取得（引数は０～１５）

    private:
        int samples = 5; //移動平均計算に使用する個数
        int ball_x{};//仮座標
        int ball_y{};//仮座標
        int ball_x_{};//最終座標（処理済み）
        int ball_y_{};//最終座標（処理済み）
        int ball_hx[BALL_HISTORY_SIZE]{};//履歴
        int ball_hy[BALL_HISTORY_SIZE]{};//履歴
        int ballvalues[16]{};//センサー値を保存
        int ballNUMoffset{}; //ずらす量（自動計算＆保存）

        int NUMball = 16; //センサーの数

        const uint8_t selectPIN[4] = {D9, D8, D4, D5}; //選択ピン
        const uint8_t outputPIN = D10;

        const float balldirs[16] = {0, 22.5, 45, 67.5, 90, 112.5, 135, 157.5, 180, 202.5, 225, 247.5, 270, 292.5, 315, 337.5}; //各センサー角度
        const uint8_t Reader[16][4] = {
            {0, 0, 0, 0},
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 1, 0},
            {1, 0, 1, 0},
            {0, 1, 1, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 1},
            {1, 0, 0, 1},
            {0, 1, 0, 1},
            {1, 1, 0, 1},
            {0, 0, 1, 1},
            {1, 0, 1, 1},
            {0, 1, 1, 1},
            {1, 1, 1, 1}
        };
        // const int link[16] = {15, 14, 13, 12, 8, 9, 10, 11, 4, 5, 6, 7, 3, 2, 1, 0}; //スプレッド仕様
        const int link[16] = {14, 13, 12, 8, 9, 10, 11, 4, 5, 6, 7, 3, 2, 1, 0, 15}; //旧仕様
};
