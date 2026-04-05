//変更禁止
#pragma once

#include <Arduino.h>
#include <math.h>
#include <Preferences.h>


class LINE {
    public:
        void setup(); //初期設定
        void read_message(byte* message); //メッセージからデータを取得
        void read(); //センサーの読み取り
        void set_border(bool set);

        bool main_stat[24]{};
        bool side_stat[3]{};
        bool oldmain_stat[24]{};
        bool oldside_stat[3]{};
        int main_value[24]{};
        int side_value[3]{};

        int stable{};

    private:
        int main_border{};
        int side_border{};

        int MODE{};
        int MAX_value{};

        // int old_main_stat[24]{};
        int main_stat_[24]{};
        int side_stat_[3]{};

        int MAX_maintotal;
        int MIN_maintotal;
        int MAX_sidetotal;
        int MIN_sidetotal;
        int step;

        const uint8_t select_PIN[3] = {D10, D9, D4}; //選択ピン
        const uint8_t output_PIN[3] = {D3, D5, D8}; //取得ピン
        const uint8_t side_PIN[3] = {D1, D2, D0};
        const bool reader[8][3] = {
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