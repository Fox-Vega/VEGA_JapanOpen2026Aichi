//変更禁止
#include "Motor.h"
#include "AIP.h"
#include "Input.h"
#include "Output.h"


void MyMOTOR::setup() { //初期設定
    for (int i = 0; i < 4; i++) {
        pinMode(motor_DPIN[i], OUTPUT);
        pinMode(motor_PPIN[i], OUTPUT);
        analogWriteFrequency(motor_DPIN[i], 970);
        analogWriteFrequency(motor_PPIN[i], 970);
    }
}

void MyMOTOR::run(int movement_azimuth_, int movement_power_, int difix_azimuth_) {
    movement_power_ = constrain(movement_power_, -pwmlimit, pwmlimit); //移動出力の制限

    //記録
    rec_azimuth = movement_azimuth_;
    rec_power = movement_power_;

    //姿勢制御の出力を取得
    int difix = 0;
    if (difix_mode) {
        difix = mymotor.difix(difix_azimuth_);
    }

    //仮出力の計算＆最高値の記録
    float peak = 0.0f;
    for (int i = 0; i < 4; i++) {
        int motor_azimuth = (movement_azimuth_ - motor_degrees[i] + 360) % 360; //モーター軸をＹ軸とした時の進行方向

        //仮出力計算
        myvector.get_cord(motor_azimuth, movement_power_);
        motor_power_[i] = myvector.get_x();

        if (fabs(motor_power_[i]) > peak) peak = fabs(motor_power_[i]); //最高値の記録
    }

    //移動に使用できる最大出力の計算
    int trim = (abs(difix) - difixlimit);
    int max_power = movement_power_;
    if (max_power > (redzone - trim)) max_power = redzone - trim;

    //出力の計算＆信号の送信
    for (int i = 0; i < 4; i++) {
        //除算防止
        float scale = (peak == 0.0f) ? 0.0f : (fabs(motor_power_[i]) / peak); //倍率を計算

        //出力計算
        if (motor_power_[i] > 0) motor_power[i] = ((max_power - trim) * scale) + difix;
        else if (motor_power_[i] < 0) motor_power[i] = ((-max_power + trim) * scale) + difix;
        else motor_power[i] = difix;

        //最終制限
        motor_power[i] = constrain(motor_power[i], -redzone, redzone);

        //信号送信
        if (motor_lock == 0) {
            int pwm = (int)round(fabs(motor_power[i]));
            if (motor_power[i] >= 0) {
                analogWrite(motor_DPIN[i], pwm);
                analogWrite(motor_PPIN[i], 0);
            } else {
                analogWrite(motor_DPIN[i], 0);
                analogWrite(motor_PPIN[i], pwm);
            }
        }
    }
}

void MyMOTOR::free() {
    for (int i = 0; i < 4; i++) {
        analogWrite(motor_DPIN[i], 0);
        analogWrite(motor_PPIN[i], 0);
    }
}

int MyMOTOR::difix(int target_azimuth) {
    float dt = (millis() - lastupdate) / 1000.0; //秒に変換

    //比例項
    float error = 0;
    if (difix_mode == 2 && cam.get_ax(1) != 999) {
        error = cam.get_x(1);
        if (error > 180) error -= 360;
        else if (error < -180) error += 360;
    } else {
        error = target_azimuth - gam.get_azimuth();
        if (error > 180) error -= 360;
        else if (error < -180) error += 360;
    }

    //微分項
    int delta = gam.get_azimuth() - prev_azimuth;
    if (delta > 180) delta -= 360;
    else if (delta < -180) delta += 360;
    float derivative = delta / dt;

    if (difix_mode == 2 && cam.get_x(1) != 999 && abs(cam.get_x(1)) > dzone) pwm = dkp * error; //deadzone外
    else if (difix_mode == 2 && cam.get_x(1) != 999) pwm = dzkp * error; //deadzone内
    else pwm = kp * error; //通常

    pwm -= kd * derivative;

    pwm = constrain(pwm, (difixlimit + sharelimit), -(difixlimit + sharelimit));

    lastupdate = millis();
    prev_azimuth = gam.get_azimuth();

    return pwm;
}

int MyMOTOR::get_azimuth() {
    return rec_azimuth;
}

int MyMOTOR::get_intensity() {
    return rec_power;
}

void MyMOTOR::lock(bool stat) {
    motor_lock = stat;
}

void MyMOTOR::set_difix(int stat) {
    difix_mode = stat;
}
