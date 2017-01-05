#ifndef _SUBSYSTEM_H_
#define _SUBSYSTEM_H_

#include <ArduinoJson.h>

class MQTT;

class Subsystem {
  public:
    virtual void process_msg(MQTT mqtt, char* topic, JsonObject& root);
    virtual void send_heartbeat(MQTT mqtt);
};

#endif
