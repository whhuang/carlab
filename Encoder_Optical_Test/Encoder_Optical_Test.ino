#include <Encoder.h>
//#include <Wire.h>
//#include <LIS3MDL.h> magnetometer
#include <LSM6.h>
Encoder leftEnc(17, 16);
Encoder rightEnc(0, 1);

void setup() {
  Serial.begin(9600);
}
/*
long oldPositionLeft  = -999;
long oldPositionRight = -999;*/

long positionLeft = 0;
long positionRight = 0;

void loop() {

  long leftEncRead = leftEnc.read();
  if (leftEncRead != 0) {
    positionLeft += leftEncRead;
    Serial.print("Left: ");
    Serial.println(positionLeft);
  }
  
  long rightEncRead = rightEnc.read();
  if (rightEncRead != 0) {
    positionLeft += leftEncRead;
    Serial.print("Left: ");
    Serial.println(positionLeft);
  }

  /*
  long newPositionLeft = leftEnc.read();
  if (newPositionLeft != oldPositionLeft) {
    oldPositionLeft = newPositionLeft;
    Serial.print("Left: ");
    Serial.println(newPositionLeft);
  }
  long newPositionRight = rightEnc.read();
  if (newPositionRight != oldPositionRight) {
    oldPositionRight = newPositionRight;
    Serial.print("Right: ");
    Serial.println(newPositionRight);
  }*/
  
}
