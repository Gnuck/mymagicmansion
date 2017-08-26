//
//  GameUIController.hpp
//  RocketDemo
//
//  Created by Jiacong Xu on 3/11/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef GameUIController_hpp
#define GameUIController_hpp

#include <stdio.h>
#include "AbstractController.hpp"
#include "AbstractUIController.hpp"
#include "GameUIControllerDelegate.hpp"
#include "GameModel.hpp"

/**
 This controller handles the display and player interaction with the UI.
 */
class GameUIController : public AbstractUIController {

private:
    
    /**
     Parent of the UI elements visible during gameplay.
     */
    std::shared_ptr<cugl::Node> gameNode;
    
    /**
     Parent of the UI elements on the pause screen.
     */
    std::shared_ptr<cugl::Node> pauseNode;
    
    std::shared_ptr<cugl::Node> winNode;
    
    std::shared_ptr<cugl::Node> loseNode;
    
    std::shared_ptr<cugl::Node> speedupNode;
    
    bool handleUIEvent(int tag);
    
    bool speeding;
    
    long speedingID;
    
public:
    /**
     The delegate that will respond to events.
     */
    GameUIControllerDelegate *delegate;
    
    std::shared_ptr<GameModel> gameModel;
    
    std::shared_ptr<cugl::Label> label;
    
    void activateGameUI();
    
    void activatePauseUI();
    
    void activateWinScreen();
    
    void activateLoseScreen();
    
    void reset();
    
    void update(float timestep);
    
    /**
     Initializes this instance. This lays out the game's UI elements.
     */
    bool init(std::shared_ptr<GameModel> gameModel);
    
    GameUIController() {}
    
    ~GameUIController() {}
    
};

#endif /* GameUIController_hpp */
