#include "sub_fan.h"

char* Fan::m_name = "subsystem/fan";

static char* Fan::State_str[] = {
  "STOPPED",
  "RUNNING"
};

Fan::Fan(int fan_pin) : fan_pin(fan_pin) {
  target_state = STOPPED;
  current_state = STOPPED;
}

Fan::State Fan::get_state() {
  return current_state;
}

void Fan::set_state(State s) {
  target_state = s;
}

// Main processing logic
void Fan::update() {
  switch (current_state) {
    case STOPPED:
      if (target_state == RUNNING) {
        digitalWrite(fan_pin, RELAY_ON);
        current_state = RUNNING;
        break;
      }
      break;

    case RUNNING:
      if (target_state == STOPPED) {
        digitalWrite(fan_pin, RELAY_OFF);
        current_state = STOPPED;
        break;
      }
      break;
  }
}

void Fan::process_msg(char* topic, JsonObject& root) {
  if (strncmp(topic, m_name, strlen(m_name)) != 0 ||
      topic[strlen(m_name)] != '/')
    return;

  send_heartbeat();
}

void Fan::send_heartbeat() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["state"] = State_str[current_state];
  root["t_state"] = State_str[target_state];

  root.printTo(mqtt.stringBuffer, sizeof(mqtt.stringBuffer));
  mqtt.client.publish(m_name, mqtt.stringBuffer);
}
