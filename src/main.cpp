#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_log.h"


i2c_config_t config = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = GPIO_NUM_21, 
    .scl_io_num = GPIO_NUM_22, 
    .sda_pullup_en = GPIO_PULLUP_ENABLE,
    .scl_pullup_en = GPIO_PULLUP_ENABLE,
    .master.clk_speed = 100000 
};

i2c_param_config(I2C_NUM_0, config);