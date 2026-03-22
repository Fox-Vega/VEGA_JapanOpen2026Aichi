#include <Arduino.h>
#include "AIP.h"
#include "Input.h"


void setup() {
    Serial.begin(9600);
    Serial1.begin(115200, SERIAL_8N1, RXD1, TXD1);

    ball.setup();
}

void loop() {
    ball.read();
    communicate.teensy_write(1); //座標送信
    // Serial.println(myvector.get_azimuth(ball.get_x(), ball.get_y()));
}