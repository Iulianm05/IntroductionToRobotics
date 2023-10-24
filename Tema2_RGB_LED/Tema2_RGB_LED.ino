const int blueSignalPin = 9;
const int greenSignalPin = 10;
const int redSignalPin = 11;
const int analogBlueSignalPin = A0;
const int analogGreenSignalPin = A1;
const int analogRedSignalPin = A2;

const int maxPotSignal = 1023;
const int minPotSignal = 0;
const int maxAnalogSignal = 255;
const int minAnalogSignal = 0;

int redSignalPot = 0;
int greenSignalPot = 0;
int blueSignalPot = 0;
int redSignalLed = 0;
int greenSignalLed = 0;
int blueSignalLed = 0;

void setup() {

  pinMode(redSignalPin,OUTPUT);
  pinMode(greenSignalPin,OUTPUT);
  pinMode(blueSignalPin, OUTPUT);
  pinMode(analogRedSignalPin,INPUT);
  pinMode(analogGreenSignalPin,INPUT);
  pinMode(analogBlueSignalPin,INPUT);

}

void loop() {
  redSignalPot = analogRead(analogRedSignalPin);
  greenSignalPot = analogRead(analogGreenSignalPin) ;
  blueSignalPot = analogRead(analogBlueSignalPin);
  redSignalLed = map(redSignalPot,minPotSignal,maxPotSignal,minAnalogSignal,maxAnalogSignal);
  greenSignalLed = map(greenSignalPot,minPotSignal,maxPotSignal,minAnalogSignal,maxAnalogSignal);
  blueSignalLed = map(blueSignalPot,minPotSignal,maxPotSignal,minAnalogSignal,maxAnalogSignal);
  analogWrite(redSignalPin,redSignalLed);
  analogWrite(greenSignalPin,greenSignalLed);
  analogWrite(blueSignalPin,blueSignalLed);


}
