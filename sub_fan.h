#ifndef __SUB_FAN_H_
#define __SUB_FAN_H_

#include "constants.h"
#include "subsystem.h"
#include "mqtt.h"

class Fan : public Subsystem {
public:
  enum State {
    OFF,
    ON
  };
  static char* State_str[];

  Fan(int fan_pin);
  void update();
  void set_state(State s);
  State get_state();
  virtual void process_msg(char* topic, JsonObject& root);
  virtual void send_heartbeat();

private:
  State target_state, current_state;
  int fan_pin;

};

extern MQTT mqtt;

#endif
