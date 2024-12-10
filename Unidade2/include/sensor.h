#pragma once

#include "esp_err.h"
#include "file_sys.h"

#define SENSOR_PIN GPIO_NUM_4
#define SENSOR_TYPE DHT11

typedef struct {
    float temperature;
    float humidity;
    float heat_index;
} sensor_data_t;

void sensor_init();
esp_err_t sensor_get_data(sensor_data_t *sensor_data);