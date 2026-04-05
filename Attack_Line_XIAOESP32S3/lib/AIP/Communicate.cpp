#include "Communicate.h"
#include "AIP.h"
#include "Input.h"


void Communicate::teensy_write(int ID) {
    //start ID data1 data2 data3 key end
    //ID1=000
    //ID2=100
    //ID3=010
    //ID4=110
    //ID5=001
    //ID6=101
    //ID7=011
    //ID8=111
    //ID10=LED

    uint8_t message1_write[7]{};

    message1_write[0] = 195;
    message1_write[6] = 231;

    if (ID == 1) {
        int a{};
        if(!line.side_stat[0]&&!line.side_stat[1]&&!line.side_stat[2]) a = 1;
        else if(line.side_stat[0]&&!line.side_stat[1]&&!line.side_stat[2]) a = 2;
        else if(!line.side_stat[0]&&line.side_stat[1]&&!line.side_stat[2]) a = 3;
        else if(line.side_stat[0]&&line.side_stat[1]&&!line.side_stat[2]) a = 4;
        else if(!line.side_stat[0]&&!line.side_stat[1]&&line.side_stat[2]) a = 5;
        else if(line.side_stat[0]&&!line.side_stat[1]&&line.side_stat[2]) a = 6;
        else if(!line.side_stat[0]&&line.side_stat[1]&&line.side_stat[2]) a = 7;
        else if(line.side_stat[0]&&line.side_stat[1]&&line.side_stat[2]) a = 8;

        message1_write[1] = a;

        for (int i = 2; i < 5; i++) {
            for (int j = 0; j < 8; j++) {
                message1_write[i] |= (line.main_stat[(i-2) * 8 + j] & 0x01) << j;
            }
        }

    } else if (ID == 10) {
        message1_write[1] = 10;
    } else if (ID == 11) {
        message1_write[1] = 11;
    } else if (ID == 12) {
        message1_write[1] = 12;
    }

    codec.encode(message1_write, 2, 4, 5);
    Serial1.write(message1_write, 7);
}