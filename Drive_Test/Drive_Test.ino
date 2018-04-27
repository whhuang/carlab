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
const int  MAX_PWR  = 255;      // max motor PWM
const long FULL_ROT = 5800;     // Encoder ticks/full rotation; 
                                // 6V, PWM 50 

const double ePos = 0.01; // Error threshold for position
const double eAng = 0.1;  // Error threshold for angle

/***********************************************
 *                GLOBAL VARIABLES             *
 ***********************************************/

/***********************************************
 *                     SETUP                   *
 ***********************************************/

void setup() {
  
  Serial.begin(9600);

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

  /*
  turnRight(90, 50);
  stopMotors(1000);
  turnLeft(90,50);
  stopMotors(1000);
  */

  // Forwards and Backwards Squares
  for (int i = 0; i < 4; i++) {
    driveForward(2.0, 200);
    turnRight(90, 50);
  }

  stopMotors(1000);

  for (int i = 0; i < 4; i++) {
    turnLeft(90, 50);
    driveBackward(2.0, 200);
  }

  stopMotors(1000);
  
}

/***********************************************
 *               HELPER FUNCTIONS              *
 ***********************************************/

void stopMotors(int time) {
  
  analogWrite(m_L1, 0);
  analogWrite(m_L2, 0);
  analogWrite(m_R1, 0);
  analogWrite(m_R2, 0);
  delay(time);
  
}

// Generic function to turn on motors until encoder value is reached
void driveMotors(bool l1, bool l2, bool r1, bool r2,
                 long parameter, int power) {

  int pwr;

  if (power > MAX_PWR) pwr = MAX_PWR;
  else pwr = power;
  
  leftEnc.write(0);
  rightEnc.write(0);

  while((abs(leftEnc.read()) < parameter) ||
        (abs(rightEnc.read()) < parameter)) {
    analogWrite(m_L1, pwr * l1);
    analogWrite(m_L2, pwr * l2);
    analogWrite(m_R1, pwr * r1);
    analogWrite(m_R2, pwr * r2);
  }
  
  while (abs(leftEnc.read()) < parameter) {
    analogWrite(m_L1, pwr * l1);
    analogWrite(m_L2, pwr * l2);
    analogWrite(m_R1, 0);
    analogWrite(m_R2, 0);
  }
  
  while (abs(rightEnc.read()) < parameter) {
    analogWrite(m_L1, 0);
    analogWrite(m_L2, 0);
    analogWrite(m_R1, pwr * r1);
    analogWrite(m_R2, pwr * r2);
  }

  stopMotors(0);
                  
}

void driveForward(long revs, int power) {

  Serial.println("Drive Forward");
  driveMotors(1, 0, 1, 0, revs * CIRC, power);
  
}

void driveBackward(double revs, int power) {

  Serial.println("Drive Backward");
  driveMotors(0, 1, 0, 1, revs * CIRC, power);
  
}

void turnRight(double degs, int power) {

  Serial.println("Turn Right");
  driveMotors(1, 0, 0, 1, degs / 360.0 * FULL_ROT, power);
  
}

void turnLeft(double degs, int power) {

  Serial.println("Turn Left");
  driveMotors(0, 1, 1, 0, degs / 360.0 * FULL_ROT, power);
  
}

