#include <SFML/Graphics.hpp>
#include "ant.hpp"
#include "food.hpp"
#include "nest.hpp"

#include <vector>

const int WINDOW_HEIGHT = 1080;
const int WINDOW_WIDTH = 1920;

class Window {
public:
  Window() {
    this->initVariables();
    this->initWindow();
  }
  ~Window() {
    delete this->window;
  }
  
  void update() {
    this->pollEvents();
    // this->updateMouse();
  }
  void render(std::vector<Food> foods, std::vector<Ant> redAnts, std::vector<Ant> blueAnts, std::vector<Pharamone> redWanderPharamones, std::vector<Pharamone> redFoodPharamones, std::vector<Pharamone> blueWanderPharamones, std::vector<Pharamone> blueFoodPharamones, std::vector<sf::RectangleShape> walls, Nest *redNest, Nest *blueNest) {
    this->window->clear(this->colBG);
    
    for (int i=0; i<foods.size(); i++) {
      this->window->draw(foods[i].getCircle());
    }
    
    for (int i=0; i<redAnts.size(); i++) {
      this->window->draw(redAnts[i].getSprite());
      // this->window->draw(ants[i].getCircle());
      if (redAnts[i].hasFood) {
        this->window->draw(redAnts[i].getFood());
      }
    }
    
    for (int i=0; i<blueAnts.size(); i++) {
      this->window->draw(blueAnts[i].getSprite());
      // this->window->draw(ants[i].getCircle());
      if (blueAnts[i].hasFood) {
        this->window->draw(blueAnts[i].getFood());
      }
    }
    
    for (int i=0; i<redWanderPharamones.size(); i++) {
      this->window->draw(redWanderPharamones[i].getCircle());
    }
    
    for (int i=0; i<redFoodPharamones.size(); i++) {
      this->window->draw(redFoodPharamones[i].getCircle());
    }
    
    for (int i=0; i<blueWanderPharamones.size(); i++) {
      this->window->draw(blueWanderPharamones[i].getCircle());
    }
    
    for (int i=0; i<blueFoodPharamones.size(); i++) {
      this->window->draw(blueFoodPharamones[i].getCircle());
    }
    
    for (int i=0; i<walls.size(); i++) {
      this->window->draw(walls[i]);
    }
    
    // this->window->draw(nest->getVisionRadius());
    this->window->draw(redNest->getCircle());
    this->window->draw(redNest->getText());
    
    // this->window->draw(blueNest->getVisionRadius());
    this->window->draw(blueNest->getCircle());
    this->window->draw(blueNest->getText());
    
    this->window->display();
  }
  
  const bool isOpen() const {
    return this->window->isOpen();
  }
private:
  sf::VideoMode videoMode;
  sf::RenderWindow *window;
  sf::Event event;
  
  // sf::Vector2i mousePosView;
  // sf::Vector2f mousePosWindow;
  
  sf::Color colBG = sf::Color(26, 26, 26, 255);
  
  void initVariables() {
    this->window = nullptr;
  }
  
  void initWindow() {
    this->videoMode.height = WINDOW_HEIGHT;
    this->videoMode.width = WINDOW_WIDTH;
    
    this->window = new sf::RenderWindow(this->videoMode, "Ant Colony Simulation");
    this->window->setFramerateLimit(120);
  }
  
  void pollEvents() {
    while (this->window->pollEvent(this->event)) {
      switch (this->event.type) {
        case sf::Event::Closed:
          this->window->close();
          break;
        case sf::Event::KeyPressed:
          if (this->event.key.code == sf::Keyboard::Escape) {
            this->window->close();
          }
          break;
        default:
          break;
      }
    }
  }
  
  // void updateMouse() {
  //   this->mousePosView = sf::Mouse::getPosition(*this->window);
  //   this->mousePosWindow = this->window->mapPixelToCoords(this->mousePosView);
  // }
};
