
#include <EEPROM.h>
// Function prototypes
void printMainMenu();
void handleMainMenuInput();

void printSensorSettingsSubMenu();
void handleSensorSettingsInput();

void printResetLoggerDataSubMenu();
void handleResetLoggerDataInput();

void printSystemStatusSubMenu();
void handleSystemStatusInput();

void printRGBLEDControlSubMenu();
void handleRGBLEDControlInput();

void AutomatLed();
void ultrasonicSensor();
void LDRSensor();
void runningSensors();

//pins
const int trigPin = 9;
const int echoPin = 10;
const int ledRedPin = 3;
const int ledGreenPin = 5;
const int ledBluePin = 6;
const int photocellPin = A0;

// Global variables
const int addressSamplingInterval = 0;
const int addressminValueUltrasonic = 1;
const int addressminValueLDR = 2;
const int addressGreenLight = 3;
const int addressBlueLight = 4;
const int addressRedLight = 5;
const int addressToggle = 6;

long duration = 0;
int distance = 0;
int intensity = 0;

int samplingInterval = 0;
int minValueUltrasonic = 0;
int minValueLDR = 0;
int mainMenuChoice = 0;
int photocellValue = 0;
int redLight = 0;
int greenLight = 0;
int blueLight = 0;
unsigned long startTime = 0;
bool ModAutomate = false;
unsigned long currentTime = 0;
unsigned long lastCurrentTime = 0;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(ledRedPin, OUTPUT);
  pinMode(ledGreenPin, OUTPUT);
  Serial.begin(9600);

  printMainMenu();

}

void loop() {
  if (Serial.available() > 0) {
    if (mainMenuChoice == 0) {
      handleMainMenuInput();
    } else {
      // Handle submenu input based on mainMenuChoice
      switch (mainMenuChoice) {
        case 1:
          handleSensorSettingsInput();
          break;
        case 2:
          handleResetLoggerDataInput();
          break;
        case 3:
          handleSystemStatusInput();
          break;
        case 4:
          handleRGBLEDControlInput();
          break;
        default:
          Serial.println("Invalid choice. Please enter a valid option.");
          printMainMenu();
          break;
      }
    }
  }
  runningSensors();
  

}

void printMainMenu() {
  Serial.println("Main Menu:");
  Serial.println("1. Sensor Settings");
  Serial.println("2. Reset Logger Data");
  Serial.println("3. System Status");
  Serial.println("4. RGB LED Control");
}

void handleMainMenuInput() {
  //Read the menu choice and parse it to int
  mainMenuChoice = Serial.parseInt();
  Serial.read(); // Consume the newline character

  switch (mainMenuChoice) {
    case 1:
      printSensorSettingsSubMenu();
      break;
    case 2:
      printResetLoggerDataSubMenu();
      break;
    case 3:
      printSystemStatusSubMenu();
      break;
    case 4:
      printRGBLEDControlSubMenu();
      break;
    default:
      Serial.println("Invalid choice. Please enter a valid option.");
      mainMenuChoice = 0;
      printMainMenu();
      break;
  }
}

void printSensorSettingsSubMenu() {
  Serial.println("1. Sensor Settings:");
  Serial.println("  1.1 Sensors Sampling Interval.");
  Serial.println("  1.2 Ultrasonic Alert Threshold");
  Serial.println("  1.3 LDR Alert Threshold.");
  Serial.println("  1.4 Back");
}

