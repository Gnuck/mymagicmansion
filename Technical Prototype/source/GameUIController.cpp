//
//  GameUIController.cpp
//  RocketDemo
//
//  Created by Jiacong Xu on 3/11/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "GameUIController.hpp"
#include "App.h"
#include "InputController.hpp"

using namespace cugl;

bool GameUIController::init(std::shared_ptr<GameModel> gameModel) {
    this->gameModel = gameModel;
    speeding = false;
    Size dimen = Application::get()->getDisplaySize();
    dimen *= GAME_WIDTH/dimen.width;
    uiScene = Scene::alloc(dimen);
    Size mockupSize(MOCKUP_WIDTH, MOCKUP_HEIGHT);
    float scale = GAME_WIDTH/mockupSize.width;
    
    Vec2 center(dimen.width/2.0f, dimen.height/2.0f);

    // Add all UI elements to the scene.
    gameNode = Node::alloc();
    pauseNode = Node::alloc();
    winNode = Node::alloc();
    loseNode = Node::alloc();
    
    gameNode->setContentSize(mockupSize);
    pauseNode->setContentSize(mockupSize);
    winNode->setContentSize(mockupSize);
    loseNode->setContentSize(mockupSize);
    
    gameNode->setAnchor(Vec2::ANCHOR_MIDDLE);
    pauseNode->setAnchor(Vec2::ANCHOR_MIDDLE);
    winNode->setAnchor(Vec2::ANCHOR_MIDDLE);
    loseNode->setAnchor(Vec2::ANCHOR_MIDDLE);
    
    gameNode->setPosition(center);
    pauseNode->setPosition(center);
    winNode->setPosition(center);
    loseNode->setPosition(center);
    
    gameNode->setScale(scale);
    pauseNode->setScale(scale);
    winNode->setScale(scale);
    loseNode->setScale(scale);
    
    uiScene->addChildWithName(gameNode, "Game");
    uiScene->addChildWithName(pauseNode, "Pause");
    uiScene->addChildWithName(winNode, "Win");
    uiScene->addChildWithName(loseNode, "Lose");
    
    
    gameNode->setVisible(true);
    pauseNode->setVisible(false);
    winNode->setVisible(false);
    loseNode->setVisible(false);
    
    center.set(MOCKUP_WIDTH/2.0f, MOCKUP_HEIGHT/2.0f);
    
    std::shared_ptr<Node> pauseButton = Node::alloc();
    float pauseButtonSize = 140;
    pauseButton->setContentSize(pauseButtonSize, pauseButtonSize);
    pauseButton->setAnchor(Vec2::ANCHOR_TOP_RIGHT);
    pauseButton->setPosition(MOCKUP_WIDTH, MOCKUP_HEIGHT);
    pauseButton->setTag(PAUSE_BUTTON_TAG);
    gameNode->addChild(pauseButton);
    
    auto pauseButtonImage = PolygonNode::allocWithTexture(App::AssetManager->get<Texture>("pause"));
    pauseButtonImage->setAnchor(Vec2::ANCHOR_MIDDLE);
    pauseButtonImage->setPosition(pauseButtonSize/2, pauseButtonSize/2);
    pauseButton->addChild(pauseButtonImage);
    
    auto pauseMask = PolygonNode::alloc(Rect(0, 0, MOCKUP_WIDTH, MOCKUP_HEIGHT));
    pauseMask->setColor(Color4(0, 0, 0, 150));
    pauseMask->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    pauseMask->setPosition(0, 0);
    pauseNode->addChild(pauseMask, 0);
    
    std::shared_ptr<Texture> pausePanelTexture = App::AssetManager->get<Texture>("pause-panel");
    auto pausePanel = PolygonNode::allocWithTexture(pausePanelTexture);
    pausePanel->setAnchor(Vec2::ANCHOR_MIDDLE);
    pausePanel->setPosition(center);
    pausePanel->setScale(2.0f);
    pauseNode->addChild(pausePanel, 1);
    
    //speedup button
    std::shared_ptr<Node> speedButton = Node::alloc();
    float speedButtonSize = 180;
    speedButton->setContentSize(speedButtonSize, speedButtonSize);
    speedButton->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    speedButton->setPosition(-15, -15);
    speedButton->setTag(SPEED_BUTTON_TAG);
    gameNode->addChildWithName(speedButton, "SpeedButton");
    
    auto speedButtonImage = PolygonNode::allocWithTexture(App::AssetManager->get<Texture>("fast-forward"));
    speedButtonImage->setAnchor(Vec2::ANCHOR_MIDDLE);
    speedButtonImage->setPosition(speedButtonSize/2, speedButtonSize/2);
    speedButton->addChild(speedButtonImage);
    
    /** COLLECTIBLES */
    for (int i = 0; i < gameModel->collectibles.size(); i++) {
        auto key = gameModel->collectibles[i];
        auto keyNode = PolygonNode::allocWithTexture(App::AssetManager->get<Texture>(key->textureName));
        keyNode->setContentSize(50, 50);
        keyNode->setAnchor(Vec2::ANCHOR_TOP_LEFT);
        keyNode->setPosition(20, 900 - i*100);
        keyNode->setColor(Color4(255, 255, 255, 100));
        gameNode->addChildWithName(keyNode, key->textureName);
    }
    
    // restart button
    std::shared_ptr<Texture> restartButtonTexture = App::AssetManager->get<Texture>("restart-button");
    auto restartButton = PolygonNode::allocWithTexture(restartButtonTexture);
    restartButton->setTag(RESTART_BUTTON_TAG);
    restartButton->setAnchor(Vec2::ANCHOR_MIDDLE);
    restartButton->setScale(1.5f);
    restartButton->setPosition(center);
    pauseNode->addChild(restartButton, 2);
    
    // resume button
    std::shared_ptr<Texture> resumeButtonTexture = App::AssetManager->get<Texture>("resume-button");
    auto resumeButton = PolygonNode::allocWithTexture(resumeButtonTexture);
    resumeButton->setTag(RESUME_BUTTON_TAG);
    resumeButton->setAnchor(Vec2::ANCHOR_MIDDLE);
    resumeButton->setScale(1.5f);
    resumeButton->setPosition(center.x - 300, center.y);
    pauseNode->addChild(resumeButton, 2);
    
    // quit button
    std::shared_ptr<Texture> quitButtonTexture = App::AssetManager->get<Texture>("quit-button");
    auto quitButton = PolygonNode::allocWithTexture(quitButtonTexture);
    quitButton->setTag(RETURN_MENU_TAG);
    quitButton->setAnchor(Vec2::ANCHOR_MIDDLE);
    quitButton->setScale(1.5f);
    quitButton->setPosition(center.x + 300, center.y);
    pauseNode->addChild(quitButton, 2);
    
    std::shared_ptr<Texture> winTexture = App::AssetManager->get<Texture>("win-screen");
    auto winScreen = PolygonNode::allocWithTexture(winTexture);
    winScreen->setAnchor(Vec2::ANCHOR_MIDDLE);
    winScreen->setPosition(center);
    winNode->addChild(winScreen);
    winNode->setTag(NEXT_LEVEL_TAG);
    
    std::shared_ptr<Texture> loseTexture = App::AssetManager->get<Texture>("lose-screen");
    auto loseScreen = PolygonNode::allocWithTexture(loseTexture);
    loseScreen->setAnchor(Vec2::ANCHOR_MIDDLE);
    loseScreen->setPosition(center);
    loseNode->addChild(loseScreen);
    loseNode->setTag(RESTART_BUTTON_TAG);
        
    return true;
}

