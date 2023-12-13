#include "LedControl.h" // need the library
#include <LiquidCrystal.h>
#include <EEPROM.h>
const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;
const byte matrixSize = 8;
const byte brightnessPin = 3;
const byte brightnessAnalogPin = A2;
const int pinY = A0; // A0 - Analog pin connected to X output
const int pinX = A1; // A1 - Analog pin connected to Y output
const int pinSW = 2; // Digital pin connected to switch output
// pin 12 is connected to the MAX7219 pin 1
// pin 11 is connected to the CLK pin 13
// pin 10 is connected to LOAD pin 12
// 1 as we are only using 1 MAX7219

const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


const int timeToDetonate = 3000;

const uint64_t WinImage = 0xc6eefed6c6c6c600; //for display W
const uint64_t Lose = 0x7e06060606060600; //for display L

bool joyMoved = false;

int minThreshold = 400;
int maxThreshold = 700;

int xValue;
int yValue;

bool possiblePress = false;
bool shouldStartCounter = false;

byte buttonState = HIGH;
byte swState = LOW;
byte lastSwState = LOW;

int buttonPressed = 0;
int lastBtnPress = 0;

unsigned long currentMillis = 0;
unsigned long lastMillis = 0;

unsigned long lastDebounceTime = 0;
unsigned long BlastMillis = 0;

unsigned long lastDetonateMillis = 0;

bool Lost = false;
bool Win = false;
bool notYet = false;

bool bombHasDetonated = true;

byte state = HIGH;
byte bState = HIGH;




long lastTime = 0;
long lastJoyTime = 0;
int interval = 50;
byte reading = LOW;
byte joyState = LOW;
byte lastReading = LOW;
byte joyReading = LOW;
byte lastJoyReading = HIGH;



LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); //DIN, CLK, LOAD, No. DRIVER
byte matrixBrightness = 2;
byte matrix[matrixSize][matrixSize] = {
{0, 0, 0, 0, 0, 1, 1, 1},
{1, 0, 0, 0, 0, 1, 1, 0},
{1, 1, 0, 0, 1, 1, 0, 0},
{1, 1, 1, 0, 0, 1, 0, 0},
{1, 1, 1, 0, 0, 0, 0, 0},
{1, 1, 1, 1, 1, 1, 1, 0},
{0, 0, 1, 1, 0, 0, 0, 0},
{1, 1, 0, 1, 1, 0, 0, 1}
};





int score = 0;
int lifes = 3;


bool inMainMenu = true; //default 
bool inGame = false;
bool inSettings = false;
bool inScore = false;
bool inScroll = false;

bool startGame = false;



int brightnessLevel;
const int brightnessLevelAddress = 1;
const int matrixIntensityLevelAddress = 2;
int intensityLevels[] = {1,3,6,9,12,15};
int brightnessAnalogLevel;
int matrixIntensityLevel;




#include "mainMenu.h"

struct coordinate{
  int x;
  int y;
  coordinate(){}
  //for left, right, up, down
  coordinate(int _x, int _y){
    x = _x;
    y = _y;
  }
};

void checkMatrix();
// To move the player on the map
coordinate left(-1,0);
coordinate right(1,0);
coordinate up(0,-1);
coordinate down(0,1);

class Bomb{
  coordinate bCoordinates;
  int delayBlink;

public:
  //Initialize the coordinates in the constructor to avoid accessing residual values for the first placed bomb.
  Bomb(){
    delayBlink = 100;
    this->bCoordinates.x = -1;
    this->bCoordinates.y = -1;
  }
  Bomb(int x, int y)
  {
    this->bCoordinates.x = x;
    this->bCoordinates.y = y;
    delayBlink = 100;
  }
  void spawnBomb(){
    lc.setLed(0,this->bCoordinates.y, this->bCoordinates.x, bState);//Blink led
    if(millis() - BlastMillis > this->delayBlink)
    {
      bState=!bState;
      BlastMillis = millis();
    }
  }
  void detonateBomb()
  {
    //Turn off all the LEDs that are in the area of the bomb.
    if((millis() - lastDetonateMillis > timeToDetonate) && bombHasDetonated == false){
      
      for (int i = this->bCoordinates.x-1; i <= this->bCoordinates.x+1; i++)
        for(int j = this->bCoordinates.y-1; j <= this->bCoordinates.y+1; j++)
        {
          if(matrix[j][i] == 1)
          {
            lc.setLed(0, j, i, LOW);
            matrix[j][i] = 0;
            score++;
          }

        }
      bombHasDetonated = true;
      
    }
  }
  //setter
  void setCoordinates(int x, int y){
    this->bCoordinates.x = x;
    this->bCoordinates.y = y;
  }
  //getter
  coordinate getBombCoordinate(){
    return bCoordinates;
  }
};

