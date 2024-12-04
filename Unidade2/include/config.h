#pragma once

#define HOSTNAME "ESP32-Lab201-Logger_X"

#define CLIENT_ID HOSTNAME

#define MQTT_SERVER "io.adafruit.com"
#define MQTT_PORT 1883

#define MQTT_TOPIC_TEMP "/temperature"
#define MQTT_TOPIC_HUMI "/humidity"
#define MQTT_TOPIC_HEAT "/heat-index"

#define WIFI_CONNECT_ATTEMPTS 3

#define WIFI_TIMEOUT_MS 10000
#define MQTT_TIMEOUT_MS 10000