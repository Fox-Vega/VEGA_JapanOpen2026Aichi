//変更禁止
#include "MyBuzzer.h"
#include "Output.h"


void MyBuzzer::setup() {
    pinMode(buzzer_PIN, OUTPUT);
}

void MyBuzzer::run(int BUZZERnote, int BUZZERduration) {
    if (USE_buzzer == 1) {
        if (BUZZERduration != 999) {
            tone(buzzer_PIN, BUZZERnote, BUZZERduration);
            delay(BUZZERduration * 1.5);
            noTone(buzzer_PIN);
            delay(1);
        } else {
            tone(buzzer_PIN, BUZZERnote);
            delay(1);
        }
    }
}

void MyBuzzer::stop() {
    noTone(buzzer_PIN);
}

void MyBuzzer::preset(int BUZZERpresetNUM) {
    if (USE_buzzer == 1) {
        if (BUZZERpresetNUM == 0) {
            int Melody_Preset1[] = {310, 310, 310, 300, 0, 350, 300, 400};
            int NoteDurs[] = {6, 6, 6, 6, 12, 4, 5, 3};
            int Size_Melody_Preset1 = sizeof(Melody_Preset1)/sizeof(Melody_Preset1[0]);
            for (int playing_Note = 0; playing_Note < Size_Melody_Preset1; playing_Note++) {
                    int noteDur = 1000 / NoteDurs[playing_Note];
                    tone(buzzer_PIN, Melody_Preset1[playing_Note], noteDur);
                    int PBTWNotes = noteDur * 1.4;
                    delay(PBTWNotes);
                    noTone(buzzer_PIN);
            }
            delay(100);
        } else if (BUZZERpresetNUM == 1) {
            int Melody_Preset1[] = {350, 360, 380};
            int NoteDurs[] = {8, 8, 8};
            int Size_Melody_Preset1 = sizeof(Melody_Preset1)/sizeof(Melody_Preset1[0]);
            for (int playing_Note = 0; playing_Note < Size_Melody_Preset1; playing_Note++) {
                    int noteDur = 1000 / NoteDurs[playing_Note];
                    tone(buzzer_PIN, Melody_Preset1[playing_Note], noteDur);
                    int PBTWNotes = noteDur * 1.4;
                    delay(PBTWNotes);
                    noTone(buzzer_PIN);
            }
            delay(100);
        }
    }
}