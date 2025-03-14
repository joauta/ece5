#include <Arduino.h>
#include <SPI.h>

// Using an Arduino UNO R4 Minima
// pin usage:
// CS  DC/RS  RESET  SDI/MOSI  SCK  SDO/MISO  LED    VCC     GND
// 10    9      8       11     13      12      5   5V/3.3V   GND

// Remember to set the pins to suit your display module!

#include <Arduino_GFX_Library.h> //Graphics library (fork of Arduino_GFX)
#include <ezButton.h>  //Button library

#define MODEL ST7796S
#define CS 10
#define CD 9
#define RST 8
#define MOSI 11
#define MISO 12
#define SCK 13

Arduino_DataBus *bus = new Arduino_HWSPI(CD, CS);
Arduino_GFX *gfx = new Arduino_ST7796(bus, RST, 0, false);

// Colors used, the ones in comments are already defined in the library.
// #define BLACK 0x0000
// #define BLUE 0x001F
// #define RED 0xF800
// #define YELLOW 0xFFE0
// #define WHITE 0xFFFF
#define CYAN 0x07FB
#define PINK 0xFDDB
#define PEACH 0xFDD2

//Joystick setup
const short X = 0;
const short Y = 1;
const short SW_PIN = 2;

const short MaxReadings = 10;

short Xreadings[MaxReadings];
short XreadIndex = 0;
short Xtotal = 0;
short X_Pos = 0;

short Yreadings[MaxReadings];
short YreadIndex = 0;
short Ytotal = 0;
short Y_Pos = 0;

char joystickDirection = 'S';
ezButton button(SW_PIN);

const short PROGMEM upPossibleIndex[40] = {
  7, 9, 10, 12, 15, 18, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 33, 34, 36, 37, 39, 40, 42, 45, 47, 48, 49, 50, 51, 52, 53, 58, 59, 60, 62, 66, 67, 68, 69
};
const short PROGMEM downPossibleIndex[40] = {
  0, 1, 2, 3, 4, 5, 6, 8, 11, 13, 14, 16, 17, 19, 20, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 35, 38, 41, 43, 44, 46, 54, 55, 56, 57, 61, 63, 64, 65
};
const short PROGMEM rightPossibleIndex[50] = {
  0, 2, 4, 6, 7, 8, 9, 10, 11, 12, 14, 15, 16, 17, 18, 20, 22, 24, 25, 27, 28, 30, 32, 34, 35, 36, 37, 38, 40, 41, 42, 43, 44, 45, 46, 48, 50, 52, 54, 55, 56, 57, 60, 61, 62, 65, 66, 67, 68, 69
};
const short PROGMEM leftPossibleIndex[50] = {
  1, 3, 5, 7, 8, 9, 10, 11, 12, 13, 15, 16, 17, 18, 19, 21, 23, 25, 26, 28, 29, 31, 33, 35, 36, 37, 38, 39, 41, 42, 43, 44, 45, 46, 47, 49, 51, 53, 54, 55, 56, 57, 58, 61, 62, 63, 66, 67, 68, 69
};

const short PROGMEM cornerPixelsIndex[70] = { //index of pelletPixelsHorizontal!!
  0, 6, 7, 23, 24, 30, 31, 34, 35, 37, 46, 48, 49, 52, 53, 54, 56, 61, 63, 64, 65, 68, 69, 72, 73, 76, 82, 83, 89, 92, 93, 96, 97, 100, 101, 102, 104, 109, 111, 112, 113, 116, 117, 119, 128, 130, 131, 134, 135, 141, 142, 158, 159, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181
};

const short PROGMEM pelletPixelsHorizontal[182][2] = { //size 166 without extra corner pixels
  {35,30}, {47,30}, {59,30}, {71,30}, {83,30}, {95,30}, {107,30}, {143,30}, {155,30}, {167,30}, {179,30}, {191,30}, {203,30}, {215,30}, {227,30}, {239,30}, {251,30}, {263,30}, {275,30}, {287,30}, {299,30}, {311,30}, {323,30}, {335,30}, {371,30}, {383,30}, {395,30}, {407,30}, {419,30}, {431,30}, {443,30}, {71,66}, {83,66}, {95,66}, {107,66}, {119,66}, {131,66}, {143,66}, {155,66}, {167,66}, {179,66}, {191,66}, {287,66}, {299,66}, {311,66}, {323,66}, {335,66}, {347,66}, {359,66}, {371,66}, {383,66}, {395,66}, {407,66}, {107,102}, {119,102}, {131,102}, {143,102}, {155,102}, {167,102}, {311,102}, {323,102}, {335,102}, {347,102}, {359,102}, {371,102}, {71,114}, {83,114}, {95,114}, {107,114}, {371,114}, {383,114}, {395,114}, {407,114}, {35,150}, {47,150}, {59,150}, {71,150}, {83,150}, {95,150}, {107,150}, {119,150}, {131,150}, {143,150}, {335,150}, {347,150}, {359,150}, {371,150}, {383,150}, {395,150}, {407,150}, {419,150}, {431,150}, {443,150}, {71,186}, {83,186}, {95,186}, {107,186}, {371,186}, {383,186}, {395,186}, {407,186}, {107,198}, {119,198}, {131,198}, {143,198}, {155,198}, {167,198}, {311,198}, {323,198}, {335,198}, {347,198}, {359,198}, {371,198}, {71,234}, {83,234}, {95,234}, {107,234}, {119,234}, {131,234}, {143,234}, {155,234}, {167,234}, {179,234}, {191,234}, {287,234}, {299,234}, {311,234}, {323,234}, {335,234}, {347,234}, {359,234}, {371,234}, {383,234}, {395,234}, {407,234}, {35,270}, {47,270}, {59,270}, {71,270}, {83,270}, {95,270}, {107,270}, {143,270}, {155,270}, {167,270}, {179,270}, {191,270}, {203,270}, {215,270}, {227,270}, {239,270}, {251,270}, {263,270}, {275,270}, {287,270}, {299,270}, {311,270}, {323,270}, {335,270}, {371,270}, {383,270}, {395,270}, {407,270}, {419,270}, {431,270}, {443,270}, {203,66}, {239,66}, {275,66}, {179,102}, {203,102}, {239,102}, {275,102}, {299,102}, {179,198}, {203,198}, {239,198}, {275,198}, {299,198}, {203,234}, {239,234}, {275,234}
};

