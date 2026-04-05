#include <Arduino.h>
#include <Wire.h>
#include <array>
#include <unordered_map>
#include <EEPROM.h>
#include "AIP.h"
#include "Input.h"
#include "Output.h"
#include "Process.h"

int Sendlate = 50;
int mode = 0;
bool UsePixel = false;
bool IsProcess= false;
bool rainbow =false;
int tact;

constexpr uint16_t CTRL_DOUBLE_CLICK_MS = 500;
bool ctrlAwaitSecond = false;
int prevTact = 0;
Timer CTRLtimer;

//myswitch は　左から順に +1, +5, +9, +100

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

void ClearPixel(){
    mypixel.multi(0, 23, MyPIXEL::COLOR::NONE);
    mypixel.show();
}

bool camC=1;
void setup() {
    delay(300);
    mybuzzer.setup();
    mybuzzer.start(1400, 90);
    Wire.begin(); //gyro
    Serial.begin(9600); //PC
    Serial7.begin(115200); //ball
    Serial2.begin(115200); //Cam1
    Serial6.begin(115200); //Cam2
    Serial3.begin(115200); //line
    Serial8.begin(115200); //UI
    analogWriteResolution(8);
    Serial.println("Setup Start");

    pinMode(LED_BUILTIN, OUTPUT);
    cat.setup();
    gam.setup();
    myswitch.setup();
    mymotor.setup();
    mypixel.setup();
    
    mybuzzer.start(1600, 90);
    mybuzzer.start(1800, 90);
    mybuzzer.start(2000, 90);
    communicate.line_write(1); //閾値
    defense.setup();
    randomSeed(analogRead(A0));
    if(myswitch.check_tact()!=0){
        UsePixel=true;
        delay(200);
        mybuzzer.start(1000, 90);
        delay(10);
        mybuzzer.start(1200, 90);
    }
    mypixel.set_mode(UsePixel);
    uint8_t camRaw = EEPROM.read(24);
    camC = (camRaw != 0);
    cam.set(camC);
    ClearPixel();
}

void serialEvent8(){
    // Serial.println("SerialEvent8");
    int size = 7;
    uint8_t message8_read[size]{};

    bool startFound8 = false;
    for (int i = 0; i < 20; i++) {
        if (Serial8.available() >= size) {
            message8_read[0] = Serial8.read();
            if (message8_read[0] == 195) {
                startFound8 = true;
                break;
            }
        } else {
            return;
        }
    }
    if (!startFound8) return;
    
    for (int i = 1; i < size; i++) message8_read[i] = Serial8.read();
    
    if(codec.decode(message8_read, 1, 4, 5) != 0) return;
    
    if (message8_read[(size - 1)] == 231) {
        int ID = message8_read[1];
        if(ID==120&&!IsProcess){
            Sendlate += 20;
        }
        if(ID==121&&!IsProcess){
            Sendlate -= 5;
        }
        Sendlate = constrain(Sendlate, 10, 200);
    }
    else return;
}

bool detectCtrlDoubleClick(int currentTact){
    bool triggered = false;
    if(ctrlAwaitSecond && currentTact != 100 && CTRLtimer.read_milli() > CTRL_DOUBLE_CLICK_MS){
        ctrlAwaitSecond = false;
    }
    if(currentTact == 100 && prevTact != 100){
        if(ctrlAwaitSecond && CTRLtimer.read_milli() <= CTRL_DOUBLE_CLICK_MS){
            triggered = true;
            ctrlAwaitSecond = false;
            CTRLtimer.reset();
        } else {
            ctrlAwaitSecond = true;
            CTRLtimer.reset();
        }
    } else if(currentTact != 100 && currentTact != 0){
        ctrlAwaitSecond = false;
    }
    prevTact = currentTact;
    return triggered;
}

void waitUntilTactNotPress(){
    int currentTact = myswitch.check_tact();
    while((currentTact%100 )!=0){
        yield();
        currentTact = myswitch.check_tact();
    }
}





Communicate::Mode uiState;

