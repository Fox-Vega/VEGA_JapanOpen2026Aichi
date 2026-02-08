//変更禁止
#pragma once

#include <Arduino.h>
#include <math.h>


class LINE {
    public:
        void setup(); //初期設定
        void get_message(byte* message); //メッセージからデータを取得
        void read(); //センサーの読み取り
        bool get_stat(byte lineNUM); //指定したセンサー（０～２３）の検出ステータスを取得　０＝失敗　１＝成功
        int get_value(byte lineNUM); //指定したセンサー（０～２３）の強度を取得　０～１５

        bool line_stat[24]{}; //最終ステータス

    private:
        int detection_border = 0;

        int line_stat_[24]{}; //仮ステータス
        int line_values[24]{}; //センサー値

        const uint8_t selectPIN[3] = {D10, D9, D4}; //選択ピン
        const uint8_t outputPIN[3] = {D3, D5, D8}; //取得ピン
        const byte Reader[8][3] = {
            {0, 0, 0},
            {1, 0, 0},
            {0, 1, 0},
            {1, 1, 0},
            {0, 0, 1},
            {1, 0, 1},
            {0, 1, 1},
            {1, 1, 1}
        };
        const int link[24] = {5, 6, 7, 4, 3, 0, 2, 1, 10, 9, 8, 11, 15, 12, 14, 13, 20, 21, 22, 23, 19, 16, 18, 17};
};