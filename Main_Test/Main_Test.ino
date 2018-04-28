#include <Encoder.h>
#include <math.h>
#include <stdlib.h>

/***********************************************
 *            CONSTANTS/DEFINITIONS            *
 ***********************************************/

/***************** DEFINITIONS *****************/

// **should we change these to enums or const vars?**

#define ID     1 // Each car has a different ID (1, 2, or 3)
#define cars   3 // Number of cars
#define fields 7 // Number of packet fields

/*************** OTHER CONSTANTS ***************/

// Vehicle Attributes
const long CIRC     = 2514.0;   // Encoder ticks/wheel revolution
const long MMPREV   = 125.664;  // mm / revolution (40 mm diameter)
const int  MAX_PWR  = 255;      // max motor PWM
const long FULL_ROT = 5000;     // Encoder ticks/full rotation; 
                                // 6V, PWM 50 

// Video Calibration

// Control Variables
// Error thresholds
const double ePos = 0.01; // Error threshold for position
const double eAng = 0.1;  // Error threshold for angle

// PID
const double pAng = 0.01;

/****************** PIN SETUP ******************/

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

/************** GLOBAL VARIABLES ***************/

// Vehicle Control
double currentPosX;
double currentPosY;
double currentAngle;

// Vehicle Control
double errorX;
double errorY;
double errorTheta;

// Communication
String field;     // one field of an arriving packet
bool   new_field; // is the next byte part of a new field
int    field_num; // how many fields have been received
bool pkt_received; // has a packet been received?

// Packet Fields
double tx; // translational error (x) in robot's reference frame
double ty; // translational error (y) in robot's reference frame
double r;  // rotational error in robot's reference frame
double v;  // desired velocity
int cr;    // red color value
int cg;    // green
int cb;    // blue

/***********************************************
 *                     SETUP                   *
 ***********************************************/

void setup() {
  
  Serial.begin(9600); // USB (Debugging)
  Serial1.begin(9600); // XBEE

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

  // Communication initialization
  field     = "";    // one field of an arriving packet
  new_field = false; // is the next byte part of a new field
  field_num = 0;     // how many fields have been received
  pkt_received = false; // no packet has been received

  // Packet fields
  tx = 0.0;  // translational error (x) in robot's reference frame
  ty = 0.0;  // translational error (y) in robot's reference frame
  r  = 0.0;  // rotational error in robot's reference frame
  v  = 0.0;  // desired velocity
  cr = 0;    // red color value
  cg = 0;    // green
  cb = 0;    // blue
  
}

/***********************************************
 *                     LOOP                    *
 ***********************************************/

void loop() {

  double dThetaR;
  double dThetaD;
  double dist;

  // wait until a packet is received
  if(pkt_received) { 

    if(!((tx == -1.0) && (ty == -1.0) && (r == -1.0))) {

      // reset pkt_received
      pkt_received = false;

      dThetaD = r * 180 / M_PI;
  
      Serial.print("Angle: ");
      Serial.println(r);
      Serial.println(dThetaD);

      if (r > eAng) {
        turnLeft(dThetaD * pAng, 50);
      }
      else if ((r < 0) && (abs(r) > eAng)) {
        turnRight(-dThetaD * pAng, 50);
      }
      else stopMotors(0);
      
    }
    
  }
  
}

/************************************************
 *               HELPER FUNCTIONS               *
 ************************************************/

/*************** VEHICLE CONTROL ****************/

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
  int pwrf;
  int pwrb;
  if (power > MAX_PWR) pwrb = MAX_PWR;
  else pwrb = power;

  pwrf = pwrb * 0.85;
  
  leftEnc.write(0);
  rightEnc.write(0);

  while(((abs(leftEnc.read()) < parameter) ||
        (abs(rightEnc.read()) < parameter)) && !Serial1.available()) {
    analogWrite(m_L1, pwrf * l1);
    analogWrite(m_L2, pwrb * l2);
    analogWrite(m_R1, pwrf * r1);
    analogWrite(m_R2, pwrb * r2);
    //Serial.println(leftEnc.read());
    //Serial.println(rightEnc.read());
  }
  /*
  while ((abs(leftEnc.read()) < parameter) && !Serial1.available()) {
    analogWrite(m_L1, pwrf * l1);
    analogWrite(m_L2, pwrb * l2);
    analogWrite(m_R1, 0);
    analogWrite(m_R2, 0);
    //Serial.println(leftEnc.read());
  }
  while ((abs(rightEnc.read()) < parameter) && !Serial1.available()) {
    analogWrite(m_L1, 0);
    analogWrite(m_L2, 0);
    analogWrite(m_R1, pwrf * r1);
    analogWrite(m_R2, pwrb * r2);
    //Serial.println(rightEnc.read());
  }
  */
  //stopMotors(0);
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

/**************** COMMUNICATION *****************/

// Automatically called when a packet is received
void serialEvent1() {
  
  // Step 1: Parse string by commas
  while (Serial1.available()) {
    char charIn = (char) Serial1.read();
    if (charIn == ',') {
      new_field = true;
      break;
    }
    field += charIn;
  }
    
  if (new_field) {
    new_field = false;
    
    // Step 2: Assign the number to one of the fields
    double num = field.toFloat();
    field = "";
    field_num++;

    // ignore data about other cars
    if (((ID - 1) * fields < field_num) && (field_num <= ID * fields)) {
      switch (field_num % fields) {
        case 1:
          tx = num; break;
        case 2:  
          ty = num; break;
        case 3:
          r = num; break;
        case 4:  
          v = num; break;
        case 5:
          cr = num; break;
        case 6:
          cg = num; break;
        case 7:
          cb = num; break;
      }
      Serial.print(String(num) + ",");
    }
  }

  if (field_num >= fields * cars) {
    field_num = 0;
    pkt_received = true;
    Serial.println();  
  }
}

/******************** OTHER *********************/

// Sets the LED colors and brightness
void rgb1(int r, int g, int b) {
  analogWrite(LED1_R, r);
  analogWrite(LED1_G, g);
  analogWrite(LED1_B, b);
}

void rgb2(int r, int g, int b) {
  analogWrite(LED2_R, r);
  analogWrite(LED2_G, g);
  analogWrite(LED2_B, b);
}

