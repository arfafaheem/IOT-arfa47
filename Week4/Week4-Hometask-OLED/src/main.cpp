#include<wire.h>
#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void drawtextdemo(){
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("Arfa Faheem");
  display.setTextSize(1);            
  display.setCursor(30, 45);
  display.println("ID: 1247");
  display.display();
}
void drawshape(){
  display.drawRect(5,15,118,40, SSD1306_WHITE);
}

void setup(){
  Wire.begin(21,22);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)){
    for(;;);
  }
  display.clearDisplay();
  drawtextdemo();
  drawshape();
  display.display();
}
