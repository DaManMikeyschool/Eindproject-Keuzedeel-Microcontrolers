#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <EEPROM.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int selectPin = 2;
const int navPin = 3;
const int buzzerPin = 8;

const unsigned long EMPTY_SCORE = 9999;

unsigned long normalHighscores[3];
unsigned long countdownHighscores[3];

const int normalAddr = 0;
const int countdownAddr = normalAddr + (3 * sizeof(unsigned long));

enum ScreenState {
  SCREEN_MENU,
  SCREEN_PLAYMODE,
  SCREEN_HIGHSCORES,
  SCREEN_CONFIRM_RESET
};

ScreenState currentScreen = SCREEN_MENU;

int menuItem = 0;
int playModeItem = 0;
int confirmItem = 0;

void setup() {
  pinMode(selectPin, INPUT_PULLUP);
  pinMode(navPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);

  delay(500);

  if (!display.begin(0x3C, true)) {
    while (1) {}
  }

  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);

  randomSeed(analogRead(A0));

  loadHighscores();
  playIntro();
  drawMenu();
}

void loop() {
  if (currentScreen == SCREEN_MENU) {
    handleMenu();
  } else if (currentScreen == SCREEN_PLAYMODE) {
    handlePlayModeMenu();
  } else if (currentScreen == SCREEN_HIGHSCORES) {
    handleHighscores();
  } else if (currentScreen == SCREEN_CONFIRM_RESET) {
    handleConfirmReset();
  }
}

void playIntro() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(16, 15);
  display.println(F("REACTION"));
  display.setCursor(24, 35);
  display.println(F("TESTER"));
  display.display();

  // Give I2C bus time before tone() interrupts
  delay(200);

  beep(523, 100);
  delay(50);
  beep(659, 100);
  delay(50);
  beep(784, 100);
  delay(50);
  beep(1047, 200);

  delay(1500);
}

void beep(int freq, int duration) {
  tone(buzzerPin, freq);
  delay(duration / 2);
  noTone(buzzerPin);
  delay(duration / 2);
}

void waitForRelease(int pin) {
  delay(50);
  while (digitalRead(pin) == LOW) {
    delay(10);
  }
  delay(100);
}

void printReactionTimeFormatted(unsigned long reactionTimeMs) {
  if (reactionTimeMs >= 1000) {
    float seconds = reactionTimeMs / 1000.0;
    display.print(seconds, 2);
    display.print(F(" s"));
  } else {
    display.print(reactionTimeMs);
    display.print(F(" ms"));
  }
}

void loadArray(unsigned long scores[], int startAddr) {
  for (int i = 0; i < 3; i++) {
    EEPROM.get(startAddr + i * sizeof(unsigned long), scores[i]);
    if (scores[i] == 0 || scores[i] > EMPTY_SCORE) {
      scores[i] = EMPTY_SCORE;
    }
  }
}

void saveArray(unsigned long scores[], int startAddr) {
  for (int i = 0; i < 3; i++) {
    EEPROM.put(startAddr + i * sizeof(unsigned long), scores[i]);
  }
}

void loadHighscores() {
  loadArray(normalHighscores, normalAddr);
  loadArray(countdownHighscores, countdownAddr);
}

void saveHighscores() {
  saveArray(normalHighscores, normalAddr);
  saveArray(countdownHighscores, countdownAddr);
}

void resetHighscores() {
  for (int i = 0; i < 3; i++) {
    normalHighscores[i] = EMPTY_SCORE;
    countdownHighscores[i] = EMPTY_SCORE;
  }
  saveHighscores();
}

bool isNewHighscore(unsigned long score, unsigned long scores[]) {
  return score < scores[2];
}

