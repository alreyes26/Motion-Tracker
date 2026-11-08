#include "orientation.h"
#include <math.h>

Orientation calculate_orientation(MotionData data)
{
    Orientation result;

    // Convert accelerometer readings into floating-point values
    float ax = (float)data.accel_x;
    float ay = (float)data.accel_y;
    float az = (float)data.accel_z;

    // Calculate pitch and roll in radians
    float pitch_rad = atan2f(-ax, sqrtf(ay * ay + az * az));
    float roll_rad = atan2f(ay, az);

    // Convert radians to degrees
    const float RAD_TO_DEG = 180.0f / 3.14159265f;

    result.pitch = pitch_rad * RAD_TO_DEG;
    result.roll = roll_rad * RAD_TO_DEG;

    return result;
}