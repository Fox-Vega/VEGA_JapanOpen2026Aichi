//変更禁止
#include "Cam.h"
#include "AIP.h"
#include "Input.h"

void Cam::read_message(byte* message, bool cam) {
    // start ID datax tarx width height end

    if (message[1] == 1) {
        x[0] = message[2] - 59;
        if (cam == 0 && !target_set && 0 < myvector.get_cordy(gam.get_azimuth(),10) ) {target = 0; target_set = 1;}
        if (cam == 0) {
            if (message[3] != 255) ax[0] = message[3] - 59;
            else ax[0] = 999;
            width[0] = message[4];
            height[0] = message[5];
        } else {
            width[0] = message[3];
            height[0] = message[4];
        }
    } else if (message[1] == 2) {
        x[1] = message[2] - 59;
        if (cam == 0 && !target_set && 0 < myvector.get_cordy(gam.get_azimuth(),10) ) {target = 1; target_set = 1;}
        if (cam == 0) {
            if (message[3] != 255) ax[1] = message[3] - 59;
            else ax[1] = 999;
            width[1] = message[4];
            height[1] = message[5];
        } else {
            width[1] = message[3];
            height[1] = message[4];
        }
    } else {
        if (cam == 0) {
            x[target] = 999;
            ax[target] = 999;
            width[target] = 999;
            height[target] = 999;
        } else if (cam == 1 && target == 0) {
            x[1] = 999;
            ax[1] = 999;
            width[1] = 999;
            height[1] = 999;
        } else {
            x[0] = 999;
            ax[0] = 999;
            width[0] = 999;
            height[0] = 999;
        }
    }
}

int Cam::get_x(bool side) {
    int a;

    if (side == 0 && target == 0) a = x[1];
    else if (side == 0 && target == 1) a = x[0];
    else if (side == 1) a = x[target];

    return a;
}

int Cam::get_ax(bool side) {
    int a;

    if (side == 0 && target == 0) a = ax[1];
    else if (side == 0 && target == 1) a = ax[0];
    else if (side == 1) a = ax[target];

    return a;
}

int Cam::get_width(bool side) {
    int a;

    if (side == 0 && target == 0) a = width[1];
    else if (side == 0 && target == 1) a = width[0];
    else if (side == 1) a = width[target];

    return a;
}

int Cam::get_height(bool side) {
    int a;

    if (side == 0 && target == 0) a = height[1];
    else if (side == 0 && target == 1) a = height[0];
    else if (side == 1) a = height[target];

    return a;
}

void Cam::set_goal(bool color) {
    target = color;
    target_set = 1;
}