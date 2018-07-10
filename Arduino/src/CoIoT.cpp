/*
  CoIoT.h - A simple wrapper for MQTT client.
  Powered by Igor Voevodov
  https://corru.ru
*/

#include "CoIoT.h"
#include "Client.h"
#include "Arduino.h"

TPatricia CoIoT::action_fn_pairs = TPatricia();

CoIoT::CoIoT(char* device_name, char* user_name, char* user_pass, Client& eth_client)
    : client_id(device_name), user_name(user_name), 
      user_pass(user_pass), eth_client(eth_client) {}
CoIoT::CoIoT(char* device_name, char* user_name, Client& eth_client)
    : client_id(device_name), user_name(user_name), 
      eth_client(eth_client) {}
CoIoT::CoIoT(char* device_name, Client& eth_client)
    : client_id(device_name), eth_client(eth_client) {}
void CoIoT::resubAll() {
    for (uint16_t i = 0; i < topicLength; ++i) {
        client.subscribe(topics[i]);
    }
}
int CoIoT::reconnect() {
    if (!client.connected()) {
        Serial.println("Connecting to MQTT server"); // TODO Delete
        if (client.connect(client_id)) {
            Serial.println("Connected to MQTT server"); // TODO Delete
            client.setCallback(callback);
            resubAll();
            return true;
        } else {
            Serial.println("Could not connect to MQTT server");
            return false;
        }
    }
    return true;
}
int CoIoT::connect(IPAddress address, uint16_t port) {
    // TODO multiconnection
    client = PubSubClient(address, port, eth_client);
    return reconnect();
}
int CoIoT::connect(const char* domain, uint16_t port) {
    // TODO multiconnection
    client = PubSubClient(domain, port, eth_client);
    return reconnect();
}

int CoIoT::connectToCloud(const char* cloudIdentificator, IPAddress address, uint16_t port) {
    // TODO
}
int CoIoT::connectToCloud(const char* cloudIdentificator, uint8_t *domain, uint16_t port) {
    // TODO
}
    
void CoIoT::loop() {
    if(!reconnect()) {
        return;
    }
    client.loop();
}
    
int CoIoT::send(const char* topic, const char* data) {
    return client.publish(topic, data);
}
int CoIoT::sendOnChange(const char* topic, const char* data) {
    // TODO
}
    
void CoIoT::on(const char* topic, TCallbackHandlerFunction function) {
    if(action_fn_pairs.Insert(topic, function) == -1 ){
        // Already exist
    } else {
        client.subscribe(topic);
        topics[topicLength++] = topic;
    }
};

void CoIoT::callback(char* topic, byte* payload, unsigned int length) {
    auto it = action_fn_pairs.Search(topic)->Elem->GetVal();
    if (it) { it(payload, length); }
    //else 
}



