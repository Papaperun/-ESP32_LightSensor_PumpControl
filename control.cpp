#include <WiFi.h>

const char* ssid = "";
const char* password = "";

WiFiServer server(80);

const int wifiLed = 5;      // LED controlled by Wi-Fi
const int pumpLed = 23;      // LED that represents the pump
const int sensorPin = 19;   // DO pin from light sensor

String header;
bool wifiLedState = false;

void setup() {
  Serial.begin(115200);
  pinMode(wifiLed, OUTPUT);
  pinMode(pumpLed, OUTPUT);
  pinMode(sensorPin, INPUT);

  digitalWrite(wifiLed, LOW);
  digitalWrite(pumpLed, LOW);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  // --- Sensor logic ---
  int sensorValue = digitalRead(sensorPin);

  // Print sensor state for debugging
  Serial.print("Sensor value: ");
  Serial.println(sensorValue);

  // --- Wi-Fi client handling ---
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;

        if (c == '\n') {
          if (currentLine.length() == 0) {
            // Web commands
            if (header.indexOf("GET /on") >= 0) {
              wifiLedState = true;
              digitalWrite(wifiLed, HIGH);
            } else if (header.indexOf("GET /off") >= 0) {
              wifiLedState = false;
              digitalWrite(wifiLed, LOW);
            }

            // --- Webpage ---
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.println("<!DOCTYPE html><html>");
            client.println("<h1>ESP32 LED & Pump Control</h1>");
            client.println("<p><a href=\"/on\"><button>Wi-Fi LED ON</button></a></p>");
            client.println("<p><a href=\"/off\"><button>Wi-Fi LED OFF</button></a></p>");

            // Display live sensor and pump state
            client.println("<p>Light Sensor: " + String(sensorValue == LOW ? "Dark" : "Bright") + "</p>");
            client.println("<p>Pump LED: " + String(digitalRead(pumpLed) ? "ON" : "OFF") + "</p>");
            client.println("</html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
  }

  // --- Pump LED logic ---
  // Only runs when Wi-Fi LED is OFF and light sensor says it's dark
  if (!wifiLedState && sensorValue == LOW) {
    digitalWrite(pumpLed, HIGH);
    Serial.println("Wi-Fi LED OFF + Dark detected → Pump LED ON");
  } else {
    digitalWrite(pumpLed, LOW);
  }

  delay(200);
}
