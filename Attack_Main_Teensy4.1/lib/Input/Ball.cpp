//変更禁止
#include "Ball.h"
#include "AIP.h"


void BALL::read_message(byte* message) {
    ball_x = ((message[2] << 8) | message[3]) - 32768;
    ball_y = ((message[4] << 8) | message[5]) - 32768;

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