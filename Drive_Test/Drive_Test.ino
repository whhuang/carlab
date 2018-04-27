#include <Encoder.h>
#include <math.h>
#include <stdlib.h>

/***********************************************
 *                   CONSTANTS                 *
 ***********************************************/

// PIN SETUP

// XBee
const int xBee_Out = 0;
const int xBee_In  = 1;

// LEDs
const int LED1_R = 2;
const int LED1_G = 3;
const int LED1_B = 4;
const int LED2_R = 5;
const int LED2_G = 6;
const int LED2_B = 7;
const int ledPin = 13;

// Encoders
Encoder leftEnc(16, 17);
Encoder rightEnc(18, 19);

// Motors
const int m_L1 = 21;
const int m_L2 = 20;
const int m_R1 = 22;
const int m_R2 = 23;

// OTHER CONSTANTS

const long CIRC     = 2514.0;   // Encoder ticks/wheel revolution
const long MMPREV   = 125.664;  // mm / revolution (40 mm diameter)
const int TOP_SPEED = 255;      // max motor PWM
const long FULL_ROT  = 6000;      // Encoder ticks/full rotation 

const double ePos = 0.01; // Error threshold for position
const double eAng = 0.1;  // Error threshold for angle

/***********************************************
 *                GLOBAL VARIABLES             *
 ***********************************************/

void setup() {
  
  Serial.begin(9600);
  // [XBee Initialization here]

  // PIN INITIALIZATION
  
  pinMode(m_L1, OUTPUT);
  pinMode(m_L2, OUTPUT);
  pinMode(m_R1, OUTPUT);
  pinMode(m_R2, OUTPUT);

  pinMode(LED1_R, OUTPUT);
  pinMode(LED1_G, OUTPUT);
  pinMode(LED1_B, OUTPUT);
  pinMode(LED2_R, OUTPUT);
  pinMode(LED2_G, OUTPUT);
  pinMode(LED2_B, OUTPUT);
  pinMode(ledPin, OUTPUT);
  
}

void loop() {

  turnRight(90, 50);
  stopMotors(1000);
  turnLeft(90,50);
  stopMotors(1000);

  /*
  for (int i = 0; i < 4; i++) {
    driveForward(2.0, 200);
    turnRight(90, 50);
  }

  stopMotors(1000);

  for (int i = 0; i < 4; i++) {
    turnLeft(90, 50);
    driveBackward(2.0, 200);
  }

  stopMotors(1000);*/
  
}

void driveForward(long revs, int power) {

  long leftEncReading;
  long rightEncReading;
  
  leftEnc.write(0);
  rightEnc.write(0);

  while((abs(leftEncReading) < revs * CIRC) ||
        (abs(rightEncReading) < revs * CIRC)) {
    analogWrite(m_L1, power);
    analogWrite(m_L2, 0);
    analogWrite(m_R1, power);
    analogWrite(m_R2, 0);
    leftEncReading = leftEnc.read();
    rightEncReading = rightEnc.read();
    Serial.print("Left: ");
    Serial.println(leftEncReading);
    Serial.print("Right: ");
    Serial.println(rightEncReading);
  }
  while (abs(leftEncReading) < revs * CIRC) {
    analogWrite(m_L1, power);
    analogWrite(m_L2, 0);
    analogWrite(m_R1, 0);
    analogWrite(m_R2, 0);
    leftEncReading = leftEnc.read();
    Serial.print("Left: ");
    Serial.println(leftEncReading);
  }
  while (abs(rightEncReading) < revs * CIRC) {
    analogWrite(m_L1, 0);
    analogWrite(m_L2, 0);
    analogWrite(m_R1, power);
    analogWrite(m_R2, 0);
    rightEncReading = rightEnc.read();
    Serial.print("Right: ");
    Serial.println(rightEncReading);
  }

  analogWrite(m_L1, 0);
  analogWrite(m_L2, 0);
  analogWrite(m_R1, 0);
  analogWrite(m_R2, 0);
  
}

void driveBackward(double revs, int power) {
  
  long leftEncReading;
  long rightEncReading;
  
  leftEnc.write(0);
  rightEnc.write(0);

  while((abs(leftEncReading) < revs * CIRC) ||
        (abs(rightEncReading) < revs * CIRC)) {
    analogWrite(m_L1, 0);
    analogWrite(m_L2, power);
    analogWrite(m_R1, 0);
    analogWrite(m_R2, power);
    leftEncReading = leftEnc.read();
    rightEncReading = rightEnc.read();
    Serial.print("Left: ");
    Serial.println(leftEncReading);
    Serial.print("Right: ");
    Serial.println(rightEncReading);
  }
  while (abs(leftEncReading) < revs * CIRC) {
    analogWrite(m_L1, 0);
    analogWrite(m_L2, power);
    analogWrite(m_R1, 0);
    analogWrite(m_R2, 0);
    leftEncReading = leftEnc.read();
    Serial.print("Left: ");
    Serial.println(leftEncReading);
  }
  while (abs(rightEncReading) < revs * CIRC) {
    analogWrite(m_L1, 0);
    analogWrite(m_L2, 0);
    analogWrite(m_R1, 0);
    analogWrite(m_R2, power);
    rightEncReading = rightEnc.read();
    Serial.print("Right: ");
    Serial.println(rightEncReading);
  }

  analogWrite(m_L1, 0);
  analogWrite(m_L2, 0);
  analogWrite(m_R1, 0);
  analogWrite(m_R2, 0);
  
}

void turnRight(double degs, int power) {

  long leftEncReading;
  long rightEncReading;
  
  leftEnc.write(0);
  rightEnc.write(0);

  while((abs(leftEncReading) < degs / 360 * FULL_ROT) ||
        (abs(rightEncReading) < degs / 360 * FULL_ROT)) {
    analogWrite(m_L1, 0);
    analogWrite(m_L2, power);
    analogWrite(m_R1, power);
    analogWrite(m_R2, 0);
    leftEncReading = leftEnc.read();
    rightEncReading = rightEnc.read();
    Serial.print("Left: ");
    Serial.println(leftEncReading);
    Serial.print("Right: ");
    Serial.println(rightEncReading);
  }

  analogWrite(m_L1, 0);
  analogWrite(m_L2, 0);
  analogWrite(m_R1, 0);
  analogWrite(m_R2, 0);
  
}

void turnLeft(double degs, int power) {

  long leftEncReading;
  long rightEncReading;
  
  leftEnc.write(0);
  rightEnc.write(0);

  while((abs(leftEncReading) < degs / 360 * FULL_ROT) ||
        (abs(rightEncReading) < degs / 360 * FULL_ROT)) {
    analogWrite(m_L1, power);
    analogWrite(m_L2, 0);
    analogWrite(m_R1, 0);
    analogWrite(m_R2, power);
    leftEncReading = leftEnc.read();
    rightEncReading = rightEnc.read();
    Serial.print("Left: ");
    Serial.println(leftEncReading);
    Serial.print("Right: ");
    Serial.println(rightEncReading);
  }

  analogWrite(m_L1, 0);
  analogWrite(m_L2, 0);
  analogWrite(m_R1, 0);
  analogWrite(m_R2, 0);
  
}

void stopMotors(int time) {
  analogWrite(m_L1, 0);
  analogWrite(m_L2, 0);
  analogWrite(m_R1, 0);
  analogWrite(m_R2, 0);
  delay(time);
}