const short PROGMEM pelletPixelsVertical[146][2] = { 
  {35,30}, {35,42}, {35,54}, {35,66}, {35,78}, {35,90}, {35,102}, {35,114}, {35,126}, {35,138}, {35,150}, {35,162}, {35,174}, {35,186}, {35,198}, {35,210}, {35,222}, {35,234}, {35,246}, {35,258}, {35,270}, {71,66}, {71,78}, {71,90}, {71,102}, {71,114}, {71,126}, {71,138}, {71,150}, {71,162}, {71,174}, {71,186}, {71,198}, {71,210}, {71,222}, {71,234}, {107,30}, {107,42}, {107,54}, {107,66}, {107,102}, {107,114}, {107,186}, {107,198}, {107,234}, {107,246}, {107,258}, {107,270}, {119,66}, {119,78}, {119,90}, {119,102}, {119,198}, {119,210}, {119,222}, {119,234}, {143,30}, {143,42}, {143,54}, {143,66}, {143,102}, {143,114}, {143,126}, {143,138}, {143,150}, {143,162}, {143,174}, {143,186}, {143,198}, {143,234}, {143,246}, {143,258}, {143,270}, {335,30}, {335,42}, {335,54}, {335,66}, {335,102}, {335,114}, {335,126}, {335,138}, {335,150}, {335,162}, {335,174}, {335,186}, {335,198}, {335,234}, {335,246}, {335,258}, {335,270}, {359,66}, {359,78}, {359,90}, {359,102}, {359,198}, {359,210}, {359,222}, {359,234}, {371,30}, {371,42}, {371,54}, {371,66}, {371,102}, {371,114}, {371,186}, {371,198}, {371,234}, {371,246}, {371,258}, {371,270}, {407,66}, {407,78}, {407,90}, {407,102}, {407,114}, {407,126}, {407,138}, {407,150}, {407,162}, {407,174}, {407,186}, {407,198}, {407,210}, {407,222}, {407,234}, {443,30}, {443,42}, {443,54}, {443,66}, {443,78}, {443,90}, {443,102}, {443,114}, {443,126}, {443,138}, {443,150}, {443,162}, {443,174}, {443,186}, {443,198}, {443,210}, {443,222}, {443,234}, {443,246}, {443,258}, {443,270}
};

const short PROGMEM specialPelletIndex[4] = { //index of pelletPixelsHorizontal!!
  7, 24, 135, 159
};

//stores which pellets are eaten
short eatenPelletsX[258] = {-1};
short eatenPelletsY[258] = {-1};
short eatenCount = 0;

//gameStates
short titleState = 0;
short endState = 2;
short playState = 1;
short winState = 3;
short gameState;

//Player
short lives;
char hitGhost = 0;
short addScore = 0;
short score = 0;
long spawnInterval = 12*1000;
long startSpawnTime;
long powerInterval = 13*1000;
long startPowerTime;
bool powerWarning = false;
short ghostCount = 0;

//Title Screen
bool start = false;
bool skipTitle = false;
long startTitleTime = 0;
long titleInterval = 2 * 1000;
char Title[10] = "PAC-MAN";
char titleText[40] = "PRESS DOWN ON THE JOYSTICK TO START!";

//Play Screen
bool skipPlayArea = false;
char scoreText[10] = "SCORE: ";
char readyText[7] = "READY?";

//LIVES
short defaultXLife = 426;
short xLife = defaultXLife;
short yLife = 306;
short spacing = 30;

//End Screen
bool skipEndTitle = false;
char endTitle[15] = "GAME OVER!";
char highScoreText[15] = "HIGHSCORE: ";
char endText[50] = "PRESS DOWN ON THE JOYSTICK TO CONTINUE";

//Win Screen
bool skipWinTitle = false;
char winTitle[15] = "YOU WIN!";
char winScore[15] = "HIGHSCORE: ";

void drawScore() {
  gfx->setTextSize(2);
  gfx->setTextColor(WHITE);
  gfx->setCursor(15, 300);
  gfx->println(scoreText);
  gfx->setCursor(90, 300);
  gfx->println(score);
}

void updateScore() {
  gfx->setTextSize(2);
  gfx->setTextColor(BLACK);
  gfx->setCursor(90, 300);
  gfx->println(score);
  score = eatenCount*10 + addScore;
  gfx->setTextColor(WHITE);
  gfx->setCursor(90, 300);
  gfx->println(score);
}

void drawLives() {
  xLife = defaultXLife;
  gfx->fillCircle(xLife, yLife, 10, YELLOW);
  xLife += spacing;
  gfx->fillCircle(xLife, yLife, 10, YELLOW);
  xLife = defaultXLife;
}

void drawTitle() {
  gfx->setTextSize(10);
  gfx->setTextColor(YELLOW);
  gfx->setCursor(35, 50);
  gfx->println(Title);
  gfx->setTextSize(2);
}

void blinkTitle() {
  long currentTitleTime = millis();
  if (currentTitleTime - startTitleTime >= titleInterval) {
    gfx->setCursor(25, 200);
    gfx->setTextColor(BLACK);
    gfx->println(titleText);
    gfx->setCursor(25, 200);
    gfx->setTextColor(WHITE);
    gfx->println(titleText);
    startTitleTime = currentTitleTime;
  }
}

void eraseTitle() {
  gfx->setTextColor(BLACK);
  gfx->setTextSize(10);
  gfx->setCursor(35, 50);
  gfx->println(Title);
  gfx->setTextSize(2);
  gfx->setCursor(25, 200);
  gfx->println(titleText);
}

