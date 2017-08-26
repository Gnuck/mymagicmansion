//
//  GameMode.cpp
//  RocketDemo
//
//  Created by Jiacong Xu on 3/10/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "GameMode.hpp"

using namespace cugl;

#pragma mark -
#pragma mark Constructors
/**
 Create a new game mode. Put any non-heap setters here. Not doing anything right
 now.
 */
GameMode::GameMode() {
}

/**
 Initializes the game mode and starts the game.
 */
void GameMode::init(int level) {
    gameController = GameController();
    gameController.init(level);
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void GameMode::dispose() {
  // Dispose game controller.
    gameController.dispose();
}

bool GameMode::returnToLevelSelect() {
    return gameController.returnToLevelSelect();
}


#pragma mark -
#pragma mark Update Loop

/**
 Updates the controllers.
 */
void GameMode::update(float dt) {
  // We only need to update the game controller here.
    gameController.update(dt);
}

/**
 Draw the controllers.
 */
void GameMode::draw(const std::shared_ptr<SpriteBatch>& _batch) {
  // We only need to draw the game controller here.
    gameController.draw(_batch);
}

