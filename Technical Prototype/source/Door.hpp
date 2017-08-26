//
//  Door.hpp
//  RocketDemo
//
//  Created by Nicholas Treat Milner-Gonzales on 3/27/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef __DOOR_H__
#define __DOOR_H__


#include <stdio.h>
#include <cugl/cugl.h>
#include <cugl/2d/physics/CUBoxObstacle.h>
#include <cugl/2d/physics/CUCapsuleObstacle.h>
#include <cugl/2d/CUWireNode.h>
#include <list>
#include <string.h>

using namespace cugl;

/**
 This is our character in the game. We extend it from a box2d physics object so
 that physics event listeners can grab this object straight away, as opposed to,
 for example, checking physics object tag then grabbing a static character
 reference.
 */
class Door : public cugl::BoxObstacle {
    
private:
    /**
     This macro disables the copy constructor (not allowed on physics objects)
     */
    CU_DISALLOW_COPY_AND_ASSIGN(Door);
    
    // The door's physics body size. This might not be the same as the
    // animations.
    cugl::Vec2 doorSize = cugl::Vec2(5.0/6, 5.0/3);
    cugl::Vec2 doorLoc;
    std::string id;
    std::string connecting;
    
    
public:
    /**
     The scene graph node for the door.
     */
    std::shared_ptr<Node> node;
    
    /**
     The degenerate constructor that calls just its parent.
     */
    Door() : BoxObstacle() {
    }
    
    /**
     Destroys this door, releasing all resources.
     */
    ~Door() {
    }
    
    /**
     Initializes a door at the given position in world coordinates. The
     box2d world object and node object should have the same coordinates.
     */
    bool init(Vec2 pos, Vec2 size, Vec2 offset, std::string id, std::string connecting, int layer, std::string textureName);
    
    /**
     Creates and initializes a door at the given position in world
     coordinates. The box2d world object and node object should have the same
     coordinates.
     */
    static std::shared_ptr<Door> alloc(Vec2 pos, Vec2 size, Vec2 offset,std::string id, std::string connecting, int layer, std::string textureName) {
        std::shared_ptr<Door> result = std::make_shared<Door>();
        
        return (result->init(pos, size, offset, id, connecting, layer, textureName) ? result : nullptr);
    }
    
    /**
     Destroys the scene graph node and the physics body connected to this door
     */
    void destroy();
    
    /**
     Set the desired position for the door
     */
    void setPosition(Vec2 pos);
    
    /**
     Return the size of the door)
     */
    Vec2 getSize();
    
    /**
     Get the name of the door connected to this door
     */
    std::string getConnecting();
    
    /**
     Layer this door is on. This is helpful during layer switching.
     */
    int layer;
};

#endif /* Door_hpp */
