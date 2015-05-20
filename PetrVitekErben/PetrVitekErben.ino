const int pwmPin1 = 3;
const int pwmPin2 = 5;
const int pwmPin3 = 6;
const int pwmPin4 = 9;
const int pwmPin5 = 10;
const int pwmPin6 = 11;
const int pwmPin7 = 13;
const int gatePin = 12;

int outPins[] = {
  pwmPin1,
  pwmPin2,
  pwmPin3,
  pwmPin4,
  pwmPin5,
  pwmPin6,
  pwmPin7,
  gatePin
};

unsigned int ramp = 0;
int sine = 0;

unsigned int chaserRamp = 0;
unsigned int chaserRampStep = 1;
unsigned int chaserRampMax = 975;//255 * 5;
unsigned int chaserOverlap = 60;
//1175;  //255 * 5;

unsigned int chaserCh1 = 0;
unsigned int chaserCh2 = 0;
unsigned int chaserCh3 = 0;
unsigned int chaserCh4 = 0;
unsigned int chaserCh5 = 0;

unsigned int chaserChannels[] = {
  chaserCh1,
  chaserCh2,
  chaserCh3,
  chaserCh4,
  chaserCh5
};

unsigned int chaserRemap[] = {
  100,
  170,
  255,
  170,
  100
};

boolean debug = false;

void setup() {
  for (int i = 0; i < 8; i++) {
    pinMode(outPins[i], OUTPUT);
  }
  if (debug) {
    Serial.begin(9600);
    Serial.println("welcome on board\nthis is erben arduino");
  }
}

void loop() {
  //testPins();
  chaser();
  delay(30);
  digitalWrite(12, 1);

}

void chaser() {

  chaserRamp += chaserRampStep;
  if (debug) {
    Serial.print(chaserRamp);
    Serial.print(" |||| ");
  }

  if (chaserRamp > chaserRampMax) {
    chaserRamp = 0;
  }

  for (int i = 0; i < 5; i++) {
    if (i < 4) {
      chaserChannels[i] = triangleInt(limitInt(chaserRamp, (i * 255) - (i * chaserOverlap), ((i + 1) * 255) - (i * chaserOverlap)), 255);
    } else {
      int chaserRampLoop = 0;
      if(chaserRamp<chaserOverlap){
        chaserRampLoop=chaserRamp+chaserRampMax;
      }else{
        chaserRampLoop=chaserRamp;
      }      
      chaserChannels[i] = triangleInt(limitInt(chaserRampLoop, (i * 255) - (i * chaserOverlap), ((i + 1) * 255) - (i * chaserOverlap)), 255);
    }
  }
  for (int i = 0; i < 5; i++) {
    if (debug) {
      Serial.print(chaserChannels[i]);
      Serial.print(" - ");
    }
    analogWrite(outPins[i + 2], map(chaserChannels[i], 0, 255, 0, chaserRemap[i]));
  }
  if (debug) {
    Serial.println("-------");
  }
}

int limitInt(int input, int inputMin, int inputMax) {
  return (max(min(input, inputMax), inputMin)) - inputMin;
}

int triangleInt(unsigned int input, unsigned int range) {
  unsigned int out = 0;
  unsigned int rangeHalf = range / 2;

  if (input < rangeHalf) {
    out = input;
  } else {
    out = (rangeHalf + 1) - (input - rangeHalf);
  }
  return map(constrain(out, 0, rangeHalf), 0, rangeHalf, 0, range);
}



