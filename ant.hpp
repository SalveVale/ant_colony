#pragma once

#include <SFML/Graphics.hpp>

#include "food.hpp"
#include "pharamone.hpp"

#include <math.h>
#include <random>
#include <iostream>

const float RAD = 57.29577;
const int PHARAMONE_FREQUENCY = 30;

class Ant {
public:
  Ant(int x, int y, float visionRadius) {
    this->boundingBox.setPosition(sf::Vector2f(x, y));
    
    this->visionCircle.setPosition(sf::Vector2f(x - visionRadius, y - visionRadius));
    this->visionCircle.setRadius(visionRadius);
    
    this->visionCircle.setFillColor(sf::Color(0, 0, 200, 20));
    
    this->boundingBox.setSize(sf::Vector2f(10, 2));
    this->boundingBox.setRotation(this->angle);

    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> distr(0, 360);
    this->angle = distr(eng);
  }
  ~Ant() {}

  bool hasFood = false;
  
  sf::RectangleShape getSprite() { return this->boundingBox; }
  
  sf::CircleShape getCircle() { return this->visionCircle; }
  
  sf::CircleShape getFood() { return this->food; }
  
  void update(std::vector<Food>& foods, std::vector<Pharamone>& wanderPharamones, std::vector<Pharamone>& foodPharamones) {
    switch (this->state) {
      case wandering:
        this->wander();
        this->move();
        this->spawnPharamoneWander(wanderPharamones);
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
          int checkPharamonex = foodPharamones[i].getCoordsX();
          int checkPharamoney = foodPharamones[i].getCoordsY();
          if (this->visionCircle.getGlobalBounds().contains(checkPharamonex, checkPharamoney)) {
            this->targetPharamoneIndex = i;
            this->targetx = checkPharamonex;
            this->targety = checkPharamoney;
            this->setState(seeFoodPharamone);
            break;
          }
        }
        break;
      case seeFood:
        if (this->boundingBox.getGlobalBounds().contains(this->targetx, this->targety)) {
          this->setState(returning);
        } else {
          this->move();
          this->spawnPharamoneWander(wanderPharamones);
        }
        break;
      case returning:
        this->wander();
        this->moveWithFood();
        this->spawnPharamoneFood(foodPharamones);
        for (int i=0; i<wanderPharamones.size(); i++) {
          int checkPharamonex = wanderPharamones[i].getCoordsX();
          int checkPharamoney = wanderPharamones[i].getCoordsY();
          if (this->visionCircle.getGlobalBounds().contains(checkPharamonex, checkPharamoney)) {
            this->targetPharamoneIndex = i;
            this->targetx = checkPharamonex;
            this->targety = checkPharamoney;
            this->setState(seeWanderPharamone);
            break;
          }
        }
        break;
      case seeFoodPharamone:
        if (this->boundingBox.getGlobalBounds().contains(this->targetx, this->targety)) {
          this->setState(wandering);
        } else {
          this->move();
          this->spawnPharamoneWander(wanderPharamones);
        }
        break;
      case seeWanderPharamone:
        if (this->boundingBox.getGlobalBounds().contains(this->targetx, this->targety)) {
          wanderPharamones.erase(wanderPharamones.begin() + this->targetPharamoneIndex);
          this->setState(returning);
        } else {
          this->moveWithFood();
          this->spawnPharamoneFood(foodPharamones);
        }
        break;
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
  
  void wander() {
    if ((this->turning > 0 && this->turning < 0.0125) || (this->turning < 0 && this->turning > -0.0125) || this->turning == 0) {
      std::random_device rd;
      std::default_random_engine eng(rd());
      std::uniform_int_distribution<int> distr(-3, 3);
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
    this->turning /= 1.07;
    // this->speed += this->acceleration;
    // this->acceleration /= 1.005;
  }
  
  void move() {
    float x = (cos(this->angle / RAD)) * this->speed;
    float y = (sin(this->angle / RAD)) * this->speed;
    this->boundingBox.move(sf::Vector2f(x, y));
    this->visionCircle.move(sf::Vector2f(x, y));
  }
  
  void moveWithFood() {
    float x = (cos(this->angle / RAD)) * this->speed;
    float y = (sin(this->angle / RAD)) * this->speed;
    this->boundingBox.move(sf::Vector2f(x, y));
    this->visionCircle.move(sf::Vector2f(x, y));
    this->food.move(x, y);
  }
  
  void alignAngleToTarget() {
    int currentx = this->boundingBox.getPosition().x;
    int currenty = this->boundingBox.getPosition().y;
    
    int xlen; int ylen;
    xlen = currentx - this->targetx;
    ylen = currenty - this->targety;
    
    // if (currentx < foodx) {
    // } else {
    // }
    this->angle = atan(ylen / xlen) * RAD;
    // std::cout << "current : " << posx << ", " << posy << " food : " << foodx << ", " << foody << " length : " << xlen << ", " << ylen << " angle : " << this->angle << std::endl;
    
    this->boundingBox.setRotation(this->angle);
  }
  
  void spawnPharamoneWander(std::vector<Pharamone>& wanderPharamones) {
    if (this->pharamoneTimer == 0) {
      this->pharamoneTimer = PHARAMONE_FREQUENCY;
      
      Pharamone *pharamone = new WanderPharamone(this->boundingBox.getPosition().x, this->boundingBox.getPosition().y);
      wanderPharamones.push_back(*pharamone);
    }
  }
  
  void spawnPharamoneFood(std::vector<Pharamone>& foodPharamones) {
    if (this->pharamoneTimer == 0) {
      this->pharamoneTimer = PHARAMONE_FREQUENCY;
      
      FoodPharamone *pharamone = new FoodPharamone(this->boundingBox.getPosition().x, this->boundingBox.getPosition().y);
      foodPharamones.push_back(*pharamone);
    }
  }
private:
  enum states {
    wandering,
    seeFood,
    seeWanderPharamone,
    seeFoodPharamone,
    returning,
  } state = wandering;
  
  // sf::Sprite sprite;
  sf::RectangleShape boundingBox;
  sf::CircleShape visionCircle;
  
  float angle;
  // float acceleration = 5;
  float turning = 0;
  float speed = 1.5;
  
  int pharamoneTimer = PHARAMONE_FREQUENCY;
  
  sf::CircleShape food;
  // int foodIndex;
  
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
    } else if (this->state == wandering && newState == seeFoodPharamone) {
      this->alignAngleToTarget();
      this->state = seeFoodPharamone;
    } else if (this->state == seeFoodPharamone && newState == wandering) {
      this->state = wandering;
    } else if (this->state == seeFood && newState == returning) {
      this->angle -= 180;
      this->visionCircle.setFillColor(sf::Color(0, 0, 200, 20));

      this->state = returning;
    } else if (this->state == returning && newState == seeWanderPharamone) {
      this->alignAngleToTarget();
      this->state = seeWanderPharamone;
    } else if (this->state == seeWanderPharamone && newState == returning) {
      this->state = returning;
    }
  }
};