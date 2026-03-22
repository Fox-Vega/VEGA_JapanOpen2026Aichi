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
    //記録
    REC_azimuth = movement_azimuth_;
    REC_power = movement_power_;

    //姿勢制御の出力を取得
    int difix = 0;
    if (difix_MODE) {
        difix = mymotor.difix(difix_azimuth_);
    }

    //仮出力の計算＆最高値の記録
    float peak = 0.0f;
    for (int i = 0; i < 4; i++) {
        int motor_deg = (movement_azimuth_ - motor_azimuth[i] + 360) % 360; //モーター軸をＹ軸とした時の進行方向
        myvector.cal_cord(motor_deg, movement_power_);
        motor_power_[i] = myvector.get_x();

        if (fabs(motor_power_[i]) > peak) peak = fabs(motor_power_[i]); //最高値の記録
    }

    //移動に使用できる最大出力の計算
    movement_power_ = constrain(movement_power_, -movement_LIMIT, movement_LIMIT);
    int max_power = pwm_LIMIT - abs(difix) + difix_LIMIT;
    if (movement_power_ < max_power) max_power = movement_power_;

    for (int i = 0; i < 4; i++) {
        //倍率計算
        float scale = (peak == 0.0f) ? 0.0f : (fabs(motor_power_[i]) / peak);

        //出力計算
        if (/*???*/) {
            if (motor_power_[i] > 0) motor_power[i] = (max_power * scale) + difix;
            else if (motor_power_[i] < 0) motor_power[i] = (-max_power * scale) + difix;
            else motor_power[i] = difix;
        } else {
            if (motor_power_[i] > 0) motor_power[i] = (max_power * scale);
            else if (motor_power_[i] < 0) motor_power[i] = (-max_power * scale);
            else motor_power[i] = 0;
        }

        //最終制限
        motor_power[i] = constrain(motor_power[i], -pwm_LIMIT, pwm_LIMIT);

        //信号送信
        if (USE_motor == 1) {
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
    float dt = (millis() - LAST_difix) / 1000.0; //秒に変換

    //比例項
    float error = 0;
    if (difix_MODE == 2 && cam.get_ax(1) != 999) {
        error = cam.get_ax(1);
        if (error > 180) error -= 360;
        else if (error < -180) error += 360;
        pwm = gkp * error;
        if (abs(cam.get_ax(1)) > ZONE_border) ZONE = 0;
        else ZONE = 1;

    } else if (difix_MODE == 2 && cam.get_x(1) != 999) {
        error = cam.get_x(1);
        if (error > 180) error -= 360;
        else if (error < -180) error += 360;
        pwm = gkp * error;
        if (abs(cam.get_x(1)) > ZONE_border) ZONE = 0;
        else ZONE = 1;

    } else {
        error = target_azimuth - gam.get_azimuth();
        if (error > 180) error -= 360;
        else if (error < -180) error += 360;
        pwm = kp * error;
    }

    //積分項
    integral += error * dt;
    integral = constrain(integral, -integral_LIMIT, integral_LIMIT);
    pwm += ki * integral;

    //微分項
    int delta = gam.get_azimuth() - PREV_azimuth;
    if (delta > 180) delta -= 360;
    else if (delta < -180) delta += 360;
    float derivative = delta / dt;
    pwm -= kd * derivative;

    pwm = constrain(pwm, -(difix_LIMIT + share_LIMIT), (difix_LIMIT + share_LIMIT));

    LAST_difix = millis();
    PREV_azimuth = gam.get_azimuth();

    return pwm;
}

int MyMOTOR::get_azimuth() {
    return REC_azimuth;
}

int MyMOTOR::get_power() {
    return REC_power;
}

void MyMOTOR::set_motor(bool stat) {
    USE_motor = stat;
}

void MyMOTOR::set_difix(int stat) {
    difix_MODE = stat;
}