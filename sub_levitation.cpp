#include "sub_levitation.h"

char* Levitation::m_name = "subsystem/levitation";

static char* Levitation::State_str[] = {
  "STOPPED",
  "RUNNING"
};

Levitation::Levitation(int levitation_pin) : levitation_pin(levitation_pin) {
  target_state = STOPPED;
  current_state = STOPPED;
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
    case STOPPED:
      if (target_state == RUNNING) {
        digitalWrite(levitation_pin, RELAY_ON);
        current_state = RUNNING;
        break;
      }
      break;

    case RUNNING:
      if (target_state == STOPPED) {
        digitalWrite(levitation_pin, RELAY_OFF);
        current_state = STOPPED;
        break;
      }
      break;
  }
}

void Levitation::process_msg(char* topic, JsonObject& root) {
  if (strncmp(topic, m_name, strlen(m_name)) != 0 ||
      topic[strlen(m_name)] != '/')
    return;

  send_heartbeat();
}

void Levitation::send_heartbeat() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["state"] = State_str[current_state];
  root["t_state"] = State_str[target_state];

  root.printTo(mqtt.stringBuffer, sizeof(mqtt.stringBuffer));
  mqtt.client.publish(m_name, mqtt.stringBuffer);
}