void drawPlayArea(int color) {

  //outer rectangle
  gfx->drawRect(11, 6, 457, 289, color);
  gfx->drawRect(23, 18, 433, 265, color);

  //big Ls
  gfx->drawRect(47, 42, 13, 97, color);
  gfx->drawRect(47, 42, 49, 13, color);

  gfx->drawRect(47, 162, 13, 97, color);
  gfx->drawRect(47, 246, 49, 13, color);

  gfx->drawRect(419, 162, 13, 97, color);
  gfx->drawRect(383, 246, 49, 13, color);

  gfx->drawRect(419, 42, 13, 97, color);
  gfx->drawRect(383, 42, 49, 13, color);

  //small Ls
  gfx->drawRect(371, 78, 25, 13, color);
  gfx->drawRect(383, 78, 13, 25, color);

  gfx->drawRect(371, 210, 25, 13, color);
  gfx->drawRect(383, 198, 13, 25, color);

  gfx->drawRect(83, 210, 25, 13, color);
  gfx->drawRect(83, 198, 13, 25, color);

  gfx->drawRect(83, 78, 25, 13, color);
  gfx->drawRect(83, 78, 13, 25, color);

  //tetris Ls
  gfx->drawRect(347, 114, 13, 25, color);
  gfx->drawRect(347, 126, 49, 13, color);

  gfx->drawRect(347, 162, 13, 25, color);
  gfx->drawRect(347, 162, 49, 13, color);

  gfx->drawRect(119, 162, 13, 25, color);
  gfx->drawRect(83, 162, 49, 13, color);

  gfx->drawRect(119, 114, 13, 25, color);
  gfx->drawRect(83, 126, 49, 13, color);

  //edge walls
  gfx->drawRect(119, 246, 13, 37, color);
  gfx->drawRect(347, 246, 13, 37, color);
  gfx->drawRect(347, 18, 13, 37, color);
  gfx->drawRect(119, 18, 13, 37, color);

  //tall walls
  gfx->drawRect(155, 114, 13, 73, color);
  gfx->drawRect(311, 114, 13, 73, color);

  //small flat walls
  gfx->drawRect(131, 78, 61, 13, color);
  gfx->drawRect(287, 78, 61, 13, color);
  gfx->drawRect(131, 210, 61, 13, color);
  gfx->drawRect(287, 210, 61, 13, color);

  //big flat walls
  gfx->drawRect(155, 42, 169, 13, color);
  gfx->drawRect(155, 246, 169, 13, color);

  //mid box
  gfx->drawRect(203, 126, 73, 49, color);
  gfx->drawRect(191, 114, 97, 73, color);

  gfx->drawRect(215, 78, 13, 37, color);
  gfx->drawRect(251, 78, 13, 37, color);
  gfx->drawRect(215, 186, 13, 37, color);
  gfx->drawRect(251, 186, 13, 37, color);

  //gate
  gfx->drawFastHLine(275, 138, 13, color);
  gfx->drawFastHLine(275, 162, 13, color);
  gfx->drawFastVLine(275, 139, 23, BLACK);
  gfx->drawFastVLine(287, 139, 23, BLACK);
  gfx->fillRect(280, 139, 3, 23, PEACH);
  
  
  //erase extra lines
  //big Ls
  gfx->drawFastVLine(59, 43, 11, BLACK);
  gfx->drawFastHLine(48, 54, 11, BLACK);
  gfx->drawFastVLine(59, 247, 11, BLACK);
  gfx->drawFastHLine(48, 246, 11, BLACK);
  gfx->drawFastVLine(419, 43, 11, BLACK);
  gfx->drawFastHLine(420, 54, 11, BLACK);
  gfx->drawFastVLine(419, 247, 11, BLACK);
  gfx->drawFastHLine(420, 246, 11, BLACK);

  //small Ls
  gfx->drawFastVLine(95, 79, 11, BLACK);
  gfx->drawFastHLine(84, 90, 11, BLACK);
  gfx->drawFastHLine(84, 210, 11, BLACK);
  gfx->drawFastVLine(95, 211, 11, BLACK);
  gfx->drawFastVLine(383, 79, 11, BLACK);
  gfx->drawFastHLine(384, 90, 11, BLACK);
  gfx->drawFastHLine(384, 210, 11, BLACK);
  gfx->drawFastVLine(383, 211, 11, BLACK);

  //tetris Ls
  gfx->drawFastVLine(119, 127, 11, BLACK);
  gfx->drawFastHLine(120, 126, 11, BLACK);
  gfx->drawFastVLine(119, 163, 11, BLACK);
  gfx->drawFastHLine(120, 174, 11, BLACK);
  gfx->drawFastVLine(359, 127, 11, BLACK);
  gfx->drawFastHLine(348, 126, 11, BLACK);
  gfx->drawFastVLine(359, 163, 11, BLACK);
  gfx->drawFastHLine(348, 174, 11, BLACK);

  //edge walls
  gfx->drawFastHLine(120, 18, 11, BLACK);
  gfx->drawFastHLine(348, 18, 11, BLACK);
  gfx->drawFastHLine(120, 282, 11, BLACK);
  gfx->drawFastHLine(348, 282, 11, BLACK);

  //mid box
  gfx->drawFastHLine(216, 186, 11, BLACK);
  gfx->drawFastHLine(252, 186, 11, BLACK);
  gfx->drawFastHLine(216, 114, 11, BLACK);
  gfx->drawFastHLine(252, 114, 11, BLACK);

}

void drawPelletArray() {
  for (short i = 0; i < 166; i++) {
    if (!wasEaten(pgm_read_word(&pelletPixelsHorizontal[i][0]), pgm_read_word(&pelletPixelsHorizontal[i][1]))) {
      gfx->fillCircle(pgm_read_word(&pelletPixelsHorizontal[i][0]), pgm_read_word(&pelletPixelsHorizontal[i][1]), 2, PEACH);
    }
  }
  for (short i = 0; i < 146; i++) {
    if (!wasEaten(pgm_read_word(&pelletPixelsVertical[i][0]), pgm_read_word(&pelletPixelsVertical[i][1]))) {
      gfx->fillCircle(pgm_read_word(&pelletPixelsVertical[i][0]), pgm_read_word(&pelletPixelsVertical[i][1]), 2, PEACH);
    }
  }
  for (short i = 0; i < 4; i++) {
    if (!wasEaten(pgm_read_word(&pelletPixelsHorizontal[pgm_read_byte(&specialPelletIndex[i])][0]), pgm_read_word(&pelletPixelsHorizontal[pgm_read_byte(&specialPelletIndex[i])][1]))) {
      gfx->fillCircle(pgm_read_word(&pelletPixelsHorizontal[pgm_read_byte(&specialPelletIndex[i])][0]), pgm_read_word(&pelletPixelsHorizontal[pgm_read_byte(&specialPelletIndex[i])][1]), 5, PEACH);
    }
  }
}

void drawEndTitle() {
  gfx->setTextSize(7);
  gfx->setCursor(40, 50);
  gfx->setTextColor(RED);
  gfx->println(endTitle);
  gfx->setTextSize(3);
  gfx->setCursor(115, 125);
  gfx->setTextColor(WHITE);
  gfx->println(highScoreText);
  gfx->setCursor(295, 125);
  gfx->println(score);
  gfx->setTextSize(2);
}

void eraseEndTitle() {
  gfx->setTextColor(BLACK);
  gfx->setTextSize(7);
  gfx->setCursor(40, 50);
  gfx->println(endTitle);
  gfx->setTextSize(3);
  gfx->setCursor(115, 125);
  gfx->println(highScoreText);
  gfx->setCursor(295, 125);
  gfx->println(score);
  gfx->setTextSize(2);
  gfx->setCursor(15, 200);
  gfx->println(endText);
}

void blinkEnd() {
  long currentTitleTime = millis();
  if (currentTitleTime - startTitleTime >= titleInterval) {
    gfx->setCursor(15, 200);
    gfx->setTextColor(BLACK);
    gfx->println(endText);
    gfx->setCursor(15, 200);
    gfx->setTextColor(WHITE);
    gfx->println(endText);
    startTitleTime = currentTitleTime;
  }
}

void drawWinTitle() {
  gfx->setTextSize(7);
  gfx->setCursor(75, 50);
  gfx->setTextColor(YELLOW);
  gfx->println(winTitle);
  gfx->setTextSize(3);
  gfx->setCursor(115, 125);
  gfx->setTextColor(WHITE);
  gfx->println(highScoreText);
  gfx->setCursor(295, 125);
  gfx->println(score);
  gfx->setTextSize(2);
}

void eraseWinTitle() {
  gfx->setTextColor(BLACK);
  gfx->setTextSize(7);
  gfx->setCursor(75, 50);
  gfx->println(winTitle);
  gfx->setTextSize(3);
  gfx->setCursor(115, 125);
  gfx->println(highScoreText);
  gfx->setCursor(295, 125);
  gfx->println(score);
  gfx->setTextSize(2);
  gfx->setCursor(15, 200);
  gfx->println(endText);
}

void blinkWin() {
  long currentTitleTime = millis();
  if (currentTitleTime - startTitleTime >= titleInterval) {
    gfx->setCursor(15, 200);
    gfx->setTextColor(BLACK);
    gfx->println(endText);
    gfx->setCursor(15, 200);
    gfx->setTextColor(WHITE);
    gfx->println(endText);
    startTitleTime = currentTitleTime;
  }
}

class Entity {
public:
  short x, y, defaultX, defaultY, radius, speed;
  int color, defaultColor;
  char currentDirection = 'S';

