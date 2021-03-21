/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
int speed = 100;
int time = 0;
boolean up = true;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);    
}

// the loop routine runs over and over again forever:
void loop() {
  
      digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(speed);               // wait for a second
      digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
      delay(speed);               // wait for a second

  if (speed > 1000 || speed < 100) {
    up = ! up;
  } else {
    time = time + 2*speed;
    if (time > 5000) {
      time = 0;
      if (up) {
        speed += 50;
      } else {
        speed -= 50;
      }
    }
  }
  
    
}
