#pragma once

#include "esp_err.h"
#include "secrets.h"

esp_err_t mqtt_init();
esp_err_t mqtt_connect();
esp_err_t mqtt_publish(const char *topic, const char *payload);
void mqtt_pool();