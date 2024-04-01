#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Vector.h"
#include "LaserTagBullet.h"
#include "LaserTagPlayer.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10


//const int playerTwoPins

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
LaserTagPlayer playerOne = LaserTagPlayer(75, 15, 0, 0, 5, ILI9341_BLACK);
LaserTagPlayer playerTwo = LaserTagPlayer(220, 320, 0, 0, 5, ILI9341_BLUE);

int delayTime = 20;
const int playerOnePins[4] = { 2, 3, 4, 5 };
const int playerTwoPins[4] = { 6, 7, 8, 12 };
int movementVelocity = 1;
double conversionFactor = 0.01745329252;
int result = 0;
unsigned long startTime = micros();
int randomStart = random(0,2);

int screenWidth = 240;
int screenHeight = 320;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  for (int i = 0; i <= sizeof(playerOnePins) / sizeof(int) - 1; i++) {
    pinMode(playerOnePins[i], INPUT_PULLUP);
  }
  for (int i = 0; i <= sizeof(playerTwoPins) / sizeof(int) - 1; i++) {
    pinMode(playerTwoPins[i], INPUT_PULLUP);
  }
  tft.begin();
  tft.fillScreen(ILI9341_WHITE);
}
void loop() {
  action();
  paintObjects();
}

void action() {
  //Serial.println("Action");
  playerOne.action(1);

  if(playerOne.getX() - playerOne.getRadius() <= 0){
    playerOne.setX(playerOne.getRadius());
  }
  else if(playerOne.getX() + playerOne.getRadius() >= screenWidth){
    playerOne.setX(screenWidth - playerOne.getRadius());
  }
  if(playerOne.getY() - playerOne.getRadius() <= 0){
    playerOne.setY(playerOne.getRadius());
  }
  else if(playerOne.getY() + playerOne.getRadius() >= screenHeight){
    playerOne.setY(screenHeight - playerOne.getRadius());
  }
  //Serial.println(playerOne.getBullets().size());
  /*for(int i = 0; i <= playerOne.getBullets().size() - 1; i ++){
    //Serial.println("In Loop:");
    //Serial.println(i);
    if(playerOne.getBullets()[i].getReleased() == false){
      //Serial.println("Unreleased, Setting");
      playerOne.getBullets()[i].setX(playerOne.getX());
      playerOne.getBullets()[i].setY(playerOne.getY());
    }
    else{
      //playerOne.getBullets()[i].action();
    }
  }*/

  for (int i = 0; i <= sizeof(playerOnePins) / sizeof(int) - 1; i++) {
    int onVelocities[2][4] = { { movementVelocity, playerOne.getXVelocity(), -movementVelocity, playerOne.getXVelocity()}, { playerOne.getYVelocity(), movementVelocity, playerOne.getYVelocity(), -movementVelocity } };
    int offVelocities[2][4] = { { 0, playerOne.getXVelocity(), 0, playerOne.getXVelocity()}, { playerOne.getYVelocity(), 0, playerOne.getYVelocity(), 0 } };
    if(digitalRead(playerOnePins[0]) + digitalRead(playerOnePins[1]) + digitalRead(playerOnePins[2]) + digitalRead(playerOnePins[3]) == 0){
      playerOne.getBullets()[0].release();
      playerOne.getBullets()[0].setXVelocity(1);
      playerOne.getBullets()[0].setYVelocity(1);
    }
    else if (digitalRead(playerOnePins[i]) == LOW) {
      playerOne.setXVelocity(onVelocities[0][i]);
      playerOne.setYVelocity(onVelocities[1][i]);
    }
    else if(digitalRead(playerOnePins[i]) == HIGH && digitalRead(playerOnePins[(i+2)%4]) == HIGH){
      playerOne.setXVelocity(offVelocities[0][i]);
      playerOne.setYVelocity(offVelocities[1][i]);
    }
  }

  playerTwo.action(3);

  if(playerTwo.getX() - playerTwo.getRadius() <= 0){
    playerTwo.setX(playerTwo.getRadius());
  }
  else if(playerTwo.getX() + playerTwo.getRadius() >= screenWidth){
    playerTwo.setX(screenWidth - playerTwo.getRadius());
  }
  if(playerTwo.getY() - playerTwo.getRadius() <= 0){
    playerTwo.setY(playerTwo.getRadius());
  }
  else if(playerTwo.getY() + playerTwo.getRadius() >= screenHeight){
    playerTwo.setY(screenHeight - playerTwo.getRadius());
  }


  for (int i = 0; i <= sizeof(playerTwoPins) / sizeof(int) - 1; i++) {
    int onVelocities[2][4] = { { -movementVelocity, playerTwo.getXVelocity(), movementVelocity, playerTwo.getXVelocity()}, { playerTwo.getYVelocity(), -movementVelocity, playerTwo.getYVelocity(), movementVelocity } };
    int offVelocities[2][4] = { { 0, playerTwo.getXVelocity(), 0, playerTwo.getXVelocity()}, { playerTwo.getYVelocity(), 0, playerTwo.getYVelocity(), 0 } };
    if (digitalRead(playerTwoPins[i]) == LOW) {
      playerTwo.setXVelocity(onVelocities[0][i]);
      playerTwo.setYVelocity(onVelocities[1][i]);
    }
    else if(digitalRead(playerTwoPins[i]) == HIGH && digitalRead(playerTwoPins[(i+2)%4]) == HIGH){
      playerTwo.setXVelocity(offVelocities[0][i]);
      playerTwo.setYVelocity(offVelocities[1][i]);
    }
  }
  if(pow(playerOne.getX() - playerTwo.getX(),2) + pow(playerOne.getY() - playerTwo.getY(),2) <= pow(playerOne.getRadius() + playerTwo.getRadius(),2)){
    Serial.println("Collision");
    if(playerOne.getColor() == ILI9341_BLACK){
      result = 1;
    }
    else{
      result = 2;
    }
  }

  if((micros() - startTime)/10000000 % 2 == 0){
    playerOne.setColor(ILI9341_BLACK);
    playerTwo.setColor(ILI9341_BLUE);
  }
  else{
    playerTwo.setColor(ILI9341_BLACK);
    playerOne.setColor(ILI9341_BLUE);
  }
}
void paintObjects() {
  if(result == 0){
    //Serial.println("Paint");
    tft.fillCircle(playerOne.getX(), playerOne.getY(), playerOne.getRadius(), playerOne.getColor());
    tft.fillCircle(playerTwo.getX(), playerTwo.getY(), playerTwo.getRadius(),playerTwo.getColor());

    /*Vector<LaserTagBullet> playerOneBullets = playerOne.getBullets();
    if (playerOneBullets.size() >= 1) {
      for (int i = 0; i <= playerOneBullets.size() - 1; i++) {
        LaserTagBullet currentBulletPlayerOne = playerOneBullets[i];
        Serial.println("P1 Angle:");
        Serial.println(currentBulletPlayerOne.getAngle());
        Serial.println("P1 Altered X:");
        Serial.println(currentBulletPlayerOne.getLength() * cos((currentBulletPlayerOne.getAngle() * 1.0) * conversionFactor));
        Serial.println("P1 Altered Y:");
        Serial.println(currentBulletPlayerOne.getLength() * sin((currentBulletPlayerOne.getAngle() * 1.0) * conversionFactor));
        tft.drawLine(currentBulletPlayerOne.getX(),currentBulletPlayerOne.getY(),currentBulletPlayerOne.getX() + (currentBulletPlayerOne.getLength() * cos((currentBulletPlayerOne.getAngle() * 1.0) * conversionFactor)),currentBulletPlayerOne.getY() + (currentBulletPlayerOne.getLength() * sin((currentBulletPlayerOne.getAngle() * 1.0) * conversionFactor)),currentBulletPlayerOne.getColor());
        }
    }
  Vector<LaserTagBullet> playerTwoBullets = playerTwo.getBullets();
    if (playerTwoBullets.size() >= 1) {
      for (int i = 0; i <= playerTwoBullets.size() - 1; i++) {
        LaserTagBullet currentBullet = playerTwoBullets[i];
        Serial.println("P2 Angle:");
        Serial.println(currentBullet.getAngle());
        Serial.println("P2 Altered X:");
        Serial.println(currentBullet.getLength() * cos((currentBullet.getAngle() * 1.0) * conversionFactor));
        Serial.println("P2 Altered Y:");
        Serial.println(currentBullet.getLength() * sin((currentBullet.getAngle() * 1.0) * conversionFactor));
        tft.drawLine(currentBullet.getX(),currentBullet.getY(),currentBullet.getX() + (int)(currentBullet.getLength() * cos((currentBullet.getAngle() * 1.0) * conversionFactor)),currentBullet.getY() + (int)(currentBullet.getLength() * sin((currentBullet.getAngle() * 1.0) * conversionFactor)),currentBullet.getColor());
        }
    }*/

    delay(delayTime);

    /*if (playerOneBullets.size() >= 1) {
      for (int i = 0; i <= playerOneBullets.size() - 1; i++) {
        LaserTagBullet currentBulletPlayerOne = playerOneBullets[i];
        tft.drawLine(currentBulletPlayerOne.getX(),currentBulletPlayerOne.getY(),currentBulletPlayerOne.getX() + (currentBulletPlayerOne.getLength() * cos((currentBulletPlayerOne.getAngle() * 1.0) * conversionFactor)),currentBulletPlayerOne.getY() + (currentBulletPlayerOne.getLength() * sin((currentBulletPlayerOne.getAngle() * 1.0) * conversionFactor)),ILI9341_WHITE);
        }
    }

    if (playerTwoBullets.size() >= 1) {
      for (int i = 0; i <= playerTwoBullets.size() - 1; i++) {
        LaserTagBullet currentBullet = playerTwoBullets[i];
        tft.drawLine(currentBullet.getX(),currentBullet.getY(),currentBullet.getX() + (int)(currentBullet.getLength() * cos((currentBullet.getAngle() * 1.0) * conversionFactor)),currentBullet.getY() + (int)(currentBullet.getLength() * sin((currentBullet.getAngle() * 1.0) * conversionFactor)),ILI9341_WHITE);
        }
    }*/

    tft.fillCircle(playerOne.getX(), playerOne.getY(), playerOne.getRadius(), ILI9341_WHITE);
    tft.fillCircle(playerTwo.getX(),playerTwo.getY(),playerTwo.getRadius(),ILI9341_WHITE);
  }
  else{
    tft.setCursor(50,50);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.println("Player " + String(result) + " wins!");\
    delay(10000);
    tft.setCursor(50,50);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.println("Player " + String(result) + " wins!");\
    playerOne = LaserTagPlayer(75, 15, 0, 0, 5, ILI9341_BLACK);
    playerTwo = LaserTagPlayer(220, 320, 0, 0, 5, ILI9341_BLUE);
    result = 0;
    startTime = micros();
  }
    
  
}