bool GameUIController::handleUIEvent(int tag) {
    CULog("%i", tag);
    switch (tag) {
        case PAUSE_BUTTON_TAG:
            this->delegate->pauseButtonPressed();
            break;
        case RESUME_BUTTON_TAG:
            this->delegate->resumeButtonPressed();
            break;
        case RESTART_BUTTON_TAG:
            this->delegate->restartButtonPressed();
            break;
        case RETURN_MENU_TAG:
            this->delegate->returnToMenuButtonPressed();
            break;
        case SPEED_BUTTON_TAG:
            this->delegate->speedButtonPressed();
            break;
        case NEXT_LEVEL_TAG:
            this->delegate->nextLevel();
            break;
        default:
            return false;
    }
    
    return true;
}

void GameUIController::activateGameUI() {
    gameNode->setVisible(true);
    pauseNode->setVisible(false);
    winNode->setVisible(false);
    loseNode->setVisible(false);
}

void GameUIController::activatePauseUI() {
    gameNode->setVisible(false);
    pauseNode->setVisible(true);
}

void GameUIController::activateWinScreen() {
    gameNode->setVisible(false);
    pauseNode->setVisible(false);
    winNode->setVisible(true);
}

void GameUIController::activateLoseScreen() {
    gameNode->setVisible(false);
    pauseNode->setVisible(false);
    loseNode->setVisible(true);
}

void GameUIController::reset() {
    gameNode->setVisible(false);
    pauseNode->setVisible(false);
    winNode->setVisible(false);
    loseNode->setVisible(false);
}

void GameUIController::update(float timestep) {
    std::shared_ptr<cugl::Node> suNode = this->gameNode->getChildByName("SpeedButton");
    
    if (suNode->isVisible()) {
        if (App::InputController.heldTapReady()) {
            std::shared_ptr<Tap> tap = App::InputController.peekHeldTap();
            Vec3 worldPos = uiScene->screenToWorldCoords(tap->position);
            Vec2 coord = Vec2(worldPos.x, worldPos.y);
            Vec2 pos = suNode->worldToNodeCoords(coord);
            
            Rect rect = suNode->getBoundingBox();
            if (rect.contains(pos)) {
                heldTaps.push_back(App::InputController.popHeldTap());
                handleUIEvent(suNode->getTag());
                speeding = true;
                speedingID = tap->id;
            }
        }
        
        bool remove = false;
        if (App::InputController.releasedTapReady(speedingID) && speeding) {
            App::InputController.consumeReleasedTap(speedingID);
            remove = true;
        }
        if (App::InputController.heldSwipeReady(speedingID) && speeding) {
            remove = true;
        }
        if (remove) {
            handleUIEvent(suNode->getTag());
            int i;
            for (i = 0; i < heldTaps.size(); i++) {
                if (heldTaps[i]->id == speedingID) {
                    break;
                }
            }
            heldTaps.erase(heldTaps.begin() + i);
            speeding = false;
            speedingID = -1;
        }
    }
    
    AbstractUIController::update(timestep);
    
    for (auto name : gameModel->character->collected) {
        auto keyNode = gameNode->getChildByName(name);
        keyNode->setColor(Color4(255, 255, 255, 255));
    }
}