  Entity(short _x, short _y, short _radius, int _color) {
    x = _x;
    y = _y;
    radius = _radius;
    color = _color;

    defaultX = x;
    defaultY = y;
    defaultColor = color;

    speed = 5;
  }

  bool movePossible(char thisDirection) {
    switch (thisDirection) {
      case 'W':
        for (short i = 0; i < 40; i++) {
          if (x == pgm_read_word(&pelletPixelsHorizontal[pgm_read_byte(&cornerPixelsIndex[pgm_read_byte(&upPossibleIndex[i])])][0]) && y == pgm_read_word(&pelletPixelsHorizontal[pgm_read_byte(&cornerPixelsIndex[pgm_read_byte(&upPossibleIndex[i])])][1])) {
            return true;
          }
        }
        break;
      case 'A':
        for (short i = 0; i < 50; i++) {
          if (x == pgm_read_word(&pelletPixelsHorizontal[pgm_read_byte(&cornerPixelsIndex[pgm_read_byte(&leftPossibleIndex[i])])][0]) && y == pgm_read_word(&pelletPixelsHorizontal[pgm_read_byte(&cornerPixelsIndex[pgm_read_byte(&leftPossibleIndex[i])])][1])) {
            return true;
          }
        }
        break;
      case 'S':
        for (short i = 0; i < 40; i++) {
          if (x == pgm_read_word(&pelletPixelsHorizontal[pgm_read_byte(&cornerPixelsIndex[pgm_read_byte(&downPossibleIndex[i])])][0]) && y == pgm_read_word(&pelletPixelsHorizontal[pgm_read_byte(&cornerPixelsIndex[pgm_read_byte(&downPossibleIndex[i])])][1])) {
            return true;
          }
        }
        break;
      case 'D':
        for (short i = 0; i < 50; i++) {
          if (x == pgm_read_word(&pelletPixelsHorizontal[pgm_read_byte(&cornerPixelsIndex[pgm_read_byte(&rightPossibleIndex[i])])][0]) && y == pgm_read_word(&pelletPixelsHorizontal[pgm_read_byte(&cornerPixelsIndex[pgm_read_byte(&rightPossibleIndex[i])])][1])) {
            return true;
          }
        }
    }
    return false;
  }

  void update() {
    switch (currentDirection) {
      case 'W':  // up
        gfx->drawArc(x, y, radius, radius, 0, 180, BLACK);
        --y;
        gfx->drawArc(x, y, radius - 1, radius - 1, 180, 360, color);
        break;
      case 'A':  // left
        gfx->drawArc(x, y, radius, radius, 270, 90, BLACK);
        --x;
        gfx->drawArc(x, y, radius - 1, radius - 1, 90, 270, color);
        break;
      case 'S':  // down
        gfx->drawArc(x, y, radius, radius, 180, 360, BLACK);
        ++y;
        gfx->drawArc(x, y, radius - 1, radius - 1, 0, 180, color);
        break;
      case 'D':  // right
        gfx->drawArc(x, y, radius, radius, 90, 270, BLACK);
        ++x;
        gfx->drawArc(x, y, radius - 1, radius - 1, 270, 90, color);
        break;
    }
  }


  void redrawPellets() {
    switch (currentDirection) { //once the 
      case 'W':
        // if (!wasEaten(x, y + 12) && !inEntity(x, y + 12) && inPelletPixelVertical(x, y + 12)) {
        //   gfx->drawPixel(x, y + 12, PEACH);
        // } else if (!wasEaten(x, y + 13) && !inEntity(x, y + 13) && inPelletPixelVertical(x, y + 13)) {
        //   gfx->fillCircle(x, y + 13, 1, PEACH);
        // } else 
        if (!wasEaten(x, y + 14) && !inEntity(x, y + 14) && inPelletPixelVertical(x, y + 14)) {
          gfx->fillCircle(x, y + 14, 2, PEACH);
        }
        if (!wasEaten(x, y + 15) && !inEntity(x, y + 15) && inSpecialPellet(x, y + 15)) {
          gfx->fillCircle(x, y + 15, 3, PEACH);
        } else if (!wasEaten(x, y + 16) && !inEntity(x, y + 16) && inSpecialPellet(x, y + 16)) {
          gfx->fillCircle(x, y + 16, 4, PEACH);
        } else if (!wasEaten(x, y + 17) && !inEntity(x, y + 17) && inSpecialPellet(x, y + 17)) {
          gfx->fillCircle(x, y + 17, 5, PEACH);
        }
        if (!wasEaten(x + 12, y + 12) && !inEntity(x + 12, y + 12) && inPelletPixelHorizontal(x + 12, y + 12)) {
          gfx->fillCircle(x + 12, y + 12, 2, PEACH);
        }
        if (!wasEaten(x - 12, y + 12) && !inEntity(x - 12, y + 12) && inPelletPixelHorizontal(x - 12, y + 12)) {
          gfx->fillCircle(x - 12, y + 12, 2, PEACH);
        }
        break;
      case 'A':
        // if (!wasEaten(x + 12, y) && !inEntity(x + 12, y) && inPelletPixelHorizontal(x + 12, y)) {
        //   gfx->drawPixel(x + 12, y, PEACH);
        // } else if (!wasEaten(x + 13, y) && !inEntity(x + 13, y) && inPelletPixelHorizontal(x + 13, y)) {
        //   gfx->fillCircle(x + 13, y, 1, PEACH);
        // } else 
        if (!wasEaten(x + 14, y) && !inEntity(x + 14, y) && inPelletPixelHorizontal(x + 14, y)) {
          gfx->fillCircle(x + 14, y, 2, PEACH);
        }
        if (!wasEaten(x + 15, y) && !inEntity(x + 15, y) && inSpecialPellet(x + 15, y)) {
          gfx->fillCircle(x + 15, y, 3, PEACH);
        } else if (!wasEaten(x + 16, y) && !inEntity(x + 16, y) && inSpecialPellet(x + 16, y)) {
          gfx->fillCircle(x + 16, y, 4, PEACH);
        } else if (!wasEaten(x + 17, y) && !inEntity(x + 17, y) && inSpecialPellet(x + 17, y)) {
          gfx->fillCircle(x + 17, y, 5, PEACH);
        }
        if (!wasEaten(x + 12, y + 12) && !inEntity(x + 12, y + 12) && inPelletPixelVertical(x + 12, y + 12)) {
          gfx->fillCircle(x + 12, y + 12, 2, PEACH);
        }
        if (!wasEaten(x + 12, y - 12) && !inEntity(x + 12, y - 12) && inPelletPixelVertical(x + 12, y - 12)) {
          gfx->fillCircle(x + 12, y - 12, 2, PEACH);
        }
        break;
      case 'S':
        // if (!wasEaten(x, y - 12) && !inEntity(x, y - 12) && inPelletPixelVertical(x, y - 12)) {
        //   gfx->drawPixel(x, y - 12, PEACH);
        // } else if (!wasEaten(x, y - 13) && !inEntity(x, y - 13) && inPelletPixelVertical(x, y - 13)) {
        //   gfx->fillCircle(x, y - 13, 1, PEACH);
        // } else 
        if (!wasEaten(x, y - 14) && !inEntity(x, y - 14) && inPelletPixelVertical(x, y - 14)) {
          gfx->fillCircle(x, y - 14, 2, PEACH);
        }
        if (!wasEaten(x, y - 15) && !inEntity(x, y - 15) && inSpecialPellet(x, y - 15)) {
          gfx->fillCircle(x, y - 15, 3, PEACH);
        } else if (!wasEaten(x, y - 16) && !inEntity(x, y - 16) && inSpecialPellet(x, y - 16)) {
          gfx->fillCircle(x, y - 16, 4, PEACH);
        } else if (!wasEaten(x, y - 17) && !inEntity(x, y - 17) && inSpecialPellet(x, y - 17)) {
          gfx->fillCircle(x, y - 17, 5, PEACH);
        }
        if (!wasEaten(x + 12, y - 12) && !inEntity(x + 12, y - 12) && inPelletPixelHorizontal(x + 12, y - 12)) {
          gfx->fillCircle(x + 12, y - 12, 2, PEACH);
        }
        if (!wasEaten(x - 12, y - 12) && !inEntity(x - 12, y - 12) && inPelletPixelHorizontal(x - 12, y - 12)) {
          gfx->fillCircle(x - 12, y - 12, 2, PEACH);
        }
        break;
      case 'D':
        // if (!wasEaten(x - 12, y) && !inEntity(x - 12, y) && inPelletPixelHorizontal(x - 12, y)) {
        //   gfx->drawPixel(x - 12, y, PEACH);
        // } else if (!wasEaten(x - 13, y) && !inEntity(x - 13, y) && inPelletPixelHorizontal(x - 13, y)) {
        //   gfx->fillCircle(x - 13, y, 1, PEACH);
        // } else 
        if (!wasEaten(x - 14, y) && !inEntity(x - 14, y) && inPelletPixelHorizontal(x - 14, y)) {
          gfx->fillCircle(x - 14, y, 2, PEACH);
        }
        if (!wasEaten(x - 15, y) && !inEntity(x - 15, y) && inSpecialPellet(x - 15, y)) {
          gfx->fillCircle(x - 15, y, 3, PEACH);
        } else if (!wasEaten(x - 16, y) && !inEntity(x - 16, y) && inSpecialPellet(x - 16, y)) {
          gfx->fillCircle(x - 16, y, 4, PEACH);
        } else if (!wasEaten(x - 17, y) && !inEntity(x - 17, y) && inSpecialPellet(x - 17, y)) {
          gfx->fillCircle(x - 17, y, 5, PEACH);
        }
        if (!wasEaten(x - 12, y + 12) && !inEntity(x - 12, y + 12) && inPelletPixelVertical(x - 12, y + 12)) {
          gfx->fillCircle(x - 12, y + 12, 2, PEACH);
        }
        if (!wasEaten(x - 12, y - 12) && !inEntity(x - 12, y - 12) && inPelletPixelVertical(x - 12, y - 12)) {
          gfx->fillCircle(x - 12, y - 12, 2, PEACH);
        }
        break;
    }
  }

