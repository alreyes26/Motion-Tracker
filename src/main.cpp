#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "mpu6050.h"

static const char *TAG = "MOTION_TRACKER";

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "STEP 1: Starting Motion Tracker");
    vTaskDelay(pdMS_TO_TICKS(10000));

    ESP_LOGI(TAG, "STEP 2: Initializing MPU6050");
    vTaskDelay(pdMS_TO_TICKS(10000));

    initialize_mpu6050();

    ESP_LOGI(TAG, "STEP 3: MPU6050 initialized");
    vTaskDelay(pdMS_TO_TICKS(10000));

    while (true)
    {
        ESP_LOGI(TAG, "Program is running normally");
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}