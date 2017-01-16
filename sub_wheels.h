#ifndef __SUB_WHEELS_H_
#define __SUB_WHEELS_H_

#include "constants.h"
#include "subsystem.h"
#include "mqtt.h"

class Wheels : public Subsystem {
public:
  enum State {
    STOPPED,
    RUNNING
  };
  static char* State_str[];

  Wheels(int wheels_pin);
  void update();
  void set_state(State s);
  State get_state();
  virtual void process_msg(char* topic, JsonObject& root);
  virtual void send_heartbeat();

private:
  static char* m_name;
  State target_state, current_state;
  int wheels_pin;

};

extern MQTT mqtt;

#endif
