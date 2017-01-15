#include "sub_inverters.h"

static char* Inverters::State_str[] = {
  "STOPPED",
  "RUNNING"
};

Inverters::Inverters(int inverters_pin) : inverters_pin(inverters_pin) {
  target_state = STOPPED;
  current_state = STOPPED;
}

Inverters::State Inverters::get_state() {
  return current_state;
}

void Inverters::set_state(State s) {
  target_state = s;
}

// Main processing logic
void Inverters::update() {
  switch (current_state) {
    case STOPPED:
      if (target_state == RUNNING) {
        digitalWrite(inverters_pin, RELAY_ON);
        current_state = RUNNING;
        break;
      }
      break;

    case RUNNING:
      if (target_state == STOPPED) {
        digitalWrite(inverters_pin, RELAY_OFF);
        current_state = STOPPED;
        break;
      }
      break;
  }
}

void Inverters::process_msg(MQTT mqtt, char* topic, JsonObject& root) {
  if (strncmp(topic, "subsystem/inverters", 10 + 3) != 0)
    return;

  mqtt.debug("INVERTERS");
}

void Inverters::send_heartbeat(MQTT mqtt) {
  JsonObject& root = mqtt.jsonBuffer.createObject();

  root["state"] = State_str[current_state];
  root["t_state"] = State_str[target_state];

  String string_status;
  root.printTo(string_status);
  mqtt.client.publish("subsystem/inverters", string_status.c_str());
}
