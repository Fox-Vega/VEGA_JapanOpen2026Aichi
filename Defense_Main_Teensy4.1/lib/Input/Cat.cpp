//変更禁止
#include "Cat.h"


void Cat::setup() {
    pinMode(PIN, INPUT);
}

bool Cat::check_catch() {
    int value = analogRead(PIN);
    if (20 > value || value > 500) {
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
            // cat = 0;
            if (millis() - start > 300) cat = 0;
        }
    }

    return cat;
}