#include "mqtt.h"

MQTT::MQTT(IPAddress server, int port) : server(server), port(port), func_subscribe_callback(NULL) {
  client.setClient(yun);
}

void MQTT::init() {
  client.setServer(server, port);
  client.setCallback(MQTT::callback);
  Bridge.begin();

  Process p;
  p.runShellCommand("hostname");
  hostname += "arduino-";
  hostname += p.readString();

  debug_topic = "arduino/" + hostname;
}

void MQTT::loop() {
  if (!client.connected()) {
    reconnect();
  }

  if (millis() - lastHeartbeat > HEARTBEAT_INTERVAL) {
    // Send heartbeat of current state
    send_heartbeat();
  }

  client.loop();
}

static void MQTT::callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = 0; // NULL TERMINATE HACK
  JsonObject& root = mqtt.jsonBuffer.parseObject((char*)payload);
  for (int i = 0; i < mqtt.subsystem_count; i++) {
    mqtt.subsystems[i]->process_msg(mqtt, topic, root);
  }
}

void MQTT::reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(hostname.c_str())) {
      Serial.println("connected");

      if (func_subscribe_callback != NULL) {
        func_subscribe_callback();
      }
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1 seconds");
      delay(1000);
    }
  }
}

void MQTT::set_subscribe_callback(void (*callback)(void)) {
  func_subscribe_callback = callback;
}

void MQTT::set_subsystems(Subsystem* s[], int c) {
  subsystems = s;
  subsystem_count = c;
}

void MQTT::send_heartbeat() {
  for (int i = 0; i < mqtt.subsystem_count; i++) {
    mqtt.subsystems[i]->send_heartbeat(mqtt);
  }
}
