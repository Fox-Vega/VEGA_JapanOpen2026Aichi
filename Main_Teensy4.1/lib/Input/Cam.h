//変更禁止
#pragma once

#include <Arduino.h>


class Cam {
    public:
        void get_message(byte* message, bool cam); //メッセージからデータを取得
        int get_x(bool side); //x取得　０＝自ゴール　１＝敵ゴール
        int get_ax(bool side); //avoidx取得  ０＝自ゴール　１＝敵ゴール
        int get_width(bool side); //幅取得　０＝自ゴール　１＝敵ゴール
        int get_height(bool side); //高さ取得　０＝自ゴール　１＝敵ゴール
        void set(bool tar); //敵ゴールの色を指定　０＝青　１＝黄

    private:
        int x[2] = {999, 999};
        int ax[2] = {999, 999}; //avoidx www
        int height[2] = {999, 999};
        int width[2] = {999, 999};
        bool target{}; //０＝青
};