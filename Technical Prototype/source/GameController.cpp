//
//  GameController.cpp
//  RocketDemo
//
//  Created by Jiacong Xu on 3/11/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "GameController.hpp"
#include "App.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

using namespace cugl;

/**
 The top level controller of the game. This controller initializes the GameModel, LevelController,
 GameUIController, and InputController.  It also contains the update-draw loop.
 */
void GameController::init(int level) {

  // Initializes all subcontrollers, box2d world, game model and scene graph
    gameModel = GameModel::alloc(0);
    
    this->level = level;
    
    returnLevelSelect = false;
    
    // MODELS
    
    // CONTROLLERS
    levelController = LevelController();
    levelController.init(level, gameModel);
    levelController.delegate = this;
    
    uiController = GameUIController();
    uiController.init(gameModel);
    uiController.delegate = this;

    // VIEWS
}

void GameController::dispose(){
    // levelController.dispose();
    // uiController.dispose();
    gameModel = nullptr;
}

void GameController::update(float dt) {
    uiController.update(dt);

    
    // Only update level if game is not paused.
    if (gameModel->gameState == GameModel::PLAYING) {
        

        if(levelController.speedUp){
            multSpeed=approach(3, multSpeed, 0.3);
            //multSpeed += 0.1;
        }
        else{
            multSpeed=approach(1,multSpeed,0.1);
        }
        for (int i = 1;i<=multSpeed;i++){
            levelController.update(dt);
        }
    }
    
    App::InputController.consumeIgnoredInput();
}

void GameController::draw(const std::shared_ptr<cugl::SpriteBatch>& _batch){
    levelController.draw(_batch);
    uiController.draw(_batch);
}

void GameController::pauseButtonPressed() {
    gameModel->gameState = GameModel::GameState::PAUSED;
    uiController.activatePauseUI();
}

void GameController::speedButtonPressed(){
    levelController.speedUp = !levelController.speedUp;
}

void GameController::resumeButtonPressed() {
    gameModel->gameState = GameModel::GameState::PLAYING;
    uiController.activateGameUI();
}

void GameController::restartButtonPressed() {
    // restart the level
    init(level);
}

void GameController::returnToMenuButtonPressed() {
    // Load title mode.
    returnLevelSelect = true;
}

void GameController::nextLevel() {
    level = level + 1;
    init(level);
}

void GameController::gameWon() {
    App::saveGame(level);
    gameModel->gameState = GameModel::GameState::LEVEL_COMPLETE;
    uiController.activateWinScreen();
    // Tell UI to display won screen.
}

void GameController::gameLost() {
    gameModel->gameState = GameModel::GameState::DEATH;
    uiController.activateLoseScreen();
    // Tell UI to display lost screen.
    multSpeed = 1;
    //reset to initial speed when starting level over (in case speed button was being pressed
}

float GameController::approach(float g, float c, float delta){
    float diff = g - c;
    
    if(diff > delta){
        return c+delta;
    }
    if(diff < -delta){
        return c - delta;
    }
    return g;
}
