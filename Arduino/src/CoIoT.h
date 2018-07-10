/*
  CoIoT.h - A simple wrapper for MQTT client.
  Powered by Igor Voevodov
  https://corru.ru
*/

#ifndef CoIot_h
#define CoIot_h

#include <Arduino.h>
#include <PubSubClient.h>
#include "Client.h"
//#include "map.h"
#include "patricia.h"

typedef std::function<void(byte*,unsigned int)> TCallbackHandlerFunction;

/*namespace CoIoT {
    class Command {
    public:
        virtual ~Command() {}
        virtual void execute() = 0;
    };
}*/

class CoIoT {
private:
    char* client_id;
    char* user_name = "";
    char* user_pass = "";
    Client& eth_client;
    PubSubClient client;
    static TPatricia action_fn_pairs;
    //CoIoT::map actions
    
    static void callback(char* topic, byte* payload, unsigned int length);
    
public:
    CoIoT(char* device_name, char* user_name, char* user_pass, Client& client);
    CoIoT(char* device_name, char* user_name, Client& client);
    CoIoT(char* device_name, Client& client);

    int reconnect();
    int connect(IPAddress address, uint16_t port);
    int connect(const char* domain, uint16_t port);
    int connectToCloud(const char* cloudIdentificator, IPAddress address, uint16_t port);
    int connectToCloud(const char* cloudIdentificator, uint8_t* domain, uint16_t port);
    
    void loop();
    
    int send(const char* topic, const char* data);
    int sendOnChange(const char* topic, const char* data);
    
    //void on(const char* topic, CoIoT::Command);
    void on(const char* topic, TCallbackHandlerFunction function);

};


#endif
