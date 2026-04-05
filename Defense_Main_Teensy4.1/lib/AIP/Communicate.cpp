//変更禁止
#include "Communicate.h"
#include "AIP.h"
#include "Input.h"
#include "Output.h"


void Communicate::line_write(int ID) {
    //ID1 = lineborder

    uint8_t message3_write[6]{};
    message3_write[0] = 195;
    message3_write[5] = 231;
    //start ID data data key end

    if (ID == 1) {
        message3_write[1] = 1;
        message3_write[2] = (line.detection_border >> 8) & 0xFF;
        message3_write[3] = line.detection_border & 0xFF;
    }

    codec.encode(message3_write, 2, 3, 4);
    Serial3.write(message3_write, 6);
}

// void Communicate::ui_write(int ID) {
//     //ID200 = linestat
//     //ID254 = usepixel0
//     //ID255 = usepixel1

//     uint8_t message7_write[9]{};
//     message7_write[0] = 195;
//     message7_write[8] = 231;
//     //start ID data data data data data key end

//     if (ID == 200) {
//         message7_write[1] = 200;
//         for (int i = 2; i < 5; i++) {
//             for (int j = 0; j < 8; j++) {
//                 message7_write[i] |= (line.line_stat[(i-2) * 8 + j] & 0x01) << j;
//             }
//         }
//     } else if (ID == 254) { //off
//         message7_write[1] = 254;
//     } else if (ID == 255) { //on
//         message7_write[1] = 255;
//     }


//     if(ID==100){
//         bool switch_[5] = {false, false, false, false, false};
//         int tact = myswitch.check_tact();
//         // Serial.print("tact: ");
//         // Serial.println(tact);

//         // switch_[0]=左, switch_[1]=中央, switch_[2]=右, switch_[4]=x10モード

//         if(tact>=100){
//             tact-=100;
//             switch_[3]=true;
//         }
//         switch_[0] = (tact == 1 || tact == 6 || tact == 10 || tact == 15);
//         switch_[1] = (tact == 5 || tact == 6 || tact == 14 || tact == 15);
//         switch_[2] = (tact == 9 || tact == 10 || tact == 14 || tact == 15);
//         switch_[4] = myswitch.check_toggle();

//         message7_write[0] = 195;
//         message7_write[1] = ID;
//         message7_write[2] = 0; //free
//         message7_write[2] = 0;
//         for (int i = 0; i < 5; i++) {
//             bitWrite(message7_write[2], i, switch_[i]);
//             // Serial.print(switch_[i]);
//         }
//         // Serial.println();
//         message7_write[3] = 0;
//         message7_write[4] = 0;
//         message7_write[5] = 0;
//         message7_write[6] = 0;
//         message7_write[7] = 0; //dont use キーが入るから
//         message7_write[8] = 231;

//     }

//     codec.encode(message7_write, 2, 6, 7);
//     Serial8.write(message7_write, 9);
// }

// azimuth: 0..359
void packAzimuth(uint16_t azimuth, uint8_t &m4, uint8_t &m5, bool buzzer) {
    uint16_t a = azimuth % 360;
    uint8_t low6 = a & 0x3F;          // bits 0-5
    uint8_t high3 = (a >> 6) & 0x07;  // bits 6-8

    m4 &= 0x03;           // keep bits0-1 (buzzer in bit1)
    m4 |= (low6 << 2);    // place low6 into bits2-7
    if (buzzer) m4 |= 0x02; else m4 &= ~0x02;

    m5 &= ~0x07;          // clear bits0-2
    m5 |= high3;          // set high3
}

void Communicate::ui_write(int ID) {
    //ID1 = usepixel0
    //ID2 = usepixel1
    //ID3 = pixel show
    //ID4 = pixel
    //ID5 = rainbow
    //ID6 = brightness

    uint8_t message8_write[8]{};
    message8_write[0] = 195;
    message8_write[7] = 231;
    //start ID data data data data key end

    switch (ID) {
        case 1: //usepixel 0
            message8_write[1] = 1;
            break;
        case 2: //usepixel 1
            message8_write[1] = 2;
            break;
        case 3: //show
            message8_write[1] = 3;
            break;
        case 4: //pixel
            message8_write[1] = 4;
            message8_write[2] = mypixel.color_MODE;
            for (int i = 3; i < 6; i++) {
                for (int j = 0; j < 8; j++) {
                    message8_write[i] |= (mypixel.pixel_STAT[(i-3) * 8 + j] & 0x01) << j;
                }
            }
            break;
        case 5: //rainbow
            message8_write[1] = 5;
            break;
        case 6: //brightness
            message8_write[1] = 6;
            message8_write[2] = mypixel.brightness;
            break;
        case 50:{
            message8_write[1] = 50;
            bitWrite(message8_write[2], 0, uiState.cam);
            bitWrite(message8_write[2], 1, uiState.rainbow);
            bitWrite(message8_write[2], 2, uiState.usepixel);
            bitWrite(message8_write[2], 3, uiState.gamreset);
            bitWrite(message8_write[2], 4, uiState.switch_[0]);
            bitWrite(message8_write[2], 5, uiState.switch_[1]);
            bitWrite(message8_write[2], 6, uiState.switch_[2]);
            bitWrite(message8_write[2], 7, uiState.switch_[3]);
            message8_write[3] = uiState.latency;
            packAzimuth(gam.get_azimuth(), message8_write[4], message8_write[5], uiState.buzzer);
            break;
        }
        case 102:{
            message8_write[1] = 102;
            message8_write[2] = uiState.mode;
            break;
        }
        case 51:{
            message8_write[1] = 51;
            message8_write[2] = param;
            break;
        }
        default:
            break;
    }

    codec.encode(message8_write, 2, 5, 6);
    Serial8.write(message8_write, 8);
}

void Communicate::setParam(bool param) {
    this->param = param;
}