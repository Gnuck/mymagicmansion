//
//  Swipe.hpp
//  RocketDemo
//
//  Created by Andy Jiang on 2017/03/26.
//  Copyright © 2017年 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Swipe_hpp
#define Swipe_hpp

#include <stdio.h>
#include <cugl/cugl.h>

using namespace cugl;

/**
 This model represents a game tile. It has a pointer to its collision geometry
 and a pointer to its scene graph node.
 */
class Swipe {
    
public:
    
    /** The position at which the swipe was initiated. */
    Vec2 initialPosition;
    
    /** The position at which the swipe ended. */
    Vec2 finalPosition;
    
    long id;
    
    /** Default constructor. */
    Swipe() {};
    
    /** Default destructor. */
    ~Swipe() {
        dispose();
    }
    
    void dispose();
    
    /**
     Initializes an empty tile with the given position and is locked status. The
     position is the bottom left corner.
     */
    void init(long id, Vec2 initialPosition, Vec2 finalPosition);
    
    static std::shared_ptr<Swipe> alloc(long id, Vec2 initialPosition, Vec2 finalPosition) {
        std::shared_ptr<Swipe> result = std::make_shared<Swipe>();
        result->init(id, initialPosition, finalPosition);
        return result;
    }
};

#endif /* Swipe_hpp */
