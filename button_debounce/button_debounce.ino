#include "U8g2lib.h"

U8G2_SSD1306_128X64_NONAME_1_4W_SW_SPI u8g2(U8G2_R0, 18, 23, 15, 16, 4);
U8G2LOG u8g2log;
uint8_t u8log_buffer[32*10];

const int  buttonPin = 21;
int buttonState = 0;

bool buttonChangeState(int pin) {
  int count = 3;
  int last_state = buttonState;
  int state;
  while ((state=digitalRead(pin)) == last_state) ;
  last_state = state;
  while (count) {
    delay(30);
    if ((state=digitalRead(pin)) == last_state) {
      count --;
    }
    else {
      return false;
    }
  }
  buttonState = state;
  return true;
}

void setup() {
  pinMode(buttonPin, INPUT);
  u8g2.begin();    
  u8g2.setFont(u8g2_font_tom_thumb_4x6_mf);  // set the font for the terminal window
  u8g2log.begin(u8g2, 32, 10, u8log_buffer);  // connect to u8g2, assign buffer
  u8g2log.print("ESP32 Alive !\n");
}

void loop() {
  if (buttonChangeState(buttonPin)) {
    u8g2log.print("Button ");
    u8g2log.print(buttonState?"press\n":"release\n");
  }
}
