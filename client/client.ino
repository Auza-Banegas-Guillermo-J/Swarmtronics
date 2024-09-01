#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

const char* ssid = "MegaBurger";
const char* password = "Daly2211";

const char* wsHost = "192.168.1.252";
const int wsPort = 8080;

WebSocketsClient webSocket;

const int LEFT_MOTOR_PIN1 = 4;
const int LEFT_MOTOR_PIN2 = 12;
const int RIGHT_MOTOR_PIN1 = 14;
const int RIGHT_MOTOR_PIN2 = 15;

void setup() {
  Serial.begin(115200);
  pinMode(LEFT_MOTOR_PIN1, OUTPUT);
  pinMode(LEFT_MOTOR_PIN2, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN1, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN2, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to WiFi");
  webSocket.begin(wsHost, wsPort, "/");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
  pinMode(LED_BUILTIN, OUTPUT); // Using built-in LED for ESP8266
}

void loop() {
  webSocket.loop();
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("Disconnected from WebSocket server");
      break;
    case WStype_CONNECTED:
      Serial.println("Connected to WebSocket server");
      break;
    case WStype_TEXT:
      handleMessage((char*)payload);
      break;
  }
}

void handleMessage(const char* message) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  int nodeId = doc["nodeId"];
  if (nodeId != 0) {
    Serial.println("Message not for this node");
    return;
  }

  const char* direction = doc["nodeDirection"];
  Serial.print("Received direction: ");
  Serial.println(direction);

  controlMotors(direction);
}

void controlMotors(const char* direction) {
  switch (direction[0]) {
    case 'w': // Forward
      digitalWrite(LEFT_MOTOR_PIN1, HIGH);
      digitalWrite(LEFT_MOTOR_PIN2, LOW);
      digitalWrite(RIGHT_MOTOR_PIN1, HIGH);
      digitalWrite(RIGHT_MOTOR_PIN2, LOW);
      Serial.println("Going Forward");
      break;
    case 's': // Backward
      digitalWrite(LEFT_MOTOR_PIN1, LOW);
      digitalWrite(LEFT_MOTOR_PIN2, HIGH);
      digitalWrite(RIGHT_MOTOR_PIN1, LOW);
      digitalWrite(RIGHT_MOTOR_PIN2, HIGH);
      Serial.println("Going Backward");
      break;
    case 'a': // Left
      digitalWrite(LEFT_MOTOR_PIN1, LOW);
      digitalWrite(LEFT_MOTOR_PIN2, HIGH);
      digitalWrite(RIGHT_MOTOR_PIN1, HIGH);
      digitalWrite(RIGHT_MOTOR_PIN2, LOW);
      Serial.println("Going Left");
      break;
    case 'd': // Right
      digitalWrite(LEFT_MOTOR_PIN1, HIGH);
      digitalWrite(LEFT_MOTOR_PIN2, LOW);
      digitalWrite(RIGHT_MOTOR_PIN1, LOW);
      digitalWrite(RIGHT_MOTOR_PIN2, HIGH);
      Serial.println("Going Right");
      break;
    default: // Stop
      digitalWrite(LEFT_MOTOR_PIN1, LOW);
      digitalWrite(LEFT_MOTOR_PIN2, LOW);
      digitalWrite(RIGHT_MOTOR_PIN1, LOW);
      digitalWrite(RIGHT_MOTOR_PIN2, LOW);
      Serial.println("Stopping");
      break;
  }
}
