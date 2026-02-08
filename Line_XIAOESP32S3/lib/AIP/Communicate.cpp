#include "Communicate.h"
#include "AIP.h"
#include "Input.h"


void Communicate::teensy_write(int ID) {
    //ID1 = linedata

    uint8_t message1_write[6]{};
    message1_write[0] = 195;
    message1_write[5] = 231;
    //start ID data data data end

    if (ID == 1) {
        for (int i = 2; i < 5; i++) {
            for (int j = 0; j < 8; j++) {
                message1_write[i] |= (line.line_stat[(i-2) * 8 + j] & 0x01) << j;
            }
        }

        message1_write[1] = 1;
    }

    Serial1.write(message1_write, 6);
}