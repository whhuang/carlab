/* Constants */
#define R 2 // the R,G, and B pins
#define G 3
#define B 4
#define ID 1 // Each car has a different ID (1, 2, or 3)
#define N 3 // number of cars

<<<<<<< HEAD
/*Golbal Variables*/
=======
/*Global Variables*/
>>>>>>> untested-code
String field = ""; // one field of an arriving packet
bool new_field = false; // is the next byte part of a new field
int field_num = 0; // how many fields have been received

// Packet Fields
double tx = 0; // translational error (x) in robot's reference frame
double ty = 0; // translational error (y)
double r = 0;  // rotational error in robot's reference frame
double v = 0;  // desired velocity
int cr = 0;    // red color value
int cg = 0;    // green
int cb = 0;    // blue


void setup() {
  Serial.begin(9600); // USB (Debugging)
  Serial1.begin(9600); // XBEE

}

void loop() {

}


// Called when a packet is received
void serialEvent1() {
  
  // Step 1: Parse String by Commas
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
    
    // Step 3: Assign the number to one of the fields
    double num = field.toFloat();
    field = "";
    field_num++;
    
    if ((ID - 1) * 7 < field_num && field_num <= ID * 7) { // ignore data about other cars
      
      switch (field_num % 7) { // [1,7]
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

  if (field_num >= 21) {
    field_num = 0;
    Serial.println();  
  }
}

// Sets the LED colors and brightness
void rgb(int r, int g, int b) {
  analogWrite(R, r);
  analogWrite(G, g);
  analogWrite(B, b);
}
