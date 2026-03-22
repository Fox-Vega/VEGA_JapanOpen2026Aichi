#include "Communicate.h"
#include "AIP.h"
#include "Input.h"


void Communicate::teensy_write(int ID) {
    //start ID data1 data2 data3 key end
    //ID １＝圧縮データ

    uint8_t message1_write[7]{};

    message1_write[0] = 195;
    message1_write[6] = 231;

    if (ID == 1) {
        message1_write[1] = 1;

        for (int i = 2; i < 5; i++) {
            for (int j = 0; j < 8; j++) {
                message1_write[i] |= (line.line_stat[(i-2) * 8 + j] & 0x01) << j;
            }
        }

    }

    codec.encode(message1_write, 2, 4, 5);
    Serial1.write(message1_write, 7); //しょうもないミス
}
