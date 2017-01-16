#include "constants.h"

#include "mqtt.h"
#include "subsystem.h"

#include "sub_braking.h"
#include "sub_compressor.h"
#include "sub_fan.h"
#include "sub_levitation.h"
#include "sub_wheels.h"


IPAddress server(192, 168, 0, 100);
MQTT mqtt = MQTT(server, 1883);

Braking* sub_braking = new Braking(RELAY_BRAKING);
Compressor* sub_compressor = new Compressor(RELAY_VFD_ENABLE,
                                            RELAY_COMPRESSOR_ENABLE,
                                            RELAY_COMPRESSOR_START);
Fan* sub_fan = new Fan(RELAY_FAN_ENABLE);
Levitation* sub_levitation = new Levitation(RELAY_LEVITATION);
Wheels* sub_wheels = new Wheels(RELAY_AUX_WHEELS);

Subsystem* subsystems[] = {
  sub_braking,
  sub_compressor,
  sub_fan,
  sub_levitation,
  sub_wheels
};

void subscribe() {
  mqtt.client.subscribe("subsystem/braking/#");
  mqtt.client.subscribe("subsystem/compressor/#");
  mqtt.client.subscribe("subsystem/fan/#");
  mqtt.client.subscribe("subsystem/levitation/#");
  mqtt.client.subscribe("subsystem/wheels/#");
}

void setup()
{
  Serial.begin(9600);

  pinMode(13, OUTPUT);

  Serial.println();
  Serial.println();
  Serial.println();
  for (int i = 0; i < 10; i++) {
    digitalWrite(13, HIGH);
    Serial.print(F("."));
    if (i % 10 == 0) {
      Serial.println();
    }
    delay(10);
    digitalWrite(13, LOW);
    delay(100);
  }
  Serial.println();
  delay(50);

  for (int i = 2; i < 10; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, RELAY_OFF);
  }

	mqtt.init();
  mqtt.set_subscribe_callback(subscribe);
  mqtt.set_subsystems(subsystems, sizeof(subsystems)/sizeof(Subsystem*));

  mqtt.loop();
  mqtt.debug("relay (" __DATE__ " " __TIME__ ")");
  Serial.println("relay (" __DATE__ " " __TIME__ ")");
}

void loop()
{
	mqtt.loop();

  // Update all items
  for (int i = 0; i < mqtt.subsystem_count; i++) {
    (mqtt.subsystems[i])->update();
  }
}
