#include "sub_propulsion.h"

static char* Propulsion::State_str[] = {
  "STOPPED",
  "RUNNING"
};

Propulsion::Propulsion(int propulsion_pin) : propulsion_pin(propulsion_pin) {
  target_state = STOPPED;
  current_state = STOPPED;
}

Propulsion::State Propulsion::get_state() {
  return current_state;
}

void Propulsion::set_state(State s) {
  target_state = s;
}

// Main processing logic
void Propulsion::update() {
  switch (current_state) {
    case STOPPED:
      if (target_state == RUNNING) {
        digitalWrite(propulsion_pin, RELAY_ON);
        current_state = RUNNING;
        break;
      }
      break;

    case RUNNING:
      if (target_state == STOPPED) {
        digitalWrite(propulsion_pin, RELAY_OFF);
        current_state = STOPPED;
        break;
      }
      break;
  }
}

void Propulsion::process_msg(MQTT mqtt, char* topic, JsonObject& root) {
  if (strncmp(topic, "subsystem/propulsion", 10 + 3) != 0)
    return;

  mqtt.debug("PROPULSION");
}

void Propulsion::send_heartbeat(MQTT mqtt) {
  JsonObject& root = mqtt.jsonBuffer.createObject();

  root["state"] = State_str[current_state];
  root["t_state"] = State_str[target_state];

  String string_status;
  root.printTo(string_status);
  mqtt.client.publish("subsystem/propulsion", string_status.c_str());
}
