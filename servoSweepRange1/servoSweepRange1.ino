// Range finder stuff
#include <NewPing.h> 
#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

// servo stuff               
#include <Servo.h>
#define SERVO_PIN 9
Servo myservo;  
 
 // constants and variables for servos
#define SERVO_MIN 45
#define SERVO_MAX 135
#define SERVO_CENTER 90
#define PING_INTERVAL 50

int pos = SERVO_MIN;    
int dir = 1;
int pingDelayCount = 999; 
unsigned int myDelay;
void setup() {
  myservo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object
    Serial.begin(9600);
}
 
 
void loop() {
  
  unsigned int cm;
  if (pingDelayCount > PING_INTERVAL) {
    pingDelayCount = 0;
    cm = sonar.ping_cm();    
  }
  
  if (cm == 0 || cm > 90) {
    pos = SERVO_CENTER;
    myservo.write(pos);   
    pingDelayCount += 20;
    delay(20);
    return;
  } 

/*
  unsigned int myDelay = 20;
  Serial.print (cm);
  Serial.print ("\t");
  Serial.print (myDelay);
  Serial.print ("\n");
  delay(myDelay); 
  pingDelayCount += myDelay;
*/

  pos += dir;
  if (pos == SERVO_MAX) {
    dir = -1;
  } else if (pos == SERVO_MIN) {
    dir = 1;
  }
  myservo.write(pos);   
 
 /* 
  if (cm < 90) {
    myDelay = cm;
  }
  */
  
  Serial.print ("\t");
  Serial.print (cm);
  Serial.print ("\t");
  Serial.print (pos);
  Serial.print ("\n");
  
  //delay(myDelay); 
//  pingDelayCount += myDelay;
 // pingDelayCount += 20as;

}
