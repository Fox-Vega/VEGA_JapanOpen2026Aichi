#include "Communicate.h"
#include "AIP.h"
#include "Input.h"


void Communicate::teensy_write(int ID) {
    //start ID data data data data key end
    //ID １＝座標データ

    uint8_t message2_write[8]{};

    message2_write[0] = 195;
    message2_write[7] = 231;

    if (ID == 1) {
        message2_write[1] = 1;

        int x = ball.get_x();
        int y = ball.get_y();
        x += 32768;
        y += 32768;
        if (x < 0) x = 0;
        else if (x > 65535) x = 65535;
        if (y < 0) y = 0;
        else if (y > 65535) y = 65535;

        message2_write[2] = (x >> 8) & 0xFF;
        message2_write[3] = x & 0xFF;
        message2_write[4] = (y >> 8) & 0xFF;
        message2_write[5] = y & 0xFF;

    }

    codec.encode(message2_write, 2, 5, 6);
    Serial2.write(message2_write, 8);
}