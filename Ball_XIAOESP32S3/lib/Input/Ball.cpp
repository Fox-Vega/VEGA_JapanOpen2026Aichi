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
}

void BALL::read() {
    //センサー値初期化
    for (int i = 0; i < 16; i++) {
        ballvalues[i] = 0;
    }

    //読み取り
    unsigned long ball_start = micros();
    while((micros() - ball_start) < 2499) {
        for (int i = 0; i < 16; i++) {
            if (Reader[i][0]) GPIO_SET(selectPIN[0]); else GPIO_CLR(selectPIN[0]);
            if (Reader[i][1]) GPIO_SET(selectPIN[1]); else GPIO_CLR(selectPIN[1]);
            if (Reader[i][2]) GPIO_SET(selectPIN[2]); else GPIO_CLR(selectPIN[2]);
            if (Reader[i][3]) GPIO_SET(selectPIN[3]); else GPIO_CLR(selectPIN[3]);

            delayMicroseconds(5);

            if (!GPIO_READ(outputPIN)) {
                ballvalues[i]++;
            }
        }
    }

    // for (int i = 0; i < 16; i++) {
    //     Serial.print(ballvalues[i]);
    //     Serial.print(" ");
    // }
    // Serial.println();

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

    //履歴データの平均
    total_x = 0;
    total_y = 0;
    for (int i = 0; i < (BALL_HISTORY_SIZE - 1); i++) {
        ball_hx[i + 1] = ball_hx[i];
        ball_hy[i + 1] = ball_hy[i];
    }
    ball_hx[0] = ball_x_;
    ball_hy[0] = ball_y_;
    for (int i = 0; i < BALL_HISTORY_SIZE; i++) {
        total_x += ball_hx[i];
        total_y += ball_hy[i];
    }
    ball_x = total_x / BALL_HISTORY_SIZE;
    ball_y = total_y / BALL_HISTORY_SIZE;

    myvector.get_cord(myvector.get_azimuth(ball_x_, ball_y_), myvector.get_magnitude(ball_x, ball_y));

    ball_x = myvector.get_x();
    ball_y = myvector.get_y();
}

int BALL::get_x() {
    return ball_x;
}

int BALL::get_y() {
    return ball_y;
}
