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


// Define the pins
const int pirPin = 2;   // PIR sensor output pin connected to digital pin 2
const int ledPin = 13;  // Built-in LED connected to digital pin 13

void setup() {
  Serial.begin(9600);        // Initialize serial communication
  pinMode(pirPin, INPUT);    // Set PIR pin as input
  pinMode(ledPin, OUTPUT);   // Set LED pin as output
}
  