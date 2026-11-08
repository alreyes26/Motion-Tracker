#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_log.h"


class i2c_driver 
{
    public:
        i2c_driver(i2c_mode, i2c_port_t, gpio_num_t, gpio_num_t, uint32_t);
        




}