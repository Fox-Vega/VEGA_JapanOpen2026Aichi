//変更禁止
#include "Communicate.h"
#include "AIP.h"
#include "Input.h"
#include "Output.h"
#include <EEPROM.h>


void Communicate::line_read(byte* message) {
    //ID1~8 = data
    //ID10 = led-10
    //ID11 = led+10
    //ID12 = FIN

    if (1 <= message[1] && message[1] <= 8) {
        line.read_message(message);
    } else if (message[1] == 10) {
        line.set_led(line.led_pwm -= 1);
    } else if (message[1] == 11) {
        line.set_led(line.led_pwm += 1);
    } else if (message[1] == 12) {
        EEPROM.put(0, line.led_pwm);
        line.autosetborder = false;
        mybuzzer.run(1500, 60);
    }
}

void Communicate::line_write(int ID) {
    //ID1 = mainborder
    //ID2 = sideborder
    //ID3 = autosetborder

    uint8_t message3_write[6]{};
    message3_write[0] = 195;
    message3_write[5] = 231;
    //start ID data data key end

    if (ID == 1) {
        message3_write[1] = 1;
        message3_write[2] = (line.main_border >> 8) & 0xFF;
        message3_write[3] = line.main_border & 0xFF;
    } else if (ID == 2) {
        message3_write[1] = 2;
        message3_write[2] = (line.side_border >> 8) & 0xFF;
        message3_write[3] = line.side_border & 0xFF;
    } else if (ID == 3) {
        message3_write[1] = 3;
    }

    codec.encode(message3_write, 2, 3, 4);
    Serial3.write(message3_write, 6);
}

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
                // bitWrite(message8_write[2], 2, uiState.usepixel);
                bitWrite(message8_write[2], 3, uiState.gamreset);
                bitWrite(message8_write[2], 4, uiState.switch_[0]);
                bitWrite(message8_write[2], 5, uiState.switch_[1]);
                bitWrite(message8_write[2], 6, uiState.switch_[2]);
                bitWrite(message8_write[2], 7, uiState.switch_[3]);
                message8_write[3] = uiState.latency;
                packAzimuth(gam.get_azimuth(), message8_write[4], message8_write[5], uiState.buzzer);
                break;
            }
        case 51:
            message8_write[1] = 51;
            message8_write[2] = param;
            break;
        case 103:
            message8_write[1] = 103;
            bitWrite(message8_write[2], 0, uiState.LineBorder);
            bitWrite(message8_write[2], 1, uiState.Kicking);
            bitWrite(message8_write[2], 2, uiState.LineLight);
            break;
        default:
            break;
    }

    codec.encode(message8_write, 2, 5, 6);
    Serial8.write(message8_write, 8);
}

void Communicate::setParam(bool param) {
    this->param = param;
}
