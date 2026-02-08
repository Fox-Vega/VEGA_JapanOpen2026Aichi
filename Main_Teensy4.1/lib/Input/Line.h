//変更禁止
#pragma once

#include <Arduino.h>
#include <math.h>


class LINE {
    public:
        void get_message(byte* message); //メッセージからデータを取得
        int get_azimuth(); //方位角を取得　Mk.2から俯瞰座標を使用しているのでgyroを使った補正は不要
        int get_magnitude(); //距離を取得
        int get_eazimuth(); //逃げる方向を取得
        int get_x(); //X座標取得
        int get_y(); //Y座標取得
        bool get_stat(byte lineNUM); //指定したセンサー（０～２３）の検出ステータスを取得　０＝失敗　１＝成功
        int get_type(); //検出ステータスを取得（反応グループの数）
        int get_pack(byte packNUM); //指定した反応グループの方位角を取得　get_typeが２なら（０～１）
        bool get_trip(); //初回反応かを確認
        bool get_in(); //ライン内かを確認

        //黒塗り（意味あるんか？）

    private:
        //黒塗り
};