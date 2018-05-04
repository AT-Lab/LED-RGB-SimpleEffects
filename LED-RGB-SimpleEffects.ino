/*
   Titolo: semplice menu per manovrare i led RGB
*/

#define DEBUG_SERIAL

// I/O pinout
#define SWDX  8  // switch destro (incrementa numero)
#define SWSX  12 // switch sinistro (avvio e stop)

// i pin dei led devono essere per forza pin PWM
#define PIN_RED 9
#define PIN_GREEN 10
#define PIN_BLUE 11

#define MIN_STROBO_DELAY 5
#define MAX_STROBO_DELAY 1000
#define NUM_SWITCH_STATE 6 // 6 combinazioni di coppie di colori

// display led 7 seg
#define a     6
#define b     7
#define c     1
#define d     2
#define e     3
#define f     4
#define g     5

typedef enum {RG, RB, GR, GB, BR, BG} color_pair;

class switch_state {
  private:
    color_pair current;
    color_pair next1;
    color_pair next2;

  public:
    switch_state(color_pair _current, color_pair _next1, color_pair _next2) {
      current = _current;
      next1 = _next1;
      next2 = _next2;
    }

    color_pair current_state() {
      return current;
    }

    color_pair random_next() {
      int r = random(2);
      color_pair next;
      return next = (r == 0) ? next1 : next2;
    }
};


switch_state array_switch_state[NUM_SWITCH_STATE] = { {RG, GR, GB}, {RB, BR, BG}, {GR, RG, RB},
  {GB, BR, BG}, {BR, RB, RG}, {BG, GR, GB}
};

// Numero visualizzato sul display
int num = -1;


volatile boolean ISRRan = 1;
unsigned long previousMillis = 0;
int ledState = LOW;

// header funzioni
void myISR();
int switch_color(color_pair dir, int delay_ms = 30);


void setup() {

  Serial.begin(9600);
  attachInterrupt(SWSX, myISR, FALLING);
  randomSeed(analogRead(1)*analogRead(3));

}

void loop() {
  if (Serial.available() > 0) {
    num = Serial.parseInt();
    Serial.println(num, DEC);
  }
  if (ISRRan) {
    switch (num) {
      case 0: strobo(0xff, 0x00, 0x0f, 70); break;
      case 1: switch_color(RG); break;
      case 2: switch_color(RB); break;
      case 3: switch_color(GR); break;
      case 4: switch_color(GB); break;
      case 5: switch_color(BR); break;
      case 6: switch_color(BG); break;
      case 7: keep_switching(); break;
      case 8: break;
      case 9: break;
      case 10: break;
      case 11: break;
      case 12: break;
      case 13: break;
      case 14: break;
    }
  }
  // delay(10);
}

void myISR() {
  ISRRan = !ISRRan;
}

void strobo(byte red, byte green, byte blue, int delay_ms) {
  // TODO: usare un potenziometro per il valore del delay, usando analogRead() e map()
  // delay_pot = map(analogRead(0), 0, 1024, MIN_STROBO_DELAY, MAX_STROBO_DELAY);
  //
  // per fissare il range minimo e massimo
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= delay_ms) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
      analogWrite(PIN_RED, red);
      analogWrite(PIN_GREEN, green);
      analogWrite(PIN_BLUE, blue);
    } else {
      ledState = LOW;
      analogWrite(PIN_RED, 0);
      analogWrite(PIN_GREEN, 0);
      analogWrite(PIN_BLUE, 0);
    }
  }
}

int switch_color(color_pair dir, int delay_ms) {
  switch (dir) {
    case RG:
      for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
        if (ISRRan) {
          // sets the value (range from 0 to 255):
          analogWrite(PIN_GREEN, fadeValue);
          analogWrite(PIN_RED, 255 - fadeValue);
          // wait for 30 milliseconds to see the dimming effect
          delay(delay_ms);
        }
        else {
          return -1;
        }
      }
      break;

    case RB:
      for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
        if (ISRRan) {
          // sets the value (range from 0 to 255):
          analogWrite(PIN_BLUE, fadeValue);
          analogWrite(PIN_RED, 255 - fadeValue);
          // wait for 30 milliseconds to see the dimming effect
          delay(delay_ms);
        }
      }
      break;

    case GR:
      for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
        if (ISRRan) {
          // sets the value (range from 0 to 255):
          analogWrite(PIN_RED, fadeValue);
          analogWrite(PIN_GREEN, 255 - fadeValue);
          // wait for 30 milliseconds to see the dimming effect
          delay(delay_ms);
        }
      }
      break;

    case GB:
      for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
        if (ISRRan) {
          // sets the value (range from 0 to 255):
          analogWrite(PIN_BLUE, fadeValue);
          analogWrite(PIN_GREEN, 255 - fadeValue);
          // wait for 30 milliseconds to see the dimming effect
          delay(delay_ms);
        }
      }
      break;

    case BR:
      for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
        if (ISRRan) {
          // sets the value (range from 0 to 255):
          analogWrite(PIN_RED, fadeValue);
          analogWrite(PIN_BLUE, 255 - fadeValue);
          // wait for 30 milliseconds to see the dimming effect
          delay(delay_ms);
        }
      }
      break;

    case BG:
      for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
        if (ISRRan) {
          // sets the value (range from 0 to 255):
          analogWrite(PIN_GREEN, fadeValue);
          analogWrite(PIN_BLUE, 255 - fadeValue);
          // wait for 30 milliseconds to see the dimming effect
          delay(delay_ms);
        }
      }
      break;

  }
}

switch_state create_next_state(color_pair dir) {
  switch (dir) {
    case RG:
      return switch_state(dir, GR, GB);
      break;

    case RB:
      return switch_state(dir, BR, BG);
      break;

    case GR:
      return switch_state(dir, RG, RB);
      break;

    case GB:
      return switch_state(dir, BR, BG);
      break;

    case BR:
      return switch_state(dir, RG, RB);
      break;

    case BG:
      return switch_state(dir, GR, GB);
      break;
  }
}

void keep_switching() {
  switch_state state = array_switch_state[random(NUM_SWITCH_STATE)];
  int exit_status = 0;
  do {
    exit_status = switch_color(state.current_state());
    state = create_next_state(state.random_next());
  }
  while (exit_status != -1 || !ISRRan);
}