void handleSensorSettingsInput() {
  int submenuChoice = Serial.parseInt();
  Serial.read(); // Consume the newline character

  switch (submenuChoice) {
    case 1:
      Serial.println("Enter a value for the sampling rate:");

      startTime = millis();
      //I wait for 5 seconds to avoid blocking the program for too long when waiting for a read value
      while (!Serial.available()) {
        if (timerWhile())
          return;
      }

      samplingInterval = Serial.parseInt();

      if (samplingInterval >= 0 && samplingInterval <= 10) {
        EEPROM.update(addressSamplingInterval, samplingInterval);
        Serial.print("The sampling interval value is now: ");
        Serial.println(samplingInterval);
      } else {
        Serial.println("The entered value does not meet the requirements.");
      }
      printMainMenu();
      mainMenuChoice = 0;
      break;

    case 2:
      Serial.println("Enter a minim value for ultrasonic sensor");

      startTime = millis();
      while (!Serial.available()) {
        if (timerWhile())
          return;
      }

      minValueUltrasonic = Serial.parseInt();

      if (minValueUltrasonic >= 0 && minValueUltrasonic <= 15) {
        //Storing value in EEPROM
        EEPROM.update(addressminValueUltrasonic, minValueUltrasonic);
        Serial.print("The minimum value is now: ");
        Serial.println(minValueUltrasonic);
      } else {
        Serial.println("The entered value does not meet the requirements.");
      }
      printMainMenu();
      mainMenuChoice = 0;
      break;

    case 3:
      Serial.println("Enter a minim value for LDR (0 - night, 255 - day)");
      startTime = millis();
      while (!Serial.available()) {
        if (timerWhile())
          return;
      }
      minValueLDR = Serial.parseInt();
      if (minValueLDR >= 0 && minValueLDR <= 255) {
        EEPROM.update(addressminValueLDR, minValueLDR);
        Serial.print("The minimum value is now: ");
        Serial.println(minValueLDR);
      } else {
        Serial.println("The entered value does not meet the requirements.");
      }
      printMainMenu();
      mainMenuChoice = 0;
      break;

    case 4:
      mainMenuChoice = 0;
      printMainMenu();
      break;
    default:
      Serial.println("Invalid choice. Please enter a valid option.");
      printSensorSettingsSubMenu();
      break;
  }
}

int timerWhile(){
  if (millis() - startTime > 5000) {
    Serial.println("Timeout: No input received. Returning to main menu.");
    printMainMenu();
    mainMenuChoice = 0;
    return 1;
  }else return 0;

}

void printResetLoggerDataSubMenu() {
  Serial.println("2. Reset Logger Data:");
  Serial.println("Are you sure?");
  Serial.println("  2.1 Yes.");
  Serial.println("  2.2 No.");
}

void handleResetLoggerDataInput() {
  int submenuChoice = Serial.parseInt();
  Serial.read(); // Consume the newline character

  switch (submenuChoice) {
    case 1:
      minValueUltrasonic = 0;
      minValueLDR = 0;
      samplingInterval = 0;
      EEPROM.update(addressminValueUltrasonic, minValueUltrasonic);
      EEPROM.update(addressminValueLDR, minValueLDR);
      EEPROM.update(addressSamplingInterval, samplingInterval);
      Serial.println("You have reset all the data.");
      mainMenuChoice = 0;
      printMainMenu();
      break;
    case 2:
      mainMenuChoice =0;
      printMainMenu();
      break;
    default:
      Serial.println("Invalid choice. Please enter a valid option.");
      printResetLoggerDataSubMenu();
      break;
  }
}

void printSystemStatusSubMenu() {
  Serial.println("3. System Status:");
  Serial.println("  3.1 Current Sensor Readings");
  Serial.println("  3.2 Current Sensor Settings.");
  Serial.println("  3.3 Display Logged Data.");
  Serial.println("  3.4 Back");
}

void handleSystemStatusInput() {
  int submenuChoice = Serial.parseInt();
  Serial.read(); // Consume the newline character

  switch (submenuChoice) {
    case 1:
      Serial.println("Printing values of sensors. Press any digit to exit");
      startTime = millis();
      while (!Serial.available()) {
        currentTime = millis();
        if(currentTime - lastCurrentTime > samplingInterval*1000){
          ultrasonicSensor();
          LDRSensor();
          lastCurrentTime = currentTime;
          Serial.println("Distance: " + String(distance));
          Serial.println("Brightness: " + String(intensity));
        }
        AutomatLed();
      }
      Serial.read();
      mainMenuChoice = 0;
      printMainMenu();

      break;
    case 2:
      samplingInterval = EEPROM.read(addressSamplingInterval);
      minValueUltrasonic = EEPROM.read(addressminValueUltrasonic);
      minValueLDR = EEPROM.read(addressminValueLDR);
      Serial.println("The sampling rate for all sensor is:" + String(samplingInterval)+".");
      Serial.println("The threshold value for the ultrasonic sensor is" + String(minValueUltrasonic) + " cm.");
      Serial.println("The threshold value for the LDR sensor is" + String(minValueLDR) + ".");
      mainMenuChoice = 0;
      printMainMenu();
      break;
    case 3:
      Serial.println("TO DO");
      mainMenuChoice = 0;
      printMainMenu();
      break;
    case 4:
      // Go back to main menu
      mainMenuChoice = 0;
      printMainMenu();
      break;
    default:
      Serial.println("Invalid choice. Please enter a valid option.");
      printSystemStatusSubMenu();
      break;
  }
}

