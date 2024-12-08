#pragma once

#include <Arduino.h>
#include "FS.h"
#include "SPIFFS.h"

#define TEMPERATURE_TYPE 0
#define HUMIDITY_TYPE 1
#define HEAT_TYPE 2

void spiffs_init();

void append_data(float value, int data_type);
void reset_data_file();
int* get_values();
void log(const char * message);