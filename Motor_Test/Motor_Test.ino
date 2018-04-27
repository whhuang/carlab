#include <Encoder.h>
#include <math.h>
#include <stdlib.h>

/***********************************************
 *                  CONSTANTS                  *
 ***********************************************/

// PIN SETUP

// Encoders
Encoder leftEnc(16, 17);
Encoder rightEnc(18, 19);

// Motors
const int m_L1 = 21;
const int m_L2 = 20;
const int m_R1 = 22;
const int m_R2 = 23;

// OTHER CONSTANTS

const int  circ = 210.59; // Encoder ticks/revolution
const int mmprev = 125.664; // mm / revolution (40 mm diameter)
const int TOP_SPEED = 255; // max motor PWM

const int scalePos = 10; //
const double ePos = 0.01; // Error threshold for position
const double eAng = 0.1;  // Error threshold for angle

/***********************************************
 *               GLOBAL VARIABLES              *
 ***********************************************/

void setup() {
  
  Serial.begin(9600);

  // PIN INITIALIZATION
  
  pinMode(m_L1, OUTPUT);
  pinMode(m_L2, OUTPUT);
  pinMode(m_R1, OUTPUT);
  pinMode(m_R2, OUTPUT);
  
}

void loop() {

  Serial.println("Left Motor Begin");
  
  analogWrite(m_L1, 0);
  analogWrite(m_L2, 0);
  analogWrite(m_R1, 0);
  analogWrite(m_R2, 0);
  delay(2000);

  Serial.println("Forwards");
  analogWrite(m_R1, 0);
  analogWrite(m_R2, 0);
  analogWrite(m_L1, 255);
  analogWrite(m_L2, 0);
  delay(1000);

  Serial.println("Backwards");
  analogWrite(m_R1, 0);
  analogWrite(m_R2, 0);
  analogWrite(m_L1, 0);
  analogWrite(m_L2, 255);
  delay(1000);

  Serial.println("Right Motor Begin");
  analogWrite(m_L1, 0);
  analogWrite(m_L2, 0);
  analogWrite(m_R1, 0);
  analogWrite(m_R2, 0);
  delay(2000);

  Serial.println("Forwards");
  analogWrite(m_L1, 0);
  analogWrite(m_L2, 0);
  analogWrite(m_R1, 255);
  analogWrite(m_R2, 0);
  delay(1000);

  Serial.println("Backwards");
  analogWrite(m_L1, 0);
  analogWrite(m_L2, 0);
  analogWrite(m_R1, 0);
  analogWrite(m_R2, 255);
  delay(1000);

  Serial.println("Both Motors Begin");

  analogWrite(m_L1, 0);
  analogWrite(m_L2, 0);
  analogWrite(m_R1, 0);
  analogWrite(m_R2, 0);
  delay(2000);

  Serial.println("Forwards");
  analogWrite(m_L1, 255);
  analogWrite(m_L2, 0);
  analogWrite(m_R1, 255);
  analogWrite(m_R2, 0);
  delay(1000);

  Serial.println("Backwards");
  analogWrite(m_L1, 0);
  analogWrite(m_L2, 255);
  analogWrite(m_R1, 0);
  analogWrite(m_R2, 255);
  delay(1000);
  
}
