#pragma once

#include <SFML/Graphics.hpp>

const int DESPAWN_DURATION = 2000;

class Pharamone {
public:
  // Pharamone(int x, int y) {
  //   this->circle.setPosition(x, y);
  //   this->circle.setRadius(1.f);
  //   this->circle.setFillColor(this->color);
    
  //   this->x = x;
  //   this->y = y;
  // }
  virtual ~Pharamone() {}
  
  int getCoordsX() { return this->x; }
  int getCoordsY() { return this->y; }
  
  sf::CircleShape getCircle() { return this->circle; }
  
  bool isDespawned() {
    this->timer--;
    if (this->timer == 0) {
      return true;
    } else {
      return false;
    }
  }
protected:
  sf::CircleShape circle;
  sf::Color color = sf::Color(0, 0, 200, 255);

  int x;
  int y;
private:
  int timer = DESPAWN_DURATION;
};

class WanderPharamone : public Pharamone {
public:
  WanderPharamone(int x, int y) {
    this->circle.setPosition(x ,y);
    this->circle.setRadius(1.f);
    this->circle.setFillColor(sf::Color(0, 0, 200, 255));
    
    this->x = x;
    this->y = y;
  }
  ~WanderPharamone() {}
};

class FoodPharamone : public Pharamone {
public:
  FoodPharamone(int x, int y) {
    this->circle.setPosition(x ,y);
    this->circle.setRadius(1.f);
    this->circle.setFillColor(sf::Color(200, 0, 0, 255));
    
    this->x = x;
    this->y = y;
  }
  ~FoodPharamone() {}
};
