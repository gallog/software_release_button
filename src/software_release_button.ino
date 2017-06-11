#include "Keyboard.h"
#include "Mouse.h"

#define SWITCH_CLICK_PRESS 4 
#define RELEASE_BUTTON 9
#define LED1 15
#define LED2 16

#define MODE_CLICK LOW
#define MODE_PRESS HIGH
#define BUTTON_PRESSED HIGH
#define BLINK_INTERVAL_MS 600
#define WAITING_CYCLE_BEFORE_PRESS 6
int mode;
int button;
boolean readyToPress;
boolean go = false;
long lastBlink;
int ledToggle = LOW;

void setup() {
  pinMode(SWITCH_CLICK_PRESS, INPUT);
  pinMode(RELEASE_BUTTON, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  readyToPress = digitalRead(RELEASE_BUTTON) != BUTTON_PRESSED; //se viene acceso col pulsante premuto prima deve essere liberato
  lastBlink = millis();
  Mouse.begin();
  Keyboard.begin();
}

void loop() {
  button = debounce(RELEASE_BUTTON);
  if (button == BUTTON_PRESSED) {
    if (readyToPress) {
      mode = debounce(SWITCH_CLICK_PRESS);
      int state = LOW;
      for (int i = 0;i <WAITING_CYCLE_BEFORE_PRESS;i++) {
        digitalWrite(LED1,state);
        digitalWrite(LED2,state);
        delay(BLINK_INTERVAL_MS);
        state = !state;
      }      
      digitalWrite(LED1,HIGH);
      digitalWrite(LED2,HIGH);
      readyToPress = false;
      
      if (mode == MODE_CLICK) {
        Mouse.click(MOUSE_LEFT);
      } else if (mode == MODE_PRESS) {
        Keyboard.write(KEY_RETURN);
      }
    }
  } else {
    readyToPress = true;
  }

  if (readyToPress) {
    digitalWrite(LED2,LOW);
    if ((millis() - lastBlink) > BLINK_INTERVAL_MS) {
      lastBlink = millis();
      ledToggle = !ledToggle;
      digitalWrite(LED1,ledToggle);
    }
  }  
}


int debounceDelay = 20;
boolean debounce(int pin)
{
  boolean state;
  boolean previousState;
  previousState = digitalRead(pin);
  for(int counter=0; counter < debounceDelay; counter++) {
    delay(1);
    state = digitalRead(pin);
    if( state != previousState) {
      counter = 0;
      previousState = state; }
  }
  return state;
}

