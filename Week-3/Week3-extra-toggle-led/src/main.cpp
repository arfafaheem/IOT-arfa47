//Week 3 Extra Classwork
//Toggle led, alternate led patterns
//Embedded IoT System Fall-2025
//Name: Arfa Faheem     Reg#: 1247
#include<arduino.h>

const int buttonPin = 32;  
const int ledPin = 4;      
const int led2Pin = 5;     

int buttonState = HIGH;     
int lastButtonState = HIGH; 
int ledState = 0;           

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);  
}

void loop() {
  buttonState = digitalRead(buttonPin);

  
  if (buttonState == LOW && lastButtonState == HIGH) {
    // Toggle LED pattern
    ledState = !ledState;  
    delay(50);  
  }
  lastButtonState = buttonState;

  
  if (ledState == 0) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(led2Pin, LOW);
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(led2Pin, HIGH);
  }
}  