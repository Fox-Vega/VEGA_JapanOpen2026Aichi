#include "Ball.h"
#include "AIP.h"
#include "Input.h"
#include <driver/gpio.h>

#define GPIO_SET(pin)   (GPIO.out_w1ts = (1UL << (pin)))
#define GPIO_CLR(pin)   (GPIO.out_w1tc = (1UL << (pin)))
#define GPIO_READ(pin)  ((GPIO.in >> (pin)) & 1U)


void BALL::setup() {
    for (int i = 0; i < 4; i++) {
        pinMode(selectPIN[i], OUTPUT);
    }
    pinMode(outputPIN, INPUT);

    ballNUMoffset = 16 - ((samples - 1) / 2);
    hsamples = sizeof(ball_hmag) / sizeof(ball_hmag[0]);
}

void BALL::read() {
    //センサー値初期化
    for (int i = 0; i < 16; i++) {
        ballvalues[i] = 0;
    }

    //読み取り
    unsigned long ball_start = micros();
    while((micros() - ball_start) < ????) {
        for (int i = 0; i < 16; i++) {
            if (Reader[i][0]) GPIO_SET(selectPIN[0]); else GPIO_CLR(selectPIN[0]);
            if (Reader[i][1]) GPIO_SET(selectPIN[1]); else GPIO_CLR(selectPIN[1]);
            if (Reader[i][2]) GPIO_SET(selectPIN[2]); else GPIO_CLR(selectPIN[2]);
            if (Reader[i][3]) GPIO_SET(selectPIN[3]); else GPIO_CLR(selectPIN[3]);

            //黒塗り（一行だけやけど）

            if (!GPIO_READ(outputPIN)) {
                ballvalues[link[i]]++;
            }
        }
    }

    //最大値
    max_ballNUM = 99;
    max_ballvalue = 0;
    for (int i = 0; i < 16; i++) {
        if (ballvalues[i] > max_ballvalue) {
            max_ballvalue = ballvalues[i];
            max_ballNUM = i;
        }
    }

    if (max_ballNUM == 99) { //ボールない
        ball_x_ = 0;
        ball_y_ = 0;
    } else { //ボールある
        // 座標計算（ベクトルの移動平均）
        total_x = 0;
        total_y = 0;
        ballNUMstart = (max_ballNUM + ballNUMoffset) % 16;
        for (int i = 0; i < samples; i++) {
            int ballNUM = (ballNUMstart + i) % 16;
            myvector.get_cord(balldirs[ballNUM], ballvalues[ballNUM]);
            total_x += myvector.get_x();
            total_y += myvector.get_y();
        }
        ball_x_ = total_x / samples;
        ball_y_ = total_y / samples;
    }

    //履歴更新
    for (int i = 0; i < (hsamples - 1); i++) { //ずらす
        ball_hmag[i + 1] = ball_hmag[i];
    }
    if (max_ballNUM != 99) ball_hmag[0] = myvector.get_magnitude(ball_x_, ball_y_); //追加
    else ball_hmag[0] = 0;

    //和を求めて平均取る
    total_mag = 0; //リセット
    for (int i = 0; i < hsamples; i++) total_mag += ball_hmag[i];
    myvector.get_cord(myvector.get_azimuth(ball_x_, ball_y_), total_mag / hsamples);

    ball_x = myvector.get_x();
    ball_y = myvector.get_y();
}

int BALL::get_x() {
    return ball_x;
}

int BALL::get_y() {
    return ball_y;
}