  short inCorner() {
    for (short i = 0; i < 70; i++) {
      if (x == pgm_read_word(&pelletPixelsHorizontal[pgm_read_word(&cornerPixelsIndex[i])][0]) && y == pgm_read_word(&pelletPixelsHorizontal[pgm_read_word(&cornerPixelsIndex[i])][1])) {
        return i;
      }
    }
    return -1;
  }

};

class Pacman : public Entity {
public:
  char desiredDirection = 0;
  bool poweredUp = false;
  short ghostsEaten = 0;

  Pacman(short _x, short _y, short _radius)
    : Entity(_x, _y, _radius, YELLOW) {}

  void Smoother(short x_pin, short y_pin) {
    Xtotal = Xtotal - Xreadings[XreadIndex];
    Ytotal = Ytotal - Yreadings[YreadIndex];
    delay(1);
    Xreadings[XreadIndex] = analogRead(x_pin);
    Yreadings[YreadIndex] = analogRead(y_pin);
    delay(1);
    Xtotal = Xtotal + Xreadings[XreadIndex];
    Ytotal = Ytotal + Yreadings[YreadIndex];
    delay(1);
    XreadIndex = XreadIndex + 1;
    YreadIndex = YreadIndex + 1;
    if (XreadIndex >= MaxReadings) XreadIndex = 0;
    if (YreadIndex >= MaxReadings) YreadIndex = 0;
    delay(1);
    X_Pos = Xtotal / MaxReadings;
    Y_Pos = Ytotal / MaxReadings;

    if (X_Pos < 450 && X_Pos < Y_Pos) {
      joystickDirection = 65;  // A
    }
    if (X_Pos > 550 && X_Pos > Y_Pos) {
      joystickDirection = 68;  // D
    }
    if (Y_Pos < 450 && X_Pos > Y_Pos) {
      joystickDirection = 87;  // W
    }
    if (Y_Pos > 550 && X_Pos < Y_Pos) {
      joystickDirection = 83;  // S
    }
  }


  void nextMove() {
    Smoother(X, Y);  //read joystick
    short cornerIndex = inCorner();
    bool wasntEaten = !wasEaten(x, y);
    if (cornerIndex != -1) {
      if (wasntEaten && cornerIndex < 54) {
        eatenPelletsX[eatenCount] = x;
        eatenPelletsY[eatenCount] = y;
        eatenCount++;
        if (inSpecialPellet(x, y)) {
          addScore += 40;
          poweredUp = true;
          ghostsEaten = 0;
          startPowerTime = millis();
          blueGhosts();
        }
        updateScore();
      }
      if (movePossible(desiredDirection)) {
        currentDirection = desiredDirection;
        joystickDirection = desiredDirection;  //prevents being able to queue two moves
        desiredDirection = 0;                  //erases queue
        update();
      } else if (movePossible(joystickDirection)) {
        currentDirection = joystickDirection;
        update();
      } else if (movePossible(currentDirection)) {
        update();
      }
    } else {
      if (abs(currentDirection - joystickDirection) < 5) {  //check if joystick direction is on the same axis as current direction
        currentDirection = joystickDirection;
      } else {
        desiredDirection = joystickDirection;
      }
      if (wasntEaten) {
        if (currentDirection == 'A' || currentDirection == 'D') {
          if (inPelletPixelHorizontal(x, y)) {
            eatenPelletsX[eatenCount] = x;
            eatenPelletsY[eatenCount] = y;
            eatenCount++;
            updateScore();
          }
        } else {
          if (inPelletPixelVertical(x, y)) {
            eatenPelletsX[eatenCount] = x;
            eatenPelletsY[eatenCount] = y;
            eatenCount++;
            updateScore();
          }
        }
      }
      update();
    }
    redrawPellets();
  }
};

class Ghost : public Entity {
public:
  char randomDirection = 'D';
  long startTime = millis();
  long interval = 2 * 1000;
  bool inGame = false;
  bool spawn = false;
  Ghost(short _x, short _y, short _radius, int _color)
    : Entity(_x, _y, _radius, _color) {}


