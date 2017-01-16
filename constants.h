#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <Arduino.h>

#define RELAY_ON LOW
#define RELAY_OFF HIGH

#define RELAY_LEVITATION        2
#define RELAY_AUX_WHEELS        3
#define RELAY_BRAKING           4
#define RELAY_VFD_ENABLE        6
#define RELAY_COMPRESSOR_START  7
#define RELAY_COMPRESSOR_ENABLE 8
#define RELAY_FAN_ENABLE        9

#define HEARTBEAT_INTERVAL 25

// Delay compressor states 100 ms
#define COMPRESSOR_DELAY_TIME 100

#define MQTT_BUFFER_SIZE 100

#endif
