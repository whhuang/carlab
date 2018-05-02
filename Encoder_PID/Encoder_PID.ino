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
const long MMPREV   = 125.664;  // mm / revolution (40 mm diameter)
const int  MAX_PWR  = 255;      // max motor PWM

long CIRC     = 2514.0;   // Encoder ticks/wheel revolution
long FULL_ROT = 5000;     // Encoder ticks/full rotation; 
                                // 6V, PWM 50 

// Video Calibration

// Control Variables
// Error thresholds
const double ePos = 0.01; // Error threshold for position
const double eAng = 0.1;  // Error threshold for angle

// PID Parameters for FULL_ROT
const double pFR = 1000;
const double iFR = 300;
const double dFR = 150;

const int L = 1;
const int R = -1;

// PID
const double pAng = 0.25;

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

// FULL_ROT PID Control
double eFR; // error
double deFR;
double ieFR = 0;

double old_eFR = 0; // previous error
double old_r = 0; // old value of theta [degrees]
double t_last_pkt = 0; // time of last packet [ms]

double goal_ticks = 0; // number of ticks until the motors should stop
int goal_direction = 0;

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

  rgb1(0,180,255);
  rgb2(0,180,255);
  
}

/***********************************************
 *                     LOOP                    *
 ***********************************************/

void loop() {

  // Step 1: Read XBee Data
  getPackets();

  /*// Step 2: When a new packet is received, do PID Control
  if(pkt_received) { 

    if(!((tx == -1.0) && (ty == -1.0) && (r == -1.0))) {

      // reset pkt_received
      pkt_received = false;

      // Set goal direction and ticks
      if (r < 0)
        goal_direction = R;
      else
        goal_direction = L;
      goal_ticks = abs(r) / (2*M_PI) * FULL_ROT;

      // Update PID parameters
      double dt = millis() - t_last_pkt;
      //Serial.println(dt);

      double change_in_ticks = abs(r - old_r) * FULL_ROT / (2 * M_PI);
      double desired_change_in_ticks = (leftEnc.read() + rightEnc.read())/2.0; // average of the encoder readings
      eFR = desired_change_in_ticks - change_in_ticks;
      deFR = (eFR - old_eFR)/dt;
      ieFR = ieFR + eFR*dt;

      // Use PID control to find new value of FULL_ROT
      //FULL_ROT = pFR * eFR + iFR * ieFR + dFR * deFR;
      //Serial.println(FULL_ROT);
      
      // Update the variables for next iteration
      t_last_pkt = millis();
      old_r = r;
      old_eFR = eFR;
      leftEnc.write(0); // reset encoder counters
      rightEnc.write(0);
    }
      
    // Step 3: Drive motors

    double ticks = (leftEnc.read() + rightEnc.read()) / 2.0;
    if (abs(goal_ticks - ticks) < 100) {
      stopMotors(0);
    } else if (goal_direction == L) {
        turnLeft(50);
    } else if (goal_direction == R) {
        turnRight(50);
    } 

  }*/
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
void driveMotors(bool l1, bool l2, bool r1, bool r2, int power) {
  int pwrf;
  int pwrb;
  if (power > MAX_PWR) pwrb = MAX_PWR;
  else pwrb = power;

  pwrf = pwrb * 0.85;
  
  leftEnc.write(0);
  rightEnc.write(0);

  analogWrite(m_L1, pwrf * l1);
  analogWrite(m_L2, pwrb * l2);
  analogWrite(m_R1, pwrf * r1);
  analogWrite(m_R2, pwrb * r2);
}

void driveForward(long revs, int power) {
  Serial.println("Drive Forward");
  driveMotors(1, 0, 1, 0, /*revs * CIRC,*/ power);
}

void driveBackward(double revs, int power) {
  Serial.println("Drive Backward");
  driveMotors(0, 1, 0, 1, /*revs * CIRC,*/ power); 
}

void turnRight(int power) {
  Serial.println("Turn Right");
  driveMotors(1, 0, 0, 1, power);
}

void turnLeft(int power) {
  Serial.println("Turn Left");
  driveMotors(0, 1, 1, 0, power);
}

/**************** COMMUNICATION *****************/

// Automatically called when a packet is received
void getPackets() {
  
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

