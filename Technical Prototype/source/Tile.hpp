//
//  Tile.hpp
//  RocketDemo
//
//  Created by Kathryn Angela Stinebruner on 3/11/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Tile_hpp
#define Tile_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "TileView.hpp"

/** The number of frames for the unlocking animation */
#define UNLOCK_FRAMES     4
#define UNLOCK_FACTOR     7

const float METERS_PER_TILE = 5.0;
const cugl::Color4 LOCK_TINT = cugl::Color4(100,100,100,1);
const cugl::Color4 LOCK_TINT_FRACTION = cugl::Color4(LOCK_TINT.r/UNLOCK_FRAMES,LOCK_TINT.g/UNLOCK_FRAMES,LOCK_TINT.b/UNLOCK_FRAMES,1);

using namespace cugl;

/**
 This model represents a game tile. It has a pointer to its collision geometry
 and a pointer to its scene graph node.
 */
class Tile {
    /** This macro disables the copy constructor (not allowed on scene graphs) */
    CU_DISALLOW_COPY_AND_ASSIGN(Tile);
    
protected:
    /**
     Tile position with 0, 0 being the lower left corner tile. Both components
     of the vector should be positive. Setting this vector should also update
     its position in the box2d world and scene graph.
     */
    cugl::Vec2 position;
    
    /** the collectible tag that locks this tile */
    std::string lockedBy;
    
    
public:
    /** The list of collider objects we insert into the box2d world */
    std::vector<std::shared_ptr<Obstacle>> colliders;
    
    /** The scene graph node for the tile. */
    std::shared_ptr<TileView> tileNode;
    
    /** The foreground layer node for the tile. */
    std::shared_ptr<cugl::Node> foregroundNode;
    
    /** The background layer node for the tile. */
    std::shared_ptr<cugl::Node> backgroundNode;
    
    /**
     Default constructor.
     */
    Tile() {};
    
    /**
     Default destructor.
     */
    ~Tile() {
        dispose();
    }
    
    /**
     Get rid of this tile.
     */
    void dispose();
    
    /**
     Initializes an empty tile with the given position and is locked status. The
     position is the bottom left corner.
     */
    void init(cugl::Vec2 position, Color4f color);
    
    /**
     Returns a newly allocated empty tile.
     */
    static std::shared_ptr<Tile> alloc(cugl::Vec2 position, Color4f color) {
        std::shared_ptr<Tile> result = std::make_shared<Tile>();
        result->init(position, color);
        
        return result;
    }
    
    /** Get the position of the tile */
    cugl::Vec2 getPosition();
    
    /** 
     Sets the position of the tile. This updates the physic objects' position
     and scene graph node's position simultanouesly. It is the caller's
     responsibility to make sure the positions are valid.
     
     When animating layer switching, we might not want to move the physics
     together with the character.
     */
    void setPosition(cugl::Vec2 position);
    
    /**
     Whether or not this tile contains a given position
     */
    bool contains(cugl::Vec2 position);
    
    /**
     Sets the visibility of this tile's view and physical objects.
     */
    void setActive(bool isActive);
    
    /**
     Returns whether the tile is locked.
     */
    bool isLocked();
    
    /**
     Returns the key of this locked tile.
     */
    std::string getLockKey();
    
    /**
     Sets the lock status of the tile.
     */
    void setLocked(std::string lockedBy, bool shouldAnimate);
};





#endif /* Tile_hpp */
