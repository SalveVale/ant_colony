#pragma once

#include <SFML/Graphics.hpp>

class Food {
public:
  Food(int x, int y) {
    this->circle.setPosition(sf::Vector2f(x, y));
    this->x = x;
    this->y = y;
    this->circle.setRadius(3);
    this->circle.setFillColor(sf::Color(10, 200, 10, 255));
  }
  ~Food() {}
  
  sf::CircleShape getCircle() { return this->circle; }
  
  int getCoordsX() { return this->x; }
  
  int getCoordsY() { return this->y; }
private:
  sf::CircleShape circle;
  int x;
  int y;
};
