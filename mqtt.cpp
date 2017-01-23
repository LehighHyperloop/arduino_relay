#include "mqtt.h"

MQTT::MQTT(IPAddress server, int port) : server(server), port(port), func_subscribe_callback(NULL) {
    client.setClient(yun);
}

void MQTT::init() {
  client.setServer(server, port);
  client.setCallback(callback);
  Bridge.begin();

  Process p;
  p.runShellCommand("hostname");
  hostname += "arduino-";
  hostname += p.readString();
  hostname.replace('\n','\0');
  hostname = String(hostname.c_str()); //filthy hack lol

  debug_topic = "debug/" + hostname;
  receiver.init(hostname);
}

void MQTT::loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
    payload[length] = 0; // NULL TERMINATE HACK
    StaticJsonBuffer<MQTT_BUFFER_SIZE> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject((char*)payload);

    mqtt.receiver.process_msg(topic, root);
}

void MQTT::reconnect() {
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect(hostname.c_str()))
        {
            Serial.println("connected");

            if (func_subscribe_callback != NULL)
            {
              func_subscribe_callback();
            }
        }
        else
        {
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
