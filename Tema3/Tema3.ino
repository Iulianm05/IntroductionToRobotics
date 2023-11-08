const int pinBTN1 = 4;
const int pinBTN2 = 2;
const int pinBTN3 = 1;

const int floorLedPin1 = 6;
const int floorLedPin2 = 7;
const int floorLedPin3 = 8;

unsigned int lastDebounceTime = 0;
unsigned int debounceDelay = 100;

const int checkingLedPin = 9;
const int buzzerPin = 5;

byte readingBtn1 = LOW;
byte readingBtn2 = LOW;
byte readingBtn3 = LOW;

byte lastReadingBtn1 = LOW;
byte lastReadingBtn2 = LOW;
byte lastReadingBtn3 = LOW;

byte buttonState1 = LOW;
byte buttonState2 = LOW;
byte buttonState3 = LOW;

int pressed1 = 0;
int pressed2 = 0;
int pressed3 = 0;

int lpressed1 = 0;
int lpressed2 = 0;
int lpressed3 = 0;

byte FloorledState = LOW;
byte checkingLedState = HIGH;
int currentFloor = floorLedPin1;

const long interval = 400;
unsigned long previousMillis = 0;

unsigned int time[100];

int stop = -1;

int calledFloor = 0;
int timeBetween = 1000;
int buzzDuration = 2000;
int freqBuzz = 400;

void setup() {

  pinMode(pinBTN1, INPUT_PULLUP);
  pinMode(pinBTN2, INPUT_PULLUP);
  pinMode(pinBTN3, INPUT_PULLUP);
  pinMode(floorLedPin1, OUTPUT);
  pinMode(floorLedPin2, OUTPUT);
  pinMode(floorLedPin3, OUTPUT);
  pinMode(checkingLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
}

//Functie pentru debounce
void debounce(byte &reading, byte &lastReading, byte &buttonState, const int pinBTN, const int led, int &pressed) {
  reading = digitalRead(pinBTN);
  if (reading != lastReading) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        pressed += 1; //verific cand se apasa butonul prin incrementarea variabilei pressed, transmisa ca parametru
      }
    }
  }
  lastReading = reading;
}