void postData(){
    uiState = {};
    uiState.cam = camC;
    uiState.rainbow = rainbow;
    uiState.usepixel = UsePixel;
    uiState.gamreset = (tact == 5);
    if (switch_states.count(tact)) {
        uiState.switch_ = switch_states.at(tact);
    } else {
        uiState.switch_.fill(false);
    }
    uiState.latency = Sendlate;
    uiState.mode = mode;
    uiState.buzzer = mybuzzer.useBUZZER;
    communicate.setUI(uiState);
    //同期　共通UIの送信
    communicate.ui_write(50);
    //個別UIの送信
    communicate.ui_write(102);

    //起動状態の送信
    communicate.setParam(false);
    communicate.ui_write(51);
}

void loop() {
    if(rainbow) mypixel.rainbow();
    else {
        mypixel.clear();
    };
    cat.check_catch();
    gam.read_azimuth();
    if (myswitch.check_toggle() == 1) {
        if(!IsProcess){
            communicate.setParam(true);
            Serial8.flush();
            communicate.ui_write(51);
            EEPROM.update(24, static_cast<uint8_t>(camC));
            IsProcess = true;
        }
        mymotor.lock(0);
        mymotor.set_difix(1);
        if(mode == 0) defense.defense_();
        else if(mode == 1) attack.attack_();
        else if(mode == 2) defense.DebugF();
    } else {
        if(IsProcess){
            CTRLtimer.reset();
            mybuzzer.stop();
            communicate.setParam(false);
            communicate.ui_write(51);
            IsProcess = false;
        }
        tact = myswitch.check_tact();
        uint32_t startTime = millis();
        uiState = {};
        // bool ctrlDoubleClickPending = false;
        while((int)(millis() - startTime) < Sendlate) {
            yield();
            tact = myswitch.check_tact();
            // if(detectCtrlDoubleClick(tact)){
            //     ctrlDoubleClickPending = true;
            // }
        }
        // if(detectCtrlDoubleClick(tact)){
        //     ctrlDoubleClickPending = true;
        // }
        // if(ctrlDoubleClickPending){
        //     mybuzzer.preset(0);
        // }
        mymotor.free();
        mymotor.lock(1);
        mymotor.set_difix(0);
        defense.reset();

        if (tact == 1) {
            camC=camC==0?1:0;
            cam.set(camC);
            mypixel.multi(0, 23, camC==0?MyPIXEL::COLOR::BLUE:MyPIXEL::COLOR::YELLOW);
            mypixel.show();
            mybuzzer.start(camC==0 ? 600 : 1900, 90);

            Serial.print("Camera switched to: ");
            Serial.println(camC?"Yellow":"Blue");
            postData();
            waitUntilTactNotPress();
            ClearPixel();
        }
        if (tact == 5) {
            gam.dir_reset();
            mybuzzer.start(1600, 90);
            postData();
            waitUntilTactNotPress();
        }
        if (tact == 9) {
            rainbow=!rainbow;
            postData();
            if(rainbow){
                mybuzzer.start(1800, 90);
                delay(10);
                mybuzzer.start(1900, 90);
                delay(10);
                mybuzzer.start(2000, 90);
            } else {
                ClearPixel();
                mybuzzer.start(2000, 90);
                delay(10);
                mybuzzer.start(1900, 90);
                delay(10);
                mybuzzer.start(1800, 90);
                mypixel.set_mode(true);
                ClearPixel();
                mypixel.set_mode(UsePixel);
                if(random(9) == 0){//10％の確率でデデドン
                    delay(300);
                    mybuzzer.start(2000, 90);
                    delay(10);
                    mybuzzer.start(1900, 90);
                    delay(10);
                    mybuzzer.start(1800, 90);
                    delay(10);
                    mybuzzer.start(1900, 90);
                    delay(200);
                    mybuzzer.start(2200, 90);
                }
            }
            waitUntilTactNotPress();

        }
        if (tact == 101){//mode++ (defense -> attack -> debug)
            mode = (mode + 1) % 3;
            switch(mode){
                case 0://defense
                    mypixel.multi(0, 23, MyPIXEL::COLOR::BLUE);
                    mypixel.show();
                    mybuzzer.start(1000, 90);
                    break;
                case 1://attack
                    mypixel.multi(0, 23, MyPIXEL::COLOR::RED);
                    mypixel.show();
                    mybuzzer.start(1250, 90);
                    delay(10);
                    mybuzzer.start(1300, 90);
                    break;
                    case 2://debug
                    mypixel.multi(0, 23, MyPIXEL::COLOR::GREEN);
                    mypixel.show();
                    mybuzzer.start(1500, 90);
                    delay(10);
                    mybuzzer.start(1550, 90);
                    delay(10);
                    mybuzzer.start(1600, 90);
                    break;
            }
            postData();
            waitUntilTactNotPress();
            ClearPixel();
        }
        if(tact==105){
            UsePixel=!UsePixel;
            mypixel.set_mode(UsePixel);
            if(UsePixel){
                mybuzzer.start(1000, 90);
                delay(10);
                mybuzzer.start(1200, 90);
            } else {
                ClearPixel();
                mybuzzer.start(1200, 90);
                delay(10);
                mybuzzer.start(1000, 90);
            }
            postData();
            waitUntilTactNotPress();
        }
        if(tact==109){
            mybuzzer.useBUZZER = !mybuzzer.useBUZZER;
            if(mybuzzer.useBUZZER){
                mybuzzer.start(500, 90);
                delay(10);
                mybuzzer.start(1000, 90);
            }
            postData();
            waitUntilTactNotPress();
        }

        postData();

        if(Serial.available()>0){
            auto read = Serial.read();
            if(read ==  'g'){
                mybuzzer.start(1200, 500);
                delay(100);
                defense.printHistory();
                delay(100);
                mybuzzer.start(1800, 90);
                mybuzzer.start(2000, 90);
            }
            while(Serial.available() > 0) {
                Serial.read();
            }
        }
    }
    mypixel.show();
}


