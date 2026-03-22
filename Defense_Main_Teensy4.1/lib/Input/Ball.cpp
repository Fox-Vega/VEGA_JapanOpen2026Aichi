//変更禁止
#include "Ball.h"
#include "AIP.h"


void BALL::get_message(byte* message) {
    ball_x = ((message[1] << 8) | message[2]) - 32768;
    ball_y = ((message[3] << 8) | message[4]) - 32768;

    if (ball_x == 0 && ball_y == 0) ball = 0;
    else ball = 1;
}

bool BALL::get_stat() {
    return ball;
}

int BALL::get_x() {
    return ball_x;
}

int BALL::get_y() {
    return ball_y;
}

int BALL::get_azimuth() {
    return myvector.get_azimuth(ball_x, ball_y);
}

int BALL::get_intensity() {
    return myvector.get_magnitude(ball_x, ball_y);
}