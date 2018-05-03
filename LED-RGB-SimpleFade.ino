/*
  Fading
*/

int ledPin1 = 9;    // LED connected to digital pin 9
int ledPin2 = 10;   // LED connected to digital pin 10
int ledPin3 = 11;    // LED connected to digital pin 11

void setup() {
  // nothing happens in setup
}

void loop() {
  // fade in from min to max in increments of 5 points:
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
    for (int fadeValue2 = 0 ; fadeValue2 <= 255; fadeValue2 += 5) {
      for (int fadeValue3 = 0 ; fadeValue3 <= 255; fadeValue3 += 5) {
        // sets the value (range from 0 to 255):
        analogWrite(ledPin1, fadeValue);
        analogWrite(ledPin2, fadeValue2);
        analogWrite(ledPin3, fadeValue3);
        // wait for 30 milliseconds to see the dimming effect
        delay(30);

      }
    }

  }

  // fade in from min to max in increments of 5 points:
  for (int fadeValue = 255 ; fadeValue <= 0; fadeValue += 5) {
    for (int fadeValue2 = 255 ; fadeValue2 <= 0; fadeValue2 += 5) {
      for (int fadeValue3 = 255 ; fadeValue3 <= 0; fadeValue3 += 5) {
        // sets the value (range from 0 to 255):
        analogWrite(ledPin1, fadeValue);
        analogWrite(ledPin2, fadeValue2);
        analogWrite(ledPin3, fadeValue3);
        // wait for 30 milliseconds to see the dimming effect
        delay(30);

      }
    }
  }
}

