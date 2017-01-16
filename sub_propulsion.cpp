#include "sub_propulsion.h"

char* Propulsion::m_name = "subsystem/propulsion";

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
  last_state = current_state;

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

  if (last_state != current_state) {
    send_heartbeat();
  }
}

void Propulsion::process_msg(char* topic, JsonObject& root) {
  if (strncmp(topic, m_name, strlen(m_name)) != 0 ||
      topic[strlen(m_name)] != '/')
    return;

  for (int i = 0; i < STATE_LENGTH; i++) {
    if (strcmp(root["t_state"], State_str[i]) == 0) {
      target_state = i;
    }
  }

  send_heartbeat();
}

void Propulsion::send_heartbeat() {
  StaticJsonBuffer<MQTT_BUFFER_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["state"] = State_str[current_state];
  root["t_state"] = State_str[target_state];

  root.printTo(mqtt.stringBuffer, sizeof(mqtt.stringBuffer));
  mqtt.client.publish(m_name, mqtt.stringBuffer);
}
