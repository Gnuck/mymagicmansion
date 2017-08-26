//
//  TitleMode.hpp
//  RocketDemo
//
//  Created by Andy Jiang on 2017/04/16.
//  Copyright © 2017年 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef TitleMode_hpp
#define TitleMode_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "TitleUIController.hpp"
#include "TitleUIControllerDelegate.hpp"

/**
 This is the top level class handling anything related to game mode. For example
 if the App wants to switch mode (if player wants to go back to the title
 screen) then this is responsible to let the App know that we want to go to
 another mode.
 */
class TitleMode : public TitleUIControllerDelegate {
private:
    TitleUIController titleUIController;
    
    int level;
    
public:
    TitleMode();
    
    ~TitleMode() {
        dispose();
    }
    
    /**
     Disposes all (non-static) resources allocated to this mode.
     */
    void dispose();
    
    /**
     Starts a new given level.
     */
    void init();
    
    /**
     Exit title mode without disposing of the title UI's scene graph.
     */
    void exit();
    
    void enterLevelSelect();
    
    void enterMainMenu();
    
    void enterCredits();
    
    void playButtonPressed();
    
    /**
     Returns true if the game is in title mode and the player has selected a level. False otherwise.
     */
    bool levelSelected();
    
    int getLevel();
    
    void selectLevel(int level);
    
    /**
     The main update loop. Any child controllers should be updated here.
     */
    void update(float timestep);
    
    /**
     The main draw call. Any child controllers should be drawn here.
     */
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch);
};

#endif /* TitleMode_hpp */