  void spawnInBox(short position) {
    gfx->fillCircle(x, y, radius, BLACK);
    drawPelletsInEntity();
    x = position;
    y = 150;
    color = defaultColor;
    inGame = false;
    gfx->fillCircle(x, y, radius, color);
  }

  void drawPelletsInEntity() {
    for (short i = -12; i <= 12; i++) {
      if (!wasEaten(x + i, y) && (inPelletPixelHorizontal(x + i, y) || inPelletPixelVertical(x + i, y))) {
        if (inSpecialPellet(x + i, y)) {
          gfx->fillCircle(x + i, y, 5, PEACH);
        } else {
          gfx->fillCircle(x + i, y, 2, PEACH);
        }
      }
      if (!wasEaten(x, y + i) && (inPelletPixelHorizontal(x, y + i) || inPelletPixelVertical(x, y + i))) {
        if (inSpecialPellet(x, y + i)) {
          gfx->fillCircle(x, y + i, 5, PEACH);
        } else {
          gfx->fillCircle(x, y + i, 2, PEACH);
        }
      }
      if (!wasEaten(x + 12, y + i) && (inPelletPixelHorizontal(x + 12, y + i) || inPelletPixelVertical(x + 12, y + i))) {
        if (inSpecialPellet(x + 12, y + i)) {
          gfx->fillCircle(x + 12, y + i, 5, PEACH);
        } else {
          gfx->fillCircle(x + 12, y + i, 2, PEACH);
        }
      }
      if (!wasEaten(x - 12, y + i) && (inPelletPixelHorizontal(x - 12, y + i) || inPelletPixelVertical(x - 12, y + i))) {
        if (inSpecialPellet(x - 12, y + i)) {
          gfx->fillCircle(x - 12, y + i, 5, PEACH);
        } else {
          gfx->fillCircle(x - 12, y + i, 2, PEACH);
        }
      }
      if (!wasEaten(x + i, y + 12) && (inPelletPixelHorizontal(x + i, y + 12) || inPelletPixelVertical(x + i, y + 12))) {
        if (inSpecialPellet(x + i, y + 12)) {
          gfx->fillCircle(x + i, y + 12, 5, PEACH);
        } else {
          gfx->fillCircle(x + i, y + 12, 2, PEACH);
        }
      }
      if (!wasEaten(x + i, y - 12) && (inPelletPixelHorizontal(x + i, y - 12) || inPelletPixelVertical(x + i, y - 12))) {
        if (inSpecialPellet(x + i, y - 12)) {
          gfx->fillCircle(x + i, y - 12, 5, PEACH);
        } else {
          gfx->fillCircle(x + i, y - 12, 2, PEACH);
        }
      }
    }
  }
  
  void reverseDirection() {
    switch(currentDirection) {
      case 'W':
        currentDirection = 'S';
        break;
      case 'A':
        currentDirection = 'D';
        break;
      case 'S':
        currentDirection = 'W';
        break;
      case 'D':
        currentDirection = 'A';
        break;
    }
  }

  void slideAnimation() {
    currentDirection = 'D';
    update();
  }
  

  void randomizeMove() {
    if (millis() - startTime >= interval) {
      long newMove = random(4);
      startTime = millis();
      switch (newMove) {
        case 0:
          randomDirection = 'W';
          break;
        case 1:
          randomDirection = 'A';
          break;
        case 
        2:
          randomDirection = 'S';
          break;
        case 3:
          randomDirection = 'D';
          break;
      }
    }
  }

  void nextMove() {
    randomizeMove();
    if (inCorner() != -1) {
      while (!movePossible(randomDirection)) {
        startTime = millis() - interval;
        randomizeMove();
      }
      currentDirection = randomDirection;
      update();
    } else {
      if (abs(currentDirection - randomDirection) < 5) {  //check if random direction is on the same axis as current direction
        randomDirection = currentDirection;
      }
      update();
    }
    redrawPellets();
  }

};


Pacman p1(239, 198, 11);
Ghost red(263, 150, 11, RED);
Ghost pink(239, 150, 11, PINK);
Ghost cyan(215, 150, 11, CYAN);

void resetEntities() {
  gfx->fillCircle(p1.x, p1.y, p1.radius, BLACK);
  gfx->fillCircle(red.x, red.y, red.radius, BLACK);
  gfx->fillCircle(pink.x, pink.y, pink.radius, BLACK);
  gfx->fillCircle(cyan.x, cyan.y, pink.radius, BLACK);

  p1.x = p1.defaultX;
  p1.y = p1.defaultY;
  red.x = red.defaultX;
  red.y = red.defaultY;
  red.color = red.defaultColor;
  red.inGame = false;
  red.spawn = false;
  pink.x = pink.defaultX;
  pink.y = pink.defaultY;
  pink.color = pink.defaultColor;
  pink.inGame = false;
  pink.spawn = false;
  cyan.x = cyan.defaultX;
  cyan.y = cyan.defaultY;
  cyan.color = cyan.defaultColor;
  cyan.inGame = false;
  cyan.spawn = false;
  ghostCount = 0;
  p1.poweredUp = false;
  p1.ghostsEaten = 0;
  powerWarning = false;
  startPowerTime = -1;
}

void gameOver() {
  gfx->fillScreen(BLACK);
  drawEndTitle();
}

void Win() {
  gfx->fillScreen(BLACK);
  drawWinTitle();
}

void drawEntities() {
  gfx->fillCircle(p1.x, p1.y, p1.radius, p1.color);
  gfx->fillCircle(red.x, red.y, red.radius, red.color);
  gfx->fillCircle(pink.x, pink.y, pink.radius, pink.color);
  gfx->fillCircle(cyan.x, cyan.y, pink.radius, cyan.color);
}

void resetEatenPellets() {
  for (short i = 0; i < 258; i++) {
    eatenPelletsX[i] = -1;
    eatenPelletsY[i] = -1;
  }
  eatenCount = 0;
}

bool wasEaten(short coordX, short coordY) {
  for (short i = 0; i < eatenCount; i++) {
    if (coordX == eatenPelletsX[i] && coordY == eatenPelletsY[i]) {
      return true;
    }
  }
  return false;
}

bool inPelletPixelHorizontal(short coordX, short coordY) {
  for (short i = 0; i < 166; i++) {
    if (coordX == pgm_read_word(&pelletPixelsHorizontal[i][0]) && coordY == pgm_read_word(&pelletPixelsHorizontal[i][1])) {
      return true;
    }
  }
  return false;
}

bool inPelletPixelVertical(short coordX, short coordY) {
  for (short i = 0; i < 146; i++) {
    if (coordX == pgm_read_word(&pelletPixelsVertical[i][0]) && coordY == pgm_read_word(&pelletPixelsVertical[i][1])) {
      return true;
    }
  }
  return false;
}

bool inSpecialPellet(short coordX, short coordY) {
  for (short i = 0; i < 4; i++) {
    if (coordX == pgm_read_word(&pelletPixelsHorizontal[pgm_read_byte(&specialPelletIndex[i])][0]) && coordY == pgm_read_word(&pelletPixelsHorizontal[pgm_read_byte(&specialPelletIndex[i])][1])) {
      return true;
    }
  }
  return false;
}

