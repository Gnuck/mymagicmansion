//
//  Collectible.hpp
//  RocketDemo
//
//  Created by Nicholas Treat Milner-Gonzales on 3/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef __COLLECTIBLE_H__
#define __COLLECTIBLE_H__

#include <stdio.h>
#include <cugl/cugl.h>
#include <cugl/2d/physics/CUBoxObstacle.h>
#include <cugl/2d/physics/CUCapsuleObstacle.h>
#include <cugl/2d/CUWireNode.h>
#include <list>

using namespace cugl;

/**
 This is our character in the game. We extend it from a box2d physics object so
 that physics event listeners can grab this object straight away, as opposed to,
 for example, checking physics object tag then grabbing a static character
 reference.
 */
class Collectible : public BoxObstacle {
    
private:
    /**
     This macro disables the copy constructor (not allowed on physics objects)
     */
    CU_DISALLOW_COPY_AND_ASSIGN(Collectible);
    
    // The collectable's physics body size. This might not be the same as the
    // animations.
    Vec2 collSize = cugl::Vec2(1.0, 1.0);
    
    
public:
    /**
     The scene graph node for the collectable.
     */
    std::shared_ptr<Node> node;

    /**
     The degenerate constructor that calls just its parent.
     */
    Collectible() : BoxObstacle() {
    }
    
    /**
     Destroys this Collectable, releasing all resources.
     */
    ~Collectible() {
    }
    
    std::string textureName;
    
    /**
     Initializes a collectable at the given position in world coordinates. The
     box2d world object and node object should have the same coordinates.
     */
    bool init(Vec2 pos, Vec2 offset, std::string name);
    
    /**
     Creates and initializes a collectable at the given position in world
     coordinates. The box2d world object and node object should have the same
     coordinates.
     */
    static std::shared_ptr<Collectible> alloc(Vec2 pos, Vec2 offset, std::string name) {
        std::shared_ptr<Collectible> result = std::make_shared<Collectible>();
        
        return (result->init(pos, offset, name) ? result : nullptr);
    }
    
    /**
     Destroys the scene graph node and the physics body connected to this collectable
     */
    void destroy();
    
};

#endif /* Collectible_hpp */
