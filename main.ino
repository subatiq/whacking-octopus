#include <LiquidCrystal.h>
 
constexpr uint8_t PIN_RS = 6;
constexpr uint8_t PIN_EN = 7;
constexpr uint8_t PIN_DB4 = 8;
constexpr uint8_t PIN_DB5 = 9;
constexpr uint8_t PIN_DB6 = 10;
constexpr uint8_t PIN_DB7 = 11;

const uint8_t LEFT_BUTTON = 5;
const uint8_t RIGHT_BUTTON = 4;
 
LiquidCrystal lcd(PIN_RS, PIN_EN, PIN_DB4, PIN_DB5, PIN_DB6, PIN_DB7);
 
byte Squid[8] = {
  B00100,
  B01010,
  B10001,
  B11011,
  B10001,
  B01010,
  B01010,
  B11111,
};

byte LeftTentacle[8] = {
  B00010,
  B00101,
  B00001,
  B00010,
  B00100,
  B01000,
  B01000,
  B00111,
};

byte LeftTentaclePrep1[8] = {
  B00110,
  B00010,
  B00010,
  B00100,
  B01100,
  B01000,
  B01000,
  B00111,
};

byte LeftTentaclePrep2[8] = {
  B00000,
  B00000,
  B00110,
  B01000,
  B10000,
  B10000,
  B10000,
  B01111,
};

byte RightTentaclePrep1[8] = {
  B01100,
  B01000,
  B01000,
  B00100,
  B00110,
  B00010,
  B00010,
  B11100,
};

byte RightTentaclePrep2[8] = {
  B00000,
  B00000,
  B01100,
  B00010,
  B00001,
  B00001,
  B00001,
  B11110,
};

byte TentacleDrop[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
};

byte RightTentacle[8] = {
  B01000,
  B10100,
  B10000,
  B01000,
  B00100,
  B00010,
  B00010,
  B11100,
};

byte DeathFish01[8] = {
  B00000,
  B00100,
  B01110,
  B10101,
  B11111,
  B01010,
  B00000,
  B00000,
};

byte DeathFish02[8] = {
  B00000,
  B00000,
  B00100,
  B01110,
  B10101,
  B11111,
  B01010,
  B00000,
};

byte Heart[8] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000,
  B00000,
};


const char SQUID = 0;
const char LEFT_TENTACLE_IDLE = 1;
const char RIGHT_TENTACLE_IDLE = 2;

const char LEFT_TENTACLE_ATTACK_01 = 3;
const char LEFT_TENTACLE_ATTACK_02 = 4;
const char RIGHT_TENTACLE_ATTACK_01 = 5;
const char RIGHT_TENTACLE_ATTACK_02 = 6;

const char DEATH_FISH_FLOAT_01 = 0x84;
const char DEATH_FISH_FLOAT_02 = 0x85;
const char DEATH_FISH_FLOAT_03 = 0x29;

const char HEART = 7;
const char TENTACLE_DROP = 0x5F;

void setup() {
  pinMode(LEFT_BUTTON, INPUT);
  pinMode(RIGHT_BUTTON, INPUT);
  Serial.begin(9600);
  // Устанавливаем размер экрана
  // Количество столбцов и строк
  lcd.begin(16, 2);
  lcd.createChar(SQUID, Squid);
  lcd.createChar(LEFT_TENTACLE_IDLE, LeftTentacle);
  lcd.createChar(LEFT_TENTACLE_ATTACK_01, LeftTentaclePrep1);
  lcd.createChar(LEFT_TENTACLE_ATTACK_02, LeftTentaclePrep2);

  lcd.createChar(RIGHT_TENTACLE_ATTACK_01, RightTentaclePrep1);
  lcd.createChar(RIGHT_TENTACLE_ATTACK_02, RightTentaclePrep2);
  lcd.createChar(RIGHT_TENTACLE_IDLE, RightTentacle);

  lcd.createChar(HEART, Heart);

  resetLeftFish();
  resetRightFish();
}

void resetSquid() {
  lcd.setCursor(6, 1);
  lcd.print(LEFT_TENTACLE_IDLE);
  lcd.setCursor(7, 1);
  lcd.print(SQUID);
  lcd.setCursor(8, 1);
  lcd.print(RIGHT_TENTACLE_IDLE);
}

unsigned int livesLeft = 3;

void drawLives() {

  lcd.setCursor(14, 0);
  lcd.print(HEART);
  lcd.setCursor(15, 0);
  lcd.print(livesLeft);
}

int getRandomFishSpawnDelay() {
  rand() % 5 + 2;
}

unsigned int score;

