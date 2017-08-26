//
//  LevelControllerDelegate.hpp
//  RocketDemo
//
//  Created by Jiacong Xu on 3/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef LevelControllerDelegate_hpp
#define LevelControllerDelegate_hpp

#include <stdio.h>

/**
 This interface defines methods that should be implemented by the GameController
 in order to listen to Level progress events.
 */
class LevelControllerDelegate {
public:
    /**
     Called when the character is dead.
     */
    virtual void gameLost() = 0;
    
    /**
     Called when the character reaches the goal.
     */
    virtual void gameWon() = 0;
};

#endif /* LevelControllerDelegate_hpp */
