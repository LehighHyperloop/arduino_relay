#include "receiver.h"
#include "mqtt.h"


Receiver::Receiver()
{
    keyword = "remote";
    verb = "set";
}

void Receiver::init(String hostname)
{
    fullPrefix = keyword + "/" + hostname + "/" + verb;
    returnTopic = keyword + "/" + hostname;
    Serial.println(fullPrefix);
}

//single class to handle response, instead of the multiple subsystems
void Receiver::process_msg(char * topic, JsonObject &root)
{
    const char * prefix_string = fullPrefix.c_str();

    if(strncmp(topic, prefix_string, strlen(prefix_string)) != 0)
        return;

    const char * bitString = root["relays"];
    const char * timeStamp = root["t"];

    int writeValue = 0;
    int count = 0;
    for(const char * i=bitString; *i; i++) //iterate through char*
    {
        digitalWrite(count+2, *i == '0' ? HIGH : LOW); //relay pins start at 2
        count++;
    }

    send_heartbeat(root);
}

void Receiver::send_heartbeat(JsonObject &root)
{
    // StaticJsonBuffer<200> jsonBuffer;
    // JsonObject& new_root = jsonBuffer.createObject();

    root.printTo(mqtt.stringBuffer, sizeof(mqtt.stringBuffer));

    mqtt.client.publish(returnTopic.c_str(), mqtt.stringBuffer);
}
