//変更禁止
#include "Line.h"
#include "AIP.h"
#include "Input.h"


void LINE::setup() {
    pinMode(ledPIN, OUTPUT);
    analogWriteFrequency(ledPIN, 20000);
}

void LINE::read_message(byte* message) {
    //メッセージ受信
    for (int i = 2; i < 5; i++) {
        for (int j = 0; j < 8; j++) {
            main_stat[(i-2) * 8 + j] = (message[i] >> j) & 0x01;
        }
    }
    if(message[1] == 1) {side_stat_[0]=0;side_stat_[1]=0;side_stat_[2]=0;}
    else if(message[1] == 2) {side_stat_[0]=1;side_stat_[1]=0;side_stat_[2]=0;}
    else if(message[1] == 3) {side_stat_[0]=0;side_stat_[1]=1;side_stat_[2]=0;}
    else if(message[1] == 4) {side_stat_[0]=1;side_stat_[1]=1;side_stat_[2]=0;}
    else if(message[1] == 5) {side_stat_[0]=0;side_stat_[1]=0;side_stat_[2]=1;}
    else if(message[1] == 6) {side_stat_[0]=1;side_stat_[1]=0;side_stat_[2]=1;}
    else if(message[1] == 7) {side_stat_[0]=0;side_stat_[1]=1;side_stat_[2]=1;}
    else if(message[1] == 8) {side_stat_[0]=1;side_stat_[1]=1;side_stat_[2]=1;}

    //初期化
    total_x = 0;
    total_y = 0;
    int sample = 0;
    bool pack_NOW = 0;
    for(int i = 0; i < 24; i++) {
        pack_x[i] = 0;
        pack_y[i] = 0;
    }

    //グループ分け
    //連続して反応しているセンサーをまとめる

    //サイド処理

    if (pack_NUM == 0 && side_NUM == 0) { //ない
        line_x = 999;
        line_y = 999;
        corner = 0;
        over = 0;
        escape_x = 0;
        escape_y = 0;
    } else { //ある
        total_x = 0;
        total_y = 0;
        line_x = 0;
        line_y = 0;

        //メイン　座標計算

        //サイド　座標計算

        //バグ防止

        //初回検知判定

        //反転判定

        //角処理

        //逃げる方向更新
        if (over == false) {
            escape_x = -line_x;
            escape_y = -line_y;
        } else {
            escape_x = line_x;
            escape_y = line_y;
        }
    }

    //最終検知を記録
    if (line_type != 0 && pack_NUM == 0 && side_NUM == 0) last_detection = millis();

    //ライン種類更新
    line_type = pack_NUM + side_NUM;

    //更新
    oldline_x = line_x;
    oldline_y = line_y;
}

void LINE::cal_C() {
    //現在地特定
}

void LINE::cal_CO() {
    int line_azimuth = gam.get_atob(myvector.get_azimuth(line_x, line_y));

    int a = 35;
    if (corner == 1) {
        if (45 - a < line_azimuth && line_azimuth < 45 + a) over = 0;
        else if (225 - a < line_azimuth && line_azimuth < 225 + a) over = 1;
    } else if (corner == 2) {
        if (135 - a < line_azimuth && line_azimuth < 135 + a) over = 0;
        else if (315 - a < line_azimuth && line_azimuth < 315 + a) over = 1;
    } else if (corner == 3) {
        if (225 - a < line_azimuth && line_azimuth < 225 + a) over = 0;
        else if (45 - a < line_azimuth && line_azimuth < 45 + a) over = 1;
    } else if (corner == 4) {
        if (315 - a < line_azimuth && line_azimuth < 315 + a) over = 0;
        else if (135 - a < line_azimuth && line_azimuth < 135 + a) over = 1;
    }
}

int LINE::get_azimuth() {
    int azimuth = 0;
    if (line_type != 0 || line_x != 0 || line_y != 0) {
        azimuth = round(myvector.get_azimuth(line_x, line_y));
    }
    return azimuth;
}

int LINE::get_magnitude() {
    int magnitude = 9999;
    if (line_type != 0 || line_x != 0 || line_y != 0) {
        magnitude = round(myvector.get_magnitude(line_x, line_y));
    }
    return magnitude;
}

int LINE::get_eazimuth() {
    int eazimuth = 0;
    if (line_type != 0 || line_x != 0 || line_y != 0) {
        eazimuth = round(myvector.get_azimuth(escape_x, escape_y));
    }
    return eazimuth;
}

int LINE::get_x() {
    return line_x;
}

int LINE::get_y() {
    return line_y;
}

bool LINE::get_main(byte mainNUM) {
    return main_stat[mainNUM];
}

bool LINE::get_side(byte sideNUM) {
    return side_stat[sideNUM];
}

int LINE::get_mainNUM() {
    return pack_NUM;
}

int LINE::get_sideNUM() {
    return side_NUM;
}

int LINE::get_pack(byte packNUM) {
    return myvector.get_azimuth(pack_x[packNUM], pack_y[packNUM]);
}

bool LINE::get_trip() {
    return trip;
}

unsigned long LINE::get_et() {
    return (millis() - last_detection);
}

void LINE::set_led(int brightness) {
    if (brightness == 0 || brightness == 999) {
        if (brightness == 0) analogWrite(ledPIN, 0);
        else if (brightness == 999) analogWrite(ledPIN, led_pwm);
    } else {
        brightness = constrain(brightness, 0, 255);
        led_pwm = brightness;
        analogWrite(ledPIN, brightness);
    }
}

void LINE::set_side(bool mode) {
    use_side = mode;
}
