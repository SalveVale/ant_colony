#include "window.hpp"
#include "ant.hpp"
#include "food.hpp"

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
  for (int i=0; i<100; i++) {
    Ant *ant = new Ant(700, 500, visionRadius);
    ants.push_back(*ant);
  }
  
  while (window.isOpen()) {
    window.update();
    
    for (int i=0; i<ants.size(); i++) {
      ants[i].update(foods);
    }
    
    window.render(foods, ants);
  }
  
  return 0;
}
