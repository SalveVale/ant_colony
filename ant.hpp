#pragma once

#include <SFML/Graphics.hpp>

#include "food.hpp"
#include "pharamone.hpp"
#include "nest.hpp"

#include <math.h>
#include <random>
#include <iostream>

const float RAD = 57.29577;

const int VISION_RADIUS_WANDER = 30;
const int VISION_RADIUS_FOOD = 15;

//higher is less frequent
const int PHARAMONE_FREQUENCY_WANDER = 90;
const int PHARAMONE_FREQUENCY_FOOD = 30;
const int PHARAMONE_FREQUENCY_DANGER = 10;

class Ant {
public:
  // Ant(int x, int y) {
  //   this->boundingBox.setPosition(sf::Vector2f(x, y));
    
  //   this->visionCircle.setPosition(sf::Vector2f(x - VISION_RADIUS_WANDER, y - VISION_RADIUS_WANDER));
  //   this->visionCircle.setRadius(VISION_RADIUS_WANDER);
    
  //   this->visionCircle.setFillColor(sf::Color(0, 0, 200, 20));
    
  //   this->boundingBox.setSize(sf::Vector2f(10, 5));
  //   this->boundingBox.setRotation(this->angle);

  //   std::random_device rd;
  //   std::default_random_engine eng(rd());
  //   std::uniform_int_distribution<int> distr(0, 360);
  //   this->angle = distr(eng);
  // }
  // ~Ant() {}

  bool hasFood = false;
  
  sf::RectangleShape getSprite() { return this->boundingBox; }
  
  sf::CircleShape getCircle() { return this->visionCircle; }
  
  sf::CircleShape getFood() { return this->food; }
  
  void update(std::vector<Food>& foods, std::vector<Pharamone>& wanderPharamones, std::vector<Pharamone>& foodPharamones, std::vector<Pharamone>& dangerPharamones, std::vector<sf::RectangleShape> walls, Nest *nest, std::vector<Ant> enemyAnts) {
    switch (this->state) {
      case wandering:
        this->wander();
        this->move();
        this->checkCollisions(walls);
        this->spawnPharamoneWander(wanderPharamones);
        this->checkForEnemies(enemyAnts);
        for (int i=0; i<foods.size(); i++) {
          int checkFoodx = foods[i].getCoordsX();
          int checkFoody = foods[i].getCoordsY();
          if (this->visionCircle.getGlobalBounds().contains(checkFoodx, checkFoody)) {
            // this->foodIndex = i;
            foods.erase(foods.begin() + i);
            
            // this->food = foods[i].getCircle();
            this->targetx = checkFoodx;
            this->targety = checkFoody;
            this->setState(seeFood);
            break;
          }
        }
        for (int i=0; i<foodPharamones.size(); i++) {
          int checkPharamonex = foodPharamones[i].getCoordsX() - 3;
          int checkPharamoney = foodPharamones[i].getCoordsY() - 3;
          if (this->visionCircle.getGlobalBounds().contains(checkPharamonex, checkPharamoney)) {
            // this->targetPharamoneIndex = i;
            foodPharamones.erase(foodPharamones.begin() + i);
            this->targetx = checkPharamonex;
            this->targety = checkPharamoney;
            this->alignAngleToTarget();
            // this->setState(seeFoodPharamone);
            break;
          }
        }
        break;
      case seeFood:
        if (this->boundingBox.getGlobalBounds().contains(this->targetx, this->targety)) {
          this->setState(returning);
        } else {
          this->move();
          this->checkCollisions(walls);
          this->spawnPharamoneWander(wanderPharamones);
        }
        break;
      case returning:
        this->wander();
        this->moveWithFood();
        this->checkCollisions(walls);
        if (nest->getVisionRadius().getGlobalBounds().contains(this->currentx, this->currenty)) {
          this->targetx = nest->getCoordsX();
          this->targety = nest->getCoordsY();
          this->setState(seeNestReturning);
        }
        // if (nest->getCircle().getGlobalBounds().contains(this->currentx, this->currenty)) {
        //   nest->addFood();
        //   this->setState(wandering);
        // }
        this->spawnPharamoneFood(foodPharamones);
        for (int i=0; i<wanderPharamones.size(); i++) {
          int checkPharamonex = wanderPharamones[i].getCoordsX();
          int checkPharamoney = wanderPharamones[i].getCoordsY();
          if (this->visionCircle.getGlobalBounds().contains(checkPharamonex, checkPharamoney)) {
            wanderPharamones.erase(wanderPharamones.begin() + i);
            // this->targetPharamoneIndex = i;
            this->targetx = checkPharamonex;
            this->targety = checkPharamoney;
            this->alignAngleToTarget();
            // this->setState(seeWanderPharamone);
            break;
          }
        }
        break;
      case seeNestReturning:
        this->moveWithFood();
        this->spawnPharamoneFood(foodPharamones);
        if (nest->getCircle().getGlobalBounds().contains(this->currentx, this->currenty)) {
          nest->addFood();
          this->setState(wandering);
        }
        break;
      case fleeing:
        this->wander();
        this->move();
        this->checkCollisions(walls);
        if (nest->getVisionRadius().getGlobalBounds().contains(this->currentx, this->currenty)) {
          this->targetx = nest->getCoordsX();
          this->targety = nest->getCoordsY();
          this->setState(seeNestFleeing);
        }
        this->spawnPharamoneDanger(dangerPharamones);
        break;
      case seeNestFleeing:
        this->move();
        this->spawnPharamoneDanger(dangerPharamones);
        if (nest->getCircle().getGlobalBounds().contains(this->currentx, this->currenty)) {
          //spawnWarrior();
          this->setState(wandering);
        }
        break;
      // case seeFoodPharamone:
      //   if (this->boundingBox.getGlobalBounds().contains(this->targetx, this->targety)) {
      //     this->setState(wandering);
      //   } else {
      //     this->move();
      //     this->checkCollisions(walls);
      //     this->spawnPharamoneWander(wanderPharamones);
      //   }
      //   break;
      // case seeWanderPharamone:
      //   if (this->boundingBox.getGlobalBounds().contains(this->targetx, this->targety)) {
      //     wanderPharamones.erase(wanderPharamones.begin() + this->targetPharamoneIndex);
      //     this->setState(returning);
      //   } else {
      //     this->moveWithFood();
      //     this->checkCollisions(walls);
      //     this->spawnPharamoneFood(foodPharamones);
      //   }
      //   break;
    }

    this->pharamoneTimer--;
  }
  
