#include <WiFi.h>
#include <PubSubClient.h>

byte Vrx_pin = 34;
byte Vry_pin = 35;
byte SW =      32;

const char *ssid = "ssid";
const char *password = "password";
const char *mqtt_broker_id ="192.168.43.88";
const int mqtt_broker_port = 1883;

const char* Vrx_topic = "Vrx";
const char* Vry_topic = "Vry";

bool Vrx_y_toggle = true;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // put your setup code here, to run once:
  pinMode(Vrx_pin, INPUT);
//  pinMode(Vry_pin, INPUT);
  pinMode(SW,      INPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  //connecting to a mqtt broker
  client.setServer(mqtt_broker_id, mqtt_broker_port);

  while(!client.connected()) {
    Serial.println("Connecting to Jetson Nano...");
    if (client.connect("Controller")) {
            Serial.println("Connected to Jetson Nano");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
  }
}

void loop() {
  if(!client.connected()){
    if (client.connect("Controller")) {
            Serial.println("Connected to Jetson Nano");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
    // put your main code here, to run repeatedly:
  int Vrx_read = analogRead(Vrx_pin);
  int Vry_read = analogRead(Vry_pin);
  int SW_read = digitalRead(SW);

  Serial.print("X axis: "); Serial.println(Vrx_read);
  Serial.print("Y axis: "); Serial.println(Vry_read);
  Serial.print("Button: "); Serial.println(SW_read) ;

  String payload_x = String(Vrx_read);
  char payload_cx[4];
  payload_x.toCharArray(payload_cx, payload_x.length() + 1);
  
  client.publish(Vrx_topic, payload_cx);

  String payload_y = String(Vry_read);
  char payload_cy[4];
  payload_y.toCharArray(payload_cy, payload_y.length() + 1);
  
  client.publish(Vry_topic, payload_cy);
  
  delay(250);
}
