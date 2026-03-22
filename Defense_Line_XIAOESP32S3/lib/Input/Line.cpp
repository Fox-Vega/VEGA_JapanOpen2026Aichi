//変更禁止
#include "Line.h"
#include "AIP.h"
#include "Input.h"
#include <driver/gpio.h>

#define GPIO_SET(pin)   (GPIO.out_w1ts = (1UL << (pin)))
#define GPIO_CLR(pin)   (GPIO.out_w1tc = (1UL << (pin)))
#define GPIO_READ(pin)  ((GPIO.in >> (pin)) & 1U)


void LINE::setup() {
    for (int i = 0; i < 3; i++) {
        pinMode(selectPIN[i], OUTPUT);
        pinMode(outputPIN[i], INPUT);
    }
}

void LINE::get_message(byte* message) {
    if (message[1] == 1) {
        detection_border = (message[2] << 8) | message[3];
    }
}

void LINE::read() {
    //初期化
    for (int i = 0; i < 24; i++) {
        line_stat[i] = 0;
        line_stat_[i] = 0;
    }

    //読み取り
    for (int k = 0; k < 3; k++) { //測定回数
        for (int i = 0; i < 8; i++) { //取得ピン切り替え
            if (Reader[link[i]][0]) GPIO_SET(selectPIN[0]); else GPIO_CLR(selectPIN[0]);
            if (Reader[link[i]][1]) GPIO_SET(selectPIN[1]); else GPIO_CLR(selectPIN[1]);
            if (Reader[link[i]][2]) GPIO_SET(selectPIN[2]); else GPIO_CLR(selectPIN[2]);

            delayMicroseconds(3);

            for (int j = 0; j < 3; j++) { //値読み取り
                line_values[(j * 8) + i] = analogRead(outputPIN[j]); //値の保存
                if (line_values[(j * 8) + i] > detection_border) { //ステータスに変換
                    line_stat_[(j * 8) + i] += 1; //仮ステータスに加算
                    if (line_stat_[(j * 8) + i] >= 2) { //最終ステータス
                        line_stat[(j * 8) + i] = 1;
                    }
                }
            }
        }
    }
}

bool LINE::get_stat(byte lineNUM) {
    return line_stat[lineNUM];
}

int LINE::get_value(byte lineNUM) {
    return line_values[lineNUM];
}
