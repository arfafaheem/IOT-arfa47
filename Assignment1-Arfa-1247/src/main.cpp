// // all the libraries need for graphics, oled, dht sensor and arduino functions
// #include<arduino.h>
// #include<wire.h>
// #include<Adafruit_GFX.h>
// #include<Adafruit_SSD1306.h>

// //defining pins
// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 64
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// #define First_BTN 32
// #define Second_BTN 33
// #define LED1 18
// #define LED2 19
// #define buzzer 11

// //now modes initilization
// int mode=1; //mode=1 both off, mode=2 alternate blink, mode=3 both on, mode=4PWM fade
// unsigned long lastPress=0;  //it remembers when the last button was pressed
// int dutycycle=0; //it shows how bright the light is  it is the duty cycle (PWN cycle)
// int fadedir=1;  //it shows the direction of fading 1-increasing brightness, 0-decreasing brightness

// void showMode(String msg){
//   display.clearDisplay();
//   display.setTextSize(1);
//   display.setTextColor(SSD1306_WHITE);
//   display.setCursor(0,20);
//   display.println("Mode is: " + msg);
//   display.display();
// }
// void setup(){
//   pinMode(First_BTN,INPUT_PULLUP);
//   pinMode(Second_BTN,INPUT_PULLUP);
//   pinMode(LED1, OUTPUT);
//   pinMode(LED2,OUTPUT);
//   pinMode(buzzer, OUTPUT);
//   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
//   display.clearDisplay();
//   showMode("OFF");
// }

// void loop(){

// }

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define BTN_MODE   12
#define BTN_RESET  13
#define LED1       2
#define LED2       4
#define LED3       18
#define BUZZER     14

// PWM setup for ESP32
const int pwmCh = 0;
const int pwmFreq = 5000;
const int pwmRes  = 8;

int mode = 0;
unsigned long lastPress = 0;
int fadeValue = 0;
int fadeDir = 1;

void showMode(const char* msg) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.print("Mode: ");
  display.println(msg);
  display.display();
}

void setup() {
  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BTN_RESET, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  showMode("OFF");

  // ✅ Only works on ESP32
  ledcSetup(pwmCh, pwmFreq, pwmRes);
  ledcAttachPin(LED3, pwmCh);
}

void loop() {
  if (digitalRead(BTN_MODE) == LOW && millis() - lastPress > 300) {
    lastPress = millis();
    mode = (mode + 1) % 4;
    switch (mode) {
      case 0: showMode("OFF"); break;
      case 1: showMode("ALT BLINK"); break;
      case 2: showMode("BOTH ON"); break;
      case 3: showMode("FADE"); break;
    }
  }

  if (digitalRead(BTN_RESET) == LOW) {
    mode = 0;
    showMode("OFF");
  }

  switch (mode) {
    case 0:
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      ledcWrite(pwmCh, 0);
      break;
    case 1:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      delay(300);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      delay(300);
      break;
    case 2:
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      ledcWrite(pwmCh, 255);
      break;
    case 3:
      ledcWrite(pwmCh, fadeValue);
      fadeValue += fadeDir * 5;
      if (fadeValue <= 0 || fadeValue >= 255) fadeDir = -fadeDir;
      delay(20);
      break;
  }
}

