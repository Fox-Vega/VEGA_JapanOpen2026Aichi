#include <Arduino.h>
#include <Wire.h>
#include "AIP.h"
#include "Input.h"
#include "Output.h"
#include "Process.h"


void setup() {
    analogWriteResolution(8);

    Wire.begin(); //gyro

    Serial.begin(9600); //PC
    Serial5.begin(115200); //ball
    Serial6.begin(115200); //Cam1
    Serial2.begin(115200); //Cam2
    Serial3.begin(115200); //line
    Serial7.begin(115200); //UI
    delay(100);

    Serial.println("Setup Start");

    pinMode(LED_BUILTIN, OUTPUT);
    cat.setup();
    gam.setup();
    myswitch.setup();
    mymotor.setup();
    mybuzzer.setup();

    mybuzzer.start(1600, 90);
    mybuzzer.start(1800, 90);
    mybuzzer.start(2000, 90);
    communicate.line_write(1); //ライン閾値送信
}

void loop() {
    cat.check_catch();
    gam.read_azimuth();

    if (myswitch.check_toggle() == 1) {
        mymotor.lock(0);
        mymotor.stabilization(1);

        attack.attack_();
    } else {
        mymotor.lock(1);
        mymotor.stabilization(0);

        mymotor.free();

        if (myswitch.check_tact() == 1) {
            cam.set(0);
            mybuzzer.start(1300, 90);
            delay(150);
        } else if (myswitch.check_tact() == 9) {
            cam.set(1);
            mybuzzer.start(1900, 90);
            delay(150);
        }
        if (myswitch.check_tact() == 5) {
            gam.dir_reset();
            mybuzzer.start(1600, 90);
            delay(150);
        }

        // for (int i = 0; i < 24; i++) {
        //     Serial.print(line.get_stat(i));
        //     Serial.print(" ");
        // }
        // Serial.print(line.get_eazimuth());

        // Serial.println(ball.get_intensity());

        // Serial.println(cam.get_x(0));
        // Serial.println(cam.get_height(1));
    }
}

// ====================================== UART ======================================

void serialEvent4() { //ball
    // start ID x1 x2 y1 y2 key end

    uint8_t message4_read[8]{};

    while(1) { //無条件
        if (Serial4.available() >= 8) {
            message4_read[0] = Serial4.read();
            if (message4_read[0] == 195) break; //スタートあれば次に進む
        } else {
            return; //データ個数が不十分なら読み取り中止
        }
    }

    for (int i = 1; i < 8; i++) message4_read[i] = Serial4.read(); //残りを読む

    if(codec.decode(message4_read, 2, 5, 6) != 0) return; //エラー出たら中止

    if (message4_read[7] == 231) ball.get_message(message4_read); //データ処理
    else return; //エンドなければ処理しない
}

void serialEvent6() { //cam1
    // start ID x tarx width height end
    // ID ０＝なし
    // ID １＝青
    // ID ２＝黄色

    uint8_t message6_read[7]{};

    while(1) { //無条件
        if (Serial6.available() >= 7) { //個数があれば読み取る
            message6_read[0] = Serial6.read();
            if (message6_read[0] == 195) break; //スタートあれば次に進む
        } else {
            return; //データ個数が不十分なら読み取り中止
        }
    }

    for (int i = 1; i < 7; i++) message6_read[i] = Serial6.read(); //残りを読む

    // Serial.print("Cam1 ( ");
    // Serial.println(
    //     "Cam1 ("
    //     + String(message6_read[1])
    //     + ","
    //     + String(message6_read[2])
    //     + ","
    //     + String(message6_read[3])
    //     + ","
    //     + String(message6_read[4])
    //     + ","
    //     + String(message6_read[5])
    //     + " )"
    // );

    if (message6_read[6] == 231) cam.get_message(message6_read, 0); //データ処理
    else return; //エンドなければ処理しない
}

void serialEvent2() { //cam2
    // start ID x width height end
    // ID ０＝なし
    // ID １＝青
    // ID ２＝黄色

    uint8_t message2_read[6]{};

    while(1) { //無条件
        if (Serial2.available() >= 6) { //個数があれば読み取る
            message2_read[0] = Serial2.read();
            if (message2_read[0] == 195) break; //スタートあれば次に進む
        } else {
            return; //データ個数が不十分なら読み取り中止
        }
    }

    for (int i = 1; i < 6; i++) message2_read[i] = Serial2.read(); //残りを読む

    if (message2_read[5] == 231) cam.get_message(message2_read, 1); //データ処理
    else return; //エンドなければ処理しない
}

void serialEvent3() { //line
    // start ID data1 data2 data3 key end
    // ID １＝圧縮データ

    uint8_t message3_read[7]{};

    while(1) { //無条件
        if (Serial3.available() >= 7) { //個数があれば読み取る
            message3_read[0] = Serial3.read();
            if (message3_read[0] == 195) break; //スタートあれば次に進む
        } else {
            return; //データ個数が不十分なら読み取り中止
        }
    }

    for (int i = 1; i < 7; i++) message3_read[i] = Serial3.read(); //残りを読む

    if(codec.decode(message3_read, 2, 4, 5) != 0) return; //エラー出たら中止

    if (message3_read[6] == 231) line.get_message(message3_read); //データ処理
    else return; //エンドなければ処理しない
}
