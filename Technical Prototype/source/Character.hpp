//
//  GameModel.cpp
//  RocketDemo
//
//  Created by Jiacong Xu on 3/16/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//
#ifndef __CHARACTER_H__
#define __CHARACTER_H__
#include <cugl/cugl.h>
#include <cugl/2d/physics/CUBoxObstacle.h>
#include <cugl/2d/physics/CUCapsuleObstacle.h>
#include <cugl/2d/CUWireNode.h>
#include <list>

/** For the character's body in relation to its texture */
#define NODE_X_OFFSET 0.05
#define CHAR_SIZE_FACTOR 0.35
/** The number of frames for the walking animation */
#define WALKING_FRAMES     8
#define WALKING_FACTOR     5
/** The number of frames for the falling animation */
#define FALLING_FRAMES     6
#define FALLING_FACTOR     4
#define END_FALLING_FACTOR 2
/** The number of frames for the climbing animation */
#define CLIMBING_FRAMES     8
#define CLIMBING_FACTOR     5
/** The number of frames for the pick up object animation */
#define PICKUP_FRAMES     4
#define PICKUP_FACTOR     5

#define EXCLAMATION_FRAMES 1

using namespace cugl;
/**
 This is our character in the game. We extend it from a box2d physics object so
 that physics event listeners can grab this object straight away, as opposed to,
 for example, checking physics object tag then grabbing a static character
 reference.
 */
class Character : public cugl::CapsuleObstacle {
    
private:
    /** 
     This macro disables the copy constructor (not allowed on physics objects) 
     */
    CU_DISALLOW_COPY_AND_ASSIGN(Character);
    
    // The character's physics body size. This might not be the same as the
    // animations.
    cugl::Vec2 charSize = cugl::Vec2(0.525, .733333);
    //cugl::Vec2 charSize = cugl::Vec2(0.895, 1.25); //1/4 tile height
    
    //NOT IMPLEMENTED IN CURRENT METHOD.  May be useful later so keeping.
    // Raycast: there are two points for raycasting, one a little above the
    // character and one below. If both hits, then we are facing a wall. If only
    // one hits, then we are facing stairs, and we climb. This is assuming we
    // are facing right. Otherwise negate x component.
    cugl::Vec2 topRayOffset = cugl::Vec2(charSize.x, charSize.y * 0.9) / 2;
    cugl::Vec2 botRayOffset = cugl::Vec2(charSize.x, -charSize.y * 0.9) / 2;
    bool topHit = false;
    bool botHit = false;
    
    // Raycast: One point for raycasting located a littel below the character.
    float rayLength = 0.1;
    
    /**
     Whether the character is facing the wall. This is mainly for movement.
     */
    bool facingWall;
    
    /**
     Whether the character is facing stairs. This is mainly for movement.
     */
    bool facingStairs;
    
    /**
     The horizontal movement speed of the character.
     */
    float moveSpeed = 1.0;
    
    float climbSpeed = 1.0;
    
    /** To slow down the animations */
    int walkingFrames;
    int fallingFrames;
    int climbingFrames;
    int pickupFrames;
    
    /** To initialize animation nodes */
    void initAnimation();

    
public:
    /** 
     The scene graph node for the Dude. 
     */
    std::shared_ptr<cugl::PolygonNode> node;
    
    /**
     The animation nodes */
    std::shared_ptr<cugl::AnimationNode> walking_node;
    std::shared_ptr<cugl::AnimationNode> falling_node;
    std::shared_ptr<cugl::AnimationNode> climbing_node;
    std::shared_ptr<cugl::AnimationNode> pickup_node;
    std::shared_ptr<cugl::AnimationNode> current_node;
    std::shared_ptr<cugl::AnimationNode> exclamation_node;
    
    /**
     Whether the character is facing right. This affects character's movement
     and animations.
     */
    bool facingRight;
    
    /**
     The scene graph Rectangle container
     */
    Rect container;
    
    /**
     True if character has reached goal door. False otherwise.
     */
    bool hitGoal = false;
    
    /**
     List of collectables obtained by the character
     */
    std::list<std::string> collected;
    
    /**
     The degenerate constructor that calls just its parent.
     */
    Character() : CapsuleObstacle() {
    }
    
    /**
     Destroys this Character, releasing all resources.
     */
    ~Character() {
    }
    
    /**
     Initializes a character at the given position in world coordinates. The
     box2d world object and node object should have the same coordinates. Assigns
     a number to the character to figure out its texture
     */
    bool init(cugl::Vec2 pos, bool facingRight);
    
    /**
     Creates and initializes a character at the given position in world 
     coordinates. The box2d world object and node object should have the same 
     coordinates.
     */
    static std::shared_ptr<Character> alloc(cugl::Vec2 pos, bool facingRight) {
        std::shared_ptr<Character> result = std::make_shared<Character>();
        
        return (result->init(pos, facingRight) ? result : nullptr);
    }

    /**
     * To switch animation nodes
     */
    void switchAnimation(std::shared_ptr<cugl::AnimationNode> next_node);
    
    /**
     * Walking animation
     */
    void animateWalking(bool on);
    
    /**
     * Falling animation
     */
    bool beginFallingStart; // Can start the begin falling animation
    bool beginFallingEnd;   // Can end the begin falling animation
    bool endFallingStart;   // Can start the end falling animation
    bool endFallingEnd;     // Can end the end falling animation
    bool fallAnimation;     // True when the fall animation is running
    void beginFalling(bool on);
    void animateFalling(bool on);
    void endFalling(bool on);
    
    /**
     * climbing animation
     */
    void animateClimbing(bool on);
    bool climbingStairs; // whether or not the character is currently climbing stairs
    int notClimbingFrames; // to make sure character is actually off of the stairs
    
    /**
     * Pickup animation
     */
    void animatePickup(bool on);
    bool pickingUpObject; //whether or not the character is currently picking up object to ensure animation runs once
    
    /**
     *make character stop moving while layer switches
     */
    void changeSpeed(float speed);
    
    
    /**
     Updates the character and its scene node position.
     */
    void update(std::shared_ptr<cugl::ObstacleWorld> world, float dt);
    
};


#endif /* __PF_DUDE_MODEL_H__ */
