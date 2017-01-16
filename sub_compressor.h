#ifndef __SUB_COMPRESSOR_H
#define __SUB_COMPRESSOR_H

#include "constants.h"
#include "subsystem.h"
#include "mqtt.h"

class Compressor : public Subsystem {
public:
  enum State {
    RUNNING,
    STOPPED,
    VFD_STARTING,
    VFD_STOPPING,
    COMPRESSOR_STARTING,
    COMPRESSOR_STOPPING,
    STATE_LENGTH
  };
  static char* State_str[];

  Compressor(int vfd_pin, int compressor_enable_pin, int compressor_start_pin);
  virtual void update();
  void set_state(State s);
  State get_state();
  virtual void process_msg(char* topic, JsonObject& root);
  virtual void send_heartbeat();

private:
  static char* m_name;
  State target_state, current_state;
  int vfd_pin, compressor_enable_pin, compressor_start_pin;

};

extern MQTT mqtt;

#endif
