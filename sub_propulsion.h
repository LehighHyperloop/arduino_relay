#ifndef __SUB_PROPULSION_H
#define __SUB_PROPULSION_H

#include "constants.h"
#include "subsystem.h"
#include "mqtt.h"

class Propulsion : public Subsystem {
public:
  enum State {
    STOPPED,
    RUNNING
  };
  static char* State_str[];

  Propulsion(int propulsion_pin);
  void update();
  void set_state(State s);
  State get_state();
  virtual void process_msg(MQTT mqtt, char* topic, JsonObject& root);
  virtual void send_heartbeat(MQTT mqtt);

private:
  State target_state, current_state;
  int propulsion_pin;

};

#endif
