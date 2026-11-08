
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "MOTION_TRACKER";

#define I2C_PORT I2C_NUM_0
#define SDA_PIN GPIO_NUM_21
#define SCL_PIN GPIO_NUM_22
#define I2C_FREQUENCY 100000

void initialize_i2c()
{
    i2c_config_t config = {};

    config.mode = I2C_MODE_MASTER;
    config.sda_io_num = SDA_PIN;
    config.scl_io_num = SCL_PIN;
    config.sda_pullup_en = GPIO_PULLUP_ENABLE;
    config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    config.master.clk_speed = I2C_FREQUENCY;

    ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &config));

    ESP_ERROR_CHECK(
        i2c_driver_install(I2C_PORT, config.mode, 0, 0, 0)
    );

    ESP_LOGI(TAG, "I2C initialized successfully");
}

void scan_i2c()
{
    ESP_LOGI(TAG, "Scanning I2C bus...");

    int devices_found = 0;

    for (int address = 0x08; address <= 0x77; address++)
    {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();

        i2c_master_start(cmd);

        i2c_master_write_byte(
            cmd,
            (address << 1) | I2C_MASTER_WRITE,
            true
        );

        i2c_master_stop(cmd);

        i2c_master_write_byte(
            cmd,
            (address << 1) | I2C_MASTER_WRITE,
            true
        );

        i2c_master_stop(cmd);

        esp_err_t result = i2c_master_cmd_begin(
            I2C_PORT,
            cmd,
            pdMS_TO_TICKS(100)
        );

        i2c_cmd_link_delete(cmd);

        if (result == ESP_OK)
        {
            ESP_LOGI(TAG, "Device found at address: 0x%02X", address);
            devices_found++;
        }
    }
    if (devices_found == 0)
    {
        ESP_LOGW(TAG, "No I2C devices found!");
    }
    else
    {
        ESP_LOGI(TAG, "Scan complete. Found %d device(s).", devices_found);
    }
}

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Starting Motion Tracker...");

    initialize_i2c();

    scan_i2c();

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

