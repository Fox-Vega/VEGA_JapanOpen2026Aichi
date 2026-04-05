#include "Ball.h"
#include "AIP.h"
#include "Input.h"


void BALL::setup() {
    for (int i = 0; i < 16; i++) {
        pinMode(ball_PIN[i], INPUT);
    }

    offset = 16 - ((samples - 1) / 2);
}

void BALL::read() {
    //センサー値初期化
    for (int i = 0; i < 16; i++) {
        ball_value[i] = 0;
    }

    //読み取り
    unsigned long ball_start = micros();
    while((micros() - ball_start) < 2499) {
        for (int i = 0; i < 16; i++) {
            if (!digitalRead(ball_PIN[i])) {
                ball_value[i]++;
            }
        }
    }

    // for (int i = 0; i < 16; i++) {
    //     Serial.print(ball_value[i]);
    //     Serial.print(" ");
    // }
    // Serial.println();

    //最大値
    int max_ballNUM = 99;
    int max_ballvalue = 0;
    for (int i = 0; i < 16; i++) {
        if (ball_value[i] > max_ballvalue) {
            max_ballvalue = ball_value[i];
            max_ballNUM = i;
        }
    }

    if (max_ballNUM == 99) { //ボールない
        ball_x_ = 0;
        ball_y_ = 0;
    } else { //ボールある
        // 座標計算（ベクトルの移動平均）
        int total_x = 0;
        int total_y = 0;
        int ballNUMstart = (max_ballNUM + offset) % 16;
        for (int i = 0; i < samples; i++) {
            int ballNUM = (ballNUMstart + i) % 16;
            myvector.cal_cord(ball_deg[ballNUM], ball_value[ballNUM]);
            total_x += myvector.get_x();
            total_y += myvector.get_y();
        }
        ball_x_ = total_x / samples;
        ball_y_ = total_y / samples;
    }

    //履歴データの平均
    for (int i = 0; i < (BALL_HISTORY_SIZE - 1); i++) {
        ball_hx[i + 1] = ball_hx[i];
        ball_hy[i + 1] = ball_hy[i];
    }
    ball_hx[0] = ball_x_;
    ball_hy[0] = ball_y_;

    total_x = 0;
    total_y = 0;
    for (int i = 0; i < BALL_AZIMUTH_SIZE; i++) {
        total_x += ball_hx[i];
        total_y += ball_hy[i];
    }
    ball_azix = total_x / BALL_AZIMUTH_SIZE;
    ball_aziy = total_y / BALL_AZIMUTH_SIZE;

    total_x = 0;
    total_y = 0;
    for (int i = 0; i < BALL_HISTORY_SIZE; i++) {
        total_x += ball_hx[i];
        total_y += ball_hy[i];
    }
    ball_magx = total_x / BALL_HISTORY_SIZE;
    ball_magy = total_y / BALL_HISTORY_SIZE;

    myvector.cal_cord(myvector.get_azimuth(ball_azix, ball_aziy), myvector.get_magnitude(ball_magx, ball_magy));

    ball_x = myvector.get_x();
    ball_y = myvector.get_y();
}

int BALL::get_x() {
    return ball_x;
}

int BALL::get_y() {
    return ball_y;
}