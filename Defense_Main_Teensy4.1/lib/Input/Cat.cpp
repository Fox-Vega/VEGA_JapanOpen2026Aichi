//変更禁止
#include "Cat.h"


void Cat::setup() {
    pinMode(PIN, INPUT);
}

bool Cat::check_catch() {
    int value = analogRead(PIN);
    if (30 > value || value > 350) {
        ocount = 0;
        icount += 1;
        if (icount >= 20) {
            cat = 1;
            start = millis();
        }
    } else {
        icount = 0;
        ocount += 1;
        if (ocount >= 80) {
            if (millis() - start > 400) cat = 0;
        }
    }

    return cat;
}