  // void checkWanderPharamones(std::vector<Pharamone> pharamones) {
  //   for (int i=0; i<pharamones.size(); i++) {
  //     int checkPharamonex = pharamones[i].getCoordsX();
  //     int checkPharamoney = pharamones[i].getCoordsY();
  //     if (this->visionCircle.getGlobalBounds().contains(checkPharamonex, checkPharamoney)) {
  //       this->targetPharamoneIndex = i;
  //       this->targetx = checkPharamonex;
  //       this->targety = checkPharamoney;
  //       this->setState(seePharamone);
  //       break;
  //     }
  //   }
  // }
  
  void checkForEnemies(std::vector<Ant> enemyAnts) {
    for (int i=0; i<enemyAnts.size(); i++) {
      if (this->visionCircle.getGlobalBounds().contains(enemyAnts[i].getCoordsX(), enemyAnts[i].getCoordsY())) {
        this->setState(fleeing);
      }
    }
  }
  
  void wander() {
    if ((this->turning > 0 && this->turning < 0.0125) || (this->turning < 0 && this->turning > -0.0125) || this->turning == 0) {
      std::random_device rd;
      std::default_random_engine eng(rd());
      std::uniform_int_distribution<int> distr(-2, 2);
      this->turning = distr(eng);
    }
    // if (this->acceleration < 1.125) {
    //   std::random_device rd;
    //   std::default_random_engine eng(rd());
    //   std::uniform_int_distribution<int> distr(2, 3);
    //   this->acceleration += distr(eng);
    // }
    this->boundingBox.setRotation(this->angle);
    // this->move();
    this->angle += turning;
    this->turning /= 1.09;
    // this->speed += this->acceleration;
    // this->acceleration /= 1.005;
  }
  
  void move() {
    float x = (cos(this->angle / RAD)) * this->speed;
    float y = (sin(this->angle / RAD)) * this->speed;
    this->boundingBox.move(sf::Vector2f(x, y));
    this->visionCircle.move(sf::Vector2f(x, y));
    this->currentx = this->boundingBox.getPosition().x;
    this->currenty = this->boundingBox.getPosition().y;
  }
  
  void moveWithFood() {
    float x = (cos(this->angle / RAD)) * this->speed;
    float y = (sin(this->angle / RAD)) * this->speed;
    this->boundingBox.move(sf::Vector2f(x, y));
    this->visionCircle.move(sf::Vector2f(x, y));
    this->currentx = this->boundingBox.getPosition().x;
    this->currenty = this->boundingBox.getPosition().y;
    this->food.move(x, y);
  }
  
