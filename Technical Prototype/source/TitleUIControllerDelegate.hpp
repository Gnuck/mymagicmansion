//
//  TitleUIControllerDelegate.hpp
//  RocketDemo
//
//  Created by Andy Jiang on 2017/04/16.
//  Copyright © 2017年 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef TitleUIControllerDelegate_hpp
#define TitleUIControllerDelegate_hpp

#include <stdio.h>

/**
 This interface defines methods that should be implemented by the GameController
 in order to listen to UI events.
 */
class TitleUIControllerDelegate {
public:
    /**
     Called when a level is selected
     */
    virtual void selectLevel(int level) = 0;
    
    virtual void enterLevelSelect() = 0;
    
    virtual void enterMainMenu() = 0;
    
    virtual void enterCredits() = 0;
    
    virtual void playButtonPressed() = 0;
};

#endif /* TitleUIControllerDelegate_hpp */
