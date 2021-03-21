#include <NewPing.h>
#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOCEN  375
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
#define MAX_SAME_COUNT 10
#define SAME_TOLERANCE 20
// our servo # counter
uint8_t servonum = 0;

void setup() {
  Serial.begin(9600);
  //Serial.println("16 channel Servo test!");

  pwm.begin();
  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}



// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  //Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  //Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000;
  pulse /= pulselength;
  //Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

  unsigned int sameCmCount = 0;
  int saveCm = 50;

void loop() {
   unsigned int uS = sonar.ping();
   unsigned int cm = uS / US_ROUNDTRIP_CM;
   
   float cmFloat = cm;
   float cm100 = cm*100;
   float  saveCm100 = saveCm*100;
   float compareCm = (cm100 - saveCm100) / cmFloat;
   Serial.print(cm100); Serial.print(", ");
   Serial.print(saveCm100); Serial.print(", ");
   Serial.println(compareCm);
   if (abs(compareCm) < SAME_TOLERANCE) {
     sameCmCount++;
   } else {
     sameCmCount = 0;
   }
   if (sameCmCount > MAX_SAME_COUNT) {
    uint16_t pulselen = SERVOCEN;
    pwm.setPWM(servonum, 0, pulselen);
    delay(50);
    return;
   }

  saveCm = cm;
  
   /*
   if (cm > 0 && cm < 20) {
     pwm.setPWM(servonum, 0, SERVOCEN);
     delay(500);
     return;
   }
   */

#define MAX_MOVE_CM 80 
#define MC_FACTOR 2  // should be about 160 / MAX_MOVE_CM

  uint16_t myMin = SERVOMIN;
  uint16_t myMax = SERVOMAX;
  uint16_t myLimit = 50;
  uint16_t myCm = cm;
  
  if (myCm > MAX_MOVE_CM  || cm == 0) {
    delay(50);
    uint16_t pulselen = SERVOCEN;
    pwm.setPWM(servonum, 0, pulselen);
    Serial.println(cm);
    return;
  }
  
  if (cm < myLimit) {
    myMin = myMin + 4*myLimit - 4*myCm;
    myMax = myMax - 4*myLimit + 4*myCm;
  }
 
  Serial.print(cm); Serial.print(", ");
  Serial.print(myCm); Serial.print(", ");
  Serial.print(myMin);  Serial.print(", ");
  Serial.print(myMax);  Serial.print(", ");
  Serial.print(saveCm); Serial.print(", ");
  Serial.print(sameCmCount);
  Serial.println(" ");
 
  for (uint16_t pulselen = myMin; pulselen < myMax; pulselen++) {
    pwm.setPWM(servonum, 0, pulselen);
  }
  
  for (uint16_t pulselen = myMax; pulselen > myMin; pulselen--) {
    pwm.setPWM(servonum, 0, pulselen);
  }

}
