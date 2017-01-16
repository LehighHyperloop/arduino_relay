#include "sub_wheels.h"

char* Wheels::m_name = "subsystem/wheels";

static char* Wheels::State_str[] = {
  "UP",
  "DOWN"
};

Wheels::Wheels(int wheels_pin) : wheels_pin(wheels_pin) {
  target_state = UP;
  current_state = UP;
}

Wheels::State Wheels::get_state() {
  return current_state;
}

void Wheels::set_state(State s) {
  target_state = s;
}

// Main processing logic
void Wheels::update() {
  last_state = current_state;

  switch (current_state) {
    case UP:
      if (target_state == DOWN) {
        digitalWrite(wheels_pin, RELAY_ON);
        current_state = DOWN;
        break;
      }
      break;

    case DOWN:
      if (target_state == UP) {
        digitalWrite(wheels_pin, RELAY_OFF);
        current_state = UP;
        break;
      }
      break;
  }

  if (last_state != current_state) {
    send_heartbeat();
  }
}

void Wheels::process_msg(char* topic, JsonObject& root) {
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

void Wheels::send_heartbeat() {
  StaticJsonBuffer<MQTT_BUFFER_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["state"] = State_str[current_state];
  root["t_state"] = State_str[target_state];

  root.printTo(mqtt.stringBuffer, sizeof(mqtt.stringBuffer));
  mqtt.client.publish(m_name, mqtt.stringBuffer);
}
