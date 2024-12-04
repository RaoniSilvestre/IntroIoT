#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "esp_err.h"

extern WiFiClient wifi_client;

void wifi_init();
esp_err_t wifi_connect();
