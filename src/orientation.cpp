#include "orientation.h"
#include <math.h>

static float filtered_pitch = 0.0f;
static float filtered_roll = 0.0f;

void initialize_orientation()
{
    filtered_pitch = 0.0f;
    filtered_roll = 0.0f;
}

Orientation calculate_orientation(MotionData data, float delta_time)
{
    Orientation result;

    const float RAD_TO_DEG = 180.0f / 3.14159265f;
    const float GYRO_SCALE = 131.0f;
    const float ALPHA = 0.98f;

    float ax = (float)data.accel_x;
    float ay = (float)data.accel_y;
    float az = (float)data.accel_z;

    float gyro_x = (float)data.gyro_x / GYRO_SCALE;
    float gyro_y = (float)data.gyro_y / GYRO_SCALE;

    float accel_pitch = atan2f(-ax, sqrtf(ay * ay + az * az)) * RAD_TO_DEG;
    float accel_roll = atan2f(ay, az) * RAD_TO_DEG;

    float gyro_pitch = filtered_pitch + gyro_y * delta_time;
    float gyro_roll = filtered_roll + gyro_x * delta_time;

    filtered_pitch = ALPHA * gyro_pitch + (1.0f - ALPHA) * accel_pitch;
    filtered_roll = ALPHA * gyro_roll + (1.0f - ALPHA) * accel_roll;

    result.pitch = filtered_pitch;
    result.roll = filtered_roll;

    return result;
}