#pragma once

#define HOSTNAME "ESP32-Lab201-Logger_X"

#define CLIENT_ID HOSTNAME

#define MQTT_TOPIC_TEMP "/lab201_logger_X/temperature"
#define MQTT_TOPIC_HUMI "/lab201_logger_X/humidity"
#define MQTT_TOPIC_HEAT "/lab201_logger_X/heat-index"

#define WIFI_CONNECT_ATTEMPTS 3

#define WIFI_TIMEOUT_MS 30000
#define MQTT_TIMEOUT_MS 10000