  void checkCollisions(std::vector<sf::RectangleShape> walls) {
    for (int i=0; i<walls.size(); i++) {
      if (walls[i].getGlobalBounds().contains(this->currentx, this->currenty)) {
        this->angle += 180;
        if (this->hasFood) {
          this->moveWithFood();
        } else {
          this->move();
        }
      }
    }
  }
  
  void alignAngleToTarget() {
    // float xlen = abs(this->currentx - this->targetx);
    // float ylen = abs(this->currenty - this->targety);
    
    // if (this->currentx < this->targetx) {
    //   if (this->currenty < this->targety) {
    //     // down and right
    //     this->angle = (atan(xlen / ylen) * RAD);
    //   } else {
    //     // up and right
    //     this->angle = (atan(ylen / xlen) * RAD) - 90;
    //   }
    // } else {
    //   if (this->currenty < this->targety) {
    //     // down and left
    //     this->angle = (atan(xlen / ylen) * RAD) + 90;
    //   } else {
    //     // up and left
    //     this->angle = (atan(ylen / xlen) * RAD) + 180;
    //   }
    // }
    
    // float xlen; float ylen;
    // if (this->currentx < this->targetx) {
    //   if (this->currenty < this->targety) {
    //     // down and right
    //     xlen = this->targetx - this->currentx;
    //     ylen = this->targety - this->currenty;
    //     this->angle = atan(ylen / xlen) * RAD;
    //   } else {
    //     // up and right
    //     xlen = this->currentx - this->targetx;
    //     ylen = this->currenty - this->targety;
    //     this->angle = atan(ylen / xlen) * RAD;
    //   }
    // } else {
    //   if (this->currenty < this->targety) {
    //     // down and left
    //     xlen = this->targetx - this->currentx; 
    //     ylen = this->targety - this->currenty;
    //     this->angle = atan(ylen / xlen) + 180;
    //   } else {
    //     // up and left
    //     xlen = this->currentx - this->targetx;
    //     ylen = this->currentx - this->targety;
    //     this->angle = atan(ylen / xlen) + 180;
    //   }
    // }
    
    float xlen; float ylen;
    if (this->currentx < this->targetx) {
      xlen = this->targetx - this->currentx;
      ylen = this->targety - this->currenty;
    } else {
      xlen = this->currentx - this->targetx;
      ylen = this->currenty - this->targety;
    }
    this->angle = atan(ylen / xlen) * RAD;
    
    if (this->currentx > this->targetx) {
      this->angle += 180;
    }

    // std::cout << this->angle << std::endl;
    
    this->boundingBox.setRotation(this->angle);
  }
  
  void spawnPharamoneWander(std::vector<Pharamone>& wanderPharamones) {
    if (this->pharamoneTimer == 0) {
      this->pharamoneTimer = this->pharamoneFrequency;
      
      WanderPharamone *pharamone = new WanderPharamone(this->currentx, this->currenty);
      wanderPharamones.push_back(*pharamone);
    }
  }
  
  void spawnPharamoneFood(std::vector<Pharamone>& foodPharamones) {
    if (this->pharamoneTimer == 0) {
      this->pharamoneTimer = this->pharamoneFrequency;
      
      FoodPharamone *pharamone = new FoodPharamone(this->currentx, this->currenty);
      foodPharamones.push_back(*pharamone);
    }
  }
  
  void spawnPharamoneDanger(std::vector<Pharamone>& dangerPharamones) {
    if (this->pharamoneTimer == 0) {
      this->pharamoneTimer = this->pharamoneFrequency;
      
      DangerPharamone *pharamone = new DangerPharamone(this->currentx, this->currenty);
      dangerPharamones.push_back(*pharamone);
    }
  }
  
  int getCoordsX() { return this->currentx; }
  int getCoordsY() { return this->currenty; }
protected:
  enum states {
    wandering,
    seeFood,
    // seeWanderPharamone,
    // seeFoodPharamone,
    returning,
    seeNestReturning,
    fleeing,
    seeNestFleeing,
  } state = wandering;
  
  // sf::Sprite sprite;
  sf::RectangleShape boundingBox;
  sf::CircleShape visionCircle;
  
  float angle;
  // float acceleration = 5;
  float turning = 0;
  const float speed = 2.5;
  
  int pharamoneFrequency = PHARAMONE_FREQUENCY_WANDER;
  int pharamoneTimer = PHARAMONE_FREQUENCY_WANDER;
  
  sf::CircleShape food;
  // int foodIndex;
  
  int currentx;
  int currenty;
  
