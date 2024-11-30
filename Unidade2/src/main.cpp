#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sensor.h"

#define DELAY_MS 1000


void vSensorTask(void *pvParameters) {
  sensor_init();
  sensor_data_t data;

  for (;;) {
    if (sensor_get_data(&data) == ESP_OK) {
      Serial.print(F(">Temperature: "));
      Serial.println(data.temperature);
      
      Serial.print(F(">Humidity: "));
      Serial.println(data.humidity);
      
      Serial.print(F(">Heat Index: "));
      Serial.println(data.heat_index);
    } else {
      Serial.println(F("PANICO!"));
    }
    vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
  }
}

void setup() {
  Serial.begin(9600);
  xTaskCreate(vSensorTask, "Sensor Task", configMINIMAL_STACK_SIZE + sizeof(sensor_data_t) + 1024, NULL, 1, NULL);
}

void loop() {}