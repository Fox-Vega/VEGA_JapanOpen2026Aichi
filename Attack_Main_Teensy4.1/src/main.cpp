#include <Arduino.h>
#include <Wire.h>
#include "AIP.h"
#include "Input.h"
#include "Output.h"
#include "Process.h"
#include <EEPROM.h>
#include <unordered_map>

void PrintDebug();

int Sendlate = 75;
bool Running = false;
bool eeprom_target = 0;
std::unordered_map<int, std::array<bool, 4>> switch_states{
    {0, {false, false, false, false}},
    {1, {true, false, false, false}},
    {6, {true, true, false, false}},
    {10, {true, false, true, false}},
    {15, {true, true, true, false}},
    {5, {false, true, false, false}},
    {9, {false, false, true, false}},
    {100, {false, false, false, true}},
    {101, {true, false, false, true}},
    {106, {true, true, false, true}},
    {110, {true, false, true, true}},
    {115, {true, true, true, true}},
    {105, {false, true, false, true}},
    {109, {false, false, true, true}}
};

//変更する
bool rainbow = false;

void setup() {
    analogWriteResolution(8);

    delay(500);
    Wire.begin(); //gyro

    Serial.begin(9600); //PC
    Serial7.begin(115200); //ball
    Serial2.begin(115200); //Cam1
    Serial6.begin(115200); //Cam2
    Serial3.begin(115200); //line
    Serial8.begin(115200); //UI

    pinMode(LED_BUILTIN, OUTPUT);

    Serial.println("Setup Start");
    digitalWrite(LED_BUILTIN, HIGH);

    cat.setup();
    Serial.println("1 of 7");
    gam.setup();
    Serial.println("2 of 7");
    myswitch.setup();
    Serial.println("3 of 7");
    kicker.setup();
    Serial.println("4 of 7");
    mymotor.setup();
    Serial.println("5 of 7");
    mybuzzer.setup();
    Serial.println("6 of 7");
    line.setup();
    Serial.println("7 of 7");
    Serial.println();

    // Serial.println("Set main_line border");
    // communicate.line_write(1); //ライン閾値送信
    // delay(250);
    // Serial.println("Set side_line border");
    // communicate.line_write(2); //ライン閾値送信
    // Serial.println();

    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Mk.3 - VEGA2026");

    mybuzzer.run(1760, 90);
    mybuzzer.run(1800, 90);
    mybuzzer.run(1860, 90);
    // mybuzzer.preset(0); //マツケン

    mypixel.set_mode(0);
    if(myswitch.fetch_tact()!=0){
        mypixel.set_mode(1);
        mypixel.multi(9, 15, 7);
        mypixel.show();
        delay(200);
        mypixel.multi(9, 15, 0);
        mypixel.show();
    }
    mypixel.set_brightness(75);
    mypixel.multi(0, 23, 9);

    line.side_border = 420;
    communicate.line_write(2);

    byte ledpwm{};
    EEPROM.get(0, ledpwm);
    line.set_led(ledpwm);
    Serial.println("LED: " + String(ledpwm));

    byte t;
    EEPROM.get(1, t);
    eeprom_target = (bool)t;
    cam.set_goal(eeprom_target);
}

void WaitUntilTactReleased(){
    int currentTact = myswitch.fetch_tact();
    while((currentTact%100 )!=0){
        yield();
        currentTact = myswitch.fetch_tact();
    }
};

static uint32_t lastKickerTime = 0;
int tact = 0;
Communicate::Mode UIstate{};

void PostData(){
        UIstate.cam = cam.target;
        UIstate.gamreset = (tact == 5);
        UIstate.rainbow = rainbow;
        if (switch_states.count(tact)) {
            UIstate.switch_ = switch_states.at(tact);
        } else {
            UIstate.switch_.fill(false);
        }
        UIstate.latency = Sendlate;
        UIstate.LineBorder = tact == 101;
        UIstate.Kicking = (millis() - lastKickerTime < 700)&&lastKickerTime!=0;
        UIstate.LineLight = tact == 109;
        communicate.setUI(UIstate);

        //共通UI送信
        communicate.ui_write(50);
        //個別UI送信
        communicate.ui_write(103);
        //起動状態の送信
        communicate.setParam(false);
        communicate.ui_write(51);
}

void loop() {
    cat.fetch();
    gam.read_azimuth();
    mypixel.clear();

    if (myswitch.fetch_toggle() == 1) {
        if(!Running){
            communicate.setParam(true);
            communicate.ui_write(51);
            Running = true;
            if (eeprom_target != cam.target) EEPROM.put(1, (byte)cam.target);
        }

        line.set_led(999);
        mymotor.set_motor(1);
        kicker.run(0);

        // kicker.run(1);
        // if (cam.get_x(0) != 999) mymotor.run(180-cam.get_x(0), 180, 0);
        // else mymotor.run(0, 180, 0);
        attack.attack_();

    } else {
        if(Running){
            communicate.setParam(false);
            Serial8.flush();
            delay(50);
            communicate.ui_write(51);
            Running = false;
        }

        if (!line.autosetborder) line.set_led(0);
        mymotor.set_motor(0);
        mymotor.set_difix(0);
        kicker.run(0);

        mymotor.free();
        attack.reset();

        // kicker.arm();
        // mypixel.set_mode(1);
        if (rainbow) {
            mypixel.rainbow();
        } else {
            mypixel.multi(0, 23, 0);
            mypixel.show();
        }

        uint32_t startTime = millis();
        tact = myswitch.fetch_tact();
        PostData();
        while ((int)(millis() - startTime) < Sendlate) {
            PrintDebug();
        }

        if (tact == 1) {
            cam.target = !cam.target;
            UIstate.cam = cam.target;
            cam.set_goal(cam.target);
            mybuzzer.run(cam.target==0 ? 1600 : 1800, 60);
            mypixel.multi(0, 23, cam.target==0 ? 3 : 4);
            mypixel.show();
            PostData();
            WaitUntilTactReleased();
        } else if (tact == 5) {
            gam.set_zero();
            mybuzzer.run(1600, 60);
            WaitUntilTactReleased();
        } else if (tact==9){
            rainbow = !rainbow;
            PostData();
            WaitUntilTactReleased();
        } else if (tact == 101) {
            communicate.line_write(3);
            line.autosetborder = true;
            PostData();
            WaitUntilTactReleased();
        } else if (tact == 105) {
            kicker.run(1);
            lastKickerTime = millis();
            PostData();
            WaitUntilTactReleased();
        } else if (tact == 109) {
            line.set_led(999);
            PostData();
            WaitUntilTactReleased();
        }
    }

    mypixel.show();
}

