#pragma once
class LaserTagBullet {
  private:
    int x;
    int xVelocity;
    int y;
    int yVelocity;
    int length;
    int angle;
    int color;
    bool released = false;
    void move(){
      if(released){
        x += xVelocity;
        y += yVelocity;
      }
    }
  public:
    LaserTagBullet(int x, int y, int xVelocity, int yVelocity, int length, int angle, int color){
      this->x = x;
      this->xVelocity = xVelocity;
      this->y = y;
      this->yVelocity = yVelocity;
      this->length = length;
      this-> angle = angle;
      this->color = color;
    }
    LaserTagBullet(){
      
    }
    void setX(int x){
      this->x = x;
    }
    void setY(int y){
      this->y = y;
    }
    void setXVelocity(int xVelocity){
      this->xVelocity = xVelocity;
    }
    void setYVelocity(int yVelocity){
      this->yVelocity = yVelocity;
    }
    void setColor(int color){
      this->color = color;
    }
    void setlength(int length){
      this->length = length;
    }
    void release(){
      this->released = true;
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
    int getLength(){
      return length;
    }
    int getAngle(){
      return angle;
    }
    bool getReleased(){
      return released;
    }
    
    void action(){
      move();
    }
};