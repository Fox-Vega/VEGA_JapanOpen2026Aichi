#include "Communicate.h"
#include "AIP.h"
#include "Input.h"


void Communicate::teensy_write(int ID) {
    uint8_t message1_write[8]{};
    //start ID data data data data key end

    if (ID == 1) {
        int x = ball.get_x();
        int y = ball.get_y();
        x += 32768;
        y += 32768;
        if (x < 0) x = 0;
        else if (x > 65535) x = 65535;
        if (y < 0) y = 0;
        else if (y > 65535) y = 65535;

        message1_write[0] = 195;
        message1_write[1] = 1;
        message1_write[2] = (x >> 8) & 0xFF;
        message1_write[3] = x & 0xFF;
        message1_write[4] = (y >> 8) & 0xFF;
        message1_write[5] = y & 0xFF;
        message1_write[7] = 231;
    }

    codec.encode(message1_write, 2, 5, 6);
    Serial1.write(message1_write, 7);
}