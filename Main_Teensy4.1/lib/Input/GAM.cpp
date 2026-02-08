//変更禁止
#include "GAM.h"
#include "AIP.h"
#include "Output.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>


Adafruit_BNO055 bno = Adafruit_BNO055(55); //0x28

constexpr int16_t kAccelOffsetX = 0;
constexpr int16_t kAccelOffsetY = 0;
constexpr int16_t kAccelOffsetZ = 7260;
constexpr int16_t kMagOffsetX = 8192;
constexpr int16_t kMagOffsetY = -8192;
constexpr int16_t kMagOffsetZ = -8192;
constexpr int16_t kGyroOffsetX = -32768;
constexpr int16_t kGyroOffsetY = 16397;
constexpr int16_t kGyroOffsetZ = 0;
constexpr int16_t kAccelRadius = 1000;
constexpr int16_t kMagRadius = -16384;


void GAM::setup() {
    if (!bno.begin()) {
        Serial.println("BNO055 : Failure");
        while(1);
    } else {
        Serial.println("BNO055 : Success");
    }
    bno.setMode(OPERATION_MODE_IMUPLUS);

    adafruit_bno055_offsets_t offsets;
    offsets.accel_offset_x = kAccelOffsetX;
    offsets.accel_offset_y = kAccelOffsetY;
    offsets.accel_offset_z = kAccelOffsetZ;
    offsets.gyro_offset_x = kGyroOffsetX;
    offsets.gyro_offset_y = kGyroOffsetY;
    offsets.gyro_offset_z = kGyroOffsetZ;
    offsets.mag_offset_x = kMagOffsetX;
    offsets.mag_offset_y = kMagOffsetY;
    offsets.mag_offset_z = kMagOffsetZ;
    offsets.accel_radius = kAccelRadius;
    offsets.mag_radius = kMagRadius;
    bno.setSensorOffsets(offsets);

    bno.setExtCrystalUse(true);
}

void GAM::read_azimuth() {
    sensors_event_t euler_event;
    bno.getEvent(&euler_event, Adafruit_BNO055::VECTOR_EULER);
    azimuth = (int)euler_event.orientation.x + yawtweak;
}

int GAM::get_azimuth() {
    return azimuth % 360;
}

void GAM::dir_reset() {
    sensors_event_t euler_event;
    bno.getEvent(&euler_event, Adafruit_BNO055::VECTOR_EULER);
    yawtweak = 360 - euler_event.orientation.x;
}

float GAM::bimuth_conv(float azimuth) {
    float result = azimuth + get_azimuth();
    while (result < 0) result += 360.0f;
    while (result >= 360.0f) result -= 360.0f;
    return result;
}

float GAM::bimuth_rev(float bimuth) {
    float result = bimuth - get_azimuth();
    // 0-360 にラップ
    while (result < 0) result += 360.0f;
    while (result >= 360.0f) result -= 360.0f;
    return result;
}

void GAM::bird_conv(float x, float y) {
    float result = myvector.get_azimuth(x, y) + get_azimuth();
    while (result < 0) result += 360.0f;
    while (result >= 360.0f) result -= 360.0f;
    myvector.get_cord(result, myvector.get_magnitude(x, y));
    n_x = myvector.get_x();
    n_y = myvector.get_y();
}

void GAM::bird_rev(float x, float y) {
    float result = myvector.get_azimuth(x, y) - get_azimuth();
    while (result < 0) result += 360.0f;
    while (result >= 360.0f) result -= 360.0f;
    myvector.get_cord(result, myvector.get_magnitude(x, y));
    n_x = myvector.get_x();
    n_y = myvector.get_y();
}

float GAM::get_x() {
    return n_x;
}

float GAM::get_y() {
    return n_y;
}