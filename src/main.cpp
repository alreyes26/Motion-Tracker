
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/i2c_master.h"
#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "MOTION_TRACKER";

// I2C configuration
#define SDA_PIN GPIO_NUM_21
#define SCL_PIN GPIO_NUM_22

#define I2C_PORT I2C_NUM_0
#define MPU6050_ADDR 0x68

// MPU6050 registers
#define REG_PWR_MGMT_1 0x6B
#define REG_WHO_AM_I   0x75
#define REG_ACCEL_XOUT 0x3B

i2c_master_bus_handle_t bus_handle = NULL;
i2c_master_dev_handle_t mpu_handle = NULL;


// Initialize I2C
void initialize_i2c()
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

    ESP_LOGI(TAG, "I2C initialized successfully");
}


// Connect to the MPU6050
void initialize_mpu6050()
{
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

    ESP_LOGI(TAG, "MPU6050 device handle created");
}


// Write one byte to an MPU6050 register
void write_register(uint8_t reg, uint8_t value)
{
    uint8_t data[2] = {reg, value};

    ESP_ERROR_CHECK(
        i2c_master_transmit(
            mpu_handle,
            data,
            sizeof(data),
            1000
        )
    );
}


// Read data from an MPU6050 register
void read_register(uint8_t reg, uint8_t *data, size_t length)
{
    ESP_ERROR_CHECK(
        i2c_master_transmit_receive(
            mpu_handle,
            &reg,
            1,
            data,
            length,
            1000
        )
    );
}


// Convert two bytes into a signed 16-bit value
int16_t combine_bytes(uint8_t high, uint8_t low)
{
    return (int16_t)((high << 8) | low);
}


// Wake and verify the MPU6050
void configure_mpu6050()
{
    // Wake sensor
    write_register(REG_PWR_MGMT_1, 0x00);

    vTaskDelay(pdMS_TO_TICKS(100));

    // Check device identity
    uint8_t identity = 0;

    read_register(REG_WHO_AM_I, &identity, 1);

    ESP_LOGI(TAG, "MPU6050 identity: 0x%02X", identity);

    if (identity != MPU6050_ADDR)
    {
        ESP_LOGE(TAG, "Unexpected MPU6050 identity!");
        return;
    }

    ESP_LOGI(TAG, "MPU6050 configured successfully");
}


// Read accelerometer and gyroscope
void read_motion_data()
{
    uint8_t data[14];

    read_register(REG_ACCEL_XOUT, data, 14);

    // Accelerometer
    int16_t accel_x = combine_bytes(data[0], data[1]);
    int16_t accel_y = combine_bytes(data[2], data[3]);
    int16_t accel_z = combine_bytes(data[4], data[5]);

    // Temperature occupies data[6] and data[7]

    // Gyroscope
    int16_t gyro_x = combine_bytes(data[8], data[9]);
    int16_t gyro_y = combine_bytes(data[10], data[11]);
    int16_t gyro_z = combine_bytes(data[12], data[13]);

    ESP_LOGI(TAG,
        "ACCEL: X=%d Y=%d Z=%d | GYRO: X=%d Y=%d Z=%d",
        accel_x, accel_y, accel_z,
        gyro_x, gyro_y, gyro_z
    );
}


// Main application
extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "Starting Motion Tracker...");

    initialize_i2c();

    initialize_mpu6050();

    configure_mpu6050();

    while (true)
    {
        read_motion_data();

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
