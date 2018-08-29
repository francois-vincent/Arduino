// for OLED connection, see:
// https://www.brainy-bits.com/connect-and-use-a-spi-oled-display/
// http://www.rhydolabz.com/displays-c-88/096-oled-display-module-spii2c-128x64-6-pin-white-p-2260.html
// oled has 8 lines of 16 chars

#include "U8g2lib.h"
#include "DHT.h"

#define DHTPIN 2     // what digital pin DHTxx is connected to

DHT dht(DHTPIN, DHT11);
const float rapport = 1.1 / 10.23;
const bool use_lm35 = false;  // use LM35 as a secondary temp sensor and compute mean with DHT11

U8X8_SSD1306_128X64_NONAME_4W_SW_SPI u8x8(13, 11, 10, 9, 8);  // D0=13, D1=11, CS=10, DC=9, Reset=8

void draw() {
  float h = dht.readHumidity();
  float t1 = dht.readTemperature();
  float t, t2;
  if (isnan(h) || isnan(t1)) {
    u8x8.drawString(1,1,"Failed to read");
    u8x8.drawString(1,2,"from DHT sensor!");
    return;
  }
  if (use_lm35) {
    t2 = analogRead(A0) * rapport;
    t = (t1+t2)/2;
  }
  else {
    t = t1;
  }
  u8x8.drawString(0,0,"----------------");
  u8x8.drawString(3,3,(String("Temp : ")+String(int(t))+String(" C")).c_str());
  u8x8.drawString(3,4,(String("Hygro: ")+String(int(h))+String(" %")).c_str());
  u8x8.drawString(0,7,"----------------");
}

void setup(void) {
  analogReference(INTERNAL);
  dht.begin();
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
}

void loop(void) {
  draw();
  delay(2000);
}

