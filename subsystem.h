#ifndef _SUBSYSTEM_H_
#define _SUBSYSTEM_H_

#include <ArduinoJson.h>

class Subsystem {
  public:
    virtual void update() {};
    virtual void process_msg(char* topic, JsonObject& root) {};
    virtual void send_heartbeat() {};
};

#endif
