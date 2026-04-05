#include "AIP.h"
#include "Codec.h"


void Codec::encode(uint8_t *message_write, int start, int end, int keyindex) {
    if((end - start + 1) <= 8) {
        uint8_t key = 0;
        for (int i = start; i <= end; i++) {
            if (message_write[i] == 195) {
                message_write[i] = 194;
                key |= (1 << (i - start));
            } else if (message_write[i] == 231) {
                message_write[i] = 230;
                key |= (1 << (i - start));
            }
        }
        message_write[keyindex] = key; // 変換した項目をビットマスクで格納
    }
}

int Codec::decode(uint8_t *message_read, int start, int end, int keyindex) {
    error = 0;
    if((end - start + 1) <= 8) {
        for (int i = start; i <= end; i++) {
            if (message_read[keyindex] & (1 << (i - start))) { // 対応ビットが立っていれば復元
                if (message_read[i] == 194) message_read[i] = 195;
                else if (message_read[i] == 230) message_read[i] = 231;
                else error = 1;
            }
        }
    }
    return error;
}