class Player{
  coordinate pCoordonates;
  int delayBlink;
  Bomb bombs;
  bool placeBomb = false;

public:
  Player(int x, int y){
    this->pCoordonates.x = x;
    this->pCoordonates.y = y;
    delayBlink = 600;
    
  }
  void blinkPlayer(){
  lc.setLed(0,this->pCoordonates.y, this->pCoordonates.x, state);
  if(millis() - lastMillis > this->delayBlink)
  {
    state=!state;
    lastMillis = millis();
  }
  }
  void movePlayer(){
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);
    this->blinkPlayer();
    checkMatrix();
    // Spawn the bomb only when the player has left the placement location.
    if(placeBomb == true && (bombs.getBombCoordinate().x !=this->pCoordonates.x || bombs.getBombCoordinate().y != this->pCoordonates.y) && bombHasDetonated ==false){
        bombs.spawnBomb();
        bombs.detonateBomb();
        checkPLayer();
    }
    //Move the player on the map
    if(xValue<minThreshold && joyMoved == false&&matrix[this->pCoordonates.y + left.y][this->pCoordonates.x +left.x] == 0 && this->pCoordonates.x>0)
    {
      
      move(left);
      
    }else{
      if(xValue>maxThreshold && joyMoved == false&&matrix[this->pCoordonates.y + right.y][this->pCoordonates.x +right.x] == 0 && this->pCoordonates.x<7)
      {
      move(right);    
      }else {
        if(yValue>maxThreshold && joyMoved == false && matrix[this->pCoordonates.y + up.y][this->pCoordonates.x + up.x] == 0 && this->pCoordonates.y>0){
          move(up);
        }else{
          if(yValue<minThreshold && joyMoved == false && matrix[this->pCoordonates.y + down.y][this->pCoordonates.x + down.x] == 0 && this->pCoordonates.y<70){
            move(down);
        }
        } 
      }
    }
    //reset joyMoved
    if(xValue >= minThreshold && xValue <= maxThreshold && yValue >= minThreshold && yValue <= maxThreshold && joyMoved == true){
      joyMoved = false;
   }
   checkBomb();
  }
  void move(coordinate direction){
    state = HIGH;
    lc.setLed(0,this->pCoordonates.y, this->pCoordonates.x, LOW);
    this->pCoordonates.x +=direction.x;
    this->pCoordonates.y += direction.y;
    joyMoved = true;
  }  

  void checkBomb()
  {
    if (possiblePress) {
      // Start the debounce counter if needed
      if (shouldStartCounter) {
        lastDebounceTime = millis();
        shouldStartCounter = false;
      }
      if((millis() - lastDebounceTime) > 50){
        swState = digitalRead(pinSW);
        if (swState != buttonState) {
          
          if (swState == LOW) {
            buttonPressed++;
          }
          possiblePress = false;
        }
      }
      
    }

    lastSwState = swState;
    if(buttonPressed != lastBtnPress && bombHasDetonated == true){
      //Turn off previous bomb
      if(bombs.getBombCoordinate().x!=-1 && bombs.getBombCoordinate().y != -1)
        lc.setLed(0, bombs.getBombCoordinate().y, bombs.getBombCoordinate().x, LOW);
      lastDetonateMillis = millis();
      this->bombs.setCoordinates(this->pCoordonates.x,this->pCoordonates.y);
      placeBomb = true;
      lastBtnPress = buttonPressed;
      bombHasDetonated = false;
      
    }

  }
  void checkPLayer(){
    //At the moment of detonation, we check if the player is in the bomb's damage area.
    if(bombHasDetonated == true){
      for (int i = this->bombs.getBombCoordinate().x-1 ; i <= this->bombs.getBombCoordinate().x+1; i++)
        for (int j = this->bombs.getBombCoordinate().y-1 ; j <= this->bombs.getBombCoordinate().y+1; j++)
        {
          if(this->pCoordonates.x == i && this->pCoordonates.y ==j){
            Lost = true;
          }
        }
    }
  }
};



Player myplayer(0,0);

void introMessage()
{
  lcd.begin(16, 2);
  lcd.print("Welcome to");
  lcd.setCursor(0, 1);
  lcd.print("Bomberman");
  delay(2000);
  lcd.clear();
}



void display(const uint64_t image){
  for (int i = 0; i < 8; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      lc.setLed(0, i, j, bitRead(row, j));
    }
  }
}

void displayMap()
{
    for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrix[row][col]);
    }
  }
}

