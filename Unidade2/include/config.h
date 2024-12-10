#pragma once

#define HOSTNAME "Ordep_1/feeds/esp32-lab201-logger-x"

#define CLIENT_ID HOSTNAME

#define MQTT_SERVER "io.adafruit.com"
#define MQTT_PORT 1883

#define MQTT_TOPIC_TEMP "-slash-temperature"
#define MQTT_TOPIC_HUMI "-slash-humidity"
#define MQTT_TOPIC_HEAT "-slash-heat-index"

#define WIFI_CONNECT_ATTEMPTS 3

#define WIFI_TIMEOUT_MS 10000
#define MQTT_TIMEOUT_MS 10000