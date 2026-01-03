#include<arduino.h>
const int buttonpin=32;
const int ledpin=4;
int buttonstate=0;
void setup(){
  pinMode(ledpin, OUTPUT);
  pinMode(buttonpin, INPUT_PULLUP);
}
void loop(){
  buttonstate=digitalRead(buttonpin);
  if(buttonstate==HIGH){
    digitalWrite(ledpin, HIGH);
  }
  else{
    digitalWrite(ledpin, LOW);
  }
}
