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
const int m_L1 = 20;
const int m_L2 = 21;
const int m_R1 = 22;
const int m_R2 = 23;

// OTHER CONSTANTS

const int circ = 210.59;     // Encoder ticks/revolution
const int mmprev = 125.664;  // mm / revolution (40 mm diameter)
const int TOP_SPEED = 255;   // max motor PWM

const double ePos = 0.01; // Error threshold for position
const double eAng = 0.1;  // Error threshold for angle

/***********************************************
 *                GLOBAL VARIABLES             *
 ***********************************************/

long leftEncPos;
long rightEncPos;

double currentPosX;
double currentPosY;
double currentAngle;

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

  // GLOBAL VARIABLE INITIALIZATION
  
  leftEncPos  = 0;
  rightEncPos = 0;

  // checks if camera is active
  currentPosX  = getCurrentPosX();  // camera function
  currentPosY  = getCurrentPosY();  // camera function
  currentAngle = getCurrentAngle(); // camera function
  
}

void loop() {

  double desiredPosX  = getDesiredPosX();  // camera function
  double desiredPosY  = getDesiredPosY();  // camera function
  double desiredAngle = getDesiredAngle(); // camera function

  currentPosX  = getCurrentPosX();  // camera function
  currentPosY  = getCurrentPosY();  // camera function
  currentAngle = getCurrentAngle(); // camera function

  if ((abs(desiredPosX  - currentPosX)  > ePos) ||
      (abs(desiredPosY  - currentPosY)  > ePos)) {
    // move to desired position
    
  }
  else if (abs(desiredAngle - currentAngle) > eAng) {
    // turn to desired angle, only once reached correct position
    
  }
  
}

/************************************************
 *               HELPER FUNCTIONS               *
 ************************************************/