// ====================================== UART ======================================

void serialEvent7() { //Ball
    // Serial.println("SerialEvent7");
    int size = 8;
    uint8_t message4_read[size]{};
    // start ID x1 x2 y1 y2 key end

    bool startFound7 = false;
    for (int i = 0; i < 20; i++) {
        if (Serial7.available() >= size) {
            message4_read[0] = Serial7.read();
            if (message4_read[0] == 195) {
                startFound7 = true;
                break;
            }
        } else {
            return;
        }
    }
    if (!startFound7) return;

    for (int i = 1; i < size; i++) message4_read[i] = Serial7.read();

    if(codec.decode(message4_read, 2, 5, 6) != 0) return;
    // Serial.println("Decoded message4;");

    if (message4_read[(size - 1)] == 231) ball.get_message(message4_read);
    else return;
}

void serialEvent2() { //Cam1
    int size = 7;
    uint8_t message6_read[size]{};
    // start ID x ax width height end

    while(1) {
        if (Serial2.available() >= size) {
            message6_read[0] = Serial2.read();
            if (message6_read[0] == 195) break;
        } else {
            return;
        }
    }

    for (int i = 1; i < size; i++) message6_read[i] = Serial2.read();
    if (message6_read[(size - 1)] == 231) cam.get_message(message6_read, 0);
    else return;
}

void serialEvent6() { //Cam2
    int size = 6;
    uint8_t message2_read[size]{};
    // start ID x width height end

    while(1) {
        if (Serial6.available() >= size) {
            message2_read[0] = Serial6.read();
            if (message2_read[0] == 195) break;
        } else {
            return;
        }
    }

    for (int i = 1; i < size; i++) message2_read[i] = Serial6.read(); //残りを読む

    if (message2_read[(size - 1)] == 231) cam.get_message(message2_read, 1);
    else return;
}

void serialEvent3() { //Line
    int size = 7;
    uint8_t message3_read[size]{};
    // start ID data1 data2 data3 key end
    // ID1 = data

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

    if (message3_read[(size - 1)] == 231) line.get_message(message3_read);
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