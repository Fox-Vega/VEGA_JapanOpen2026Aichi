#include <Attack.h>
#include "Input.h"
#include "Output.h"
#include "Process.h"
#include "AIP.h"

void Attack::setup() {
    //おまじない（マツケンサンバ）
}

void Attack::attack_() {
    //回り込み度数に変換
    //反応しているセンサーをステータスに変換

    // Serial.println(
    //     "  L: "
    //     + String(attack.Lfront) + ","
    //     + String(attack.Lright) + ","
    //     + String(attack.Lback) + ","
    //     + String(attack.Lleft) + " "
    // );

    if (/*平行移動条件*/) {
        special = 1;
        flow();
    } else if (/*押し込み条件*/) {
        special = 2;
        push();
    } else {
        special = 0;
        ver_start = 999;
    }

    if (special == 0) {
        if (line.get_mainNUM() != 0) { //ラインある
            if(back){back=false;back_start=0;}
            mymotor.run(line.get_eazimuth(), 240, 0);
        } else if (ball.get_stat() == 1) { //ラインない　ボールある
            if(back){back=false;back_start=1;}
            wrap(0);
        } else { //ラインない　ボールない
            if (!back){back=true;if(back_start!=0)back_start=millis();}
            mymotor.set_difix(1);
            if (back && (millis() - back_start) < 500) mymotor.run(180, 160, 0);
            else mymotor.run(0, 0, 0);
        }
    }
}



//=========================================関数=========================================



int Attack::wrap(int mode) {
    //回り込みとシュートモーション
}


void Attack::flow() {
    //平行移動
}

void Attack::push() {
    //押し込み
}