//変更禁止
#pragma once

#include <Arduino.h>


class Attack {
    public:
        void setup(); //初期設定
        void attack_(); //実行
        int wrap(int mode); //０＝モーター動く　１＝モーター動かない
        void GFoLOP(); //後ろのラインにひっかがって前側のボールが取れないとき

        bool Lfront{};
        bool Lright{};
        bool Lback{};
        bool Lleft{};

    private:
        int ball_azimuth{};
        int movement_azimuth{}; //移動方向
        int speed{}; //速度

        bool back = false; //後退フラグ
        unsigned long back_start{};

        int special = 0; //1GFoLOP
        int ver_start = 999;

        //GF用
        const int ver_speed = 200;
        const int hor_speed = 190;

        const int ver_fastdeg = 15; //速い角度の範囲
        const int ver_fastspeed = 160; //NL用
        const int ver_slowspeed = 120; //NL用

        const int ver_enddeg = 140;//ライン側のver継続範囲
        const int ver_keep = 15; //フィールド側のver継続範囲

        const int hor_deg = 35; //horが発動できる範囲　180基準片側

        const int ver_catoffset = 30;//縦ラインでボールを捕獲したときに移動方向をずらす量
};