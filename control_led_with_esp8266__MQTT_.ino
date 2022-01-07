#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//i'm using mosquitto as the MQTT BROKER 

#define LED 5 // D1

// WiFi
const char *ssid = "TT_4A58"; // Enter your WiFi name
const char *password = "ucqefd64y7";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "test.mosquitto.org";
const char *topic = "esp8266/led";
const char *mqtt_username = "";
const char *mqtt_password = "";
const int mqtt_port = 1883;
const char *client_id = "esp8266-client" ; 

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
    // Set software serial baud to 115200;
    Serial.begin(115200);
    // connecting to a WiFi network
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
    //connecting to a mqtt broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);
    while (!client.connected()) {
        /*String client_id = "esp8266-client-";
        client_id+= String(WiFi.macAddress());*/
        
        Serial.println("Connecting to public mqtt broker.....");
        if (client.connect(client_id, mqtt_username, mqtt_password)) {
            Serial.println("mqtt broker connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
    // publish and subscribe
    client.publish(topic,"hello MQTT BROKER");
    client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    String message ;
    for (int i = 0; i < length; i++) {      
        message = message + (char)payload[i];  // convert *byte to string
    }
    String msgString = String(message); 
    Serial.println(msgString);
    if (msgString == "on") { // LED on
      digitalWrite(LED, HIGH); 
      }   
    else if (msgString == "off") {  // LED off
    digitalWrite(LED, LOW); 
    } 
    Serial.println();
    Serial.println("-----------------------");
}

void loop() {
    client.loop();
}
