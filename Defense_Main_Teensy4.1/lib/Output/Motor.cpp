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
        myvector.get_cord(motor_azimuth, movement_power_);
        motor_power_[i] = myvector.get_x();

        if (fabs(motor_power_[i]) > peak) peak = fabs(motor_power_[i]); //最高値の記録
    }

    //移動に使用できる最大出力の計算
    movement_power_ = constrain(movement_power_, -pwmlimit, pwmlimit);
    int max_power = redzone - abs(difix) + difixlimit;
    if (movement_power_ < max_power) max_power = movement_power_;

    for (int i = 0; i < 4; i++) {
        //倍率計算
        float scale = (peak == 0.0f) ? 0.0f : (fabs(motor_power_[i]) / peak);

        //出力計算
        if (difix_mode != 2 || (difix_mode == 2 && ((difix <= 0 && (i == 0 || i == 1)) || (0 <= difix && (i == 2 || i == 3))))) {
            if (motor_power_[i] > 0) motor_power[i] = (max_power * scale) + difix;
            else if (motor_power_[i] < 0) motor_power[i] = (-max_power * scale) + difix;
            else motor_power[i] = difix;
        } else {
            if (motor_power_[i] > 0) motor_power[i] = (max_power * scale);
            else if (motor_power_[i] < 0) motor_power[i] = (-max_power * scale);
            else motor_power[i] = 0;
        }

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
        error = cam.get_ax(1);
        if (error > 180) error -= 360;
        else if (error < -180) error += 360;
        if (abs(cam.get_ax(1)) > dzone) pwm = dkp * error;
        else pwm = dzkp * error;

    } else if (difix_mode == 2 && cam.get_x(1) != 999) {
        error = cam.get_x(1);
        if (error > 180) error -= 360;
        else if (error < -180) error += 360;
        if (abs(cam.get_x(1)) > dzone) pwm = dkp * error;
        else pwm = dzkp * error;

    } else {
        error = target_azimuth - gam.get_azimuth();
        if (error > 180) error -= 360;
        else if (error < -180) error += 360;
        pwm = kp * error;
    }

    //微分項
    int delta = gam.get_azimuth() - prev_azimuth;
    if (delta > 180) delta -= 360;
    else if (delta < -180) delta += 360;
    float derivative = delta / dt;
    pwm -= kd * derivative;

    pwm = constrain(pwm, -(difixlimit + sharelimit), (difixlimit + sharelimit));

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
