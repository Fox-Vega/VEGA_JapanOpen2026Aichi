//変更禁止
#include "Line.h"
#include "AIP.h"
#include "Input.h"


void LINE::get_message(byte* message) {
        //メッセージ処理

        //初期化
        //グループ分け

    if (pack_NUM == 0) { //ラインない
        line_x = 999;
        line_y = 999;
    } else { //ラインある
            //計算
            //俯瞰座標に変換
            //ゼロ除算防止処理
            //初回検知判定
            //反転判定
            //逃げる方向更新
            //IN判定更新
            //条件付きリセット
    }

        //ライン種類更新
        //更新
}

int LINE::get_azimuth() {
    if (line_x == 0 && line_y == 0) {
        return 0;
    }
    return round(myvector.get_azimuth(line_x, line_y));
}

int LINE::get_magnitude() {
    int magnitude = 999;
    if (line_type != 0) {
        magnitude = myvector.get_magnitude(line_x, line_y);
    }
    return magnitude;
}

int LINE::get_eazimuth() {
    if (escape_x == 0 && escape_y == 0) {
        return 0;
    }
    return round(myvector.get_azimuth(escape_x, escape_y));
}

int LINE::get_x() {
    return line_x;
}

int LINE::get_y() {
    return line_y;
}

bool LINE::get_stat(byte lineNUM) {
    return line_stat[lineNUM];
}

int LINE::get_type() {
    return line_type;
}

int LINE::get_pack(byte packNUM) {
    return myvector.get_azimuth(pack_x[packNUM], pack_y[packNUM]);
}

bool LINE::get_trip() {
    return trip;
}

bool LINE::get_in() {
    return in;
}