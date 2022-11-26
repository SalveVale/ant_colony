#include "window.hpp"
#include "ant.hpp"
#include "food.hpp"
#include "pharamone.hpp"

const float VISION_RADIUS = 40.f;
const sf::Color WALL_COLOR = sf::Color(10, 10, 10, 255);

std::vector<sf::RectangleShape> initWalls(std::vector<sf::RectangleShape>& vec);

int main() {
  Window window;
  
  std::vector<sf::RectangleShape> walls;
  initWalls(walls);
  
  std::vector<Food> foods;
  for(int i=0; i<5; i++) {
    for(int j=0; j<5; j++) {
      Food *food = new Food(800 + 8 * i, 700 + 8 * j);
      foods.push_back(*food);
    }
  }
  
  std::vector<Ant> ants;
  for (int i=0; i<75; i++) {
    Ant *ant = new Ant(700, 500, VISION_RADIUS);
    ants.push_back(*ant);
  }
  
  std::vector<Pharamone> wanderPharamones;
  std::vector<Pharamone> foodPharamones;
  
  while (window.isOpen()) {
    window.update();
    
    for (int i=0; i<ants.size(); i++) {
      ants[i].update(foods, wanderPharamones, foodPharamones, walls);
    }
    
    for (int i=0; i<wanderPharamones.size(); i++) {
      if (wanderPharamones[i].isDespawned()) {
        wanderPharamones.erase(wanderPharamones.begin() + i);
      }
    }
    
    for (int i=0; i<foodPharamones.size(); i++) {
      if (foodPharamones[i].isDespawned()) {
        foodPharamones.erase(foodPharamones.begin() + i);
      }
    }
    
    window.render(foods, ants, wanderPharamones, foodPharamones, walls);
  }
  
  return 0;
}

std::vector<sf::RectangleShape> initWalls(std::vector<sf::RectangleShape>& vec) {
  sf::RectangleShape wall1;
  wall1.setPosition(0, 0);
  wall1.setSize(sf::Vector2f(20, 1080));
  wall1.setFillColor(WALL_COLOR);
  vec.push_back(wall1);

  sf::RectangleShape wall2;
  wall2.setPosition(0, 0);
  wall2.setSize(sf::Vector2f(1920, 20));
  wall2.setFillColor(WALL_COLOR);
  vec.push_back(wall2);

  sf::RectangleShape wall3;
  wall3.setPosition(1920, 1060);
  wall3.setSize(sf::Vector2f(-1920, 20));
  wall3.setFillColor(WALL_COLOR);
  vec.push_back(wall3);

  sf::RectangleShape wall4;
  wall4.setPosition(1900, 1080);
  wall4.setSize(sf::Vector2f(20, -1080));
  wall4.setFillColor(WALL_COLOR);
  vec.push_back(wall4);

  return vec;
}
