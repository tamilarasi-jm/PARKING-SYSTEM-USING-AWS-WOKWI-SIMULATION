#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "secrets.h"

/*
  Smart Parking IoT System (ESP32 + AWS IoT)
  ------------------------------------------
  - Publishes ENTRY / EXIT events via MQTT (TLS)
  - AWS IoT Core → Lambda → DynamoDB
*/

/* ========== CONFIG FROM secrets.h ========== */
const char* ssid        = WIFI_SSID;
const char* password    = WIFI_PASSWORD;

const char* mqtt_server = MQTT_ENDPOINT;
const int   mqtt_port   = MQTT_PORT;
const char* mqtt_topic  = MQTT_TOPIC;

/* ========== BUTTON PINS ========== */
const int entryButtonPin = 18;
const int exitButtonPin  = 19;

/* ========== GLOBAL OBJECTS ========== */
WiFiClientSecure espClient;
PubSubClient client(espClient);

/* ========== WIFI SETUP ========== */
void setup_wifi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println("IP: " + WiFi.localIP().toString());
}

/* ========== MQTT RECONNECT ========== */
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to AWS IoT...");

    if (client.connect("ParkingSensor01")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

/* ========== PUBLISH EVENT ========== */
void publishEvent(const char* eventType) {
  StaticJsonDocument<200> doc;
  doc["event_type"] = eventType;
  doc["spot_id"]    = "SPOT-01";

  char buffer[256];
  serializeJson(doc, buffer);

  Serial.println("Publishing: " + String(buffer));

  if (client.publish(mqtt_topic, buffer)) {
    Serial.println("Event published!");
  } else {
    Serial.println("Publish failed!");
  }
}

/* ========== SETUP ========== */
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(entryButtonPin, INPUT_PULLUP);
  pinMode(exitButtonPin, INPUT_PULLUP);

  setup_wifi();

  // TLS Config (from secrets.h)
  espClient.setCACert(AWS_ROOT_CA);
  espClient.setCertificate(DEVICE_CERT);
  espClient.setPrivateKey(PRIVATE_KEY);

  client.setServer(mqtt_server, mqtt_port);

  Serial.println("Setup complete!");
  Serial.println("Press ENTRY (GPIO18) or EXIT (GPIO19)");
}

/* ========== LOOP ========== */
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  static bool entryPressed = false;
  static bool exitPressed  = false;

  // ENTRY BUTTON
  if (digitalRead(entryButtonPin) == LOW && !entryPressed) {
    Serial.println("\n=== ENTRY EVENT ===");
    publishEvent("ENTRY");
    entryPressed = true;
  }
  if (digitalRead(entryButtonPin) == HIGH) {
    entryPressed = false;
  }

  // EXIT BUTTON
  if (digitalRead(exitButtonPin) == LOW && !exitPressed) {
    Serial.println("\n=== EXIT EVENT ===");
    publishEvent("EXIT");
    exitPressed = true;
  }
  if (digitalRead(exitButtonPin) == HIGH) {
    exitPressed = false;
  }

  delay(200); // debounce
}
