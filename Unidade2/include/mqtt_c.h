#pragma once

#include "esp_err.h"
#include <Arduino.h>

esp_err_t mqtt_init();
esp_err_t mqtt_connect();
esp_err_t mqtt_publish(const char *topic, const char *payload);
String mqtt_get_topic(const char *subtopic);
void mqtt_disconnect();
void mqtt_pool();