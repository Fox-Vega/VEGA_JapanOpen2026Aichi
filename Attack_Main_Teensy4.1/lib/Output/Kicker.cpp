#include "Kicker.h"


void Kicker::setup(){
    pinMode(charge_PIN,OUTPUT);
    pinMode(discharge_PIN,OUTPUT);
    digitalWrite(charge_PIN,LOW);
    digitalWrite(discharge_PIN,LOW);
}

void Kicker::run(bool kick){
    if (kick == 1){ //キックしたい
        if (MODE == 0 || MODE == 2) { //キック可能
            MODE = 1; //キックする
            LAST_kick = millis();
        }
    }

    if (MODE == 1) {
        if ((millis() - LAST_kick) < 150) { //0.1虚無
            digitalWrite(charge_PIN, LOW);
            digitalWrite(discharge_PIN, LOW);

        } else if((millis() - LAST_kick) < 400) { //リリース
            digitalWrite(charge_PIN, LOW);
            digitalWrite(discharge_PIN, HIGH);

        } else if((millis() - LAST_kick) < 500) { //0.1虚無
            digitalWrite(charge_PIN, LOW);
            digitalWrite(discharge_PIN ,LOW);

        } else if((millis() - LAST_kick) < 3000) { //チャージ
            digitalWrite(charge_PIN, HIGH);
            digitalWrite(discharge_PIN, LOW);

        } else { //アーム
            MODE = 0;
            digitalWrite(charge_PIN, HIGH);
            digitalWrite(discharge_PIN,LOW);

        }
    }
}

void Kicker::arm() {
    if (MODE == 0) {
        MODE = 2;
        LAST_arm = millis();
    }

    if (MODE == 2) {
        if ((millis() - LAST_kick) < 100) { //0.1虚無
            digitalWrite(charge_PIN, LOW);
            digitalWrite(discharge_PIN, LOW);

        } else { //アーム
            digitalWrite(charge_PIN, HIGH);
            digitalWrite(discharge_PIN,LOW);
        }
    }
}