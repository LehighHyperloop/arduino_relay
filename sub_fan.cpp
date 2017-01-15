#include "sub_fan.h"

static char* Fan::State_str[] = {
  "OFF",
  "ON"
};

Fan::Fan(int fan_pin) : fan_pin(fan_pin) {
  target_state = OFF;
  current_state = OFF;
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
    case OFF:
      if (target_state == ON) {
        digitalWrite(fan_pin, RELAY_ON);
        target_state = OFF;
        break;
      }
      break;

    case ON:
      if (target_state == OFF) {
        digitalWrite(fan_pin, RELAY_OFF);
        target_state = OFF;
        break;
      }
      break;
  }
}

void Fan::process_msg(MQTT mqtt, char* topic, JsonObject& root) {
  if (strncmp(topic, "subsystem/fan", 10 + 3) != 0)
    return;

  mqtt.debug("FAN");
}

void Fan::send_heartbeat(MQTT mqtt) {
  JsonObject& root = mqtt.jsonBuffer.createObject();

  root["state"] = State_str[current_state];
  root["t_state"] = State_str[target_state];

  String string_status;
  root.printTo(string_status);
  mqtt.client.publish("subsystem/fan", string_status.c_str());
}
