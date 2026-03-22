//変更禁止
#include "MySwitch.h"


void MySWITCH::setup() {
    pinMode(toggleswitchPIN, INPUT);
    for (int i = 0; i < 3; i++){
        pinMode(tactswitchPIN[i], INPUT_PULLUP);
    }
}

int MySWITCH::check_toggle() {
    toggle_stat = 0;
    if (digitalRead(toggleswitchPIN) == HIGH) {
        toggle_stat = 1;
    }
    return toggle_stat;
}

int MySWITCH::check_tact() {
    pushed_tact = 0;
    for (int i = 0; i < 3; i++) {
        if (digitalRead(tactswitchPIN[i]) == LOW) {
            if (i == 0) {
                pushed_tact += 1;
            } else if (i == 1) {
                pushed_tact += 5;
            } else {
                pushed_tact += 9;
            }
        }
    }
    return pushed_tact;
}