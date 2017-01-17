#ifndef _RECEIVER_H_
#define _RECEIVER_H_

#include <ArduinoJson.h>
#include <Bridge.h>

class MQTT;

class Receiver {
public:
    Receiver();
    void init(String hostname);
    void process_msg(char* topic, JsonObject& root);
    void send_heartbeat(JsonObject &root);
    String hostname;
    String keyword;
    String verb;
    String fullPrefix;
    String returnTopic;
};

extern MQTT mqtt;

#endif
