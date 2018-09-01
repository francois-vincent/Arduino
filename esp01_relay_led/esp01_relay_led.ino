#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <Wifi_Credentials.h>

#define LED_BUILTIN 1
#define RELAY 2

const char* ssid     = JELUGA_SSID;
const char* password = JELUGA_PASSWD;
const char* deviceid = "esp8266_relay";

int relay_state = 0;
int led_state = 0;

ESP8266WebServer server(80);

void handleRoot();           // function prototypes for HTTP handlers
void handleOn();
void handleOff();
void handleToggle();
void handleNotFound();

static void ToggleLED()
{
  digitalWrite(LED_BUILTIN, led_state);
  led_state= !led_state;
}

static void shutdownLED()
{
  led_state= 0;
  digitalWrite(LED_BUILTIN, 1);
}

static int toggleRelay()
{
  digitalWrite(LED_BUILTIN, relay_state);
  relay_state= !relay_state;
  digitalWrite(RELAY, relay_state);
  return relay_state;
}

static void shutdownRelay()
{
  relay_state= 0;
  digitalWrite(LED_BUILTIN, 1);
  digitalWrite(RELAY, 0);
}

static void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    ToggleLED();
    delay(300);
  }
  shutdownLED();
  MDNS.begin(deviceid);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RELAY, OUTPUT);
  connectWiFi();
  server.on("/", HTTP_GET, handleRoot);
  server.on("/toggle", HTTP_POST, handleToggle);
  server.on("/off", HTTP_POST, handleOff);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {                 // MAIN LOOP
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/plain", deviceid);
}

void handleToggle() {
  server.send(200, "text/plain", toggleRelay()?"on":"off");
}

void handleOff() {
  shutdownRelay();
  server.send(200, "text/plain", "off");
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");
}

