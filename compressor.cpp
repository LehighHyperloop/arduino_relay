#include "sub_compressor.h"

static char* Compressor::State_str[] = {
    "RUNNING",
    "STOPPED",
    "VFD_STARTING",
    "VFD_STOPPING",
    "COMPRESSOR_STARTING",
    "COMPRESSOR_STOPPING"
};

static unsigned long startTime;
final static unsigned long delayTime = 100; //100 ms to be safe

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
            if((target_state == RUNNING) && (millis() - startTime > delayTime) {
                digitalWrite(compressor_enable_pin, RELAY_ON);
                current_state = COMPRESSOR_STARTING;
                startTime = millis();
                break;
            }
            break;
        case COMPRESSOR_STARTING:
            if((target_state == RUNNING) && (millis() - startTime > delayTime) {
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
            if((target_state == STOPPED) && (millis() - startTime > delayTime) {
                digitalWrite(vfd_pin, RELAY_OFF);
                current_state = VFD_STOPPING;
                startTime = millis();
                break;
            }
            break;
        case VFD_STOPPING:
            if((target_state == STOPPED) && (millis() - startTime > delayTime) {
                current_state = STOPPED;
                break;
            }
            break;
    }
}

void Compressor::process_msg(MQTT mqtt, char* topic, JsonObject& root) {
  if (strncmp(topic, "subsystem/compressor", 10 + 3) != 0)
    return;

  mqtt.debug("COMPRESSOR");
}

void Compressor::send_heartbeat(MQTT mqtt) {
  JsonObject& root = mqtt.jsonBuffer.createObject();

  root["state"] = State_str[current_state];
  root["t_state"] = State_str[target_state];

  String string_status;
  root.printTo(string_status);
  mqtt.client.publish("subsystem/compressor", string_status.c_str());
}
