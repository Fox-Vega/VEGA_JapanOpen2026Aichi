//変更禁止
#include "MySwitch.h"


void MySWITCH::setup() {
    pinMode(toggle_PIN, INPUT);
    for (int i = 0; i < 3; i++){
        pinMode(tact_PIN[i], INPUT_PULLUP);
    }
}

int MySWITCH::fetch_toggle() {
    toggle = 0;
    if (digitalRead(toggle_PIN) == HIGH) {
        toggle = 1;
    }
    return toggle;
}

int MySWITCH::fetch_tact() {
    tact = 0;
    for (int i = 0; i < 4; i++) {
        if (digitalRead(tact_PIN[i]) == LOW) {
            if (i == 0) {
                tact += 1;
            } else if (i == 1) {
                tact += 5;
            } else if (i == 2) {
                tact += 9;
            } else {
                tact += 100;
            }
        }
    }
    return tact;
}