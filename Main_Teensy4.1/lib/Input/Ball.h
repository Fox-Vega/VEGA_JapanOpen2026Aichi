//変更禁止
#pragma once

#include <Arduino.h>


class BALL {
    public:
        void get_message(byte* message); //メッセージからデータを取得
        bool get_stat(); //検出ステータスを取得　０＝失敗　１＝成功
        int get_x(); //Ⅹ座標取得
        int get_y(); //Ｙ座標取得
        int get_azimuth(); //方位角を取得
        int get_intensity(); //強度を取得

    private:
        bool ball = false; //検出ステータス
        int ball_x{}; //最終座標
        int ball_y{}; //最終座標
};