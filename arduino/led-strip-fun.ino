#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <PubSubClientTools.h>

#define WIFI_SSID "Civilian"
#define WIFI_PASS "esp8266fun"
// #define MQTT_SERVER "test.mosquitto.org"
#define MQTT_SERVER "broker.mqttdashboard.com"

WiFiClient espClient;
PubSubClient client(MQTT_SERVER, 1883, espClient);
PubSubClientTools mqtt(client);

int value = 0;
String s = "";

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  Serial.print(s+"Connecting to WiFi: "+WIFI_SSID+" ");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(s+" connected with IP: " + WiFi.localIP());

  // Connect to MQTT
  Serial.print(s+"Connecting to MQTT: "+MQTT_SERVER+" ... ");
  if (client.connect("ESP8266Client")) {
    Serial.println("connected");

    mqtt.subscribe("test/inTopic1", topic1_subscriber);
    mqtt.subscribe("test/inTopic2", topic2_subscriber);
  } else {
    Serial.println(s+"failed, rc="+client.state());
  }
}

void loop() {
  client.loop();
}

void publisher() {
  ++value;
  mqtt.publish("test/outTopic", s+"hello world "+value);
}

void topic1_subscriber(String topic, String message) {
  Serial.println(s+"Message arrived in function 1 ["+topic+"] "+message);
}

void topic2_subscriber(String topic, String message) {
  Serial.println(s+"Message arrived in function 2 ["+topic+"] "+message);
}