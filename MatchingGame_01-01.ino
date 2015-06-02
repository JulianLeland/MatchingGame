/*
Matching Game
Rev 01.01
Julian Leland

This program controls lights and buttons for a simple texture matching game.
This code was written originally for Barrett Technology.
*/

int switchPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
int LEDPins[] = {A0, A1, A2, A3, A4, A5, 10, 11};
// This array lists location and texture combinations as {location, texture}
int matchArray[8][2] = {{1,1}, {2,2}, {3,3}, {4,1}, {5,4}, {6,3}, {7,4}, {8,2}};
int unsolvedPins[] = {0, 1, 2, 3, 4, 5, 6, 7};
int pin1 = -1; // Set these to -1 to show that they haven't been set yet
int pin2 = -1;
int buttonState;
int lastButtonState = HIGH; // High = no button press
long lastDebounceTime = 0;
long debounceDelay = 50;
boolean gameStart = false;
boolean gameWon = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for(int i = 0; i < sizeof(switchPins)/sizeof(int); i++) {
    pinMode(switchPins[i], INPUT_PULLUP);  // Set all switch pins to be inputs
  }
  for(int i = 0; i < sizeof(LEDPins)/sizeof(int); i++) {
    pinMode(LEDPins[i], OUTPUT);    // Set all LED pins to be outputs
  }
  for(int i = 0; i < sizeof(LEDPins)/sizeof(int); i++) {
    digitalWrite(LEDPins[i], HIGH);  // Turn on all LEDs
  }
  delay(2000);                      // Wait 2 s
  for(int i = 0; i < sizeof(LEDPins)/sizeof(int); i++) {
    digitalWrite(LEDPins[i], LOW);  // Turn off all LEDs
  }
  delay(2000);
  Serial.println("Starting to scan buttons...");
  while(gameStart == false){
    for(int i = 0; i < sizeof(switchPins)/sizeof(int); i++) {
      /* Debug code
      digitalWrite(LEDPins[i], HIGH);
      Serial.print("Scanning button");
      Serial.println(i);
      delay(1000);
      digitalWrite(LEDPins[i], LOW);
      */
      boolean btnPress = HIGH;
      btnPress = digitalRead(switchPins[i]);
      Serial.println(btnPress);
      if(btnPress == LOW){
        gameStart = true;
        break;
      }
    }
  } 
} 

void loop() {
  // put your main code here, to run repeatedly:
  // Check if we've won already
  gameWon = true; // Assume we've won
  for(int i = 0; i < sizeof(unsolvedPins)/sizeof(int); i++) {
    if(unsolvedPins[i] != -1){
      gameWon = false; // If one of the pins is not "null", then we haven't won
      Serial.println("Haven't won yet!");
    }
  }
  if(gameWon == false) {
    Serial.println("In gameWon loop");
    Serial.print("pin1 = ");
    Serial.println(pin1);
    Serial.print("pin2 = ");
    Serial.println(pin2);
    for(int i = 0; i < sizeof(unsolvedPins)/sizeof(int); i++) {
      Serial.println("Checking buttons:");
      if(unsolvedPins[i] != -1){ // Only do these steps for unsolved buttons. Solved ones are left alone.
        Serial.print("Button ");
        Serial.println(i);
        boolean btnPress = HIGH;
        btnPress = digitalRead(switchPins[i]);
        /* Debouncing code - not using
        if(btnPress != lastButtonState) {
          lastDebounceTime = millis();
        }
        if ((millis() - lastDebounceTime) > debounceDelay) {
          if (reading != buttonState) {
            buttonState = reading;
          }
        }
        */
        if(btnPress == LOW) { // If there's a button pressed, store the location of that button
          Serial.println("Button pressed!");
          if(pin1 == -1) {
            Serial.println("Setting pin1");
            pin1 = i; // Set pin1 to the currently pressed button
            digitalWrite(LEDPins[i], HIGH); // Turn on the LED
          }
          else {
            Serial.println("Setting pin2");
            pin2 = i;
            digitalWrite(LEDPins[i], HIGH);
          } 
        }
      }  
    }
    if((pin1 != -1) && (pin2 != -1)) { // Check that two buttons have been pressed
      Serial.println("Checking if multiple presses");
      if(matchArray[pin1][1] == matchArray[pin2][1]) { // Check that texture indicators match
        Serial.println("Texture match!");
        for(int i = 0; i < 5; i++) {
          digitalWrite(LEDPins[pin1], HIGH);
          digitalWrite(LEDPins[pin2], HIGH);
          delay(100);                      // Wait 2 s
          digitalWrite(LEDPins[pin1], LOW );
          digitalWrite(LEDPins[pin2], LOW);
          delay(100);
        }
        digitalWrite(LEDPins[pin1], HIGH);
        digitalWrite(LEDPins[pin2], HIGH); // Just for good measure - the LEDs shouldn't have been turned off.
        unsolvedPins[pin1] = -1;
        unsolvedPins[pin2] = -1;
        pin1 = -1;
        pin2 = -1;
      }
      else { // Boo. You failed to match.
        Serial.println("No match");
        digitalWrite(LEDPins[pin1], LOW);
        digitalWrite(LEDPins[pin2], LOW); // Turn off LEDs.
        pin1 = -1;
        pin2 = -1; // Clear pin memory
      }
    }
  }
  else {
    // Hooray! We've won! Flash lights to celebrate
    for(int i = 0; i < 3; i++) {
      for(int j = 0; j < sizeof(LEDPins)/sizeof(int); j++) {
        digitalWrite(LEDPins[j], HIGH);  // Turn on all LEDs
      }
      delay(500);                      // Wait 2 s
      for(int j = 0; j < sizeof(LEDPins)/sizeof(int); j++) {
        digitalWrite(LEDPins[j], LOW);
      }
      delay(500);
    }
    for(int i = 0; i < sizeof(switchPins)/sizeof(int); i++) {
      digitalWrite(LEDPins[i], HIGH);
      delay(200);
      digitalWrite(LEDPins[i], LOW);
    }
  }

}
