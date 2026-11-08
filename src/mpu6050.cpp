
#include "mpu6050.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/i2c_master.h"
#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "MPU6050";

#define SDA_PIN GPIO_NUM_21
#define SCL_PIN GPIO_NUM_22

#define I2C_PORT I2C_NUM_0
#define MPU6050_ADDR 0x68

#define REG_PWR_MGMT_1 0x6B
#define REG_WHO_AM_I 0x75
#define REG_ACCEL_XOUT 0x3B

static i2c_master_bus_handle_t bus_handle = NULL;
static i2c_master_dev_handle_t mpu_handle = NULL;


// Initialize I2C and connect to MPU6050
void initialize_mpu6050()
{
    i2c_master_bus_config_t config = {};

    config.i2c_port = I2C_PORT;
    config.sda_io_num = SDA_PIN;
    config.scl_io_num = SCL_PIN;
    config.clk_source = I2C_CLK_SRC_DEFAULT;
    config.glitch_ignore_cnt = 7;
    config.flags.enable_internal_pullup = true;

    ESP_ERROR_CHECK(
        i2c_new_master_bus(&config, &bus_handle)
    );

    i2c_device_config_t device_config = {};

    device_config.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    device_config.device_address = MPU6050_ADDR;
    device_config.scl_speed_hz = 100000;

    ESP_ERROR_CHECK(
        i2c_master_bus_add_device(
            bus_handle,
            &device_config,
            &mpu_handle
        )
    );

    ESP_LOGI(TAG, "I2C device connected");

    // Wake up MPU6050
    uint8_t wake_data[2] = {REG_PWR_MGMT_1, 0x00};

    ESP_ERROR_CHECK(
        i2c_master_transmit(
            mpu_handle,
            wake_data,
            sizeof(wake_data),
            1000
        )
    );

    vTaskDelay(pdMS_TO_TICKS(100));

    // Verify identity
    uint8_t identity = 0;
    uint8_t reg = REG_WHO_AM_I;

    ESP_ERROR_CHECK(
        i2c_master_transmit_receive(
            mpu_handle,
            &reg,
            1,
            &identity,
            1,
            1000
        )
    );

    ESP_LOGI(TAG, "MPU6050 identity: 0x%02X", identity);

    if (identity != MPU6050_ADDR)
    {
        ESP_LOGE(TAG, "Unexpected sensor identity!");
        abort();
    }

    ESP_LOGI(TAG, "MPU6050 initialized successfully");
}


// Combine two bytes into one signed value
static int16_t combine_bytes(uint8_t high, uint8_t low)
{
    return (int16_t)((high << 8) | low);
}


// Read accelerometer and gyroscope
MotionData read_motion_data()
{
    uint8_t reg = REG_ACCEL_XOUT;
    uint8_t data[14];

    ESP_ERROR_CHECK(
        i2c_master_transmit_receive(
            mpu_handle,
            &reg,
            1,
            data,
            sizeof(data),
            1000
        )
    );

    MotionData motion;

    motion.accel_x = combine_bytes(data[0], data[1]);
    motion.accel_y = combine_bytes(data[2], data[3]);
    motion.accel_z = combine_bytes(data[4], data[5]);

    motion.gyro_x = combine_bytes(data[8], data[9]);
    motion.gyro_y = combine_bytes(data[10], data[11]);
    motion.gyro_z = combine_bytes(data[12], data[13]);

    return motion;
}
