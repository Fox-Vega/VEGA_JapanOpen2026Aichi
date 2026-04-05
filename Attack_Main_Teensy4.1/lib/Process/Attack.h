//変更禁止
#pragma once

#include <Arduino.h>


class Attack {
    public:
        void setup(); //初期設定
        void reset();
        void attack_(); //実行
        int wrap(int mode); //０＝モーター動く　１＝モーター動かない
        void flow(); //後ろのラインにひっかがって前側のボールが取れないとき　とか書いてあるけど、どこでも発動する
        void push();

        bool Lfront{};
        bool Lright{};
        bool Lback{};
        bool Lleft{};

    private:
        int ball_wdeg{};
        int movement_azimuth{}; //移動方向
        int speed{}; //速度

        bool back = false; //後退フラグ
        unsigned long back_start{};

        int special = 0; //1flow 2push
        int ver_start = 999;

        bool kick_ZONE;
        unsigned long kick_TIME;
        unsigned long face_TIME;

        bool side;

        bool lineavoid{};
        bool lineavoid_azimuth{};


        bool run = 0;
        bool kickoff{};
        unsigned long kickoff_TIME{};
        int kickoff_azimuth{};

        unsigned long start_TIME{};
        bool start_FIN{};

        //Flow用
        const int trace_avoid = 15;
        const int ver_speed = 180; //縦速度
        const int hor_speed = 180; //横速度
        //ver
        const int ver_SCmode = 70; //可変速モードを有効化する範囲（開始時に判定）　０基準片側
        const int ver_fastdeg = 15; //速い範囲　０基準片側
        const int ver_fastspeed = 180; //速い速度
        const int ver_slowspeed = 170; //遅い速度
        const int ver_catoffset = 20; //ボールを捕獲したときの移動方向
        const int ver_enddeg = 120;//ライン側のver継続範囲
        const int ver_keep = 20; //フィールド側のver継続範囲
        const int near_border = 20;

        const int hor_deg = 35; //horが発動できる範囲　180基準片側

        byte invert{};

        int push_count;
};