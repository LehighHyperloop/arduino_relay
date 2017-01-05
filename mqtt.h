#ifndef __MQTT_H__
#define __MQTT_H__

#include "constants.h"
#include "subsystem.h"

#include <ArduinoJson.h>
#include <Bridge.h>
#include <BridgeClient.h>
#include <PubSubClient.h>

class MQTT {
private:
  IPAddress server;
  int port;
  BridgeClient yun;
  String hostname;
  String debug_topic;

  static void callback(char* topic, byte* payload, unsigned int length);
  void reconnect();

  unsigned long lastHeartbeat;

  void send_heartbeat();
  void recv_set();

  void (*func_subscribe_callback)(void);
  Subsystem** subsystems;
  int subsystem_count;

public:
  MQTT(IPAddress server, int port);
  void init();
  void loop();
  void set_subscribe_callback(void (*callback)(void));
  void set_subsystems(Subsystem* s[], int c);

  void debug(char* str) { client.publish(debug_topic.c_str(), str); }

  PubSubClient client;

  StaticJsonBuffer<200> jsonBuffer;
};

extern MQTT mqtt;

#endif
