#include "SPIFFS.h"
#include "config.h"
#include "file_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "mqtt.h"
#include "sensor.h"
#include "wifi_handle.h"
#include <Arduino.h>

#define DELAY_MS 1000

QueueHandle_t data_queue;

void vMqttTask(void *pvParameters)
{
    mqtt_init();

    char topic_buffer[64];

    sensor_data_t data_buffer;

    for (;;) {
        if (xQueueReceive(data_queue, &data_buffer, portMAX_DELAY) == pdTRUE) {
            mqtt_pool();
            mqtt_publish(MQTT_TOPIC_TEMP,
                         String(data_buffer.temperature).c_str());
            mqtt_publish(MQTT_TOPIC_HUMI, String(data_buffer.humidity).c_str());
            mqtt_publish(MQTT_TOPIC_HEAT,
                         String(data_buffer.heat_index).c_str());
            append_data(data_buffer.temperature, TEMPERATURE_TYPE);
            append_data(data_buffer.humidity, HUMIDITY_TYPE);
            append_data(data_buffer.heat_index, HEAT_TYPE);
        }
    }
}

void vSensorTask(void *pvParameters)
{
    sensor_init();
    sensor_data_t data;

    for (;;) {
        if (sensor_get_data(&data) == ESP_OK) {

            xQueueSend(data_queue, &data, portMAX_DELAY);

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

void setup()
{
    Serial.begin(115200);
    wifi_init();
    spiffs_init();

    data_queue = xQueueCreate(2, sizeof(sensor_data_t));

    xTaskCreate(vSensorTask, "Sensor Task",
                configMINIMAL_STACK_SIZE + sizeof(sensor_data_t) + 1024, NULL,
                1, NULL);
    xTaskCreate(vMqttTask, "MQTT Task",
                configMINIMAL_STACK_SIZE + sizeof(sensor_data_t) + 1024, NULL,
                1, NULL);
}

void loop() {}