#ifndef __SUB_BRAKING_H
#define __SUB_BRAKING_H

#include "constants.h"
#include "subsystem.h"
#include "mqtt.h"

class Braking : public Subsystem {
public:
  enum State {
    OFF,
    ON,
    STATE_LENGTH
  };
  static char* State_str[];

  Braking(int braking_pin);
  virtual void update();
  void set_state(State s);
  State get_state();
  virtual void process_msg(char* topic, JsonObject& root);
  virtual void send_heartbeat();

private:
  static char* m_name;
  State target_state, current_state, last_state;
  int braking_pin;

};

extern MQTT mqtt;

#endif
