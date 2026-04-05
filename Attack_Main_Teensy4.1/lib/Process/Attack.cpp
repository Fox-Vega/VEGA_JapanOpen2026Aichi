#include <Attack.h>
#include "Input.h"
#include "Output.h"
#include "Process.h"
#include "AIP.h"

void Attack::setup() {
    //おまじない（マツケンサンバ）
}

void Attack::reset() {
    run = 0;
    kickoff = 0;
    lineavoid = 0;
}

void Attack::attack_() {
    if (millis() - start_TIME < 500 && !start_FIN) mymotor.set_difix(0);
    else mymotor.set_difix(1);

    //回り込み度数
    ball_wdeg = ball.get_azimuth();
    if (ball_wdeg > 180) ball_wdeg -= 360;

    if (!run) {
        run = 1;
        start_FIN = 0;
        start_TIME = millis();
    }
    if (millis() - start_TIME < 500 && !kickoff && cat.fetch()) {
        kickoff = 1;
        kickoff_TIME = millis();
        kickoff_azimuth = gam.get_azimuth();
        start_FIN = 0;
    }

    // if (line.get_et() < 300) {
    //     side = 0;
    // } else if (line.get_sideNUM() == 0) {
    //     side = 1;
    // }



    //前側が反応し、後ろが反応していない
    //前側が先に条件にかかるから、後ろ側が反応していることだけが条件
    Lfront = Lright = Lback = Lleft = 0;
    for (int i=22;i<=26;i++) {int a=i;if(24<=a)a-=24; Lfront=(line.get_main(a))?1:Lfront; } //22 23 0 01 02(26)
    for (int i=3;i<=8;i++) {int a=i;if(24<=a)a-=24; Lright=(line.get_main(a))?1:Lright; } //03 04 05 06 07 08
    for (int i=8;i<=16;i++) {int a=i;if(24<=a)a-=24; Lback=(line.get_main(a))?1:Lback; } //08 09 10 11 12 13 14 15 16 使わないだろうけど一応ね
    for (int i=16;i<=21;i++) {int a=i;if(24<=a)a-=24; Lleft=(line.get_main(a))?1:Lleft; } //16 17 18 19 20 21

    // Serial.println(
    //     "  L: "
    //     + String(attack.Lfront) + ","
    //     + String(attack.Lright) + ","
    //     + String(attack.Lback) + ","
    //     + String(attack.Lleft) + " "
    // );

    if (special == 1 || /*ウルトラロング条件式*/) {
        special = 1;
        flow();
    } else if (special == 2 || /*ウルトラロング条件式*/) {
        special = 2;
        push();
    } else {
        special = 0;
        ver_start = 999;
        push_count = 0;
    }

    if (special == 0 && !kickoff) {
        if (line.get_mainNUM() != 0) { //ラインある  || (side && line.get_sideNUM() != 0)
            if (!lineavoid) {
                lineavoid = 1;
                lineavoid_azimuth = gam.get_azimuth();
            }
            if(back){back=false;back_start=0;}
            if (100 < ball.get_azimuth() && ball.get_azimuth() < 260) {
                if(0 <= ball_wdeg && (250 < line.get_azimuth() && line.get_azimuth() < 290)) { //ボール右　ライン左
                    if (invert == 0) invert = 1; //右回り込み
                } else if (0 > ball_wdeg && (70 < line.get_azimuth() && line.get_azimuth() < 110)) {
                    if (invert == 0) invert = 2; //左回り込み
                }
            }
            mymotor.run(line.get_eazimuth(), 240, lineavoid_azimuth);
        } else if (ball.get_stat() == 1) { //ラインない　ボールある
            lineavoid = 0;
            if(back){back=false;back_start=1;}
            wrap(0);
        } else { //ラインない　ボールない
            lineavoid = 0;
            if (!back){back=true;if(back_start!=0)back_start=millis();}
            mymotor.set_difix(1);
            if (back && (millis() - back_start) < 500) mymotor.run(180, 160, 0);
            else mymotor.run(0, 0, 0);
        }
    } else {
        //kickoff
        if (millis() - kickoff_TIME < 1) { //ボールを密着
            mymotor.set_difix(1);
            mymotor.run(0, 230, kickoff_azimuth);
        } else if (millis() - kickoff_TIME < 80) { //蹴る
            kicker.run(1);
        } else {
            kickoff = 0;
        }
    }
}



//=========================================関数=========================================



int Attack::wrap(int mode) {
    //ぎりぎりキッカーを扱えるようになった
    //キッカーあると超楽しい
    //全国前夜に思いついた、疑似的除けっていう動作がめっちゃよかった
}


void Attack::flow() {
    //滑らかライントレース
    //マルチ防止が話題だけれど、すぐに実装できる仕様だから暇な時に...
}

void Attack::push() {
    //押し込み
}