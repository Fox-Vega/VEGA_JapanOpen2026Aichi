//変更禁止
#include "Line.h"
#include "AIP.h"
#include "Input.h"
#include <driver/gpio.h>

#define GPIO_SET(pin)   (GPIO.out_w1ts = (1UL << (pin)))
#define GPIO_CLR(pin)   (GPIO.out_w1tc = (1UL << (pin)))
#define GPIO_READ(pin)  ((GPIO.in >> (pin)) & 1U)

Preferences prefs;

void LINE::setup() {
    for (int i = 0; i < 3; i++) {
        pinMode(select_PIN[i], OUTPUT);
        pinMode(output_PIN[i], INPUT);
        pinMode(side_PIN[i], INPUT);
    }

    prefs.begin("lineborder", true);  // 読み取り専用
    main_border = prefs.getInt("main", 0);
    side_border = prefs.getInt("side", 0);
    prefs.end();
}

void LINE::read_message(byte* message) {
    if (message[1] == 1) {
        main_border = (message[2] << 8) | message[3];
    } else if (message[1] == 2) {
        side_border = (message[2] << 8) | message[3];
    } else if (message[1] == 3) {
        line.set_border(1);
    }
}

void LINE::read() {
    //初期化
    for (int i = 0; i < 24; i++) {
        main_stat[i] = 0;
        main_stat_[i] = 0;
    }
    for (int i = 0; i < 3; i++) {
        side_stat[i] = 0;
        side_stat_[i] = 0;
    }

    //読み取り
    for (int k = 0; k < 3; k++) { //測定回数
        for (int i = 0; i < 8; i++) { //取得ピン切り替え
            if (reader[i][0]) GPIO_SET(select_PIN[0]); else GPIO_CLR(select_PIN[0]);
            if (reader[i][1]) GPIO_SET(select_PIN[1]); else GPIO_CLR(select_PIN[1]);
            if (reader[i][2]) GPIO_SET(select_PIN[2]); else GPIO_CLR(select_PIN[2]);

            for (int j = 0; j < 3; j++) { //値読み取り
                main_value[link[(j * 8) + i]] = analogRead(output_PIN[j]); //値の保存
                if (main_value[link[(j * 8) + i]] > main_border) { //ステータスに変換
                    main_stat_[link[(j * 8) + i]] += 1; //仮ステータスに加算
                    if (main_stat_[link[(j * 8) + i]] >= 2) { //最終ステータス
                        main_stat[link[(j * 8) + i]] = 1;
                    }
                }
            }
        }
        for (int i = 0; i < 3; i++) {
            side_value[i] = analogRead(side_PIN[i]);
            if (side_value[i] > side_border) {
                side_stat_[i] += 1;
                if (side_stat_[i] >= 1) {
                    side_stat[i] = 1;
                }
            }
        }
    }

    bool stable_ = 1;
    for (int i = 0; i < 24; i++) {
        if (oldmain_stat[i] != main_stat[i]) stable_ = 0;
        oldmain_stat[i] = main_stat[i];
    }
    for (int i = 0; i < 3; i++) {
        if (oldside_stat[i] != side_stat[i]) stable_ = 0;
        oldside_stat[i] = side_stat[i];
    }
    if (stable_) {
        stable += 1;
        if (stable > 50) stable = 50;
    } else {
        stable = 0;
    }

    // Serial.print(side_value[0]);
    // Serial.print(" ");
    // Serial.println(side_value[1]);
}

void LINE::set_border(bool set) {
    if (set) {
        if (MODE == 0) {
            MODE = 1;
        }
    }
    if (MODE == 1) {
        //光量取得
        MAX_value = 0;
        for (int i = 0; i < 24; i++) {
            if (MAX_value < main_value[i]) {
                MAX_value = main_value[i];
            }
        }
        for (int i = 0; i < 3; i++) {
            if (MAX_value < side_value[i]) {
                MAX_value = side_value[i];
            }
        }

        //光量調整
        if (MAX_value < 2950) {
            communicate.teensy_write(11);
            delay(20);
        } else if (3050 < MAX_value) {
            communicate.teensy_write(10);
            delay(20);
        } else {
            MODE = 2;
        }

    } else if (MODE == 2) {
        int MIN_mainvalue = 9999;
        int MAX_mainvalue = 0;
        int MIN_sidevalue = 9999;
        int MAX_sidevalue = 0;

        for (int i = 0; i < 24; i++) {
            if (MAX_mainvalue < main_value[i]) {
                MAX_mainvalue = main_value[i];
            }
            if (MIN_mainvalue > main_value[i]) {
                MIN_mainvalue = main_value[i];
            }
        }
        for (int i = 0; i < 3; i++) {
            if (MAX_sidevalue < side_value[i]) {
                MAX_sidevalue = side_value[i];
            }
            if (MIN_sidevalue > side_value[i]) {
                MIN_sidevalue = side_value[i];
            }
        }

        MAX_maintotal += MAX_mainvalue;
        MIN_maintotal += MIN_mainvalue;
        MAX_sidetotal += MAX_sidevalue;
        MIN_sidetotal += MIN_sidevalue;
        step += 1;

        if (step >= 50) {
            MAX_maintotal = MAX_maintotal / step;
            MIN_maintotal = MIN_maintotal / step;
            MAX_sidetotal = MAX_sidetotal / step;
            MIN_sidetotal = MIN_sidetotal / step;

            // main_border = (MIN_mainvalue + MAX_mainvalue) / 2;
            main_border = ((MIN_maintotal + MAX_maintotal) / 2) - ((MAX_maintotal - MIN_maintotal) / 6);
            // side_border = (MIN_sidevalue + MAX_sidevalue) / 2;
            // side_border = ((MIN_sidetotal + MAX_sidetotal) / 2) + ((MAX_sidetotal - MIN_sidetotal) / 1.1);

            prefs.begin("lineborder", false);
            prefs.putInt("main", main_border);
            prefs.putInt("side", side_border);
            prefs.end();

            MAX_maintotal = 0;
            MIN_maintotal = 0;
            MAX_sidetotal = 0;
            MIN_sidetotal = 0;
            step = 0;

            MODE = 0;
            communicate.teensy_write(12);
        }
    }
}