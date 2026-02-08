//変更禁止
#pragma once

#include <Arduino.h>


class MySWITCH {
    public:
        void setup();
        int check_toggle();//トグルスイッチの状態を確認　０＝奥　１＝手前
        int check_tact();//タクトスイッチの状態を確認（加算）　無＝０　左＝１ 中央＝５ 右＝９

    private:
        byte pushed_tact;
        byte toggle_stat = 0;

        const byte toggleswitchPIN = 31;
        const byte tactswitchPIN[3] = {26, 27, 30};
};