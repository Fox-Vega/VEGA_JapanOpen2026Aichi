//変更禁止
#pragma once

#include <Arduino.h>


class MySWITCH {
    public:
        void setup();
        int fetch_toggle();//トグルスイッチの状態を確認　０＝奥　１＝手前
        int fetch_tact();//タクトスイッチの状態を確認（加算）　無＝０　左＝１ 中央＝５ 右＝９

    private:
        int tact{};
        bool toggle{};

        const int toggle_PIN = 33;
        const int tact_PIN[4] = {30, 31, 32, 27};
};