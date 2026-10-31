#include <iostream>
#include "freeteros/FreeterOS.h"
#include "freeteros/task.h"
#include "esp_log.h"

static const char *TAG = "MOTION_TRACKER";

extern "C void app_main() 
{
    ESP_LOGI(TAG, "Starting Motion Tracker Initialized");

    while(true)
    {
        ESP_LOGI(TAG, "Motion Tracker Running");
    }
    vTaskDelay(pdms.ToTicks(1000));
    
}