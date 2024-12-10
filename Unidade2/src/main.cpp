#include "SPIFFS.h"
#include "config.h"
#include "file_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mqtt_c.h"
#include "ntp_s.h"
#include "sensor.h"
#include "wifi_h.h"
#include <Arduino.h>

#define DELAY_MS 10000

QueueHandle_t data_queue;

void vMqttTask(void *pvParameters) {
  mqtt_init();

  sensor_data_t data_buffer;

  for (;;) {
    if (xQueueReceive(data_queue, &data_buffer, portMAX_DELAY) == pdTRUE) {
      if (mqtt_connect() == ESP_OK) {
        mqtt_pool();

        String temp_topic = mqtt_get_topic(MQTT_TOPIC_TEMP);

        mqtt_publish(temp_topic.c_str(), String(52).c_str());
        log(("Watermark: " + String(uxTaskGetStackHighWaterMark(NULL)))
                .c_str());

        String humi_topic = mqtt_get_topic(MQTT_TOPIC_HUMI);
        mqtt_publish(humi_topic.c_str(), String(data_buffer.humidity).c_str());

        String heat_topic = mqtt_get_topic(MQTT_TOPIC_HEAT);
        mqtt_publish(heat_topic.c_str(),
                     String(data_buffer.heat_index).c_str());
      } else {
        // XXX PÂNICO!
      }

      append_data(data_buffer.temperature, TEMPERATURE_TYPE);
      append_data(data_buffer.humidity, HUMIDITY_TYPE);
      append_data(data_buffer.heat_index, HEAT_TYPE);

      vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
    }
  }
  vTaskDelete(NULL);
}

void vSensorTask(void *pvParameters) {
  sensor_init();
  sensor_data_t data;

  for (;;) {
    if (sensor_get_data(&data) == ESP_OK) {
      xQueueSend(data_queue, &data, portMAX_DELAY);
    } else {
      log("PÂNICO AO PEGAR DADOS");
    }
    vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
  }
  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(115200);
  SPIFFS.format();
  spiffs_init();
  wifi_init();
  ntp_init();

  data_queue = xQueueCreate(2, sizeof(sensor_data_t));

  xTaskCreate(vSensorTask, "Sensor Task",
              configMINIMAL_STACK_SIZE + sizeof(sensor_data_t) + 1024, NULL, 1,
              NULL);
  xTaskCreate(vMqttTask, "MQTT Task",
              configMINIMAL_STACK_SIZE + sizeof(sensor_data_t) + 2048, NULL, 1,
              NULL);
}

void loop() {}