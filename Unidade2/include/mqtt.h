#pragma once

#include "esp_err.h"

esp_err_t mqtt_init();
esp_err_t mqtt_connect();
esp_err_t mqtt_publish(const char *topic, const char *payload);
void mqtt_pool();