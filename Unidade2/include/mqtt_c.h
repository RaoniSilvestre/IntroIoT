#pragma once

#include "esp_err.h"

esp_err_t mqtt_init();
esp_err_t mqtt_connect();
esp_err_t mqtt_publish(const char *topic, const char *payload);
char *mqtt_get_topic(char *src, const char *subtopic);
void mqtt_disconnect();
void mqtt_pool();