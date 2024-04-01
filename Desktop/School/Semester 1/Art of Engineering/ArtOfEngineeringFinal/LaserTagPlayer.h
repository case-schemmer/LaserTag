#pragma once
#include "LaserTagBullet.h"
#include "Vector.h"

const int storageSize = 10;
LaserTagBullet storageArray[storageSize];

class LaserTagPlayer {
  private:
    int x;
    int xVelocity;
    int y;
    int yVelocity;
    int radius;
    unsigned long timeAtBulletFire = 0;
    int color;
    Vector<LaserTagBullet> bullets;
    void move(){
      x += xVelocity;
      y += yVelocity;
      if(bullets.size() >= 1){
        for(int i = 0; i <= bullets.size() - 1; i ++){
          bullets[i].action();
        }
      }
    }
  public:
    LaserTagPlayer(int x, int y, int xVelocity, int yVelocity, int radius, int color){
      this->x = x;
      this->xVelocity = xVelocity;
      this->y = y;
      this->yVelocity = yVelocity;
      this->radius = radius;
      this->color = color;
      bullets.setStorage(storageArray);
    }
    void setX(int x){
      this->x = x;
    }
    void setY(int y){
      this->y = y;
    }
    void setColor(int color){
      this->color = color;
    }
    void setRadius(int radius){
      this->radius = radius;
    }
    void setXVelocity(int xVelocity){
      this->xVelocity = xVelocity;
    }
    void setYVelocity(int yVelocity){
      this->yVelocity = yVelocity;
    }

    int getX(){
      return x;
    }
    int getY(){
      return y;
    }
    int getColor(){
      return color;
    }
    int getRadius(){
      return radius;
    }
    int getXVelocity(){
      return xVelocity;
    }
    int getYVelocity(){
      return yVelocity;
    }
    Vector<LaserTagBullet> getBullets(){
      return bullets;
    }
    
    void createBullet(int value){
      //int randomAngle = random(0,8);
      //int randomAngle = 135;
      Serial.println("Color:");
      Serial.println(color);
      Serial.println("RA");
      Serial.println(value * 8);
      LaserTagBullet bullet = LaserTagBullet(x,y,0,0,8,value * 8,ILI9341_RED);
      bullets.push_back(bullet);
      
    }

    void action(int value){
      move();
      /*Serial.println("Micros:");
      Serial.println(micros());
      Serial.println("TABF:");
      Serial.println((timeAtBulletFire));*/
      if(((micros() - timeAtBulletFire) >= 10000000 || timeAtBulletFire == 0) && (bullets.size() == 0 || bullets[0].getReleased())){
        //Serial.println("Bullet Created");
        createBullet(value);
        timeAtBulletFire = micros();
      }
    }
};