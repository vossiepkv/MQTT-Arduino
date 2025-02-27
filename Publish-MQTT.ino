#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include <Wire.h>

const int trigPin = 9;  
const int echoPin = 10;

float duration, distance;  


#include "secret.h" 
// Please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // Your network SSID (name)
char pass[] = SECRET_PASS;    // Your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // The WiFi radio's status

WiFiClient  client;
MqttClient mqttClient(wifiClient);

const char broker[] = "test.mosquitto.org";
int        port     = 1883;
const char topic[]  = "Wave";


const long interval = 8000;
unsigned long previousMillis = 0;

int count = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // Wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to WiFi");

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

pinMode(trigPin, OUTPUT);  
pinMode(echoPin, INPUT);  

}

void loop() {
  // put your main code here, to run repeatedly:
  mqttClient.poll();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    digitalWrite(trigPin, LOW);  
	  delayMicroseconds(2);  
	  digitalWrite(trigPin, HIGH);  
	  delayMicroseconds(10);  
	  digitalWrite(trigPin, LOW);  

    duration = pulseIn(echoPin, HIGH); 

    distance = (duration*.0343)/2;  

 
    Serial.print("Sending message to topic: ");
    Serial.println(topic);
    Serial.println(distance);

    // send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(Wave);
    mqttClient.print(distance);
    mqttClient.endMessage();

    

    Serial.println();

    delay (500);
}
