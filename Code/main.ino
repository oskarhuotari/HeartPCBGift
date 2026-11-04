const uint8_t pins[6] = {
  (1 << 0),
  (1 << 1),
  (1 << 2),
  (1 << 3),
  (1 << 4),
  (1 << 5)
};

unsigned long lastPressTime = 0;
unsigned long lastLoopTime = 0;
const unsigned long pressInterval = 200;
const unsigned long sweepInterval = 35;
const unsigned long randInterval = 200;

bool buttonDown = false;

char mode = 0;

char anode = 0;
char cathode = 0;

char changeModeIfPressed() {
  // check whether to register press
  if ((PINB & (1 << 4)) == 0 && buttonDown == false) {
    if (millis() - lastPressTime >= pressInterval) {
      lastPressTime = millis(); 
      buttonDown = true;
      return 1;
    }
  } else if ((PINB & (1 << 4)) != 0){
    buttonDown = false;
  }
  return 0;
}

void updateLeds() {
  if (cathode == anode) {
    anode+=1;
  }
  DDRD = 0x00;
  PORTD = 0x00;
  DDRD = pins[cathode] | pins[anode];
  PORTD = pins[anode];
  anode+=1;
  if (anode>5) {
    cathode+=1;
    anode=0;
  }
  if (cathode>5) {
    cathode = 0;
    anode = 0;
  }
}

void randomLed() {
  int random1, random2;

  do {
    random1 = random(6);
    random2 = random(6);
  } while (random1 == random2);

  DDRD = 0x00;
  PORTD = 0x00;
  DDRD = pins[random1] | pins[random2];
  PORTD = pins[random1];
}

void setup() {
  // Config button as input and then pull-up (pulled down when pressed)
  DDRB &= ~(1 << 4);
  PORTB |= (1 << 4);
}

void loop() {
  mode += changeModeIfPressed();
  if (mode > 3) {mode=0;}

  switch (mode) {

    // ON MODE
    case 0:
      updateLeds();
      break;

    // SWEEP MODE  
    case 1:
      if (millis() - lastLoopTime >= sweepInterval) {
        lastLoopTime = millis();
        updateLeds();
      }
      break;

    // RANDOM LED MODE
    case 2:
      if (millis() - lastLoopTime >= randInterval) {
        lastLoopTime = millis();
        randomLed();
      }
      break;

    //OFF MODE  
    case 3:
      DDRD = 0x00;
      PORTD = 0x00;
      break;
  }
}
