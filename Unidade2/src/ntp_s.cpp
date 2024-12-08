#include "ntp_s.h"

#include "time.h"
#include <WiFi.h>

const char *ntp_srvr = "pool.ntp.org";
const long gmtOffset_sec = 3 * 3600;
const int daylightOffset_sec = 3600;

void ntp_init() { configTime(gmtOffset_sec, daylightOffset_sec, ntp_srvr); }