bool inEntity(short coordX, short coordY) {
  if (sqrt(pow(abs(p1.x - coordX), 2) + pow(abs(p1.y - coordY), 2)) <= 13) {
    return true;
  }
  if (sqrt(pow(abs(red.x - coordX), 2) + pow(abs(red.y - coordY), 2)) <= 13) {
    return true;
  }
  if (sqrt(pow(abs(pink.x - coordX), 2) + pow(abs(pink.y - coordY), 2)) <= 13) {
    return true;
  }
  if (sqrt(pow(abs(cyan.x - coordX), 2) + pow(abs(cyan.y - coordY), 2)) <= 13) {
    return true;
  }
  return false;
}


char ghostHitPacman() {
  if (sqrt(pow(abs(p1.x - red.x), 2) + pow(abs(p1.y - red.y), 2)) <= 23) { 
    return 'R';
  }
  if (sqrt(pow(abs(p1.x - pink.x), 2) + pow(abs(p1.y - pink.y), 2)) <= 23) {
    return 'P';
  }
  if (sqrt(pow(abs(p1.x - cyan.x), 2) + pow(abs(p1.y - cyan.y), 2)) <= 23) {
    return 'C';
  }
  return 0;
}

void updateLives() {
  --lives;
  delay(2000);
  resetEntities();
  drawPelletArray();
  gfx->fillCircle(xLife, yLife, 10, BLACK);
  xLife += spacing;
  if (lives <= 0) {
    gameState = endState;
  } else {
    drawEntities();
    beginAnimation();
  }
}

bool spawningCollision() {
  if ((pink.x == 299 && (pink.y > 126 && pink.y < 174))) {
    pink.reverseDirection();
    return true;
  }
  if ((red.x == 299 && (red.y > 126 && red.y < 174))) {
    red.reverseDirection();
    return true;
  } 
  if (cyan.x == 299 && (cyan.y > 126 && cyan.y < 174)) {
    cyan.reverseDirection();
    return true;
  }
  return false;
}

void ghostHitGhost() {
  if (pink.inGame && red.inGame) {
    if (abs(pink.x - red.x) <= 22 && abs(pink.y - red.y) <= 22) {
      pink.reverseDirection();
      red.reverseDirection();
    }
  }
  if (cyan.inGame && red.inGame) {
    if (abs(cyan.x - red.x) <= 22 && abs(cyan.y - red.y) <= 22) {
      cyan.reverseDirection();
      red.reverseDirection();
    }
  }
  if (cyan.inGame && pink.inGame) {
    if (abs(cyan.x - pink.x) <= 22 && abs(cyan.y - pink.y) <= 22) {
      cyan.reverseDirection();
      pink.reverseDirection();
    }
  }
}

void nextMoveIfInGame() {
  p1.nextMove();
  if (red.inGame) {
    red.nextMove();
  }
  if (pink.inGame) {
    pink.nextMove();
  }
  if (cyan.inGame) {
    cyan.nextMove();
  }
}

void blueGhosts() {
  if (red.inGame) {
    red.color = BLUE;
    gfx->fillCircle(red.x, red.y, red.radius, red.color);
  }
  if (pink.inGame) {
    pink.color = BLUE;
    gfx->fillCircle(pink.x, pink.y, pink.radius, pink.color);
  }
  if (cyan.inGame) {
    cyan.color = BLUE;
    gfx->fillCircle(cyan.x, cyan.y, cyan.radius, cyan.color);
  }
}

void checkSpawn() {
    if (millis() - startSpawnTime >= spawnInterval) {
      if (red.x == 263 && red.y == 150) {
        red.spawn = true;
        gfx->fillRect(280, 139, 3, 23, BLACK);
      }
      if (pink.x == 263 && pink.y == 150) {
        pink.spawn = true;
        gfx->fillRect(280, 139, 3, 23, BLACK);
      }
      if (cyan.x == 263 && cyan.y == 150) {
        cyan.spawn = true;
        gfx->fillRect(280, 139, 3, 23, BLACK);
      }
      startSpawnTime = millis();
    }
}

void checkPower() {
  if (!powerWarning) {
    if (millis() - startPowerTime >= (powerInterval - 2*1000)) {
      if (red.color == BLUE) {
        red.color = WHITE;
        gfx->fillCircle(red.x, red.y, red.radius, red.color);
      }
      if (pink.color == BLUE) {
        pink.color = WHITE;
        gfx->fillCircle(pink.x, pink.y, pink.radius, pink.color);
      }
      if (cyan.color == BLUE) {
        cyan.color = WHITE;
        gfx->fillCircle(cyan.x, cyan.y, cyan.radius, cyan.color);
      }
      powerWarning = true;
    }
  } else {
    if (millis() - startPowerTime >= powerInterval) {
      if (red.color == WHITE) {
        red.color = red.defaultColor;
        gfx->fillCircle(red.x, red.y, red.radius, red.color);
      }
      if (pink.color == WHITE) {
        pink.color = pink.defaultColor;
        gfx->fillCircle(pink.x, pink.y, pink.radius, pink.color);
      }
      if (cyan.color == WHITE) {
        cyan.color = cyan.defaultColor;
        gfx->fillCircle(cyan.x, cyan.y, cyan.radius, cyan.color);
      }
      p1.poweredUp = false;
      p1.ghostsEaten = 0;
      powerWarning = false;
    }
  }
}

short nextSpotInBox() {
  if (!red.spawn && !pink.spawn && !cyan.spawn) {
    if (!((red.x == 263 && red.y == 150) || (pink.x == 263 && pink.y == 150) || (cyan.x == 263 && cyan.y == 150))) {
      return 263;
    } else if (!((red.x == 239 && red.y == 150) || (pink.x == 239 && pink.y == 150) || (cyan.x == 239 && cyan.y == 150))) {
      return 239;
    } else {
      return 215;
    }
  } else {
    if (red.spawn) {
      if (red.x > 286) {
        if (!((pink.x == 263 && pink.y == 150) || (cyan.x == 263 && cyan.y == 150))) {
          return 263;
        } else {
          return 239;
        }
      } else {
        if (!((pink.x == red.x - 24 && pink.y == red.x - 24) || (cyan.x == red.x - 24 && cyan.y == red.x - 24))) {
          return red.x - 24;
        } else {
          return red.x - 48;
        }
      }
    }
    if (pink.spawn) {
      if (pink.x > 286) {
        if (!((red.x == 263 && red.y == 150) || (cyan.x == 263 && cyan.y == 150))) {
          return 263;
        } else {
          return 239;
        }
      } else {
        if (!((red.x == pink.x - 24 && red.y == pink.x - 24) || (cyan.x == pink.x - 24 && cyan.y == pink.x - 24))) {
          return pink.x - 24;
        } else {
          return pink.x - 48;
        }
      }
    }
    if (cyan.spawn) {
      if (cyan.x > 286) {
        if (!((red.x == 263 && red.y == 150) || (pink.x == 263 && pink.y == 150))) {
          return 263;
        } else {
          return 239;
        }
      } else {
        if (!((red.x == cyan.x - 24 && red.y == cyan.x - 24) || (pink.x == cyan.x - 24 && pink.y == cyan.x - 24))) {
          return cyan.x - 24;
        } else {
          return cyan.x - 48;
        }
      }
    }
  }
}

