//Declare constants for the input and output pins
const int blueSignalPin = 9;
const int greenSignalPin = 10;
const int redSignalPin = 11;
const int analogBlueSignalPin = A0;
const int analogGreenSignalPin = A1;
const int analogRedSignalPin = A2;

//Constants for the map parameters
const int maxPotSignal = 1023;
const int minPotSignal = 0;
const int maxAnalogSignal = 255;
const int minAnalogSignal = 0;

//Variables for signal of potentiometers(0-1023) and signal of led (0-254)
int redSignalPot = 0;
int greenSignalPot = 0;
int blueSignalPot = 0;
int redSignalLed = 0;
int greenSignalLed = 0;
int blueSignalLed = 0;

void setup() {
  // Define the output and input pins
  pinMode(redSignalPin,OUTPUT);
  pinMode(greenSignalPin,OUTPUT);
  pinMode(blueSignalPin, OUTPUT);
  pinMode(analogRedSignalPin,INPUT);
  pinMode(analogGreenSignalPin,INPUT);
  pinMode(analogBlueSignalPin,INPUT);

}

void loop() {

  //Read the signal transmitted from the potentiometer to the input pins.
  redSignalPot = analogRead(analogRedSignalPin);
  greenSignalPot = analogRead(analogGreenSignalPin) ;
  blueSignalPot = analogRead(analogBlueSignalPin);

  //Map the signal
  redSignalLed = map(redSignalPot,minPotSignal,maxPotSignal,minAnalogSignal,maxAnalogSignal);
  greenSignalLed = map(greenSignalPot,minPotSignal,maxPotSignal,minAnalogSignal,maxAnalogSignal);
  blueSignalLed = map(blueSignalPot,minPotSignal,maxPotSignal,minAnalogSignal,maxAnalogSignal);
  
  //Write the signal to the corresponding LED pin
  analogWrite(redSignalPin,redSignalLed);
  analogWrite(greenSignalPin,greenSignalLed);
  analogWrite(blueSignalPin,blueSignalLed);
}
