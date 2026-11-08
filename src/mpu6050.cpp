
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

}
