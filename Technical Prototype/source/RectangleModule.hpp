//
//  RectangleModule.hpp
//  RocketDemo
//
//  Created by Jiacong Xu on 3/21/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef RectangleModule_hpp
#define RectangleModule_hpp

#include <stdio.h>
#include "AbstractTileModule.hpp"

/**
 The basic rectangle module that makes out much of the game.
 */
class RectangleModule : public AbstractTileModule {
public:
    /**
     Returns the scene graph node that contains everything necessary to render
     this object. The node will be created in world space, according to the rect
     so it should be repositioned after added to the parent node.
     */
    std::shared_ptr<cugl::Node> generateNewNode(int i, int j);
    
    /** For animation nodes */
    std::shared_ptr<cugl::AnimationNode> generateNewAnimationNode(int i, int j, bool isAnimation = false, int frames = 0);
    
    /**
     Returns the list of colliders that make up its physics. Each will be in
     world coordinates, and this should be in the same coordinate system (both
     origin and scale) as the returned scene graph node.
     */
    std::vector<std::shared_ptr<cugl::Obstacle>> generateNewColliders();
    
    RectangleModule() {}
    
    ~RectangleModule() {}
};

#endif /* RectangleModule_hpp */
