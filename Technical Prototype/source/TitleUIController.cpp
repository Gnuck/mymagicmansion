//
//  TitleUIController.cpp
//  RocketDemo
//
//  Created by Andy Jiang on 2017/04/16.
//  Copyright © 2017年 Game Design Initiative at Cornell. All rights reserved.
//

#include "TitleUIController.hpp"
#include "App.h"
#include "InputController.hpp"
#include "Constants.h"

using namespace cugl;

void TitleUIController::initLevelSelect(int index) {
    Size dimen = Application::get()->getDisplaySize();
    dimen *= (GAME_WIDTH/dimen.width);
    Size mockupSize(MOCKUP_WIDTH, MOCKUP_HEIGHT);
    Vec2 center(dimen.width/2.0f, dimen.height/2.0f);
    float scale = GAME_WIDTH/mockupSize.width;
    
    std::shared_ptr<Node> levelSelectNode = Node::alloc();
    levelSelectNode->setContentSize(mockupSize);
    levelSelectNode->setAnchor(Vec2::ANCHOR_MIDDLE);
    levelSelectNode->setPosition(center);
    levelSelectNode->setScale(scale);
    uiScene->addChildWithName(levelSelectNode, "Level Select");
    levelSelectNode->setVisible(false);
    
    center.set(MOCKUP_WIDTH/2.0f, MOCKUP_HEIGHT/2.0f);
    
    /** Level Select */
    std::shared_ptr<Texture> levelSelectTexture;
    if (index == 0) {
        levelSelectTexture = App::AssetManager->get<Texture>("level-select-background-1");
    } else if (index == 1) {
        levelSelectTexture = App::AssetManager->get<Texture>("level-select-background-2");
    } else {
        levelSelectTexture = App::AssetManager->get<Texture>("level-select-background-3");
    }
    auto levelSelectBackground = PolygonNode::allocWithTexture(levelSelectTexture);
    levelSelectBackground->setAnchor(Vec2::ANCHOR_MIDDLE);
    levelSelectBackground->setPosition(center);
    levelSelectNode->addChild(levelSelectBackground, 0);
    
    std::shared_ptr<Texture> levelWindowTexture = App::AssetManager->get<Texture>("level-select-window");
    std::shared_ptr<Texture> unlockedLevelTexture = App::AssetManager->get<Texture>("level complete");
    
    current_level = App::readSaveFile();
    
    auto levelSelectBackImage = PolygonNode::allocWithTexture(App::AssetManager->get<Texture>("level-select-back"));
    Size backSize(300, 150);
    auto levelSelectBack = Node::alloc();
    levelSelectBack->setContentSize(backSize);
    levelSelectBack->setAnchor(Vec2::ANCHOR_MIDDLE);
    levelSelectBack->setPosition(MOCKUP_WIDTH/2, LEVEL_TEXT_BOTTOM_MARGIN + levelSelectBackImage->getHeight()/2);
    levelSelectBack->setTag(RETURN_MENU_TAG);
    levelSelectNode->addChild(levelSelectBack);
    
    levelSelectBackImage->setAnchor(Vec2::ANCHOR_MIDDLE);
    levelSelectBackImage->setPosition(backSize.width/2, backSize.height/2);
    levelSelectBack->addChild(levelSelectBackImage);
    
    Size arrowSize(130, 400);
    if (index > 0) {
        auto leftArrow = Node::alloc();
        leftArrow->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
        leftArrow->setPosition(0, MOCKUP_HEIGHT/2);
        leftArrow->setContentSize(arrowSize);
        if (index == 1) {
            leftArrow->setTag(FIRST_SCREEN);
        } else if (index == 2) {
            leftArrow->setTag(SECOND_SCREEN);
        }
        levelSelectNode->addChild(leftArrow);
        
        auto leftArrowImage = PolygonNode::allocWithTexture(App::AssetManager->get<Texture>("level-arrow-left"));
        leftArrowImage->setAnchor(Vec2::ANCHOR_MIDDLE_LEFT);
        leftArrowImage->setPosition(30, arrowSize.height/2);
        leftArrow->addChild(leftArrowImage);
    }
    
    if (index < 2) {
        auto rightArrow = Node::alloc();
        rightArrow->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        rightArrow->setPosition(MOCKUP_WIDTH, MOCKUP_HEIGHT/2);
        rightArrow->setContentSize(arrowSize);
        if (index == 1) {
            rightArrow->setTag(THIRD_SCREEN);
        } else if (index == 0) {
            rightArrow->setTag(SECOND_SCREEN);
        }
        levelSelectNode->addChild(rightArrow);
        
        auto rightArrowImage = PolygonNode::allocWithTexture(App::AssetManager->get<Texture>("level-arrow-right"));
        rightArrowImage->setAnchor(Vec2::ANCHOR_MIDDLE_RIGHT);
        rightArrowImage->setPosition(arrowSize.width - 30, arrowSize.height/2);
        rightArrow->addChild(rightArrowImage);
    }
    
    levelSelectNodes.push_back(levelSelectNode);
}