void handleInterrupt() {
  possiblePress = true; // Indicate that a button press might have occurred
  shouldStartCounter = true; // Indicate the need to start the debounce counter
}
//check if the player win the game
void checkMatrix(){
  for(int i=0;i<8;i++)
    for(int j=0; j<8; j++){
      if(matrix[i][j] == 1)
        notYet = true;
    }
  if(notYet == false)
  {
    Win = true;
    
  }
  notYet = false;

}

void displayScore()
{
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Score:" + String(score));
  lcd.setCursor(1,9);
  lcd.print("Lifes:" + String(lifes));
}

void playGame()
{
  if(!startGame)
    displayMap();
  if(!Lost)
  {
    if(!Win)
    {
      displayScore();
      myplayer.movePlayer();
    }
      
    else display(WinImage);
    delay(10); 
  }
  else{
    display(Lose);
    delay(2000);
    inGame=false;
    inMainMenu = true;
    Lost = !Lost;
    startGame = false;
    mainMenu();
  }
}





void readFromJoystick()
{
   if(inGame)
  {
    playGame();
  }
  else
  {
    //joystick button
  reading = digitalRead(pinSW);

  if (reading != lastReading) {
    lastTime = millis();
  }
  if (millis() - lastTime > interval) {
    if (reading != swState) {
      swState = reading;
      if (swState == LOW) {
        if (inMainMenu) {  //we are in main menu
          enterMainMenu();
        }
        else if(inSettings)
        {
          inSett[currentSetting] = !inSett[currentSetting];
          SetMatrixBrightness();
          
          
        }
}
    }
  }
  if(inSett[0] == true)
  {
     enterSettingsOptions(currentSetting);
  }
  lastReading = reading;
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  
 

  // if ((xValue > minThreshold && xValue < maxThreshold) || (yValue > minThreshold && yValue < maxThreshold)) {
  //   joyReading = LOW;
  // }
  // if ((xValue < minThreshold || xValue > maxThreshold) || (yValue < minThreshold || yValue > maxThreshold)) {
  //   joyReading = HIGH;
  // }
  //  if (joyReading != lastJoyReading) {
  //   lastJoyTime = millis();
  // }
  //   if (millis() - lastJoyTime > interval)
  //     if (joyReading != joyState) {
  //       joyState = joyReading;
  //       if (joyReading == HIGH)
  //         joyMoved = false;
  //   }

  if (yValue > maxThreshold && joyMoved == false) {  //up
    joyMoved = true;
    
    if (inMainMenu) {  //we are in main menu
      if (currentOption > firstPosition) {
        currentOption--;
        mainMenu();
      }
    }else if(inSettings)
    {
      if (currentSetting > firstPosition) {
        currentSetting--;
        enterSettings();
      }
    }
  }
  if (yValue < minThreshold && joyMoved == false) {  //down
    joyMoved = true;
    if (inMainMenu) {  //we are in main menu
      if (currentOption < lastPosition) {
        // Serial.println("Ok2");
        currentOption++;
        mainMenu();
      }
      
    }else if(inSettings)
    {
      if (currentSetting < lastSettingsPosition) {
        currentSetting++;
        enterSettings();
      }
    }
  

}
    if (xValue > maxThreshold && joyMoved == false) {
      joyMoved = true;
      if(inSett[0]==true)
      {

      }else if(inSett[1]==true)
      {
        moveToRight();
      }
      
      else if(inSettings)
      {
        
        mainMenu();
      }
    }

    if(xValue < minThreshold && joyMoved == false)
    {
      joyMoved = true;
      if(inSett[1]==true)
      {
        moveToLeft();
      }

    }

    if(xValue >= minThreshold && xValue <= maxThreshold && yValue >= minThreshold && yValue <= maxThreshold && joyMoved == true){
      joyMoved = false;
   }
   delay(20);
  }
  
  }






void setup() {
  Serial.begin(9600);
  pinMode(pinSW, INPUT_PULLUP);
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness); // sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear screen
  brightnessLevel = EEPROM.read(brightnessLevelAddress);
  if(brightnessLevel<10)
  {
    brightnessLevel = 12;
  }
  analogWrite(brightnessPin, brightnessLevel);
  matrixIntensityLevel = EEPROM.read(matrixIntensityLevelAddress);
  lc.setIntensity(0,intensityLevels[matrixIntensityLevel]);
  introMessage();
  mainMenu();
  // displayMap();

  attachInterrupt(digitalPinToInterrupt(pinSW), handleInterrupt, FALLING);
}

void loop() {
  readFromJoystick();
}



