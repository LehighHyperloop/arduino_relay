#ifndef __SUB_PROPULSION_H
#define __SUB_PROPULSION_H

#include "constants.h"
#include "subsystem.h"
#include "mqtt.h"

class Propulsion : public Subsystem {
public:
  enum State {
    STOPPED,
    RUNNING,
    STATE_LENGTH
  };
  static char* State_str[];

  Propulsion(int propulsion_pin);
  virtual void update();
  void set_state(State s);
  State get_state();
  virtual void process_msg(char* topic, JsonObject& root);
  virtual void send_heartbeat();

private:
  static char* m_name;
  State target_state, current_state;
  int propulsion_pin;

};

extern MQTT mqtt;

#endif
