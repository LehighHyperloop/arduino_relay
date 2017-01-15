#include "sub_levitation.h"

static char* Levitation::State_str[] = {
  "OFF",
  "ON"
};

Levitation::Levitation(int levitation_pin) : levitation_pin(levitation_pin) {
  target_state = OFF;
  current_state = OFF;
}

Levitation::State Levitation::get_state() {
  return current_state;
}

void Levitation::set_state(State s) {
  target_state = s;
}

// Main processing logic 
void Levitation::update() {
  switch (current_state) {
    case OFF:
      if (target_state == ON) {
        digitalWrite(levitation_pin, RELAY_ON);
        target_state = OFF;
        break;
      }
      break;

    case ON:
      if (target_state == OFF) {
        digitalWrite(levitation_pin, RELAY_OFF);
        target_state = OFF;
        break;
      }
      break;
  }
}

void Levitation::process_msg(MQTT mqtt, char* topic, JsonObject& root) {
  if (strncmp(topic, "subsystem/levitation", 10 + 3) != 0)
    return;

  mqtt.debug("LEVITATION");
}

void Levitation::send_heartbeat(MQTT mqtt) {
  JsonObject& root = mqtt.jsonBuffer.createObject();

  root["state"] = State_str[current_state];
  root["t_state"] = State_str[target_state];

  String string_status;
  root.printTo(string_status);
  mqtt.client.publish("subsystem/levitation", string_status.c_str());
}
