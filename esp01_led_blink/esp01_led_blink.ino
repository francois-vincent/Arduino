// LED blink for ESP-01
// through Timer li
#include <SimpleTimer.h>
#define LED_BUILTIN 1

int pin_state = LOW;
SimpleTimer timer;

static void ToggleMyLED()
{
  if (pin_state == LOW) {
    pin_state = HIGH;
  }
  else {
    pin_state = LOW;
  }
  digitalWrite(LED_BUILTIN, pin_state);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
                                    // The onboard blue LED is connected to GPIO0 (TXD) of the ESP-01 module
  timer.setInterval(500, ToggleMyLED);
}

void loop() {
  timer.run();
}