void loop() {

  debounce(readingBtn2, lastReadingBtn2, buttonState2, pinBTN2, floorLedPin2, pressed2);
  debounce(readingBtn1, lastReadingBtn1, buttonState1, pinBTN1, floorLedPin1, pressed1);
  debounce(readingBtn3, lastReadingBtn3, buttonState3, pinBTN3, floorLedPin3, pressed3);
  unsigned long currentMillis = millis();
  //cu stop controlez if-urile, pentru a nu intra intr-un mod haotic in ele
  if (stop == -1) {
    digitalWrite(currentFloor, HIGH);
    stop = 0;
  } else {
    //verific daca s-a schimbat starea butonului 3
    if (pressed3 != lpressed3 && pressed3 != 1 && currentFloor != floorLedPin3) {
      Serial.print("Bines");
      time[0] = millis();
      lpressed3 = pressed3;
      tone(buzzerPin, freqBuzz, buzzDuration);
      stop = 0;
      calledFloor = 3;
    }
    if (calledFloor == 3) {
      //licarire bec operational
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        if (digitalRead(checkingLedPin) == LOW) {
          checkingLedState = HIGH;
        } else {
          checkingLedState = LOW;
        }
      }
      // in time[i] retin timpul anterior, pentru a simula un delay(timeBetween) milisecunde
      if ((millis() - time[0]) > timeBetween && stop == 0 && time[0] != 0 && calledFloor == 3) {
        tone(buzzerPin, freqBuzz, buzzDuration);
        FloorledState = !FloorledState;
        digitalWrite(currentFloor, FloorledState);
        time[1] = millis();
        stop = 1;
      }
      if ((millis() - time[1]) > timeBetween && stop == 1) {
        tone(buzzerPin, freqBuzz, buzzDuration);
        Serial.print("Am intrat");
        FloorledState = !FloorledState;
        digitalWrite(currentFloor, FloorledState);
        stop = 2;
        time[2] = millis();
        currentFloor = floorLedPin2;
      }
      if ((millis() - time[2]) > timeBetween && stop == 2) {
        tone(buzzerPin, freqBuzz, buzzDuration);
        // Serial.print("Am intrat");
        FloorledState = !FloorledState;
        digitalWrite(currentFloor, FloorledState);
        stop = 3;
        time[3] = millis();
      }
      if ((millis() - time[3]) > timeBetween && stop == 3) {
        tone(buzzerPin,freqBuzz, buzzDuration);
        // Serial.print("Am intrat");
        FloorledState = !FloorledState;
        digitalWrite(currentFloor, FloorledState);
        stop = 4;
        time[4] = millis();
        currentFloor = floorLedPin3;
      }
      if ((millis() - time[4]) > timeBetween && stop == 4) {
        tone(buzzerPin, freqBuzz, buzzDuration);
        // Serial.print("Am intrat");
        FloorledState = !FloorledState;
        digitalWrite(currentFloor, FloorledState);
        stop = 5;
        time[5] = millis();
      }
      if (stop == 5) {
        tone(buzzerPin, 2*freqBuzz, buzzDuration/2);
        if ((millis() - time[5]) > timeBetween) {
          time[5] = millis();
          stop = 6;
          calledFloor = 0;
          checkingLedState = HIGH;
        }
      }
    }

    //pentru etajul 1 (3->1)
    if (pressed1 != lpressed1 && pressed1 != 1 && currentFloor != floorLedPin1) {
      // Serial.print("Bine");
      time[6] = millis();
      lpressed1 = pressed1;
      tone(buzzerPin, freqBuzz, buzzDuration);
      stop = 0;
      calledFloor = 1;
    }

    // Pentru etajul 1
    if (calledFloor == 1) {
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        if (digitalRead(checkingLedPin) == LOW) {
          checkingLedState = HIGH;
        } else {
          checkingLedState = LOW;
        }
      }
      if ((millis() - time[6]) > timeBetween && stop == 0 && time[6] != 0 && calledFloor == 1) {
        tone(buzzerPin,freqBuzz, buzzDuration);
        FloorledState = !FloorledState;
        digitalWrite(currentFloor, FloorledState);
        time[7] = millis();
        stop = 1;
        currentFloor = floorLedPin2;
      }
      if ((millis() - time[7]) > timeBetween && stop == 1) {
        tone(buzzerPin, freqBuzz, buzzDuration);
        Serial.print("Am intrat111");
        FloorledState = !FloorledState;
        digitalWrite(currentFloor, FloorledState);
        stop = 2;
        time[8] = millis();
      }
      if ((millis() - time[8]) > timeBetween && stop == 2) {
        tone(buzzerPin, freqBuzz, buzzDuration);
        Serial.print("Am intrat");
        FloorledState = !FloorledState;
        digitalWrite(currentFloor, FloorledState);
        stop = 3;
        time[9] = millis();
        currentFloor = floorLedPin1;
      }
      if ((millis() - time[9]) > timeBetween && stop == 3) {
        tone(buzzerPin,freqBuzz, buzzDuration);
        Serial.print("Am intrat");
        FloorledState = !FloorledState;
        digitalWrite(currentFloor, FloorledState);
        stop = 4;
        time[10] = millis();
      }
      if (stop == 4) {
        tone(buzzerPin, 2*freqBuzz, buzzDuration/2);
        if ((millis() - time[10]) > timeBetween) {
          time[10] = millis();
          stop = 6;
          calledFloor = 0;
          checkingLedState = HIGH;
        }
      }
    }
  }
  digitalWrite(checkingLedPin, checkingLedState);
}

//Acest cod merge doar pentru cazul 1->3 si 3->1