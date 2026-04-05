#include <Arduino.h>
#include "AIP.h"
#include "Input.h"


void setup() {
    Serial.begin(9600);

    // Serial1.setRx(PA10);
    // Serial1.setTx(PA9);
    Serial2.setRx(RXD2);
    Serial2.setTx(TXD2);
    Serial2.begin(115200);

    pinMode(PA15, OUTPUT);

    ball.setup();
}

void loop() {
    ball.read();
    communicate.teensy_write(1); //座標送信
    // Serial.println(myvector.get_azimuth(ball.get_x(), ball.get_y()));

    // digitalWrite(PA15, HIGH);
    // delay(100);
    // digitalWrite(PA15, LOW);
    // delay(100);
}