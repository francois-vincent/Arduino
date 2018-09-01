// button state read on ESP8266-01
// button must be connected between GPIO0 and GND, with pull-up on GPIO0
// also led state is reversed

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <Wifi_Credentials.h>

const char* ssid     = JELUGA_SSID;
const char* password = JELUGA_PASSWD;
const char* deviceid = "esp8266_button";

#define LED_BUILTIN 1
#define BUTTON 2

const char* host_string = "Host: raspberrypi.local";
const char* host = host_string+6;
const int port = 5000;

int button_state = 1;
int led_state = 0;

WiFiClient client;

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

static bool buttonChangeState(int pin) {
  int last_state = button_state;
  int state;
  if ((state=digitalRead(pin)) == last_state) {
    return false;
  }
  last_state = state;
  int count = 3;
  while (count) {
    delay(30);
    if ((state=digitalRead(pin)) == last_state) {
      count --;
    }
    else {
      return false;
    }
  }
  button_state = state;
  return true;
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

static bool ConnectRaspi()
{
  return client.connect(host, port);
}

static void ToggleRaspiLED()
{
  client.println("GET /toggle HTTP/1.1");
  client.println(host_string);
  client.println("Connection: close");
  client.println();
}

void setup() {
  pinMode(BUTTON, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  connectWiFi();
}

void loop() {
  if (buttonChangeState(BUTTON) && !button_state) {
    if (ConnectRaspi()) {
      ToggleRaspiLED();
      ToggleLED();
    }
    else {
      int count = 6;
      while (count) {
        ToggleLED();
        delay(150);
        count--;
      }
    }
  }
}