int leftFishPos = 0;
int rightFishPos = 15;

void resetLeftFish() {
  lcd.setCursor(0, 1);
  lcd.print("      ");
  // lcd.setCursor(leftFishPos, 1);
  leftFishPos = - getRandomFishSpawnDelay();
}

void resetRightFish() {
  lcd.setCursor(9, 1);
  lcd.print("      ");
  // lcd.setCursor(rightFishPos, 1);
  rightFishPos = 15 + getRandomFishSpawnDelay();
}

void resetScore() {
  score = 0;
  lcd.setCursor(0, 0);
  lcd.print("           ");
}

void resetLives() {
  livesLeft = 3;
}

void hurt() {
  livesLeft--;
}

void moveLeftFish(int pos) {
  if (pos > 0) {
    lcd.setCursor(pos - 1, 1);
    lcd.print(' ');
  }
  if (pos >= 6) {
    hurt();
    resetLeftFish();
    return;
  }
  lcd.setCursor(pos, 1);
  lcd.print(DEATH_FISH_FLOAT_01);
}

void moveRightFish(int pos) {
  if (pos < 15) {
    lcd.setCursor(pos + 1, 1);
    lcd.print(' ');
  }
  if (pos <= 8) {
    hurt();
    resetRightFish();
    return;
  }
  lcd.setCursor(pos, 1);
  lcd.print(DEATH_FISH_FLOAT_02);
}

void hitLeft() {
  if (leftFishPos >= 5) {
    resetLeftFish();
    score++;
  }
  // lockControls();
  lcd.setCursor(6, 1);
  lcd.print(LEFT_TENTACLE_ATTACK_01); 
  delay(100);
  lcd.setCursor(6, 1);
  lcd.print(LEFT_TENTACLE_ATTACK_02); 
  delay(100);
  lcd.setCursor(5, 1);
  lcd.print(TENTACLE_DROP);
  lcd.setCursor(6, 1);
  lcd.print(TENTACLE_DROP); 
  delay(100);
  // unlockControls();
  lcd.setCursor(5, 1);
  lcd.print(" ");
  lcd.setCursor(6, 1);
  lcd.print(LEFT_TENTACLE_ATTACK_02); 
  delay(30);
  lcd.setCursor(6, 1);
  lcd.print(LEFT_TENTACLE_IDLE);
  delay(30);
}

void hitRight() {
  if (rightFishPos <= 9) {

    // delay(50);
    resetRightFish();
    score++;
  }
  lcd.setCursor(8, 1);
  lcd.print(RIGHT_TENTACLE_ATTACK_01); 
  delay(100);
  lcd.setCursor(8, 1);
  lcd.print(RIGHT_TENTACLE_ATTACK_02); 
  delay(100);
  lcd.setCursor(8, 1);
  lcd.print(TENTACLE_DROP);
  lcd.setCursor(9, 1);
  lcd.print(TENTACLE_DROP); 
  delay(100);
  lcd.setCursor(9, 1);
  lcd.print(" ");
  lcd.setCursor(8, 1);
  lcd.print(RIGHT_TENTACLE_ATTACK_02); 
  delay(30);
  lcd.setCursor(8, 1);
  lcd.print(RIGHT_TENTACLE_IDLE);
  delay(30);
}

void drawScore() {
  lcd.setCursor(0, 0);
  lcd.print(score);
}

int topScore = 0;
bool gameRunning = false;

void updateTopScore() {
  if (score > topScore) {
    topScore = score;
  }
}

void stopGame() {
  gameRunning = false;
  updateTopScore();
  resetLives();
  resetScore();
}

void startGame() {
  gameRunning = true; 
  lcd.clear();
  resetSquid();
}

void drawMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Top: ");
  lcd.print(topScore);
  lcd.setCursor(0, 1);
  lcd.print("Press to play!");
}

void loop() {
  int leftState = digitalRead(LEFT_BUTTON);
  int rightState = digitalRead(RIGHT_BUTTON);

  if (!gameRunning) {
    drawMenu();
    if (leftState == HIGH || rightState == HIGH) {
       startGame();
    }
    delay(300);
    return;
  }

  drawScore();
  drawLives();

  Serial.println(rightState);

  if (leftState == HIGH) {
    hitLeft();  
  }
  else if (rightState == HIGH) {
    hitRight();
  }
  else  {
    resetSquid();
  }

  moveLeftFish(leftFishPos);
  moveRightFish(rightFishPos);
  leftFishPos++;
  rightFishPos--;

  if (livesLeft == 0) {
    stopGame();  
  }

  delay(300);
}
