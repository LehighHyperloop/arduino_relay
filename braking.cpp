#include "sub_braking.h"

static char* Braking::State_str[] = {
  "OFF",
  "ON"
};

Braking::Braking(int braking_pin) : braking_pin(braking_pin) {
  target_state = OFF;
  current_state = OFF;
}

Braking::State Braking::get_state() {
  return current_state;
}

void Braking::set_state(State s) {
  target_state = s;
}

// Main processing logic
void Braking::update() {
  switch (current_state) {
    case OFF:
      if (target_state == ON) {
        digitalWrite(braking_pin, RELAY_ON);
        current_state = ON;
        break;
      }
      break;

    case ON:
      if (target_state == OFF) {
        digitalWrite(braking_pin, RELAY_OFF);
        current_state = OFF;
        break;
      }
      break;
  }
}

void Braking::process_msg(MQTT mqtt, char* topic, JsonObject& root) {
  if (strncmp(topic, "subsystem/braking", 10 + 3) != 0)
    return;

  mqtt.debug("BRAKING");
}

void Braking::send_heartbeat(MQTT mqtt) {
  JsonObject& root = mqtt.jsonBuffer.createObject();

  root["state"] = State_str[current_state];
  root["t_state"] = State_str[target_state];

  String string_status;
  root.printTo(string_status);
  mqtt.client.publish("subsystem/braking", string_status.c_str());
}
