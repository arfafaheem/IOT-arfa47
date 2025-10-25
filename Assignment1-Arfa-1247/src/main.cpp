#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define BTN_MODE   25
#define BTN_RESET  26
#define LED1       16
#define LED2       17
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
