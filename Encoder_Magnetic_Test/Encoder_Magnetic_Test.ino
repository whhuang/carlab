#include <Encoder.h>
//#include <Wire.h>
//#include <LIS3MDL.h> magnetometer
#include <LSM6.h>
Encoder leftEnc(17, 16);
Encoder rightEnc(19, 18);

// ~2514 ticks = 1 revolution, may need adjustment

void setup() {
  Serial.begin(9600);
}

long oldPositionLeft  = -999;
long oldPositionRight = -999;

void loop() {

  long newPositionLeft = leftEnc.read() % 2514;
  if (newPositionLeft != oldPositionLeft) {
    oldPositionLeft = newPositionLeft;
    Serial.print("Left: ");
    Serial.println(newPositionLeft);
  }
  
  long newPositionRight = rightEnc.read() % 2514;
  if (newPositionRight != oldPositionRight) {
    oldPositionRight = newPositionRight;
    Serial.print("Right: ");
    Serial.println(newPositionRight);
  }
  
}
