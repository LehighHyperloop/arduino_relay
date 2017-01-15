#ifndef __SUB_LEVITATION_H_
#define __SUB_LEVITATION_H_

#include "constants.h"
#include "subsystem.h"
#include "mqtt.h"

class Levitation : public Subsystem {
public:
  enum State {
    OFF,
    ON
  };
  static char* State_str[];

  Levitation(int levitation_pin);
  void update();
  void set_state(State s);
  State get_state();
  virtual void process_msg(MQTT mqtt, char* topic, JsonObject& root);
  virtual void send_heartbeat(MQTT mqtt);

private:
  State target_state, current_state;
  int levitation_pin;

};

#endif