// Inserts score into sorted top 3 and returns:
// 0 = not inserted, 1 = #1, 2 = #2, 3 = #3
int insertHighscoreGetPlace(unsigned long score, unsigned long scores[], int startAddr) {
  if (score >= scores[2]) return 0;

  scores[2] = score;

  for (int i = 2; i > 0; i--) {
    if (scores[i] < scores[i - 1]) {
      unsigned long temp = scores[i];
      scores[i] = scores[i - 1];
      scores[i - 1] = temp;
    }
  }

  saveArray(scores, startAddr);

  for (int i = 0; i < 3; i++) {
    if (scores[i] == score) return i + 1;
  }
  return 3;
}

void drawMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("=== MAIN MENU ==="));

  display.setCursor(0, 20);
  if (menuItem == 0) display.print(F("> "));
  else display.print(F("  "));
  display.println(F("Play"));

  display.setCursor(0, 35);
  if (menuItem == 1) display.print(F("> "));
  else display.print(F("  "));
  display.println(F("Highscores"));

  display.setCursor(0, 55);
  display.println(F("R=Nav  L=Select"));
  display.display();
}

void handleMenu() {
  if (digitalRead(navPin) == LOW) {
    menuItem++;
    if (menuItem > 1) menuItem = 0;
    beep(850, 50);
    drawMenu();
    waitForRelease(navPin);
    return;
  }

  if (digitalRead(selectPin) == LOW) {
    beep(1200, 70);
    waitForRelease(selectPin);

    if (menuItem == 0) {
      currentScreen = SCREEN_PLAYMODE;
      playModeItem = 0;
      drawPlayModeMenu();
    } else {
      currentScreen = SCREEN_HIGHSCORES;
      drawHighscores();
    }
    return;
  }
}

void drawPlayModeMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("=== PLAY MODE ==="));

  display.setCursor(0, 18);
  if (playModeItem == 0) display.print(F("> "));
  else display.print(F("  "));
  display.println(F("Normal"));

  display.setCursor(0, 32);
  if (playModeItem == 1) display.print(F("> "));
  else display.print(F("  "));
  display.println(F("Countdown"));

  display.setCursor(0, 46);
  if (playModeItem == 2) display.print(F("> "));
  else display.print(F("  "));
  display.println(F("Back"));

  display.setCursor(0, 58);
  display.println(F("R=Nav L=Select"));
  display.display();
}

void handlePlayModeMenu() {
  if (digitalRead(navPin) == LOW) {
    playModeItem++;
    if (playModeItem > 2) playModeItem = 0;
    beep(850, 50);
    drawPlayModeMenu();
    waitForRelease(navPin);
    return;
  }

  if (digitalRead(selectPin) == LOW) {
    beep(1200, 70);
    waitForRelease(selectPin);

    if (playModeItem == 0) {
      playNormalMode();
      waitForRelease(selectPin);
      currentScreen = SCREEN_MENU;
      drawMenu();
    } else if (playModeItem == 1) {
      playCountdownMode();
      waitForRelease(selectPin);
      currentScreen = SCREEN_MENU;
      drawMenu();
    } else {
      currentScreen = SCREEN_MENU;
      drawMenu();
    }
    return;
  }
}

void drawHighscores() {
  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println(F("N:"));
  if (normalHighscores[0] == EMPTY_SCORE) display.println(F("1.N/A"));
  else {
    display.print(F("1."));
    display.print(normalHighscores[0]);
    display.println(F("ms"));
  }
  if (normalHighscores[1] == EMPTY_SCORE) display.println(F("2.N/A"));
  else {
    display.print(F("2."));
    display.print(normalHighscores[1]);
    display.println(F("ms"));
  }
  if (normalHighscores[2] == EMPTY_SCORE) display.println(F("3.N/A"));
  else {
    display.print(F("3."));
    display.print(normalHighscores[2]);
    display.println(F("ms"));
  }

  display.setCursor(64, 0);
  display.println(F("C:"));
  if (countdownHighscores[0] == EMPTY_SCORE) {
    display.setCursor(64, 10);
    display.println(F("1.N/A"));
  } else {
    display.setCursor(64, 10);
    display.print(F("1."));
    display.print(countdownHighscores[0]);
    display.println(F("ms"));
  }

  if (countdownHighscores[1] == EMPTY_SCORE) {
    display.setCursor(64, 20);
    display.println(F("2.N/A"));
  } else {
    display.setCursor(64, 20);
    display.print(F("2."));
    display.print(countdownHighscores[1]);
    display.println(F("ms"));
  }

  if (countdownHighscores[2] == EMPTY_SCORE) {
    display.setCursor(64, 30);
    display.println(F("3.N/A"));
  } else {
    display.setCursor(64, 30);
    display.print(F("3."));
    display.print(countdownHighscores[2]);
    display.println(F("ms"));
  }

  display.setCursor(0, 56);
  display.println(F("R=Reset  L=Back"));
  display.display();
}

