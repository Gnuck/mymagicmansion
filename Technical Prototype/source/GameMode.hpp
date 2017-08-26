//
//  GameMode.hpp
//  RocketDemo
//
//  Created by Jiacong Xu on 3/10/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef GameMode_hpp
#define GameMode_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "GameController.hpp"

/**
 This is the top level class handling anything related to game mode. For example
 if the App wants to switch mode (if player wants to go back to the title
 screen) then this is responsible to let the App know that we want to go to
 another mode.
 */
class GameMode {
private:
    GameController gameController;

  
public:
#pragma mark -
#pragma mark Constructors
  /**
   Creates a new game mode without any heap pointers. Currently we don't do
   anything here.
   */
  GameMode();
  
  /**
   This is the destructor of the an instance of the class. Dispose heap memory
   here.
   */
  ~GameMode() {
    dispose();
  }
  
  /**
   Disposes all (non-static) resources allocated to this mode.
   */
  void dispose();
  
  /**
   Starts a new given level.
   */
  void init(int level);
    
  bool returnToLevelSelect();
  
#pragma mark -
#pragma mark Gameplay Handling
  /**
   The main update loop. Any child controllers should be updated here.
   */
  void update(float timestep);
    
  /**
   The main draw call. Any child controllers should be drawn here.
   */
  void draw(const std::shared_ptr<cugl::SpriteBatch>& batch);
};

#endif /* GameMode_hpp */
