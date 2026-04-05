#include "MyPIXEL.h"
#include "AIP.h"
#include "Output.h"


void MyPIXEL::setup() {
    set_brightness(brightness);
    clear();
    show();
}

void MyPIXEL::set_brightness(int a) {
    brightness = a;
    communicate.ui_write(6);
}

void MyPIXEL::uni(int pixel, int color) {
    if (USE_pixel) {
        pixel %= pixel_NUM;
        if (color_MODE != color && color_MODE != 0) show();
        color_MODE = color;
        pixel_STAT[pixel] = 1;
    }
}

void MyPIXEL::multi(int PIXELNUMstart, int PIXELNUMend, int color) {
    for (int i = PIXELNUMstart; i <= PIXELNUMend; i++) {
        int a = i;
        a %= pixel_NUM;
        mypixel.uni(a, color);
    }
}

void MyPIXEL::closest(int azimuth, int color, int range) {
    float ClosestPIXEL = (azimuth / 360.0f * pixel_NUM);
    ClosestPIXEL = round(ClosestPIXEL);

    if (ClosestPIXEL >= pixel_NUM) {
        ClosestPIXEL = 0;
    }

    int pixel_START = ClosestPIXEL - (range / 2);
    if (pixel_START < 0) {
        pixel_START += pixel_NUM;
    }

    for (int i = 0; i < range; i++) {
        int j = (pixel_START + i) % pixel_NUM;
        mypixel.uni(j, color);
    }
}

void MyPIXEL::range(int start, int end, int color) {
    start = ((start % 360) + 360) % 360;
    end = ((end % 360) + 360) % 360;

    int start_pixel = (int)round(start / 360.0f * pixel_NUM) % pixel_NUM;
    int end_pixel = (int)round(end / 360.0f * pixel_NUM) % pixel_NUM;

    mypixel.multi(start_pixel, end_pixel, color);
}

void MyPIXEL::rainbow() {
    communicate.ui_write(5);
}

void MyPIXEL::show() {
    communicate.ui_write(4);
    communicate.ui_write(3);
    clear();
}

void MyPIXEL::clear() {
    color_MODE = 0;
    for (int i = 0; i < pixel_NUM; i++) {
        pixel_STAT[i] = 0;
    }
}

void MyPIXEL::set_mode(bool stat) {
    if (!stat) {
        communicate.ui_write(1);
        USE_pixel = 0;
    } else {
        communicate.ui_write(2);
        USE_pixel = 1;
    }
}