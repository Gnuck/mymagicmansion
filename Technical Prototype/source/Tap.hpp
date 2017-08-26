//
//  Tap.hpp
//  RocketDemo
//
//  Created by Andy Jiang on 2017/03/24.
//  Copyright © 2017年 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Tap_hpp
#define Tap_hpp

#include <stdio.h>
#include <cugl/cugl.h>

using namespace cugl;

/**
 This model represents a game tile. It has a pointer to its collision geometry
 and a pointer to its scene graph node.
 */
class Tap {
    
public:
    
    /** The position of the touch. Defined as the initial position where the user touched. */
    Vec2 position;
    
    long id;

    /** Default constructor. */
    Tap() {};
    
    /** Default destructor. */
    ~Tap() {
        dispose();
    }
    
    void dispose();
    
    /**
     Initializes an empty tile with the given position and is locked status. The
     position is the bottom left corner.
     */
    void init(long id, Vec2 position);
    
    static std::shared_ptr<Tap> alloc(long id, Vec2 position) {
        std::shared_ptr<Tap> result = std::make_shared<Tap>();
        result->init(id, position);
        return result;
    }
};

#endif /* Tap_hpp */
