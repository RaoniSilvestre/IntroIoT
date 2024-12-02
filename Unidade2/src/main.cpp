#include <Arduino.h>
#include "SPIFFS.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sensor.h"
#include "file_sys.h"

#define DELAY_MS 1000

void vSensorTask(void *pvParameters)
{
  sensor_init();
  spiffs_init();

  sensor_data_t data;

  for (;;)
  {
    if (sensor_get_data(&data) == ESP_OK)
    {
      append_data(data.temperature, TEMPERATURE_TYPE);
      append_data(data.humidity, HUMIDITY_TYPE);
      append_data(data.heat_index, HEAT_TYPE);

      Serial.print(F(">Temperature: "));
      Serial.println(data.temperature);

      Serial.print(F(">Humidity: "));
      Serial.println(data.humidity);

      Serial.print(F(">Heat Index: "));
      Serial.println(data.heat_index);
    }
    else
    {
      Serial.println(F("PANICO!"));
    }
    vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
  }
}

void setup()
{
  Serial.begin(9600);
  xTaskCreate(vSensorTask, "Sensor Task", configMINIMAL_STACK_SIZE + sizeof(sensor_data_t) + 1024, NULL, 1, NULL);
}

void loop() {}