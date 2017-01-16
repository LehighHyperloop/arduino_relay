#include "sub_fan.h"

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
  char* pre = "subsystem/fan";
  if (strstr(pre, topic) - pre != 0)
   return;
  Serial.println("FAN");

  send_heartbeat();
}

void Fan::send_heartbeat() {
  JsonObject& root = mqtt.jsonBuffer.createObject();

  root["state"] = State_str[current_state];
  root["t_state"] = State_str[target_state];

  root.printTo(mqtt.stringBuffer, sizeof(mqtt.stringBuffer));
  mqtt.client.publish("subsystem/fan", mqtt.stringBuffer);
}
