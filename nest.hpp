#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>

const float RADIUS = 50;

class Nest {
public:
  Nest(int x, int y) {
    if (!this->font.loadFromFile("resources/Roboto/Roboto-Bold.tff")) std::cout << "Failed to load font" << std::endl;
    this->foodCountText.setFont(this->font);
    this->foodCountText.setCharacterSize(12);
    this->foodCountText.setPosition(x, y);
    this->foodCountText.setString("0");
    
    this->circle.setPosition(x - RADIUS, y - RADIUS);
    this->circle.setRadius(RADIUS);
    this->circle.setFillColor(sf::Color(100, 100, 50, 255));
  }
  ~Nest() {}
  
  sf::CircleShape getCircle() { return this->circle; }
  sf::Text getText() { return this->foodCountText; }
  
  void addFood() {
    this->foodCount++;
    this->foodCountText.setString(std::to_string(this->foodCount));
  }
private:
  sf::CircleShape circle;
  
  int foodCount = 0;
  
  // static sf::Font font;
  sf::Font font;
  sf::Text foodCountText;
};
