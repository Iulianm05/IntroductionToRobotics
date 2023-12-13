const byte arrows[][8] = {
  { 0b00000,
    0b00100,
    0b01010,
    0b10001,
    0b00000,
    0b00000,
    0b00000,
    0b00000 
  },
  { 0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b10001,
    0b01010,
    0b00100,
    0b00000 
  },
  { 0b00100,
    0b01010,
    0b10001,
    0b00000,
    0b00000,
    0b10001,
    0b01010,
    0b00100 
  },

  { 0b00000,
    0b00000,
    0b00010,
    0b00100,
    0b01000,
    0b00100,
    0b00010,
    0b00000 
  },
  { 0b00000,
    0b00000,
    0b01000,
    0b00100,
    0b00010,
    0b00100,
    0b01000,
    0b00000 }
};

const uint8_t LEVELS[][8] = {
  { 0b00000,
    0b11111,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b11111,
    0b00000 },
  { 0b00000,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b00000 }
};

String const mainMenuOptions[] = { "START", "SETTINGS", "HIGH SCORE", "GAME INFO", "ABOUT" };

int currentOption = 0;
const int firstPosition = 0;
const int lastPosition = sizeof(mainMenuOptions) / sizeof(String) - 1;

const int arrowUp = 0;
const int arrowDown = 1;
const int arrowBoth = 2;
const int arrowLeft = 3;
const int arrowRight = 4;

String const settingsOptions[] = { "LCD Brightness", "Matrix Brigtness"};
int currentSetting = 0;


const int lastSettingsPosition = sizeof(settingsOptions) / sizeof(String) - 1;

bool inSett[] = {false,false};
void SetMatrixBrightness();
void enterSettings();


void mainMenu()
{
  inMainMenu = true;
  inGame = false;
  inSettings = false;
  inScore = false;
  inScroll = false;

  // menuMatrix();

  lcd.createChar(3, arrows[arrowBoth]);
  lcd.createChar(2, arrows[arrowUp]);
  lcd.createChar(1, arrows[arrowDown]);

  lcd.clear();
  lcd.setCursor(4, 0);  //title
  lcd.print("MAIN MENU");

  lcd.setCursor(1, 1);  //current option
  lcd.print(">");
  lcd.print(mainMenuOptions[currentOption]);

  lcd.setCursor(14, 1);
  //print up or down arrow
  if (currentOption == firstPosition) {
    lcd.write(1);
  } else if (currentOption == lastPosition) {
    lcd.write(2);
  } else {
    lcd.write(3);
  }
  delay(100);

}

void enterMainMenu()
{
  if(currentOption == 0)
  {
    inMainMenu = false;
    inGame = true;
    inSettings = false;
    inScore = false;
    inScroll = false;
  }
  else if(currentOption == 1)
  {
    enterSettings();
  }
}



void moveToRight()
{
    if (currentSetting == 1) // matrix brightnesss
    {
       
      if (matrixIntensityLevel < 5)
        {matrixIntensityLevel ++; }
          
    }
    SetMatrixBrightness(); 
}

void moveToLeft()
{
  if(currentSetting == 1)
  {
    if(matrixIntensityLevel > 0)
      matrixIntensityLevel --;
  }
  SetMatrixBrightness(); 
}


void lcdMatrixBrightness(int level) {

  lcd.createChar(4, LEVELS[0]);  // empty
  lcd.createChar(5, LEVELS[1]);  //full


  //title
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(settingsOptions[currentSetting]);

  lcd.setCursor(4, 1);
  lcd.print("-");
  for (int lvl = 0; lvl < 6; lvl++) {
    if (level >= 0) {
      lcd.write(5);
    } else {
      lcd.write(4);
    }
    level--;
  }
  lcd.print("+");

  lcd.setCursor(15, 1);
  lcd.write(3);
}



void enterSettingsOptions(int currentSetting){
  //enter in current selected option

  if (currentSetting == 0) // lcd brightness
      {
        
        brightnessAnalogLevel = analogRead(brightnessAnalogPin);
        brightnessLevel = map(brightnessAnalogLevel, 0, 1024, 0, 255);

        analogWrite(brightnessPin, brightnessLevel); // make change on lcd
        Serial.println(brightnessLevel);
        //update in EEPROM

        EEPROM.update(brightnessLevelAddress, brightnessLevel);
        brightnessLevel = EEPROM.read(brightnessLevelAddress);
        //update in EEPROM

      }
}

void SetMatrixBrightness()
{
  if (currentSetting == 1)
      {
        lcdMatrixBrightness(matrixIntensityLevel);       
        lc.setIntensity(0,intensityLevels[matrixIntensityLevel]);
        for(int i=0;i<8;i++)
        {
          for(int j=0; j<8; j++)
          {
            lc.setLed(0, i, j, 1);
          }
        }

        //update in EEPROM

        EEPROM.update(matrixIntensityLevelAddress, matrixIntensityLevel);
        brightnessLevel = EEPROM.read(matrixIntensityLevelAddress);
      } 

}


void enterSettings()
{
  inMainMenu = false;
  inSettings = true;
  inGame = false;
  inScore = false;
  inScroll = false;
  lcd.clear();
  lcd.setCursor(2, 0);  //title
  lcd.print("SETTINGS MENU");

  lcd.setCursor(0, 1);  //current option
  lcd.print(">");
  lcd.print(settingsOptions[currentSetting]);

  lcd.setCursor(15, 1);
  //print up or down arrow
  if (currentSetting == firstPosition) {
    lcd.write(1);
  } else if (currentSetting == lastSettingsPosition) {
    lcd.write(2);
  } else {
    lcd.write(3);
  }
  delay(100);
}
