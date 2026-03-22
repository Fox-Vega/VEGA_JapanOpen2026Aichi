//変更禁止
#pragma once

#include <Arduino.h>


class Cam {
    public:
        void read_message(byte* message, bool cam); //メッセージからデータを取得
        int get_x(bool side); //x取得　０＝自ゴール　１＝敵ゴール
        int get_ax(bool side); //avoidx取得（999=敵いない）  ０＝自ゴール　１＝敵ゴール
        int get_width(bool side); //幅取得　０＝自ゴール　１＝敵ゴール
        int get_height(bool side); //高さ取得　０＝自ゴール　１＝敵ゴール
        void set_goal(bool color); //敵ゴールの色を指定　０＝青　１＝黄

        bool target{};
        bool target_set{};

    private:
        int x[2] = {999, 999};
        int ax[2] = {999, 999};
        int width[2] = {999, 999};
        int height[2] = {999, 999};
};