//
//  StairModule.hpp
//  RocketDemo
//
//  Created by Kathryn Angela Stinebruner on 3/23/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef StairModule_hpp
#define StairModule_hpp

#include <stdio.h>
#include "AbstractTileModule.hpp"

/**
 The basic stair module that makes stairs the game.
 */
class StairModule : public AbstractTileModule {
private:
    int direction = 0;
public:
    /**
     Returns the scene graph node that contains everything necessary to render
     this object. The node will be created in world space, according to the rect
     so it should be repositioned after added to the parent node.
     */
    std::shared_ptr<cugl::Node> generateNewNode(int i, int j);
    
    /**
     Returns the list of colliders that make up its physics. Each will be in
     world coordinates, and this should be in the same coordinate system (both
     origin and scale) as the returned scene graph node.
     */
    std::vector<std::shared_ptr<cugl::Obstacle>> generateNewColliders();
    
    StairModule() {}
    
    ~StairModule() {}
    
    /** turns the stairs to face left */
    void setDirection(int dir);
};

#endif /* StairModule_hpp */