void PrintDebug() {
    Serial.print("Line: ");
    for (int i = 0; i < 24; i++) Serial.print(line.get_main(i));
    Serial.print(" ");
    // for (int i = 0; i < 3; i++) Serial.print(line.get_side(i));
    Serial.print("  Line_azi: " + String(line.get_azimuth()));
    // Serial.print("  sideNUM: " + String(line.get_sideNUM()));
    Serial.print("  Cat: " + String(cat.fetch()));
    Serial.print("  Cam_x: " + String(cam.get_x(1)));
    Serial.print("  Cam_ax: " + String(cam.get_ax(1)));
    Serial.print("  Cam_height: " + String(cam.get_height(1)));
    Serial.print("  BCam_x: " + String(cam.get_x(0)));
    Serial.print("  Ball_azi: " + String(ball.get_azimuth()));
    Serial.println("  Ball_int: " + String(ball.get_intensity()));
}


// ====================================== UART ======================================

void serialEvent7() { //Ball
    int size = 8;
    uint8_t message7_read[size]{};
    // start ID x1 x2 y1 y2 key end

    while(1) {
        if (Serial7.available() >= size) {
            message7_read[0] = Serial7.read();
            if (message7_read[0] == 195) break;
        } else {
            return;
        }
    }

    for (int i = 1; i < size; i++) message7_read[i] = Serial7.read();

    if(codec.decode(message7_read, 2, 5, 6) != 0) return;

    if (message7_read[(size - 1)] == 231) ball.read_message(message7_read);
    else return;
}

void serialEvent2() { //Cam1
    int size = 7;
    uint8_t message2_read[size]{};
    // start ID x ax width height end

    while(1) {
        if (Serial2.available() >= size) {
            message2_read[0] = Serial2.read();
            if (message2_read[0] == 195) break;
        } else {
            return;
        }
    }

    for (int i = 1; i < size; i++) message2_read[i] = Serial2.read();

    // Serial.println(
    //     "Cam1( "
    //     + String(message2_read[1]) + ","
    //     + String(message2_read[2]) + ","
    //     + String(message2_read[3]) + ","
    //     + String(message2_read[4]) + ","
    //     + String(message2_read[5]) + " )"
    // );

    if (message2_read[(size - 1)] == 231) cam.read_message(message2_read, 0);
    else return;
}

void serialEvent6() { //Cam2
    int size = 6;
    uint8_t message6_read[size]{};
    // start ID x width height end

    while(1) {
        if (Serial6.available() >= size) {
            message6_read[0] = Serial6.read();
            if (message6_read[0] == 195) break;
        } else {
            return;
        }
    }

    for (int i = 1; i < size; i++) message6_read[i] = Serial6.read(); //残りを読む

    // Serial.println(
    //     "Cam2( "
    //     + String(message6_read[1]) + ","
    //     + String(message6_read[2]) + ","
    //     + String(message6_read[3]) + ","
    //     + String(message6_read[4]) + " )"
    // );

    if (message6_read[(size - 1)] == 231) cam.read_message(message6_read, 1);
    else return;
}

void serialEvent3() { //Line
    int size = 7;
    uint8_t message3_read[size]{};
    // start ID data1 data2 data3 key end
    // ID1 = data
    // Serial.print(millis() - LT);
    while(1) {
        if (Serial3.available() >= size) {
            message3_read[0] = Serial3.read();
            if (message3_read[0] == 195) break;
        } else {
            return;
        }
    }

    for (int i = 1; i < size; i++) message3_read[i] = Serial3.read();

    if(codec.decode(message3_read, 2, 4, 5) != 0) return;

    if (message3_read[(size - 1)] == 231) communicate.line_read(message3_read);
    // if (message3_read[(size - 1)] == 231) line.read_message(message3_read);
    else return;
}

void serialEvent8(){
    int size = 7;
    uint8_t message8_read[size]{};

    while(1) {
        if (Serial8.available() >= size) {
            message8_read[0] = Serial8.read();
            if (message8_read[0] == 195) break;
        } else {
            return;
        }
    }

    for (int i = 1; i < size; i++) message8_read[i] = Serial8.read();

    if(codec.decode(message8_read, 1, 4, 5) != 0) return;

    if (message8_read[(size - 1)] == 231) {
        int ID = message8_read[1];
        if (ID==120) Sendlate += 20;
        else if (ID==121)Sendlate -= 5;
        Sendlate = constrain(Sendlate, 10, 200);
    }
    else return;
}

// ====================================== MEMO ======================================


//UART受信について
//ボールはデータ量８　カギ２５６
//ラインはデータ量７　カギ２４５
//前カムはデータ量７　カギ無し
//後カムはデータ量６　カギ無し

//UART送信について
//ラインはデータ量６　カギ２３４
//ユアイはデータ量８　カギ２５６