#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <Arduino.h>

#define RELAY_ON LOW
#define RELAY_OFF HIGH

#define RELAY_BASE              1

#define RELAY_LEVITATION        1
#define RELAY_AUX_WHEELS        2
#define RELAY_BRAKING           3
#define RELAY_VFD_ENABLE        5
#define RELAY_COMPRESSOR_START  6
#define RELAY_COMPRESSOR_ENABLE 7
#define RELAY_FAN_ENABLE        8

#define HEARTBEAT_INTERVAL 25

// Delay compressor states 100 ms
#define COMPRESSOR_DELAY_TIME 100

#endif
