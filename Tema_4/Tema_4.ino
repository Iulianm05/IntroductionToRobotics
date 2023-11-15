// Declare all the joystick pins
const int pinSW = 2; // Digital pin connected to switch output
const int pinX = A0; // A0 - Analog pin connected to X output
const int pinY = A1; // A1 - Analog pin connected to Y output

// Declare all the segments pins
const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;

const int segSize = 8;
int buttonPressed = 0;
int lastBtnPress = 0;

bool commonAnode = false; // Modify if you have common anode
byte state = HIGH;
byte swState = LOW;
byte lastSwState = LOW;
int xValue = 0;
int yValue = 0;

byte buttonState = HIGH;

//Declare variables for intrerupt function
volatile bool possiblePress = false; 
volatile bool shouldStartCounter = false; 
volatile bool possiblePressLong = false;
volatile bool shouldStartCounter2 = false; 

unsigned long lastDebounceTime2 = 0;

const int debounceDelay = 50;
const int resetDelay = 2500; 

bool joyMoved = false;

int minThreshold = 400;
int maxThreshold = 600;

int currentSegment = 7;


unsigned long lastDebounceTime = 0;

//I have rearranged the order of the segments in the hope of finding a pattern.
// The new order is as follows: 0 - C, 1 - D, 2 - E, 3 - G, 4 - F, 5 - A, 6 - B, 7 - DP.
int segments[segSize] = { 
  pinC, pinD, pinE, pinG, pinF, pinA, pinB, pinDP
};
byte states[segSize] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};

void setup() {
  // TODO: Initialize joystick pins and commonAnode
  pinMode(pinSW, INPUT_PULLUP);
  // Initialize all the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
    attachInterrupt(digitalPinToInterrupt(pinSW), handleInterrupt, FALLING);
  Serial.begin(9600);
  // TODO: Check if commonAnode should be modified here

}

void loop() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  digitalWrite(segments[currentSegment],state);
  delay(200);
  state = !state;
  MoveSegment();
  CheckPress();
  
}


//Function to check if the joystick has been moved for a specific segment displacement.
void MoveSegment(){
    //For left
  if(xValue>maxThreshold && joyMoved == false && currentSegment == 7)
  {
    currentSegment = 0;  
    joyMoved = true;
  }else{
    
    if(xValue>maxThreshold && joyMoved == false &&(currentSegment ==0)){
     
      currentSegment = 2;
      joyMoved = true;
    }else{
      if(xValue>maxThreshold && joyMoved == false &&(currentSegment ==6)){
         
          currentSegment = 4;
          joyMoved = true;
      }else{
        if(xValue>maxThreshold && joyMoved == false &&currentSegment ==1){         
          currentSegment += 1;
          joyMoved = true;
        }
        else{
          if(xValue>maxThreshold && joyMoved == false && currentSegment ==5){         
          currentSegment -= 1;
          
          joyMoved = true;
        }else{
          if(xValue>maxThreshold && joyMoved == false && currentSegment ==3){         
            currentSegment =3;
          
            joyMoved = true;
          }
        }
        }
        
      }
    }
  }
  //for right
  if(xValue<minThreshold && joyMoved == false && currentSegment == 2){
    
    currentSegment = 0;
    joyMoved = true;
  }
  else{
    if(xValue<minThreshold && joyMoved == false && currentSegment == 4)
    {    
    currentSegment = 6;
    joyMoved = true;
    }
    else{
      if(xValue<minThreshold && joyMoved == false && currentSegment == 1)
      {  
        currentSegment = 0;
        joyMoved = true;
      }
      else{
        if(xValue<minThreshold && joyMoved == false && currentSegment == 3){
          
          currentSegment = 3;
          joyMoved = true;
        }else {
          if(xValue<minThreshold && joyMoved == false && currentSegment == 0){
          
          currentSegment = 7;
          joyMoved = true;
        }
        else{
          if(xValue<minThreshold && joyMoved == false && currentSegment ==5){         
          currentSegment =6 ;
          
          joyMoved = true;
        }
        }
        }
      }
    }
  }
 

  //for Down
  if(yValue<minThreshold && joyMoved == false && (currentSegment == 5 || currentSegment == 6 || currentSegment ==4))
  {
    
    
    currentSegment = 3;  
    joyMoved = true;
  }else{
    
    if(yValue<minThreshold && joyMoved == false &&(currentSegment ==3 ||currentSegment == 0 || currentSegment ==2)){
     
      currentSegment = 1;
      joyMoved = true;
    }
  }


  //for up

  if(yValue>maxThreshold && joyMoved == false && (currentSegment == 0 || currentSegment == 1 || currentSegment ==2))
  {
    
    
    currentSegment = 3;  
    joyMoved = true;
  }else{
    
    if(yValue>maxThreshold && joyMoved == false &&(currentSegment ==3 ||currentSegment == 4 || currentSegment ==6)){
     
      currentSegment = 5;
      joyMoved = true;
    }
  }

  if(xValue >= minThreshold && xValue <= maxThreshold && yValue >= minThreshold && yValue <= maxThreshold){
    joyMoved = false;
  }
  
}

void CheckPress()
{
  if (possiblePress) {
    // Start the debounce counter if needed
    if (shouldStartCounter) {
      lastDebounceTime = millis();
      shouldStartCounter = false;
    }
    if((millis() - lastDebounceTime) > debounceDelay){
      swState = digitalRead(pinSW);
      if (swState != buttonState) {
        
        if (swState == LOW) {
          buttonPressed++;
        }
        possiblePress = false;
      }
    }
    
}

if(possiblePressLong){
    if (shouldStartCounter2) {
      lastDebounceTime2 = millis();
      shouldStartCounter2 = false;
    }

    if ((millis() - lastDebounceTime2) > resetDelay) {
      // Read the current state of the button
      swState = digitalRead(pinSW);
      // If the button state has changed (i.e., if it was not a false press)
      if (swState != buttonState) {
        
        if (swState == LOW) {
          currentSegment =7;
          for(int i=0;i<segSize;i++)
          {
            states[i]=LOW;
          }
        }
      }
      // If the states match, it was a false alarm due to noise
      // Reset the possiblePress flag
      possiblePressLong = false;
  }
}
  //If the button was short-pressed, then the state at the currentSegment position toggles; otherwise, the segments will be updated and displayed
  lastSwState = swState;
  if(buttonPressed != lastBtnPress){
    states[currentSegment] = !states[currentSegment];
    lastBtnPress = buttonPressed;
    
  }
  else{
    SwitchLed();
    
  }
}

void SwitchLed()
{
  for(int i=0;i<segSize;i++)
  {
    digitalWrite(segments[i],states[i]);
  }
}

void handleInterrupt() {
  // Serial.print("Sunt in intrerupere\n");
  possiblePress = true; // Indicate that a button press might have occurred
  shouldStartCounter = true; // Indicate the need to start the debounce counter
  possiblePressLong = true;
  shouldStartCounter2 = true;
}



