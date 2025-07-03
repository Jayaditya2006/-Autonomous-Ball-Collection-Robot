#include <AFMotor.h>       // Include Adafruit Motor Shield library for motor control
#include <NewPing.h>       // Include NewPing library for ultrasonic sensor
#include <Servo.h>         // Include Servo library for controlling servo motors

// Define pins for ultrasonic sensor
#define TRIGGER_PIN A1        // Ultrasonic sensor trigger pin connected to A1
#define ECHO_PIN A0           // Ultrasonic sensor echo pin connected to A0
#define MAX_DISTANCE 200      // Maximum measurable distance in cm

// Initialize ultrasonic sensor using NewPing library
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Initialize servo object for controlling arm
Servo myservo; 

// Variables to hold sensor readings
unsigned int DISTANCE = 0;    // Stores distance measured by ultrasonic sensor
unsigned int IR_VALUE = 0;    // Placeholder for IR sensor reading (not used in this code)

// Initialize DC motors using Adafruit Motor Shield
AF_DCMotor motorLeft(1);   // Motor connected to M1 (left motor)
AF_DCMotor motorRight(2);  // Motor connected to M2 (right motor)

/*
  MOTOR CONFIGURATION:
  - The robot uses 4 DC motors.
  - Motors on the same side (front and back) are connected together.
  - Both left-side motors rotate together, and both right-side motors rotate together.
  - This enables synchronized movement for each side, useful for forward, backward, and turning motions.
*/

void setup() {
  Serial.begin(9600);             // Start serial communication for debugging

  motorLeft.setSpeed(200);       // Set speed of left motor (range: 0–255)
  motorRight.setSpeed(200);      // Set speed of right motor (range: 0–255)

  myservo.attach(A2);            // Attach servo to analog pin A2
  myservo.write(0);              // Initialize servo position
}



void loop() {
  // Measure distance using ultrasonic sensor
  DISTANCE = sonar.ping_cm();    

  // Print distance to serial monitor
  Serial.print("distance");                   
  Serial.println(DISTANCE);                   

  // If object is between 10 and 40 cm, move forward briefly
  if(DISTANCE > 10 && DISTANCE < 40) {
    moveForward();
    delay(500);
    stopMotors();
  }
  // If object is very close (<10 cm but not 0), stop and collect ball
  else if(DISTANCE < 10 && DISTANCE != 0) {
    stopMotors();
    //Collectball();   // Uncomment this line to activate ball collection
  }
  // If no object is detected, start searching for ball
  else {
    findBall();
  }
}



void moveForward() {
  // Move both motors forward
  motorLeft.run(FORWARD);
  motorRight.run(FORWARD);
}



void moveBackward() {
  // Move both motors backward
  motorLeft.run(BACKWARD);
  motorRight.run(BACKWARD);
}



void turnLeft() {
  // Turn the robot left by rotating left motor backward and right motor forward
  motorLeft.run(BACKWARD);
  motorRight.run(FORWARD);
}



void turnRight() {
  // Turn the robot right by rotating left motor forward and right motor backward
  motorLeft.run(FORWARD);
  motorRight.run(BACKWARD);
}



void stopMotors() {
  // Stop both motors
  motorLeft.run(RELEASE);
  motorRight.run(RELEASE);
}



void findBall() {
  // Rotate in place to search for the ball
  turnRight();
  delay(2000);
  stopMotors();
}



void collectBall() {
  // Simple servo movement sequence for ball collection
  // The commented version below provides smoother motion

  // for (int angle = 0; angle <= 180; angle++) {
  //   armServo.write(angle); 
  //   delay(10); 
  // }
  // delay(1000); 
  // for (int angle = 180; angle >= 0; angle--) {
  //   armServo.write(angle); 
  //   delay(10); 
  // }
  // delay(1000); 

  myservo.write(0);
  delay(500);

  myservo.write(90);
  delay(700);   // Extra time to push the ball fully

  myservo.write(180);
  delay(500);

  myservo.write(90);
  delay(700);

  myservo.write(0);
  delay(500);
}
