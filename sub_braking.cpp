#include "sub_braking.h"

char* Braking::m_name = "subsystem/braking";

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
  last_state = current_state;

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

  if (last_state != current_state) {
    send_heartbeat();
  }
}

void Braking::process_msg(char* topic, JsonObject& root) {
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

void Braking::send_heartbeat() {
  StaticJsonBuffer<MQTT_BUFFER_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["state"] = State_str[current_state];
  root["t_state"] = State_str[target_state];

  root.printTo(mqtt.stringBuffer, sizeof(mqtt.stringBuffer));
  mqtt.client.publish(m_name, mqtt.stringBuffer);
}
