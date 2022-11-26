#pragma once

#include <SFML/Graphics.hpp>

const float RADIUS = 50;

class Nest {
public:
  Nest(int x, int y) {
    this->circle.setPosition(x - RADIUS, y - RADIUS);
    this->circle.setRadius(RADIUS);
    this->circle.setFillColor(sf::Color(100, 100, 50, 255));
  }
  ~Nest() {}
  
  sf::CircleShape getCircle() { return this->circle; }
private:
  sf::CircleShape circle;
};
