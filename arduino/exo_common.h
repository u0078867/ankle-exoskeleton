
#pragma once

#include <Arduino.h>
#include <avr/dtostrf.h>
#include <stdlib.h>     // strtod

#define DEBUG_PRINT(...)  Serial.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...)  Serial.println(__VA_ARGS__)
#define MILLIS()    millis()
#define DELAY(x)    delay(x)
#define DTOSTRF(...)    dtostrf(__VA_ARGS__)   // avr-gcc does not support sprintf() for floats
#define STRTOD(x)       strtod(x, NULL)
#define STRTOUL(x)      strtoul(x, NULL, 10)


#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif
