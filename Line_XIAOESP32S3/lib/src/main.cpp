#include <Arduino.h>
#include "AIP.h"
#include "Input.h"


void setup() {
    Serial.begin(9600); //PC
    Serial1.begin(115200, SERIAL_8N1, RXD1, TXD1); //teensy

    line.setup();
}

void loop() {
    line.read();
    communicate.teensy_write(1);
}

void serialEvent1() { //teensy
    // ID1 = border

    uint8_t message1_read[6] = {0};
    // start ID data1 data2 key end

    while(1) { //無条件
        if (Serial1.available() >= 6) { //個数があれば読み取る
            message1_read[0] = Serial1.read();
            if (message1_read[0] == 195) break; //スタートあれば次に進む
        } else {
            return; //データ個数が不十分なら読み取り中止
        }
    }

    for (int i = 1; i < 6; i++) message1_read[i] = Serial1.read(); //残りの６個を読む

    if(codec.decode(message1_read, 2, 3, 4) != 0) return; //エラー出たら中止

    if (message1_read[5] == 231) line.get_message(message1_read); //データ処理
    else return; //エンドなければ処理しない
}