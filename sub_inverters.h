#ifndef __SUB_INVERTERS_H
#define __SUB_INVERTERS_H

#include "constants.h"
#include "subsystem.h"
#include "mqtt.h"

class Inverters : public Subsystem {
public:
  enum State {
    STOPPED,
    RUNNING
  };
  static char* State_str[];

  Inverters(int inverters_pin);
  void update();
  void set_state(State s);
  State get_state();
  virtual void process_msg(MQTT mqtt, char* topic, JsonObject& root);
  virtual void send_heartbeat(MQTT mqtt);

private:
  State target_state, current_state;
  int inverters_pin;

};

#endif
