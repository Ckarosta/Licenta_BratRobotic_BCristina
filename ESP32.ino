#include <WiFi.h>
#include <PubSubClient.h>

#define RXD2 16   
#define TXD2 17   

const char* ssid = "Camera 301B";
const char* password = "Camera301B";
const char* mqtt_server = "192.168.1.105"; 

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Conectare la ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectat");
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectare la MQTT...");
    if (client.connect("ESP32TestClient")) {
      Serial.println("Conectat!");
    } else {
      Serial.print("Eroare, rc=");
      Serial.print(client.state());
      Serial.println("Incerc din nou in 5 secunde...");
      delay(500);
    }
  }
}

void setup() {
  Serial.begin(115200); //Serial Monitor
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); //Serial2 - hardware UART
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (Serial2.available()) {
    String linie = Serial2.readStringUntil('\n');
    linie.trim();

    if (linie.startsWith("COLOR:")) {
      String culoare = linie.substring(6);
      culoare.trim();
      String payload = "{\"color\":\"" + culoare + "\"}";
      client.publish("brat/culoare", payload.c_str());
      Serial.println("Trimis MQTT: " + payload);
    }
  }
}
