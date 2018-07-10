/*
 Basic CoIoT example

 This sketch demonstrates the basic capabilities of the library.
 
 Don't forget to change:
 SSID
 PASSWORD
 SERVERDOMAIN
 
*/

#include <CoIoT.h>
#include <ESP8266WiFi.h>

int ledPin = LED_BUILTIN; 
const char *ssid = "SSID";     // WiFi (Access point) name
const char *pass = "PASSWORD"; // Password


void actionOne(byte* payload,unsigned int length) { // Common function
    digitalWrite(ledPin, HIGH);
}
void actionTwo(byte* payload,unsigned int length) {
    digitalWrite(ledPin, LOW);
}

WiFiClient wclient;
CoIoT System("BEST_DEVICE_ID", wclient);

void setup() {
    pinMode(ledPin, OUTPUT);
    Serial.begin(38400);
    
    // Connecting to wi-fi
    if (WiFi.status() != WL_CONNECTED) {
        Serial.print("Connecting to ");
        Serial.print(ssid);
        Serial.println("...");
        WiFi.begin(ssid, pass);
        if (WiFi.waitForConnectResult() != WL_CONNECTED)
            return;
        Serial.println("WiFi connected");
    }
    
    System.connect("SERVERDOMAIN", 1883);
    
    System.on("/led/0", actionOne);
    System.on("/led/1", actionTwo);
    
    System.send("/test", "we start");
}

void loop() {
    System.loop(); // Check topics
}
