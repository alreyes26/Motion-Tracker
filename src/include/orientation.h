#ifndef ORIENTATION_H
#define ORIENTATION_H

#include "mpu6050.h"

struct Orientation
{
    float pitch;
    float roll;
};

void initialize_orientation();

Orientation calculate_orientation(MotionData data, float delta_time);

#endif