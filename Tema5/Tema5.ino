// Define connections to the shift register
const int latchPin = 11; // Connects to STCP (latch pin) on the shift register
const int clockPin = 10; // Connects to SHCP (clock pin) on the shift register
const int dataPin = 12; // Connects to DS (data pin) on the shift register

// Define connections to the digit control pins for a 4-digit display
const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;

const int startStopPin = 2;
const int lapPin = 8;
const int resetPin = 3;
// Store the digits in an array for easy access
int displayDigits[] = {segD1, segD2, segD3, segD4};
const int displayCount = 4; // Number of digits in the display

// Define the number of unique encodings (0-9, A-F for hexadecimal)
const int encodingsNumber = 16;

volatile byte buttonStateInterrupt = HIGH;

// Define byte encodings for the hexadecimal characters 0-F
byte byteEncodings[encodingsNumber] = {
  // A B C D E F G DP 
  B11111100, // 0 
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
};

// Variables for controlling the display update timing
unsigned long lastIncrement = 0;
unsigned long delayCount = 100; // Delay between updates (milliseconds)
unsigned long number = 0; // The number being displayed
unsigned long ltime = 0;
unsigned long lastDebounceTimeStart = 0;
unsigned long lastDebounceTimeReset = 0;
unsigned long lastDebounceTimeLap = 0;
unsigned long debounceDelay = 50;

byte buttonState = HIGH;
byte buttonState2 = HIGH;
byte buttonState3 = HIGH;
byte readingStart;
byte lastReadingStart = HIGH;

byte readingReset;
byte lastReadingReset = HIGH;

byte readingLap;
byte lastReadingLap = HIGH;

bool start = false;
bool reset[2] = {false, false};
int countPressReset = -1;
int numLap = 0;
int lastnumLap = 0;
int laps[4] = {0, 0, 0, 0};

const int numOfLaps = 4;

bool possiblePress = false;
bool shouldStartCounter = false;

void setup() {
  // Initialize the pins connected to the shift register as outputs
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(startStopPin, INPUT_PULLUP);
  pinMode(lapPin, INPUT_PULLUP);
  pinMode(resetPin, INPUT_PULLUP);

  // Initialize digit control pins and set them to LOW (off)
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
  attachInterrupt(digitalPinToInterrupt(startStopPin), handleInterrupt, CHANGE);
  // Begin serial communication for debugging purposes
  Serial.begin(9600);
}

void loop() {
  DebounceStartOrStop();
  if (start) {
    Counting();
    // TODO: Display the incremented number on the 7-segment display using multiplexing
  } else if (!start) {
    // number = 0;
    writeNumber(number);
    DebounceReset();
    LapOrReset();
  }
}

void writeReg(int digit) {
  // Prepare to shift data by setting the latch pin low
  digitalWrite(latchPin, LOW);
  // Shift out the byte representing the current digit to the shift register
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  // Latch the data onto the output pins by setting the latch pin high
  digitalWrite(latchPin, HIGH);
}

void activateDisplay(int displayNumber) {
  // Turn off all digit control pins to avoid ghosting
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  // Turn on the current digit control pin
  digitalWrite(displayDigits[displayNumber], LOW);
}

void writeNumber(int number) {
  int currentDisplay = 3;
  int lastDigit = 0;
  int currentNumber = number;
  while (currentNumber != 0) {
    lastDigit = currentNumber % 10;
    activateDisplay(currentDisplay);

    writeReg(byteEncodings[lastDigit]);
    //Illuminate the DP on digit 2 to display the timer correctly (000.0).
    if (currentDisplay == 2) {
      writeReg(B00000001);
    }
    currentDisplay--;
    currentNumber = currentNumber / 10;

    writeReg(B00000000);
  }
  //Set the digit values to 0 on the unused digits.
  while (currentDisplay >= 0) {
    activateDisplay(currentDisplay);
    writeReg(byteEncodings[0]);
    if (currentDisplay == 2) {
      writeReg(B00000001);
    }
    currentDisplay--;
    writeReg(B00000000);
  }
}

void DebounceStartOrStop() {
  if (possiblePress) {
    if (shouldStartCounter) {
      lastDebounceTimeStart = millis();
      shouldStartCounter = false;
    }
    if((millis() - lastDebounceTimeStart) > debounceDelay){
      readingStart = digitalRead(startStopPin);
      if (readingStart != buttonState) {
        buttonState = readingStart;
        if (buttonState == HIGH) {
          start = !start;
          Serial.print("ok\n");
          reset[0] = false;
          reset[1] = false;
          countPressReset = -1;
        }
        possiblePress = false;
      }
    }
    lastReadingStart = readingStart;
}
}
void DebounceReset() {
  readingReset = digitalRead(resetPin);
  if (readingReset != lastReadingReset) {
    lastDebounceTimeReset = millis();
  }
  if ((millis() - lastDebounceTimeReset) > debounceDelay) {
    if (readingReset != buttonState2) {
      buttonState2 = readingReset;
      if (buttonState2 == HIGH) {
        countPressReset++;
        reset[countPressReset % 2] = true;
        Serial.print("OKKK\n");
        numLap = 0;
        lastnumLap = 0;
      }
    }
  }

  lastReadingReset = readingReset;
}

void DebounceLap() {
  readingLap = digitalRead(lapPin);
  if (readingLap != lastReadingLap) {
    lastDebounceTimeLap = millis();
  }
  if ((millis() - lastDebounceTimeLap) > debounceDelay) {
    if (readingLap != buttonState3) {
      buttonState3 = readingLap;
      if (buttonState3 == HIGH) {
        numLap++;
      }
    }
  }

  lastReadingLap = readingLap;
}

void Counting() {
  if ((millis() - lastIncrement) > delayCount) {
    number++;
    lastIncrement = millis();
  }

  number %= 10000;
  writeNumber(number);
  DebounceLap();
  //Save laps
  if (numLap != lastnumLap) {
    laps[numLap % numOfLaps] = number;
    Serial.println(laps[numLap % numOfLaps]);
    lastnumLap = numLap;
  }
}

void LapOrReset() {
  if (reset[0] == true) {
    DebounceLap();
    //Cycle through the laps
    if (numLap != lastnumLap) {
      Serial.print("numlap\n");
      number = laps[numLap % numOfLaps];
      // writeNumber(number);
      lastnumLap = numLap;
    } else if (numLap == 0) {
        number = 0;
        //reset laps if reset[1] is true
        if (reset[1] == true) {
          for (int i = 0; i < numOfLaps; i++) {
            laps[i] = 0;
          }
          reset[1] = false;
      }
    }
  }
}

void handleInterrupt() {
  possiblePress = true; // Indicate that a button press might have occurred
  shouldStartCounter = true; // Indicate the need to start the debounce counter
}