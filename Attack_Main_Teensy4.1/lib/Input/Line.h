//変更禁止
#pragma once

#include <Arduino.h>
#include <math.h>


class LINE {
    public:
        void setup();
        void read_message(byte* message); //メッセージからデータを取得
        int get_azimuth(); //方位角を取得
        int get_magnitude(); //距離を取得
        int get_eazimuth(); //逃げる方向を取得
        int get_x(); //X座標取得
        int get_y(); //Y座標取得
        bool get_main(byte mainNUM); //指定したメインセンサー（０～２３）の検出ステータスを取得　０＝失敗　１＝成功
        bool get_side(byte sideNUM); //指定したサイドセンサー（０～２）の検出ステータスを取得　０＝失敗　１＝成功
        int get_mainNUM(); //検出ステータスを取得（反応グループの数）
        int get_sideNUM(); //検出ステータスを取得（反応グループの数）
        int get_pack(byte packNUM); //指定したメイン反応グループの方位角を取得　get_packNUMが２なら（０～１）
        bool get_trip(); //初回反応か
        unsigned long get_et(); //反応が終わった時から何ミリ秒経ったか
        void set_led(int pwm); //LEDの明るさ
        void set_side(bool mode); //サイドを使うか


        int main_stat[24]{}; //ステータス
        int side_stat[3]{};

        byte led_pwm{};

        int main_border = 0; //1600@中学部室 1200@高校部室 1400@ビッグコート
        int side_border = 0; //1600@中学部室 1200@高校部室 1400@ビッグコート

        bool over = false; //反転判定
        bool autosetborder{};

    private:
        int line_x{}; //座標（処理済み）
        int line_y{}; //座標（処理済み）
        int oldline_x{}; //前回の座標（処理済み）
        int oldline_y{}; //前回の座標（処理済み）
        int escape_x{}; //逃げる座標（処理済み）
        int escape_y{}; //逃げる座標（処理済み）
        float total_x{}; //X座標合計値
        float total_y{}; //Y座標合計値

        bool trip = false;

        bool use_side = false;

        int pack_NUM{};
        int side_NUM{};

        int line_type{}; //反応グループの数
        float pack_x[24]{}; //各反応グループのⅩ座標
        float pack_y[24]{}; //各反応グループのＹ座標
        int main_values[24]{}; //センサー値

        bool side_stat_[3]{}; //仮サイド
        bool oldside_stat_[3]{};

        unsigned long last_detection{}; //反応が終わった時の時間を記録
        unsigned long side_lock[3]{};

        const int main_r = 1200; //ラインセンサーの半径
        const int side_r = 2000;
        const float over_border = 140.0f; //反転判定を行う変化量

        const int ledPIN = 37;
        const int side_degs[3] = {90, 180, 270};
        const int main_degs[24] = {0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300, 315, 330, 345}; //各センサー角度
};
