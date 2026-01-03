// //Task A
// //Name: Arfa Faheem RegNo: 23-NTU-CS-1247
// //Embedded IOT Fall 2025
// //BS AI 5TH
// //Submitted to: Sir Nasir

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h> //display 128x64, 128x32

//Here i configure pins the red button is on 25 which is controlling all modes
#define BTN_MODE   25
#define BTN_RESET  13
#define LED1       16
#define LED2       17
#define LED3       18

//this is for oled configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//this is for PWM, channel 0 for led3 and frequency is 5000,
//resolution is set to 10 bit for smoothness
#define LED3_CH 0
#define PWM_FREQ 5000
#define PWM_RES 10


int mode = 0; // Mode 0: OFF, Mode 1: Alternate Blink, ,Mode 2: Both ON, Mode 3: PWM Fade                  
unsigned long lastModePress = 0;
unsigned long lastResetPress = 0;
const unsigned long debounceDelay = 250;  // debounce time is in ms
//250 ms debounce delay = wait 0.25 seconds after a press before reading the button again.

void showModeOnOLED() { //this function is to show mode on oled display
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);

  if (mode == 0) display.println("Both OFF"); 
  else if (mode == 1) display.println("Alt Blink");  //alternate blink means led 1 on, led 2 off and vice versa
  else if (mode == 2) display.println("Both ON");
  else if (mode == 3) display.println("PWM Fade"); //pwm fade means led3 will fade high and low

  display.display();
}

void setup() {
  Serial.begin(115200);
// here pin mode is set for buttons, leds 
  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BTN_RESET, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  // PWM setup for LED3
  ledcSetup(LED3_CH, PWM_FREQ, PWM_RES);
  ledcAttachPin(LED3, LED3_CH);

  // Initialize OLED
  Wire.begin(21, 22);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED initialization failed");
    while (1); //print error and halt
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Task A - LED Modes"); //This is start of oled 
  display.display();
  delay(1000);
//this will call the function to display current mode on oled 
  showModeOnOLED();
}

void loop() {
  unsigned long currentTime = millis(); //used for debouncing, it takes current time in milliseconds

  //this is for debouncing to prevent multiple button press changes
  if (digitalRead(BTN_MODE) == LOW && (currentTime - lastModePress > debounceDelay)) { //current time in milliseconds by millis() function
    lastModePress = currentTime;
    mode = (mode + 1) % 4;  // it will handle Cycle 0 to 3
    showModeOnOLED();
  }

  //here, i handle debounce with reset button to go to mode 0 where both leds off
  if (digitalRead(BTN_RESET) == LOW && (currentTime - lastResetPress > debounceDelay)) {
    lastResetPress = currentTime; 
    mode = 0;
    showModeOnOLED();
  }

  
  if (mode == 0) {  // Both OFF
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    
  }

  else if (mode == 1) {  // Alternate Blink
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    delay(700);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    delay(700);
  }

  else if (mode == 2) {  // Both ON
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
  }

  else if (mode == 3) {  // PWM Fade on LED3 only
    digitalWrite(LED1, LOW); //led 1 and 2 will be off in this mode
    digitalWrite(LED2, LOW);
    for (int duty = 0; duty <= 1023; duty += 5) { //duty cycle is with 5 small steps for more smoothing
      //fading up/ fade in
      ledcWrite(LED3_CH, duty);
      delay(7);
    }
    for (int duty = 1023; duty >= 0; duty -= 5) {
      //fading down
      ledcWrite(LED3_CH, duty);
      delay(7);
    }
  }
}

// //Task B
// //Name: Arfa Faheem RegNo: 23-NTU-CS-1247
// //Embedded IOT Fall 2025
// //BS AI 5TH
// //Submitted to: Sir Nasir


#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// pins for led1 and buzzer , here i use only one led for toggling behaviour
#define BTN       25
#define LED1      16
#define BUZZER    14

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

bool ledState = LOW;
unsigned long lastButtonTime = 0;        // debounce timer
const unsigned long debounceDelay = 150; // ms
const unsigned long longPressTime = 1500; // ms

// this function is to show event on oled display whether its short one for toggle on long one for buzzer
void showEventOnOLED(const char* msg) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);
  display.println(msg);
  display.display();
}

void setup() {
  Serial.begin(115200);
// button is set as input, led and buzzer as output
  pinMode(BTN, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(LED1, ledState);
  digitalWrite(BUZZER, LOW); // initially buzzer is off

  // PWM buzzer setup
  ledcSetup(0, 2000, 10); // channel 0, 2 kHz frequency, 10-bit resolution
  ledcAttachPin(BUZZER, 0); 

  // Initialize OLED
  Wire.begin(21, 22);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED initialization failed");
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,9);
  display.println("Arfa's Task B");
  display.display();
  delay(1000);
}

void loop() {
  int buttonState = digitalRead(BTN); // it gets button state

  // Check if button pressed and debounce time passed (handled using millis())
  if (buttonState == LOW && (millis() - lastButtonTime > debounceDelay)) {
    lastButtonTime = millis(); 

    unsigned long pressStart = millis();//records time when button was first pressed

    // Wait until button released
    while (digitalRead(BTN) == LOW) { //loop continues till button is pressed
      delay(1);
    }

    unsigned long pressDuration = millis() - pressStart; //it calcukates press duration

    // this is for short press
    if (pressDuration < longPressTime) { 
      ledState = !ledState;  //this is for toggling led, if led on then off on button press and vice versa
      digitalWrite(LED1, ledState); //updates led new state
      showEventOnOLED("Short Press:Toggle");
    }
    //this is for long press
    else {
      showEventOnOLED("Long Press:Buzzer");
      for (int i = 0; i < 3; i++) {  //it will play buzzer 3 times
        ledcWriteTone(0, 2000); //play 2khz tone on pwm channel 0
        delay(200);
        ledcWrite(0, 0); //stops buzzer
        delay(100);
      }
    }
  }
}


