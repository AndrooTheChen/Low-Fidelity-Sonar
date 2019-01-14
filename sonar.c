#include <Servo.h>

/* Pins on Arduino board */
#define PIN_7           7
#define PIN_8           8
#define PIN_9           9
#define PIN_10          10
#define PIN_11          11
#define PIN_12          12

/* Important values for movement */
#define ANG_START       10
#define ANG_END         170
#define MIN_DIST        1000
#define ALARM_DIST      40
#define DEADSPOT_DIST   20
#define ANG_INC         10

Servo scan_servo;
Servo ptr_servo;

/*
 * setup()
 * DESCRIPTION: Sets up Arduino and assigns inputs and outputs to pins
 * RETURN VALUE: none
 * OUTPUT: none
 */
void setup()
{
   Serial.begin(9600);
   scan_servo.attach(PIN_11);
   ptr_servo.attach(PIN_12);

   /* Initialize input and output pins for scanner 1 */
   pinMode(PIN_7, OUTPUT);
   pinMode(PIN_8, INPUT);

   /* Initialize input and output pins for scanner 2 */
   pinMode(PIN_9, OUTPUT);
   pinMode(PIN_10, INPUT);
}


/*
 * loop()
 * DESCRIPTION: Main loop for program
 * RETURN VALUE: none
 * OUTPUT: none
 */
void loop()
{
    int scanDist1, scanDist2, ctr, indang, ang, minDist;
    scanDist1 = scanDist2 = ctr = indang = 0;
    minDist = MIN_DIST;

    /* This loop rotates the servo 180 degrees */
    for (ang = ANG_START; ang <= ANG_END; ang += ANG_INC) {
      /* Initialize the servo to current angle ang */
      scan_servo.write(ang);
      delay(100);

      /* Save distance of object currently in front of sensor 1 */
      scanDist1 = scan(PIN_7, PIN_8);

      /* Check if scanned distance is the closest (lowest value) and is */
      /* within scanning range                                          */
      if (minDist > scanDist1 && scanDist1 > DEADSPOT_DIST){
        minDist = scanDist1;
        indang = ang;
      }

      /* Save distance of object currently in front of sensor 2 */
      scanDist2 = scan(PIN_9, PIN_10);

      /* Check if scanned distance is the closest (lowest value) and is */
      /* within scanning range                                          */
      if (minDist > scanDist2 && scanDist2 > DEADSPOT_DIST){
        minDist = scanDist2;
        indang = ang;
      }

  //    Serial.println(scanDist1);
  //    Serial.println(scanDist2);
  //    Serial.println(indang);

      /* Rotate pointer servo to the closest available object with recorded distance */
      ptr_servo.write(indang);

      /* If the closest object is within alarming distance, sound alarm */
      if(minDist < ALARM_DIST){
        tone(6,440,1000);
      }
    }

    /* Reset scanning servo position*/
    scan_servo.write(0);
    delay(300);
   }


 /*
  * scan()
  * DESCRIPTION: This function tells the scanner at assigned pinIn and pinOut
  *   to send and recieve a signal in order to find the distance between the
  *   sensor itself and the object in front of it.
  * RETURN VALUE: int
  * INPUT: int pinIn  - input pin for device
  *        int pinOut - ouptut in for device
  * OUTPUT: distance of closest object
  */
int scan(int pinIn, int pinOut) {
  long time, distance;

  digitalWrite(pinIn,LOW);
  delayMicroseconds(2);
  digitalWrite(pinIn,HIGH);
  delayMicroseconds(10);
  digitalWrite(pinIn,LOW);

  time = pulseIn(pinOut,HIGH);
  distance = time*.017;
  delay(400);

  return distance;
}
