#ifndef ORIENTATION_H
#define ORIENTATION_H

#include "mpu6050.h"

// Stores calculated orientation angles
struct Orientation
{
    float pitch;
    float roll;
};

// Calculates orientation from accelerometer data
Orientation calculate_orientation(MotionData data);

#endif