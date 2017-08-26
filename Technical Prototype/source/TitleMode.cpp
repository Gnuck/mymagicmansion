//
//  TitleMode.cpp
//  RocketDemo
//
//  Created by Andy Jiang on 2017/04/16.
//  Copyright © 2017年 Game Design Initiative at Cornell. All rights reserved.
//

#include "TitleMode.hpp"
#include "App.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace cugl;

#pragma mark -
#pragma mark Constructors
TitleMode::TitleMode() {
}

/**
 Initializes the game mode and starts the game.
 */
void TitleMode::init() {
    titleUIController = TitleUIController();
    titleUIController.init();
    titleUIController.delegate = this;
    level = 0;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void TitleMode::dispose() {
}

void TitleMode::exit() {
    level = 0;
    titleUIController.deactivate();
}

void TitleMode::enterLevelSelect() {
    titleUIController.activateLevelSelectUI();
}

void TitleMode::enterMainMenu() {
    titleUIController.activateMainMenuUI();
}

void TitleMode::enterCredits() {
    titleUIController.activateCreditsUI();
}

void TitleMode::playButtonPressed() {
    int current_level = App::readSaveFile();
    selectLevel(current_level);
}

bool TitleMode::levelSelected() {
    return level != 0;
}

int TitleMode::getLevel() {
    return level;
}

void TitleMode::selectLevel(int level) {
    this->level = level;
}

#pragma mark -
#pragma mark Update Loop
/**
 Updates the controllers.
 */
void TitleMode::update(float dt) {
    titleUIController.update(dt);
    App::InputController.consumeIgnoredInput();
}

/**
 Draw the controllers.
 */
void TitleMode::draw(const std::shared_ptr<SpriteBatch>& _batch) {
    titleUIController.draw(_batch);
}

