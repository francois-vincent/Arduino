#include "U8g2lib.h"
#include <WiFi.h>

U8X8_SSD1306_128X64_NONAME_4W_SW_SPI u8x8(18, 23, 15, 16, 4);  // SCK=18, MOSI=23, CS=15, DC=16, Res=4

const char* ssid     = "Jeluga";
const char* password = "GA123456789";

void setup() {
  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.drawString(0,0,"Connecting WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  u8x8.drawString(0,0,"WiFi Connected  ");
  u8x8.drawString(0,1,WiFi.localIP().toString().c_str());
}

void loop() {
  // put your main code here, to run repeatedly:

}
