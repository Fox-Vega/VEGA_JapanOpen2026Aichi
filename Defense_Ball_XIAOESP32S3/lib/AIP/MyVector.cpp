//変更禁止
#include "AIP.h"
#include "MyVector.h"


float MyVECTOR::get_azimuth(float x, float y) {
    if (x == 0 && y == 0) {
        azimuth = 0;
    } else {
        theta = atan2(y, x);
        azimuth = 90.0f - (theta * 180.0f / M_PI);
        if (azimuth < 0) azimuth += 360;
        else if (azimuth >= 360) azimuth -= 360;
    }
    return azimuth;
}

float MyVECTOR::get_magnitude(float x, float y) {
    return sqrtf(x * x + y * y);
}

float MyVECTOR::get_cordx(float azimuth, float magnitude) {
    if (magnitude < 0) {
        azimuth += 180;
        if (azimuth >= 360) azimuth -= 360;
        magnitude = -magnitude;
    }
    theta = 90 - azimuth;
    while (theta > 180) theta -= 360;
    while (theta < -180) theta += 360;
    return round(cos(theta * M_PI / 180.0) * magnitude);
}

float MyVECTOR::get_cordy(float azimuth, float magnitude) {
    if (magnitude < 0) {
        azimuth += 180;
        if (azimuth >= 360) azimuth -= 360;
        magnitude = -magnitude;
    }
    theta = 90 - azimuth;
    while (theta > 180) theta -= 360;
    while (theta < -180) theta += 360;
    return round(sin(theta * M_PI / 180.0) * magnitude);
}

void MyVECTOR::get_cord(float azimuth, float magnitude) {
    if (magnitude < 0) {
        azimuth += 180;
        if (azimuth >= 360) azimuth -= 360;
        magnitude = -magnitude;
    }
    theta = 90 - azimuth;
    while (theta > 180) theta -= 360;
    while (theta < -180) theta += 360;
    x = round(cos(theta * M_PI / 180.0) * magnitude);
    y = round(sin(theta * M_PI / 180.0) * magnitude);
}

float MyVECTOR::get_x() {
    return x;
}

float MyVECTOR::get_y() {
    return y;
}

float MyVECTOR::get_vectordegree(float ax, float ay, float bx, float by) {
    float a_magnitude = sqrtf(ax * ax + ay * ay);
    float b_magnitude = sqrtf(bx * bx + by * by);
    if (a_magnitude == 0.0f || b_magnitude == 0.0f) return 0;
    double cos_theta = (ax * bx + ay * by) / (a_magnitude * b_magnitude);
    if (cos_theta > 1.0f) cos_theta = 1.0f;
    if (cos_theta < -1.0f) cos_theta = -1.0f;
    return degrees(acos(cos_theta));
}