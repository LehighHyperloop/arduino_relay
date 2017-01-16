#include "sub_compressor.h"

char* Compressor::m_name = "subsystem/compressor";

static char* Compressor::State_str[] = {
    "RUNNING",
    "STOPPED",
    "VFD_STARTING",
    "VFD_STOPPING",
    "COMPRESSOR_STARTING",
    "COMPRESSOR_STOPPING"
};

static unsigned long startTime;

Compressor::Compressor(int vfd_pin, int compressor_enable_pin, int compressor_start_pin) \
                       : vfd_pin(vfd_pin), compressor_enable_pin(compressor_enable_pin), \
                       compressor_start_pin(compressor_start_pin) {
  target_state = STOPPED;
  current_state = STOPPED;
}

Compressor::State Compressor::get_state() {
  return current_state;
}

void Compressor::set_state(State s) {
  target_state = s;
}

// Main processing logic
void Compressor::update() {
  last_state = current_state;

  //gnarly looking state switch, hope this is right
  switch(current_state)
  {
    case STOPPED:
      if(target_state == RUNNING) {
        digitalWrite(vfd_pin, RELAY_ON);
        current_state = VFD_STARTING;
        startTime = millis();
        break;
      }
      break;
    case VFD_STARTING:
      if((target_state == RUNNING) && (millis() - startTime > COMPRESSOR_DELAY_TIME)) {
        digitalWrite(compressor_enable_pin, RELAY_ON);
        current_state = COMPRESSOR_STARTING;
        startTime = millis();
        break;
      }
      break;
    case COMPRESSOR_STARTING:
      if((target_state == RUNNING) && (millis() - startTime > COMPRESSOR_DELAY_TIME)) {
        digitalWrite(compressor_start_pin, RELAY_ON);
        current_state = RUNNING;
        startTime = millis();
        break;
      }
      break;
    case RUNNING:
      digitalWrite(compressor_start_pin, RELAY_OFF); //don't need to start it when it is running
      if(target_state == STOPPED) {
        digitalWrite(compressor_enable_pin, RELAY_OFF);
        current_state = COMPRESSOR_STOPPING;
        startTime = millis();
        break;
      }
      break;
    case COMPRESSOR_STOPPING:
      if((target_state == STOPPED) && (millis() - startTime > COMPRESSOR_DELAY_TIME)) {
        digitalWrite(vfd_pin, RELAY_OFF);
        current_state = VFD_STOPPING;
        startTime = millis();
        break;
      }
      break;
    case VFD_STOPPING:
      if((target_state == STOPPED) && (millis() - startTime > COMPRESSOR_DELAY_TIME)) {
        current_state = STOPPED;
        break;
      }
      break;
  }

  if (last_state != current_state) {
    send_heartbeat();
  }
}

void Compressor::process_msg(char* topic, JsonObject& root) {
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

void Compressor::send_heartbeat() {
  StaticJsonBuffer<MQTT_BUFFER_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["state"] = State_str[current_state];
  root["t_state"] = State_str[target_state];

  root.printTo(mqtt.stringBuffer, sizeof(mqtt.stringBuffer));
  mqtt.client.publish(m_name, mqtt.stringBuffer);
}
