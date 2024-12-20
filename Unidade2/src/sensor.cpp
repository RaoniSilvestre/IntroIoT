#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

#include "esp_check.h"
#include "sensor.h"

DHT sensor(SENSOR_PIN, SENSOR_TYPE);

void sensor_init() {
    sensor.begin();
}

esp_err_t sensor_get_data(sensor_data_t *sensor_data) {
    esp_err_t ret = ESP_OK;

    float t, h, hi;

    t = sensor.readTemperature();
    h = sensor.readHumidity();
    if (isnan(t) || isnan(h)) {
        return ESP_FAIL;
    }

    hi = sensor.computeHeatIndex(t, h, false);
    sensor_data->temperature = t;
    sensor_data->humidity = h;
    sensor_data->heat_index = hi;

    return ret;
}