void spawnAnimation() {
  if (red.spawn) {
    if (red.x <= 276 || (red.x <= 286 && !spawningCollision())) {
      red.slideAnimation();
      if (!pink.inGame) {
        pink.slideAnimation();
      }
      if (!cyan.inGame) {
        cyan.slideAnimation();
      }
    } else if (red.x != 299 && !spawningCollision()) {
      red.slideAnimation();
    } else if (red.x == 299) {
      red.spawn = false;
      red.inGame = true;
      ghostCount += 1;
      while (red.currentDirection == 'D' || red.currentDirection == 'A') {
        red.startTime = millis() - red.interval;
        red.randomizeMove();
        red.currentDirection = red.randomDirection;
      }
      gfx->fillRect(280, 139, 3, 23, PEACH);
    }
  } else if (pink.spawn) {
    if (pink.x <= 276 || (pink.x <= 286 && !spawningCollision())) {
      pink.slideAnimation();
      if (!red.inGame) {
        red.slideAnimation();
      }
      if (!cyan.inGame) {
        cyan.slideAnimation();
      }
    } else if (pink.x != 299 && !spawningCollision()) {
      pink.slideAnimation();
    } else if (pink.x == 299) {
      pink.spawn = false;
      pink.inGame = true;
      ghostCount += 1;
      while (pink.currentDirection == 'D' || pink.currentDirection == 'A') {
        pink.startTime = millis() - pink.interval;
        pink.randomizeMove();
        pink.currentDirection = pink.randomDirection;
      }
      gfx->fillRect(280, 139, 3, 23, PEACH);
    }
  } else if (cyan.spawn) {
    if (cyan.x <= 276 || (cyan.x <= 286 && !spawningCollision())) {
      cyan.slideAnimation();
      if (!red.inGame) {
        red.slideAnimation();
      }
      if (!pink.inGame) {
        pink.slideAnimation();
      }
    } else if (cyan.x != 299 && !spawningCollision()) {
      cyan.slideAnimation();
    } else if (cyan.x == 299) {
      cyan.spawn = false;
      cyan.inGame = true;
      ghostCount += 1;
      while (cyan.currentDirection == 'D' || cyan.currentDirection == 'A') {
        cyan.startTime = millis() - cyan.interval;
        cyan.randomizeMove();
        cyan.currentDirection = cyan.randomDirection;
      }
      gfx->fillRect(280, 139, 3, 23, PEACH);
    }
  }
}

void eatGhost() {
  switch (hitGhost) {
    case 'R':
      if (red.color == BLUE || red.color == WHITE) {
        delay(1000);
        p1.ghostsEaten += 1;
        addScore += p1.ghostsEaten*200;
        red.spawnInBox(nextSpotInBox());
        gfx->drawCircle(p1.x, p1.y, p1.radius, p1.color);
        ghostCount -= 1;
      } else {
        updateLives();
      }
      break;
    case 'P':
      if (pink.color == BLUE || pink.color == WHITE) {
        delay(1000);
        p1.ghostsEaten += 1;
        addScore += p1.ghostsEaten*200;
        pink.spawnInBox(nextSpotInBox());
        gfx->drawCircle(p1.x, p1.y, p1.radius, p1.color);
        ghostCount -= 1;
      } else {
        updateLives();
      }
      break;
    case 'C':
      if (cyan.color == BLUE || cyan.color == WHITE) {
        delay(1000);
        p1.ghostsEaten += 1;
        addScore += p1.ghostsEaten*200;
        cyan.spawnInBox(nextSpotInBox());
        gfx->drawCircle(p1.x, p1.y, p1.radius, p1.color);
        ghostCount -= 1;
      } else {
        updateLives();
      }
      break;
  }
}

void beginAnimation() {
  red.spawn = true;
  gfx->setTextSize(2);
  gfx->setTextColor(YELLOW);
  gfx->setCursor(205, 59);
  gfx->println(readyText);
  delay(2000);
  gfx->fillRect(280, 139, 3, 23, BLACK);
  while (red.x != 299) {
    spawnAnimation();
    delay(16.67);
  }
  gfx->setTextColor(BLACK);
  gfx->setCursor(205, 59);
  gfx->println(readyText);
  startSpawnTime = millis();
}

void setup() {
  SPI.begin();
  gfx->begin();
  gfx->setRotation(1);
  gfx->fillScreen(BLACK);
  button.setDebounceTime(50);
  randomSeed(analogRead(4));

  // Joystick Setup
  for (short i = 0; i < MaxReadings; i++) {
    Xreadings[i] = 0;
    Yreadings[i] = 0;
  }
}

void loop() {
  //Initializes button
  button.loop();


  if (gameState == titleState) {
    //Sets up Title Screen
    if (skipTitle == false) {
      drawTitle();
      skipTitle = true;
      skipPlayArea = false;
    }

    //Checks button to start the game
    if (button.isPressed()) {
      start = true;
    }

    if (start == false) {
      //Text animation
      blinkTitle();

    } else if (start == true) {
      //sets up playState
      eraseTitle();
      resetEatenPellets();
      resetEntities();
      score = 0;
      addScore = 0;
      gameState = playState;
      lives = 3;
      start = false;
      skipTitle = false;
    }
  } else if (gameState == playState) {
    //Sets up playState
    if (skipPlayArea == false) {
      drawPlayArea(BLUE);
      drawPelletArray();
      drawEntities();
      drawScore();
      drawLives();
      beginAnimation();
      skipPlayArea = true;
    }
    if (eatenCount == 258) {
      addScore += lives*100;
      score = addScore + eatenCount*10;
      gameState = winState;
    }
    //Checks collisions with ghosts and updates entities
    hitGhost = ghostHitPacman();
    if (hitGhost == 0) { 
      nextMoveIfInGame();
      ghostHitGhost();
      checkSpawn();
      spawnAnimation();
      if (p1.poweredUp) {
        checkPower();
      }
    } else if (!p1.poweredUp) {
      updateLives();
    } else {
      eatGhost();
      updateScore();
    }
  } else if (gameState == winState) {
    //Ends game, returns to title screen
    if (skipWinTitle == false) {
      Win();
      skipWinTitle = true;
    }

    blinkWin();

    if (!(button.isPressed() || button.getState() != LOW)) {  // change when win condition is finish, only for testing
      gameState = titleState;
      skipWinTitle = false;
      eraseWinTitle();
    }

  } else if (gameState == endState) {
    //Ends game, returns to title screen
    if (skipEndTitle == false) {
      gameOver();
      skipEndTitle = true;
    }

    blinkEnd();

    if (!(button.isPressed() || button.getState() != LOW)) {
      gameState = titleState;
      skipEndTitle = false;
      eraseEndTitle();
    }
  }
  if (!(red.spawn || pink.spawn || cyan.spawn)) {
    if (ghostCount == 0 || ghostCount == 1) {
      delay(8);
    } if (ghostCount == 2) {
      delay(6);
    } else {
      delay(4);
    }
  } else {
    delay(4);
  }
}
