
#pragma once

#include <WiFi101.h>

#include "../exo_glob.h"
#if SAMPLES_STREAM_PROTOCOL == 1
#include <WiFiUdp.h>
#endif


void displayWifiData();
void displayCurrentNet();