bool TitleUIController::init() {
    Size dimen = Application::get()->getDisplaySize();
    dimen *= (GAME_WIDTH/dimen.width);
    uiScene = Scene::alloc(dimen);
    Size mockupSize(MOCKUP_WIDTH, MOCKUP_HEIGHT);
    float scale = GAME_WIDTH/mockupSize.width;
    
    Vec2 center(dimen.width/2.0f, dimen.height/2.0f);
    
    mainMenuNode = Node::alloc();
    mainMenuNode->setContentSize(mockupSize);
    mainMenuNode->setAnchor(Vec2::ANCHOR_MIDDLE);
    mainMenuNode->setPosition(center);
    mainMenuNode->setScale(scale);
    uiScene->addChildWithName(mainMenuNode, "Main Menu");
    mainMenuNode->setVisible(true);
    
    creditsNode = Node::alloc();
    creditsNode->setContentSize(mockupSize);
    creditsNode->setAnchor(Vec2::ANCHOR_MIDDLE);
    creditsNode->setPosition(center);
    creditsNode->setScale(scale);
    uiScene->addChildWithName(creditsNode, "Credits");
    creditsNode->setVisible(false);
    
    center.set(MOCKUP_WIDTH/2.0f, MOCKUP_HEIGHT/2.0f);
    
    /** Main Menu */
    std::shared_ptr<Texture> mainMenuTexture = App::AssetManager->get<Texture>("main-menu");
    auto mainMenuBackground = PolygonNode::allocWithTexture(mainMenuTexture);
    mainMenuBackground->setAnchor(Vec2::ANCHOR_MIDDLE);
    mainMenuBackground->setPosition(center);
    mainMenuNode->addChild(mainMenuBackground, 0);
    
    float levelsSize = 400;
    auto levelsButton = Node::alloc();
    levelsButton->setAnchor(Vec2::ANCHOR_MIDDLE);
    levelsButton->setContentSize(levelsSize, levelsSize);
    levelsButton->setPosition(585, 210);
    levelsButton->setTag(LEVEL_SELECT_TAG);
    mainMenuNode->addChild(levelsButton);
    
    auto levelsText = PolygonNode::allocWithTexture(App::AssetManager->get<Texture>("main-menu-text1"));
    levelsText->setAnchor(Vec2::ANCHOR_MIDDLE);
    levelsText->setPosition(levelsSize/2 + 40, levelsSize/2 - 50);
    levelsButton->addChild(levelsText);
    
    auto creditsButton = Node::alloc();
    creditsButton->setAnchor(Vec2::ANCHOR_MIDDLE);
    creditsButton->setContentSize(levelsSize, levelsSize);
    creditsButton->setPosition(1365, 155);
    creditsButton->setTag(CREDITS_TAG);
    mainMenuNode->addChild(creditsButton);
    
    auto creditsText = PolygonNode::allocWithTexture(App::AssetManager->get<Texture>("main-menu-text2"));
    creditsText->setAnchor(Vec2::ANCHOR_MIDDLE);
    creditsText->setPosition(levelsSize/2, levelsSize/2);
    creditsButton->addChild(creditsText);
    
    auto playButton = PolygonNode::allocWithTexture(App::AssetManager->get<Texture>("play-button"));
    playButton->setAnchor(Vec2::ANCHOR_TOP_LEFT);
    playButton->setPosition(865, 324);
    playButton->setTag(PLAY_TAG);
    mainMenuNode->addChild(playButton);
    
    /** Credits */
    auto credits = PolygonNode::allocWithTexture(App::AssetManager->get<Texture>("credits"));
    credits->setAnchor(Vec2::ANCHOR_MIDDLE);
    credits->setPosition(center);
    credits->setTag(RETURN_MENU_TAG);
    creditsNode->addChild(credits);

    initLevelSelect(0);
    initLevelSelect(1);
    initLevelSelect(2);
    
    return true;
}

