#include "window.hpp"
#include "ant.hpp"

int main() {
  Window window;
  std::vector<Ant> ants;
  for (int i=0; i<1; i++) {
    Ant *ant = new Ant(700, 500, 39);
    ants.push_back(*ant);
  }
  
  ants[0].wander();
  
  while (window.isOpen()) {
    window.update();
    
    // for (int i=0; i<ants.size(); i++) {
    //   ants[i].wander();
    // }
    
    window.render(ants);
  }
  
  return 0;
}
