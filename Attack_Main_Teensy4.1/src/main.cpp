#include <Arduino.h>
#include <Wire.h>
#include "AIP.h"
#include "Input.h"
#include "Output.h"
#include "Process.h"
#include <EEPROM.h>


void setup() {
    analogWriteResolution(8);

    delay(500);
    Wire.begin(); //gyro

    Serial.begin(9600); //PC
    Serial7.begin(115200); //ball
    Serial2.begin(115200); //Cam1
    Serial6.begin(115200); //Cam2
    Serial3.begin(115200); //line
    Serial8.begin(115200); //UI

    pinMode(LED_BUILTIN, OUTPUT);

    Serial.println("Setup Start");
    digitalWrite(LED_BUILTIN, HIGH);

    cat.setup();
    Serial.println("1 of 7");
    gam.setup();
    Serial.println("2 of 7");
    myswitch.setup();
    Serial.println("3 of 7");
    kicker.setup();
    Serial.println("4 of 7");
    mymotor.setup();
    Serial.println("5 of 7");
    mybuzzer.setup();
    Serial.println("6 of 7");
    line.setup();
    Serial.println("7 of 7");
    Serial.println();

    // Serial.println("Set main_line border");
    // communicate.line_write(1); //ライン閾値送信
    // delay(250);
    // Serial.println("Set side_line border");
    // communicate.line_write(2); //ライン閾値送信
    // Serial.println();

    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Mk.3 - VEGA2026");

    mybuzzer.run(1750, 100);
    mybuzzer.run(1800, 100);
    mybuzzer.run(1850, 100);
    // mybuzzer.preset(0); //マツケン

    mypixel.set_mode(1); //Neopixel ON/OFF
    mypixel.set_brightness(30);
    mypixel.multi(0, 23, 9);

    byte ledpwm{};
    EEPROM.get(0, ledpwm);
    line.set_led(ledpwm);
    Serial.println("LED: " + String(ledpwm));
}

void loop() {
    mypixel.multi(0, 23, 9);
    cat.fetch();
    gam.read_azimuth();

    if (myswitch.fetch_toggle() == 1) {
        line.set_led(999);
        mymotor.set_motor(1);
        mymotor.set_difix(1);
        kicker.run(0);

        // kicker.run(1);
        // mymotor.run(0, 180, 0);
        attack.attack_();
        //ここに　Defense

    } else {
        if (!line.autosetborder) line.set_led(0);
        mymotor.set_motor(0);
        mymotor.set_difix(0);
        kicker.run(0);

        mymotor.free();
        // kicker.arm();
        // mypixel.set_mode(1);
        //処理あればここに

        // mypixel.rainbow();

        if (myswitch.fetch_tact() == 1) {
            if (cam.target == 0) {
                cam.set_goal(1);
                mybuzzer.run(1800, 60);
                mypixel.multi(0, 23, 4);
                mypixel.show();
                delay(500);
            } else {
                cam.set_goal(0);
                mybuzzer.run(1400, 60);
                mypixel.multi(0, 23, 3);
                mypixel.show();
                delay(500);
            }
        } else if (myswitch.fetch_tact() == 5) {
            gam.set_zero();
            mybuzzer.run(1600, 60);
            delay(100);

        //開始位置設定ここに置こうかな

        } else if (myswitch.fetch_tact() == 101) {
            communicate.line_write(3);
            line.autosetborder = true;
            mybuzzer.run(800, 200);
        } else if (myswitch.fetch_tact() == 105) {
            kicker.run(1);
            mybuzzer.run(1200, 100);
        } else if (myswitch.fetch_tact() == 109) {
            line.set_led(999);
        }

        //シリアルゾーン
        Serial.print("Line: ");
        for (int i = 0; i < 24; i++) Serial.print(line.get_main(i));
        Serial.print("  Lazi: " + String(line.get_azimuth()));
        Serial.print("  sideNUM: " + String(line.get_sideNUM()));
        Serial.print("  Cat: " + String(cat.fetch()));
        Serial.print("  Cam_x: " + String(cam.get_x(1)));
        Serial.print("  Cam_ax: " + String(cam.get_ax(1)));
        Serial.print("  Ball_azi: " + String(ball.get_azimuth()));
        Serial.println("  Ball_int: " + String(ball.get_intensity()));

        // mypixel.multi(0, 23, 9);
        // mypixel.closest(ball.get_azimuth(), 1, 1);
    }
    mypixel.show();
}



// ====================================== UART ======================================

void serialEvent7() { //Ball
    int size = 8;
    uint8_t message7_read[size]{};
    // start ID x1 x2 y1 y2 key end

    while(1) {
        if (Serial7.available() >= size) {
            message7_read[0] = Serial7.read();
            if (message7_read[0] == 195) break;
        } else {
            return;
        }
    }

    for (int i = 1; i < size; i++) message7_read[i] = Serial7.read();

    if(codec.decode(message7_read, 2, 5, 6) != 0) return;

    if (message7_read[(size - 1)] == 231) ball.read_message(message7_read);
    else return;
}

void serialEvent2() { //Cam1
    int size = 7;
    uint8_t message2_read[size]{};
    // start ID x ax width height end

    while(1) {
        if (Serial2.available() >= size) {
            message2_read[0] = Serial2.read();
            if (message2_read[0] == 195) break;
        } else {
            return;
        }
    }

    for (int i = 1; i < size; i++) message2_read[i] = Serial2.read();

    // Serial.println(
    //     "Cam1( "
    //     + String(message6_read[1]) + ","
    //     + String(message6_read[2]) + ","
    //     + String(message6_read[3]) + ","
    //     + String(message6_read[4]) + ","
    //     + String(message6_read[5]) + " )"
    // );

    if (message2_read[(size - 1)] == 231) cam.read_message(message2_read, 0);
    else return;
}

void serialEvent6() { //Cam2
    int size = 6;
    uint8_t message6_read[size]{};
    // start ID x width height end

    while(1) {
        if (Serial6.available() >= size) {
            message6_read[0] = Serial6.read();
            if (message6_read[0] == 195) break;
        } else {
            return;
        }
    }

    for (int i = 1; i < size; i++) message6_read[i] = Serial6.read(); //残りを読む

    // Serial.println(
    //     "Cam2( "
    //     + String(message2_read[1]) + ","
    //     + String(message2_read[2]) + ","
    //     + String(message2_read[3]) + ","
    //     + String(message2_read[4]) + " )"
    // );

    if (message6_read[(size - 1)] == 231) cam.read_message(message6_read, 1);
    else return;
}

void serialEvent3() { //Line
    int size = 7;
    uint8_t message3_read[size]{};
    // start ID data1 data2 data3 key end
    // ID1 = data
    // Serial.print(millis() - LT);
    while(1) {
        if (Serial3.available() >= size) {
            message3_read[0] = Serial3.read();
            if (message3_read[0] == 195) break;
        } else {
            return;
        }
    }

    for (int i = 1; i < size; i++) message3_read[i] = Serial3.read();

    if(codec.decode(message3_read, 2, 4, 5) != 0) return;

    if (message3_read[(size - 1)] == 231) communicate.line_read(message3_read);
    // if (message3_read[(size - 1)] == 231) line.read_message(message3_read);
    else return;
}

// ====================================== MEMO ======================================


//UART受信について
//ボールはデータ量８　カギ２５６
//ラインはデータ量７　カギ２４５
//前カムはデータ量７　カギ無し
//後カムはデータ量６　カギ無し

//UART送信について
//ラインはデータ量６　カギ２３４
//ユアイはデータ量８　カギ２５６