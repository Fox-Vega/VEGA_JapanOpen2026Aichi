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

    if (1 <= message[1] && message[1] <= 8) {
        line.read_message(message);
    } else if (message[1] == 10) {
        line.set_led(line.led_pwm -= 5);
    } else if (message[1] == 11) {
        line.set_led(line.led_pwm += 5);
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
        default:
            break;
    }

    codec.encode(message8_write, 2, 5, 6);
    Serial8.write(message8_write, 8);
}