void handleHighscores() {
  if (digitalRead(navPin) == LOW) {
    beep(300, 90);
    delay(30);
    beep(220, 120);
    confirmItem = 0;
    currentScreen = SCREEN_CONFIRM_RESET;
    drawConfirmReset();
    waitForRelease(navPin);
    return;
  }

  if (digitalRead(selectPin) == LOW) {
    beep(900, 70);
    currentScreen = SCREEN_MENU;
    drawMenu();
    waitForRelease(selectPin);
    return;
  }
}

void drawConfirmReset() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F("Reset highscores?"));
  display.setCursor(0, 16);
  display.println(F("Are you sure?"));

  display.setCursor(0, 40);
  if (confirmItem == 0) display.print(F("> "));
  else display.print(F("  "));
  display.println(F("No"));

  display.setCursor(64, 40);
  if (confirmItem == 1) display.print(F("> "));
  else display.print(F("  "));
  display.println(F("Yes"));

  display.setCursor(0, 58);
  display.println(F("R=Nav L=Select"));
  display.display();
}

void handleConfirmReset() {
  if (digitalRead(navPin) == LOW) {
    confirmItem++;
    if (confirmItem > 1) confirmItem = 0;
    beep(850, 50);
    drawConfirmReset();
    waitForRelease(navPin);
    return;
  }

  if (digitalRead(selectPin) == LOW) {
    beep(1200, 70);
    waitForRelease(selectPin);

    if (confirmItem == 0) {
      currentScreen = SCREEN_HIGHSCORES;
      drawHighscores();
    } else {
      resetCountdownAnimation();
      resetHighscores();

      display.clearDisplay();
      display.setCursor(0, 20);
      display.println(F("Highscores reset!"));
      display.display();

      beep(400, 100);
      delay(40);
      beep(700, 100);
      delay(40);
      beep(1000, 160);

      delay(800);
      currentScreen = SCREEN_HIGHSCORES;
      drawHighscores();
    }
    return;
  }
}

void resetCountdownAnimation() {
  for (int i = 3; i >= 1; i--) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 10);
    display.println(F("Deleting highscores"));
    display.setTextSize(2);
    display.setCursor(54, 32);
    display.println(i);
    display.display();

    beep(300, 90);
    delay(350);
  }
}

void showFalseStart() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(8, 18);
  display.println(F("FALSE"));
  display.setCursor(20, 40);
  display.println(F("START!"));
  display.display();

  beep(180, 220);
  delay(50);
  beep(140, 350);

  delay(1400);
  currentScreen = SCREEN_MENU;
  drawMenu();
}

void showTooSlow() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(8, 18);
  display.println(F("TOO SLOW"));
  display.setTextSize(1);
  display.setCursor(18, 46);
  display.println(F("You lose!"));
  display.display();

  beep(220, 180);
  delay(60);
  beep(180, 260);

  delay(1400);
}

