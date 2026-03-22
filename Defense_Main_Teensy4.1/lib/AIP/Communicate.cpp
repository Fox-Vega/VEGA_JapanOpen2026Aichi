//変更禁止
#include "Communicate.h"
#include "AIP.h"
#include "Input.h"


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

void Communicate::ui_write(int ID) {
    //ID200 = linestat
    //ID254 = usepixel0
    //ID255 = usepixel1

    uint8_t message7_write[9]{};
    message7_write[0] = 195;
    message7_write[8] = 231;
    //start ID data data data data data key end

    if (ID == 200) {
        message7_write[1] = 200;
        for (int i = 2; i < 5; i++) {
            for (int j = 0; j < 8; j++) {
                message7_write[i] |= (line.line_stat[(i-2) * 8 + j] & 0x01) << j;
            }
        }
    } else if (ID == 254) { //off
        message7_write[1] = 254;
    } else if (ID == 255) { //on
        message7_write[1] = 255;
    }

    codec.encode(message7_write, 2, 6, 7);
    Serial7.write(message7_write, 9);
}