bool TitleUIController::handleUIEvent(int tag) {
    switch (tag) {
        case 0:
            return false;
        case LEVEL_SELECT_TAG:
            this->delegate->enterLevelSelect();
            break;
        case RETURN_MENU_TAG:
            this->delegate->enterMainMenu();
            break;
        case CREDITS_TAG:
            this->delegate->enterCredits();
            break;
        case PLAY_TAG:
            this->delegate->playButtonPressed();
            break;
        case FIRST_SCREEN:
            switchLevelSelect(0);
            break;
        case SECOND_SCREEN:
            switchLevelSelect(1);
            break;
        case THIRD_SCREEN:
            switchLevelSelect(2);
            break;
        default:
            if (current_level>=tag){
                // make sure level is reached before selecting
                this->delegate->selectLevel(tag);
            }
            break;
    }
    return true;
}

void TitleUIController::activateLevelSelectUI() {
    updateWindows();
    
    if (current_level <= 10) {
        levelSelectNodes[0]->setVisible(true);
    } else if (current_level > 10 && current_level <= 20) {
        levelSelectNodes[1]->setVisible(true);
    } else {
        levelSelectNodes[2]->setVisible(true);
    }
    mainMenuNode->setVisible(false);
}

void TitleUIController::updateWindows() {
    int n = 1;
    current_level = App::readSaveFile();
    std::shared_ptr<Texture> levelWindowTexture = App::AssetManager->get<Texture>("level-select-window");
    std::shared_ptr<Texture> unlockedLevelTexture = App::AssetManager->get<Texture>("level complete");
    Vec2 topLeft = Vec2(LEVEL_LEFT_MARGIN, LEVEL_TOP_MARGIN);
    for (int a = 0; a < levelSelectNodes.size(); a++) {
        auto node = levelSelectNodes[a];
        for (int j = 0; j < 2; j++) {
            for (int i = 0; i < 5; i++) {
                if (n > MAX_LEVELS) {
                    break;
                }
                node->removeChildByTag(n);
                string name = "label"+std::to_string(n);
                node->removeChildByName(name);
                
                std::shared_ptr<PolygonNode> levelWindow;
                if (n>current_level){ // not yet reached level
                    levelWindow = PolygonNode::allocWithTexture(levelWindowTexture);
                } else {
                    levelWindow = PolygonNode::allocWithTexture(unlockedLevelTexture);
                }
                levelWindow->setAnchor(Vec2::ANCHOR_TOP_LEFT);
                
                int xOffset = i*(LEVEL_WINDOW_SIDE + LEVEL_WINDOW_HORIZONTAL_GAP);
                int yOffset = j*(LEVEL_WINDOW_SIDE + LEVEL_WINDOW_HORIZONTAL_GAP);
                Vec2 loc = topLeft + Vec2(xOffset, yOffset);
                loc.y = MOCKUP_HEIGHT - loc.y;
                
                levelWindow->setPosition(loc);
                levelWindow->setTag(n);
                node->addChild(levelWindow);
                if (levelWindow->getTexture() == unlockedLevelTexture) {
                    // Put the level number in the window
                    std::shared_ptr<Font> font = App::AssetManager->get<Font>(LEVEL_SELECT_FONT);
                    auto label = Label::alloc(std::to_string(n), font);
                    label->setForeground(Color4::WHITE);
                    label->setPosition(loc.x+unlockedLevelTexture->getWidth()/2, loc.y-(unlockedLevelTexture->getHeight()*5/11));
                    label->setHorizontalAlignment(Label::HAlign::CENTER);
                    string name = "label"+std::to_string(n);
                    node->addChildWithName(label, name);
                }
                n++;
            }
        }
    }
}

void TitleUIController::switchLevelSelect(int index) {
    for (auto node : levelSelectNodes) {
        node->setVisible(false);
    }
    levelSelectNodes[index]->setVisible(true);
}

void TitleUIController::activateMainMenuUI() {
    for (auto node : levelSelectNodes) {
        node->setVisible(false);
    }
    mainMenuNode->setVisible(true);
    creditsNode->setVisible(false);
}

void TitleUIController::activateCreditsUI() {
    for (auto node : levelSelectNodes) {
        node->setVisible(false);
    }
    mainMenuNode->setVisible(false);
    creditsNode->setVisible(true);
}

void TitleUIController::deactivate() {
    for (auto node : levelSelectNodes) {
        node->setVisible(false);
    }
    mainMenuNode->setVisible(false);
    creditsNode->setVisible(false);
}
