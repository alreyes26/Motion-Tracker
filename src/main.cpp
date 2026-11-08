#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "mpu6050.h"
#include "orientation.h"

#include <stdint.h>

static const char *TAG = "MOTION_TRACKER";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Starting Motion Tracker...");

    initialize_mpu6050();
    initialize_orientation();

    ESP_LOGI(TAG, "Motion sensor initialized!");

    int64_t previous_time = esp_timer_get_time();

    while (true)
    {
        MotionData data = read_motion_data();

        int64_t current_time = esp_timer_get_time();

        float delta_time = (current_time - previous_time) / 1000000.0f;

        previous_time = current_time;

        Orientation angles = calculate_orientation(data, delta_time);

        ESP_LOGI(TAG,
            "PITCH: %.2f degrees | ROLL: %.2f degrees",
            angles.pitch,
            angles.roll
        );

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}