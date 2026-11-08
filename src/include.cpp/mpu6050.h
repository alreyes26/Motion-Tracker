
#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>

// Stores all six sensor measurements
struct MotionData
{
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;

    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
};

// Initializes the MPU6050
void initialize_mpu6050();

// Reads accelerometer and gyroscope measurements
MotionData read_motion_data();

#endif
