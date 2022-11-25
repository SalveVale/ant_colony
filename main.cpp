#include "window.hpp"
#include "ant.hpp"
#include "food.hpp"
#include "pharamone.hpp"

const float visionRadius = 30.f;

int main() {
  Window window;
  
  std::vector<Food> foods;
  for(int i=0; i<5; i++) {
    for(int j=0; j<5; j++) {
      Food *food = new Food(800 + 8 * i, 700 + 8 * j);
      foods.push_back(*food);
    }
  }
  
  std::vector<Ant> ants;
  for (int i=0; i<50; i++) {
    Ant *ant = new Ant(700, 500, visionRadius);
    ants.push_back(*ant);
  }
  
  std::vector<Pharamone> wanderPharamones;
  std::vector<Pharamone> foodPharamones;
  
  while (window.isOpen()) {
    window.update();
    
    for (int i=0; i<ants.size(); i++) {
      ants[i].update(foods, wanderPharamones, foodPharamones);
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
    
    window.render(foods, ants, wanderPharamones, foodPharamones);
  }
  
  return 0;
}
