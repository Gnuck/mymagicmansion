//
//  GameUIControllerDelegate.hpp
//  RocketDemo
//
//  Created by Jiacong Xu on 3/19/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef GameUIControllerDelegate_hpp
#define GameUIControllerDelegate_hpp

#include <stdio.h>

/**
 This interface defines methods that should be implemented by the GameController
 in order to listen to UI events.
 */
class GameUIControllerDelegate {
public:
    /**
     Called when the pause button is pressed.
     */
    virtual void pauseButtonPressed() = 0;
    
    /**
     Called when the speed button is pressed.
     */
    virtual void speedButtonPressed() = 0;
    
    /**
     Called when the resume button is pressed.
     */
    virtual void resumeButtonPressed() = 0;
    
    /**
     Called when the restart button is pressed.
     */
    virtual void restartButtonPressed() = 0;
    
    /**
     Called when the back to level select button is pressed.
     */
    virtual void returnToMenuButtonPressed() = 0;
    
    virtual void nextLevel() = 0;
};

#endif /* GameUIControllerDelegate_hpp */
