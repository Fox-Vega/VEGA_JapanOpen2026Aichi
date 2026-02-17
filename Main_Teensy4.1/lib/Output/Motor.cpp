//変更禁止
#include "Motor.h"
#include "AIP.h"
#include "Input.h"
#include "Output.h"


void MyMOTOR::setup() {
    for (int i = 0; i < 4; i++) {
        pinMode(motor_DPIN[i], OUTPUT);
        pinMode(motor_PPIN[i], OUTPUT);
        analogWriteFrequency(motor_DPIN[i], 970);
        analogWriteFrequency(motor_PPIN[i], 970);
    }
}

void MyMOTOR::run(int movement_azimuth, int power_, int dir_azimuth) {
    power_ = constrain(power_, -pwmlimit, pwmlimit); //制限かける

    //記録
    azimuth = movement_azimuth;
    intensity = power_;

    //姿勢制御の出力を取得
    int difix = 0;
    if (motor_stabilization) {
        difix = mymotor.difix(dir_azimuth);
    }

    float peak = 0.0f;
    for (int i = 0; i < 4; i++) {
        int azimuth_motor = (movement_azimuth - motor_degrees[i] + 360) % 360; //モーター軸をＹ軸にした時の進行方向

        //仮出力計算
        myvector.get_cord(azimuth_motor, power_);
        motor_power_[i] = myvector.get_x();

        if (fabs(motor_power_[i]) > peak) peak = fabs(motor_power_[i]); //最高値を記録
    }

    // int trim{};
    int trim = (abs(difix) - difixlimit);
    if (trim > sharelimit) trim = sharelimit;
    int mp = 210 - trim;

    max_power = power_;
    if (power_ > mp) max_power = mp;

    for (int i = 0; i < 4; i++) {
        // peak が 0 の場合は倍率 0 にして除算を避ける
        scale = (peak == 0.0f) ? 0.0f : (fabs(motor_power_[i]) / peak); //倍率を計算

        //出力計算（float）
        if (motor_power_[i] > 0) motor_power[i] = ((max_power - trim) * scale) + difix;
        else if (motor_power_[i] < 0) motor_power[i] = ((-max_power + trim) * scale) + difix;
        else motor_power[i] = difix;

        motor_power[i] = constrain(motor_power[i], -210, 210);

        //信号送信
        if (motor_lock == 0) {
            float out = motor_power[i];
            int pwm = (int)round(fabs(out));
            if (pwm > pwmlimit) pwm = pwmlimit;
            if (out >= 0) {
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
    float dt = (millis() - lastupdate) / 1000.0; //秒

    float error = 0;
    if (motor_stabilization == 2 && cam.get_ax(1) != 999) {
        //角度差
        error = cam.get_x(1);
        if (error > 180) {
            error -= 360;
        } else if (error < -180) {
            error += 360;
        }
    } else {
        //角度差
        error = target_azimuth - gam.get_azimuth();
        if (error > 180) {
            error -= 360;
        } else if (error < -180) {
            error += 360;
        }
    }

    //微分項
    int delta = gam.get_azimuth() - prev_azimuth;
    if (delta > 180) {
        delta -= 360;
    } else if (delta < -180) {
        delta += 360;
    }
    float derivative = delta / dt;

    int pwm = 0;
    if (motor_stabilization == 2 && cam.get_x(1) != 999 && abs(cam.get_x(1)) > dzone) pwm = dkp * error - kd * derivative;
    else if (motor_stabilization == 2 && cam.get_x(1) != 999) pwm = dzkp * error;
    else pwm = kp * error - kd * derivative;

    lastupdate = millis();
    prev_azimuth = gam.get_azimuth();

    return pwm;
}

int MyMOTOR::get_azimuth() {
    return azimuth;
}

int MyMOTOR::get_intensity() {
    return intensity;
}

void MyMOTOR::lock(bool stat) {
    motor_lock = stat;
}

void MyMOTOR::stabilization(int stat) {
    motor_stabilization = stat;
}
