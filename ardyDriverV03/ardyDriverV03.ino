/* Encoder Library - TwoKnobs Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */

#include <Encoder.h>

// Change these pin numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder encoder1(2, 3);
//   avoid using pins with LEDs attached

void setup() {
  Serial.begin(9600);
  Serial.println("Encoder interrupt");
}

long encoderPos  = -999;

void loop() {
  long newEncoderPos, newRight;
  newEncoderPos = encoder1.read();
  if (newEncoderPos != encoderPos) {
    Serial.print("Encdoer Position = ");
    Serial.print(newEncoderPos);
    Serial.println();
    encoderPos = newEncoderPos;
  }
  // if a character is sent from the serial monitor,
  // reset both back to zero.
  if (Serial.available()) {
    Serial.read();
    Serial.println("Reset encoder zero");
    encoder1.write(0);
  }
}

