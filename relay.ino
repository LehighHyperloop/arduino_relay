#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <BridgeClient.h>
#include <Bridge.h>

#include "constants.h"
#include "mqtt.h"
#include "receiver.h"

IPAddress server(192, 168, 0, 100);
MQTT mqtt = MQTT(server, 1883);

void subscribe()
{
  mqtt.client.subscribe(mqtt.receiver.fullPrefix.c_str());
}

void setup()
{
    Serial.begin(9600);

    /*flash onboard LED for debug*/
    Serial.println();
    Serial.println();
    Serial.println();
    for (int i = 0; i < 10; i++)
    {
        digitalWrite(13, HIGH);
        Serial.print(F("."));
        if (i % 10 == 0)
        {
            Serial.println();
        }
        delay(10);
        digitalWrite(13, LOW);
        delay(100);
    }
    Serial.println();
    delay(50);
    /**/

    //set up relays and start them all at off
    for (int i = 2; i < 10; i++)
    {
        pinMode(i, OUTPUT);
        digitalWrite(i, RELAY_OFF);
    }

    mqtt.init();
    mqtt.set_subscribe_callback(subscribe);

    mqtt.loop();
    mqtt.debug("relay (" __DATE__ " " __TIME__ ")");
    Serial.println("relay (" __DATE__ " " __TIME__ ")");
}

void loop()
{
	mqtt.loop();
}
