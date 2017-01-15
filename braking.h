#ifndef __SUB_BRAKING_H
#define __SUB_BRAKING_H

#include "constants.h"
#include "subsystem.h"
#include "mqtt.h"

class Braking : public Subsystem {
public:
  enum State {
    OFF,
    ON
  };
  static char* State_str[];

  Braking(int braking_pin);
  void update();
  void set_state(State s);
  State get_state();
  virtual void process_msg(MQTT mqtt, char* topic, JsonObject& root);
  virtual void send_heartbeat(MQTT mqtt);

private:
  State target_state, current_state;
  int braking_pin;

};

#endif
