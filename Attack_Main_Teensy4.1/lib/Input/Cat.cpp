//変更禁止
#include "Cat.h"


void Cat::setup() {
    pinMode(cat_PIN, INPUT);
}

bool Cat::fetch() {
    cat_value = analogRead(cat_PIN);
    if (cat_value < 250) {
        ocount = 0;
        icount += 1;
        if (icount >= 20) {
            cat = 1;
            LAST_cat = millis();
        }
    } else {
        icount = 0;
        ocount += 1;
        if (ocount >= 100) {
            // cat = 0;
            st = millis() - LAST_cat;
            if (st >= 0) cat = 0;
        }
    }

    return cat;
}