#include "constants.h"

#include "mqtt.h"
#include "subsystem.h"
#include "sub_fan.h"

IPAddress server(192, 168, 0, 100);
MQTT mqtt = MQTT(server, 1883);

Fan* sub_fan = new Fan(RELAY_BASE + RELAY_FAN_ENABLE);

Subsystem* subsystems[] = {
  sub_fan
};

void subscribe() {
  mqtt.client.subscribe("subsystem/fan/#");
}

void setup()
{
  Serial.begin(9600);
	mqtt.init();
  mqtt.set_subscribe_callback(subscribe);
  mqtt.set_subsystems(subsystems, sizeof(subsystems)/sizeof(Subsystem*));

  mqtt.loop();
  mqtt.debug("relay (" __DATE__ " " __TIME__ ")");
}

void loop()
{
	mqtt.loop();

  /*
  JsonObject& root = mqtt.jsonBuffer.createObject();
  root["unit"] = "F";
  root["data"] = 73;

  String string_status;
  root.printTo(string_status);
  mqtt.client.publish("sensor/front/temp", string_status.c_str());
  */
}
