#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "mpu6050.h"

static const char *TAG = "MOTION_TRACKER";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Starting Motion Tracker...");

    initialize_mpu6050();

    ESP_LOGI(TAG, "Motion sensor initialized!");

    while (true)
    {
        MotionData data = read_motion_data();

        ESP_LOGI(TAG,
            "ACCEL: X=%d Y=%d Z=%d | GYRO: X=%d Y=%d Z=%d",
            data.accel_x,
            data.accel_y,
            data.accel_z,
            data.gyro_x,
            data.gyro_y,
            data.gyro_z
        );

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}