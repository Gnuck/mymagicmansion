//
//  GameController.hpp
//  RocketDemo
//
//  Created by Jiacong Xu on 3/11/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef GameController_hpp
#define GameController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "AbstractController.hpp"
#include "LevelController.hpp"
#include "GameUIController.hpp"
#include "GameModel.hpp"
#include "InputController.hpp"
#include "AudioController.hpp"
#include "GameUIControllerDelegate.hpp"
#include "LevelControllerDelegate.hpp"

/**
 The top level controller of the game. This controller initializes the GameModel
 LevelController, GameUIController, and InputController. It also contains the
 update-draw loop.
 */
class GameController : public AbstractController, public GameUIControllerDelegate,
public LevelControllerDelegate {
protected:
    bool returnLevelSelect;
    
    int level;
    
    // MODELS
    std::shared_ptr<GameModel> gameModel;
    
    // CONTROLLERS
    
    /** The level controller */
    LevelController levelController;
    
    /** The UI controller */
    GameUIController uiController;
    
    /** The input controller */
    InputController inputController;
    
    // VIEWS
    
    /** The root of our scene graph. */
    std::shared_ptr<cugl::Scene> scene;
    
public:
    /**
     Initializes and sets up a game for the given level.
     */
    void init(int level);
    
    void update(float dt);
    
    /**
     Overriden draw method.
     */
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch);
    
    /**
     Disposes of all (non-static) resources allocated to this controller
     */
    void dispose();
    
    /**
     Called when the pause button is pressed.
     */
    void pauseButtonPressed();
    
    /**Called when speedup button is pressed.
     */
    void speedButtonPressed();
    
    /**
     Called when the resume button is pressed.
     */
    void resumeButtonPressed();
    
    /**
     Called when the restart button is pressed.
     */
    void restartButtonPressed();
    
    /**
     Called when the back to level select button is pressed.
     */
    void returnToMenuButtonPressed();
    
    void nextLevel();
    
    /**
     Called when the character is dead.
     */
    void gameLost();
    
    /**
     Called when the character reaches the goal.
     */
    void gameWon();
    
    bool returnToLevelSelect() {
        return returnLevelSelect;
    }
    
    bool speedUp = false;
    
    float multSpeed =  1;
    void speedUpPressed();
    
    /**
     Smoothing function for speeding up the gameplay
     @param g = goal speed (timestep)
     @param c = current speed (timestep)
     @param delta = amount to increase the current speed towards the goal speel
     */
    float approach(float g, float c, float delta);
};

#endif /* GameController_hpp */