void celebratePlace(unsigned long reactionTime, int place) {
  for (int i = 0; i < 3; i++) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 10);
    display.print(F("Time: "));
    printReactionTimeFormatted(reactionTime);
    display.println();

    display.setTextSize(2);
    display.setCursor(0, 35);
    display.print(F("NEW #"));
    display.print(place);
    display.println(F("!"));
    display.display();

    beep(523, 70);
    delay(20);
    beep(659, 70);
    delay(20);
    beep(784, 70);
    delay(20);
    beep(1047, 130);
    delay(80);

    display.clearDisplay();
    display.display();
    delay(50);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 15);
  display.print(F("Time: "));
  printReactionTimeFormatted(reactionTime);
  display.println();

  display.setCursor(0, 35);
  display.print(F("You got place #"));
  display.print(place);
  display.display();

  delay(1200);
}

void showResult(unsigned long reactionTime, unsigned long scores[]) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 15);
  display.print(F("Time: "));
  printReactionTimeFormatted(reactionTime);
  display.println();

  display.setCursor(0, 35);
  display.print(F("Best: "));
  if (scores[0] == EMPTY_SCORE) display.println(F("N/A"));
  else {
    display.print(scores[0]);
    display.println(F(" ms"));
  }

  display.display();
  beep(250, 220);
  delay(1400);
}

void playNormalMode() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 15);
  display.println(F("Get ready..."));
  display.display();
  delay(900);

  unsigned long waitTime = random(2000, 5000);
  unsigned long startWait = millis();

  display.clearDisplay();
  display.setCursor(0, 15);
  display.println(F("Wait for signal..."));
  display.display();

  while (millis() - startWait < waitTime) {
    if (digitalRead(selectPin) == LOW) {
      showFalseStart();
      waitForRelease(selectPin);
      return;
    }
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(18, 20);
  display.println(F("PRESS!"));
  display.display();

  beep(900, 40);

  unsigned long startTime = millis();

  while (digitalRead(selectPin) == HIGH) {
    if (millis() - startTime > 10000) {
      showTooSlow();
      return;
    }
    delay(1);
  }

  unsigned long reactionTime = millis() - startTime;

  if (isNewHighscore(reactionTime, normalHighscores)) {
    int place = insertHighscoreGetPlace(reactionTime, normalHighscores, normalAddr);
    celebratePlace(reactionTime, place);
  } else {
    showResult(reactionTime, normalHighscores);
  }

  waitForRelease(selectPin);
}

void playCountdownMode() {
  delay(150);
  waitForRelease(selectPin);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 15);
  display.println(F("Countdown mode"));
  display.display();
  delay(700);

  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(54, 20);
  display.println(F("3"));
  display.display();
  beep(700, 80);

  for (int i = 0; i < 70; i++) {
    if (digitalRead(selectPin) == LOW) {
      showFalseStart();
      waitForRelease(selectPin);
      return;
    }
    delay(10);
  }

  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(54, 20);
  display.println(F("2"));
  display.display();
  beep(700, 80);

  for (int i = 0; i < 70; i++) {
    if (digitalRead(selectPin) == LOW) {
      showFalseStart();
      waitForRelease(selectPin);
      return;
    }
    delay(10);
  }

  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(54, 20);
  display.println(F("1"));
  display.display();
  beep(700, 80);

  for (int i = 0; i < 70; i++) {
    if (digitalRead(selectPin) == LOW) {
      showFalseStart();
      waitForRelease(selectPin);
      return;
    }
    delay(10);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(18, 20);
  display.println(F("PRESS!"));
  display.display();
  beep(900, 40);

  unsigned long startTime = millis();
  unsigned long timeoutStart = millis();

  while (digitalRead(selectPin) == HIGH) {
    if (millis() - timeoutStart > 10000) {
      showTooSlow();
      return;
    }
    delay(1);
  }

  unsigned long reactionTime = millis() - startTime;

  if (isNewHighscore(reactionTime, countdownHighscores)) {
    int place = insertHighscoreGetPlace(reactionTime, countdownHighscores, countdownAddr);
    celebratePlace(reactionTime, place);
  } else {
    showResult(reactionTime, countdownHighscores);
  }

  waitForRelease(selectPin);
  delay(300);
}