void printRGBLEDControlSubMenu() {
  Serial.println("4. RGB LED Control:");
  Serial.println("  4.1 Manual Color Control.");
  Serial.println("  4.2 LED: Toggle Automatic ON/OFF.");
  Serial.println("  4.3 Back");
}

void handleRGBLEDControlInput() {
  int submenuChoice = Serial.parseInt();
  Serial.read(); // Consume the newline character

  switch (submenuChoice) {
    case 1:
      Serial.println("You must add values between 0 and 255");
      Serial.println("Red:");
      startTime = millis();
      while (!Serial.available()) {
        if(timerWhile())
          return;
      }
      redLight = Serial.parseInt();
      Serial.println(redLight);
      EEPROM.update(addressRedLight,redLight);
      startTime = millis();
      Serial.println("Green:");
      while (!Serial.available()) {
        if(timerWhile())
          return;
      }
      
      greenLight = Serial.parseInt();
      Serial.println(int(greenLight));
      EEPROM.update(addressGreenLight, greenLight);
      startTime = millis();
      Serial.println("Blue:");
      while (!Serial.available()) {
        if(timerWhile())
          return;
      }
      
      blueLight = Serial.parseInt();
      Serial.println(blueLight);
      EEPROM.update(addressBlueLight, blueLight);
      if((redLight>=0 && redLight<=255)&&(blueLight>=0 && blueLight<=255)&&(greenLight>=0 && greenLight<=255))
      {
      }
      else{
        Serial.println("Invalid Data");
        redLight = 0;
        blueLight = 0;
        greenLight = 0;
      }
      mainMenuChoice = 0;
      printMainMenu();
      break;
    case 2:
      // Call function for LED: Toggle Automatic ON/OFF
      ModAutomate = !ModAutomate;
      EEPROM.update(addressToggle, ModAutomate);
      if(ModAutomate == true){
        Serial.println("Toggle Automatic ON");
      }else{
        Serial.println("Toggle Automatic OFF");
      }
      mainMenuChoice =  0;
      printMainMenu();
      break;
    case 3:
      // Go back to main menu
      mainMenuChoice = 0;
      printMainMenu();
      break;
    default:
      Serial.println("Invalid choice. Please enter a valid option.");
      printRGBLEDControlSubMenu();
      break;
  }
}

void ultrasonicSensor(){
    // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Sound wave reflects from the obstacle, so to calculate the distance we
  // consider half of the distance traveled.
  distance = duration*0.034/2;
  // Prints the distance on the Serial Monitor
  
  
}

void LDRSensor(){
  photocellValue = analogRead(photocellPin);  
  intensity = map(photocellValue,100,700,0,255);
  // Serial.println("LDR: " + String(intensity));
}
//Function for controlling the RGB LED.
void AutomatLed()
{
  ModAutomate = EEPROM.read(addressToggle);
  if(ModAutomate == true){
    minValueLDR = EEPROM.read(addressminValueLDR);
    minValueUltrasonic = EEPROM.read(addressminValueUltrasonic);
    if(intensity<minValueLDR || distance<minValueUltrasonic){
    redLight = 255;
    greenLight = 0;
    blueLight = 0;
    analogWrite(ledRedPin,int(redLight));
    analogWrite(ledGreenPin,int(greenLight));
    analogWrite(ledBluePin,int(blueLight));
  }else{
    redLight = 0;
    greenLight = 255;
    blueLight = 0;
    analogWrite(ledRedPin,int(redLight));
    analogWrite(ledGreenPin,int(greenLight));
    analogWrite(ledBluePin,int(blueLight));
  }
  }else{
    redLight = EEPROM.read(addressRedLight);
    greenLight = EEPROM.read(addressGreenLight);
    blueLight = EEPROM.read(addressBlueLight);
    analogWrite(ledRedPin,int(redLight));
    analogWrite(ledGreenPin,int(greenLight));
    analogWrite(ledBluePin,int(blueLight));
  }
}

void runningSensors()
{
  samplingInterval = EEPROM.read(addressSamplingInterval);
  currentTime = millis();
  if(currentTime - lastCurrentTime > samplingInterval*1000){
    ultrasonicSensor();
    LDRSensor();
    lastCurrentTime = currentTime;
  }
  AutomatLed();
}