  int targetx;
  int targety;
  int targetPharamoneIndex;

  void setState(states newState) {
    if (this->state == wandering && newState == seeFood) {
      this->alignAngleToTarget();
      this->visionCircle.setFillColor(sf::Color(200, 0, 0, 20));

      this->food.setRadius(3.f);
      this->food.setPosition(sf::Vector2f(this->targetx, this->targety));
      this->food.setFillColor(sf::Color(10, 200, 10, 255));
      this->hasFood = true;

      this->state = seeFood;
    // } else if (this->state == wandering && newState == seeFoodPharamone) {
    //   this->alignAngleToTarget();
    //   this->state = seeFoodPharamone;
    // } else if (this->state == seeFoodPharamone && newState == wandering) {
    //   this->state = wandering;
    } else if (this->state == seeFood && newState == returning) {
      this->pharamoneFrequency = PHARAMONE_FREQUENCY_FOOD;
      this->angle = this->angle - 180;
      this->visionCircle.setRadius(VISION_RADIUS_FOOD);
      this->visionCircle.move(VISION_RADIUS_FOOD, VISION_RADIUS_FOOD);
      this->visionCircle.setFillColor(sf::Color(0, 200, 0, 20));

      this->state = returning;
    // } else if (this->state == returning && newState == seeWanderPharamone) {
    //   this->alignAngleToTarget();
    //   this->state = seeWanderPharamone;
    // } else if (this->state == seeWanderPharamone && newState == returning) {
    //   this->state = returning;
    // } else if (this->state == returning && newState == wandering) {
    //   this->visionCircle.setRadius(VISION_RADIUS_WANDER);
    //   this->visionCircle.move(-VISION_RADIUS_FOOD, -VISION_RADIUS_FOOD);
    //   this->pharamoneFrequency = PHARAMONE_FREQUENCY_WANDER;
    //   this->food.setFillColor(sf::Color::Transparent);
    //   this->state = wandering;
    } else if (this->state == returning && newState == seeNestReturning) {
      this->alignAngleToTarget();
      this->state = seeNestReturning;
    } else if (this->state == seeNestReturning && newState == wandering) {
      this->visionCircle.setRadius(VISION_RADIUS_WANDER);
      this->visionCircle.move(-VISION_RADIUS_FOOD, -VISION_RADIUS_FOOD);
      this->pharamoneFrequency = PHARAMONE_FREQUENCY_WANDER;
      this->food.setFillColor(sf::Color::Transparent);
      this->state = wandering;
    } else if (this->state == wandering && newState == fleeing) {
      this->angle = this->angle - 180;
      this->pharamoneFrequency = PHARAMONE_FREQUENCY_DANGER;
      this->state = fleeing;
    } else if (this->state == fleeing && newState == seeNestFleeing) {
      this->alignAngleToTarget();
      this->state = seeNestFleeing;
    } else if (this->state == seeNestFleeing && newState == wandering) {
      this->pharamoneFrequency = PHARAMONE_FREQUENCY_WANDER;
      this->state = wandering;
    }
  }
};

class Red_Ant : public Ant {
public:
Red_Ant(int x, int y) {
    this->boundingBox.setPosition(sf::Vector2f(x, y));
    this->boundingBox.setFillColor(sf::Color::Red);
    
    this->visionCircle.setPosition(sf::Vector2f(x - VISION_RADIUS_WANDER, y - VISION_RADIUS_WANDER));
    this->visionCircle.setRadius(VISION_RADIUS_WANDER);
    
    this->visionCircle.setFillColor(sf::Color(0, 0, 200, 20));
    
    this->boundingBox.setSize(sf::Vector2f(10, 5));
    this->boundingBox.setRotation(this->angle);

    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> distr(0, 360);
    this->angle = distr(eng);
  }

~Red_Ant() {}
};

class Blue_Ant : public Ant {
public:
Blue_Ant(int x, int y) {
    this->boundingBox.setPosition(sf::Vector2f(x, y));
    this->boundingBox.setFillColor(sf::Color::Blue);
    
    this->visionCircle.setPosition(sf::Vector2f(x - VISION_RADIUS_WANDER, y - VISION_RADIUS_WANDER));
    this->visionCircle.setRadius(VISION_RADIUS_WANDER);
    
    this->visionCircle.setFillColor(sf::Color(0, 0, 200, 20));
    
    this->boundingBox.setSize(sf::Vector2f(10, 5));
    this->boundingBox.setRotation(this->angle);

    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> distr(0, 360);
    this->angle = distr(eng);
  }

~Blue_Ant() {}
};