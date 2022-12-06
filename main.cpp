#include "window.hpp"
#include "ant.hpp"
#include "food.hpp"
#include "pharamone.hpp"
#include "nest.hpp"

const sf::Color WALL_COLOR = sf::Color(10, 10, 10, 255);
const int RED_SPAWN_X = 200;
const int RED_SPAWN_Y = 200;
const int BLUE_SPAWN_X = 1800;
const int BLUE_SPAWN_Y = 900;

std::vector<sf::RectangleShape> initWalls(std::vector<sf::RectangleShape>& vec);

int main() {
  sf::Font font;
  font.loadFromFile("Roboto-Bold.tff");

  Window window;
  
  std::vector<sf::RectangleShape> walls;
  initWalls(walls);
  
  std::vector<Food> foods;
  // for(int i=0; i<55; i++) {
  //   for(int j=0; j<2; j++) {
  //     Food *food = new Food(200 + (8 * i), 700 + (8 * j));
  //     foods.push_back(*food);
  //   }
  // }
  // for(int i=0; i<2; i++) {
  //   for(int j=0; j<55; j++) {
  //     Food *food = new Food(200 + (8 * i), 200 + (8 * j));
  //     foods.push_back(*food);
  //   }
  // }
  for(int i=0; i<5; i++) {
    for(int j=0; j<5; j++) {
      Food *food = new Food(900 + (8 * i), 1000 + (8 * j));
      foods.push_back(*food);
    }
  }
  // for(int i=0; i<60; i++) {
  //   for(int j=0; j<2; j++) {
  //     Food *food = new Food(216 + (8 * i), 200 + (8 * j));
  //     foods.push_back(*food);
  //   }
  // }
  
  Nest *redNest = new Nest(RED_SPAWN_X, RED_SPAWN_Y, font);
  Nest *blueNest = new Nest(BLUE_SPAWN_X, BLUE_SPAWN_Y, font);
  
  std::vector<Ant> redAnts;
  for (int i=0; i<31; i++) {
    Red_Ant *ant = new Red_Ant(RED_SPAWN_X, RED_SPAWN_Y);
    redAnts.push_back(*ant);
  }
  
  std::vector<Ant> blueAnts;
  for (int i=0; i<20; i++) {
    Blue_Ant *ant = new Blue_Ant(BLUE_SPAWN_X, BLUE_SPAWN_Y);
    blueAnts.push_back(*ant);
  }
  
  std::vector<Pharamone> redWanderPharamones;
  std::vector<Pharamone> redFoodPharamones;
  std::vector<Pharamone> redDangerPharamones;
  std::vector<Pharamone> blueWanderPharamones;
  std::vector<Pharamone> blueFoodPharamones;
  std::vector<Pharamone> blueDangerPharamones;
  
  while (window.isOpen()) {
    window.update();
    
    for (int i=0; i<redAnts.size(); i++) {
      redAnts[i].update(foods, redWanderPharamones, redFoodPharamones, redDangerPharamones, walls, redNest, blueAnts);
    }
    
    for (int i=0; i<blueAnts.size(); i++) {
      blueAnts[i].update(foods, blueWanderPharamones, blueFoodPharamones, blueDangerPharamones, walls, blueNest, redAnts);
    }
    
    for (int i=0; i<redWanderPharamones.size(); i++) {
      if (redWanderPharamones[i].isDespawned()) {
        redWanderPharamones.erase(redWanderPharamones.begin() + i);
      }
    }
    
    for (int i=0; i<redFoodPharamones.size(); i++) {
      if (redFoodPharamones[i].isDespawned()) {
        redFoodPharamones.erase(redFoodPharamones.begin() + i);
      }
    }
    
    for (int i=0; i<redDangerPharamones.size(); i++) {
      if (redDangerPharamones[i].isDespawned()) {
        redDangerPharamones.erase(redDangerPharamones.begin() + i);
      }
    }
    
    for (int i=0; i<blueWanderPharamones.size(); i++) {
      if (blueWanderPharamones[i].isDespawned()) {
        blueWanderPharamones.erase(blueWanderPharamones.begin() + i);
      }
    }
    
    for (int i=0; i<blueFoodPharamones.size(); i++) {
      if (blueFoodPharamones[i].isDespawned()) {
        blueFoodPharamones.erase(blueFoodPharamones.begin() + i);
      }
    }
    
    for (int i=0; i<blueDangerPharamones.size(); i++) {
      if (blueDangerPharamones[i].isDespawned()) {
        blueDangerPharamones.erase(blueDangerPharamones.begin() + i);
      }
    }
    
    window.render(foods, redAnts, blueAnts, redWanderPharamones, redFoodPharamones, redDangerPharamones, blueWanderPharamones, blueFoodPharamones, blueDangerPharamones, walls, redNest, blueNest);
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
