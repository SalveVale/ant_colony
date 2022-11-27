#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>

const float RADIUS = 50;
const float NEST_VISION_RADIUS = 150;

class Nest {
public:
  Nest(int x, int y, sf::Font font) {
    // if (!this->font.loadFromFile("resources/Roboto/Roboto-Bold.tff")) std::cout << "Failed to load font" << std::endl;
    // this->font.loadFromFile("resources/Roboto/Roboto-Regular.tff");
    // this->font.loadFromFile("Roboto-Bold.tff");
    // if (!this->font.loadFromFile("Roboto-Bold.tff")) std::cout << "Failed to load font";
    this->foodCountText.setFont(font);
    this->foodCountText.setCharacterSize(12);
    this->foodCountText.setPosition(x, y);
    this->foodCountText.setString("0");
    
    this->circle.setPosition(x - RADIUS, y - RADIUS);
    this->circle.setRadius(RADIUS);
    this->circle.setFillColor(sf::Color(100, 100, 50, 255));
    
    this->visionRadius.setPosition(x - NEST_VISION_RADIUS, y - NEST_VISION_RADIUS);
    this->visionRadius.setRadius(NEST_VISION_RADIUS);
    this->visionRadius.setFillColor(sf::Color(100, 100, 50, 40));
    
    this->posx = x - RADIUS;
    this->posy = y - RADIUS;
  }
  ~Nest() {}
  
  sf::CircleShape getCircle() { return this->circle; }
  sf::CircleShape getVisionRadius() { return this->visionRadius; }
  sf::Text getText() { return this->foodCountText; }
  
  int getCoordsX() { return this->posx; }
  int getCoordsY() { return this->posy; }
  
  void addFood() {
    this->foodCount++;
    this->foodCountText.setString(std::to_string(this->foodCount));
    std::cout << this->foodCount << std::endl;
  }
private:
  sf::CircleShape circle;
  sf::CircleShape visionRadius;
  
  int posx;
  int posy;
  
  int foodCount = 0;
  
  // static sf::Font font;
  // sf::Font font;
  sf::Text foodCountText;
};
