#include <WiFi.h>
#include <WiFiClient.h>
#define LED 1

int pin_state = LOW;
const char* ssid     = "Jeluga";
const char* password = "GA123456789";
const char* raspi = "192.168.1.24";
const int port = 5000;
WiFiClient client;

static void ToggleMyLED()
{
  if (pin_state == LOW) {
    pin_state = HIGH;
  }
  else {
    pin_state = LOW;
  }
  digitalWrite(LED, pin_state);
}

static bool ConnectRaspi()
{
  bool isConnected = false;
  if (client.connect(raspi, port)) {
    isConnected = true;
  }
  else {
    Serial.println("Connnection failed");
  }
  return isConnected;
}

static void ToggleRaspiLED()
{
  client.println("GET /toggle HTTP/1.1");
  client.println("Host: 192.168.1.24");
  client.println("Connection: close");
  client.println();
}

void setup() {
  // initialize digital pin LED as an output.
  pinMode(LED, OUTPUT);
  // initialize serial port for console
  Serial.begin(57600);
  Serial.println();
  Serial.println("Connecting to Wifi...");
  WiFi.begin((char*)ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to SSID: ");
  Serial.println(ssid);
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Connecting to Raspi...");
}

void loop() {
  while (true) {
    if (ConnectRaspi()) {
      ToggleMyLED();
      ToggleRaspiLED();
    }
    delay(1000);
  }
}
