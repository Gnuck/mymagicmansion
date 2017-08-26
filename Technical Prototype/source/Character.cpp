//
//  GameModel.cpp
//  RocketDemo
//
//  Created by Jiacong Xu on 3/16/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//
#include "Character.hpp"
#include <cugl/2d/CUPolygonNode.h>
#include <cugl/2d/CUTexturedNode.h>
#include <cugl/assets/CUAssetManager.h>
#include "App.h"
#include <iostream>
#include <string>
#include "Constants.h"


using namespace cugl;

bool Character::init(Vec2 pos, bool facingRight) {
    // The size of the character should be a constant.
    if (CapsuleObstacle::init(pos, charSize)) {
        setDensity(1.0);
        setFriction(0.0f);
        setFixedRotation(true);
        setBodyType(b2_dynamicBody);
        //setDebugColor(Color4::GREEN);
        setRestitution(0);
        setFriction(0);
        setName(CHARACTER);
        this->facingRight = facingRight;
        
        // For scene graph node
        if (facingRight){
            container = Rect(pos.x-NODE_X_OFFSET, pos.y, charSize.x, charSize.y);
        } else {
            container = Rect(pos.x+NODE_X_OFFSET, pos.y, charSize.x, charSize.y);
        }
        Vec2 center = Vec2(container.getMidX(), container.getMidY());
        std::shared_ptr<Texture> texture = App::AssetManager->get<Texture>(CHARACTER);

        node = PolygonNode::alloc();
        
        // Animation
        initAnimation();
        current_node = walking_node;
        node->addChild(current_node);
        
        node->setScale(CHAR_SIZE_FACTOR*container.size.width / texture->getWidth(),
                         CHAR_SIZE_FACTOR*container.size.height / texture->getHeight());
        node->setPosition(center);
        beginFallingStart = false;
        beginFallingEnd = false;
        endFallingStart = false;
        endFallingEnd = false;
        climbingStairs = false;
        notClimbingFrames = 0;
        pickingUpObject = false;
        fallAnimation = false;
        
        return true;
    }
    
    return false;
}


void Character::initAnimation(){
    // Walking
    std::shared_ptr<Texture>walking_texture = App::AssetManager->get<Texture>(WALKING_TEXTURE);
    walking_node = AnimationNode::alloc(walking_texture, 1, WALKING_FRAMES, WALKING_FRAMES);
    if (node != nullptr && walking_node != nullptr) {
        walking_node->setPosition(node->getContentSize().width/2.0f,
                                  node->getContentSize().height-
                                  node->getContentSize().height/2.0f);
        //node->addChildWithName(walking_node, "walking");
    }
    walkingFrames = 0;
    
    // Falling
    std::shared_ptr<Texture>falling_texture = App::AssetManager->get<Texture>(FALLING_TEXTURE);
    falling_node = AnimationNode::alloc(falling_texture, 1, FALLING_FRAMES, FALLING_FRAMES);
    if (node != nullptr && falling_node != nullptr) {
        falling_node->setPosition(node->getContentSize().width/2.0f,
                                  node->getContentSize().height-
                                  node->getContentSize().height/2.0f);
        //node->addChildWithName(falling_node, "falling");
    }
    fallingFrames = 0;
    
    // Pick up object
    std::shared_ptr<Texture>pickup_texture = App::AssetManager->get<Texture>(PICKUP_TEXTURE);
    pickup_node = AnimationNode::alloc(pickup_texture, 1, PICKUP_FRAMES, PICKUP_FRAMES);
    pickup_node->setScale(2.8);
    if (node != nullptr && pickup_node != nullptr) {
        pickup_node->setPosition(node->getContentSize().width/2.0f,
                                  node->getContentSize().height-
                                  node->getContentSize().height/2.0f);
        //node->addChildWithName(pickup_node, "pickup");
    }
    pickupFrames = 0;
    
    // Climb stairs
    std::shared_ptr<Texture>climbing_texture = App::AssetManager->get<Texture>(CLIMBING_TEXTURE);
    climbing_node = AnimationNode::alloc(climbing_texture, 1, CLIMBING_FRAMES, CLIMBING_FRAMES);
    climbing_node->setScale(1.1); //CHANGE FOR FUTURE ANIMATION STRIP
    climbing_node->cugl::Node::setAnchor(0.5f, 0.6f);
    if (node != nullptr && climbing_node != nullptr) {
        climbing_node->setPosition(node->getContentSize().width/2.0f,
                                 node->getContentSize().height-
                                 node->getContentSize().height/2.0f);
        //node->addChildWithName(climbing_node, "climbing");
    }
    climbingFrames = 0;
    
    // Exclamation mark
    std::shared_ptr<Texture> exclamation_texture = App::AssetManager->get<Texture>(EXCLAMATION_TEXTURE);
    exclamation_node = AnimationNode::alloc(exclamation_texture, 1, EXCLAMATION_FRAMES, EXCLAMATION_FRAMES);
    exclamation_node->setScale(1.2);
    exclamation_node->setFrame(0);
    
    // Shared
    if (facingRight){
        walking_node->flipHorizontal(0);
        falling_node->flipHorizontal(0);
        pickup_node->flipHorizontal(0);
        climbing_node->flipHorizontal(0);
    } else {
        walking_node->flipHorizontal(1);
        falling_node->flipHorizontal(1);
        pickup_node->flipHorizontal(1);
        climbing_node->flipHorizontal(1);
    }
}

void Character::update(std::shared_ptr<ObstacleWorld> world, float dt) {
    CapsuleObstacle::update(dt);
    if (node != nullptr) {
        node->setAngle(getAngle());
    }

    // Now detect if we are facing anything. These are based on the raycast
    // performed in the last frame.
    facingWall = topHit && botHit;
    facingStairs = !topHit && botHit;
    
    // Reset the hits.
    topHit = false;
    botHit = false;
    
    // Turn around if facing wall
    if (facingWall && isActive()) {
        facingRight = !facingRight;
    }
    
    int sign = facingRight? 1 : -1;
    Vec2 dir = Vec2(0.1, 0) * sign;
    Vec2 start = Vec2(getPosition().x + sign * topRayOffset.x, getPosition().y + topRayOffset.y);
    
    
    // Now update movement here.
    setLinearVelocity(moveSpeed * sign, getLinearVelocity().y);
    
    if (facingStairs) {
        Vec2 pos = getPosition();
        pos.y += climbSpeed * dt;
        setPosition(pos);
        climbingStairs = true;
    }
    
    node->setPosition(getPosition().x+NODE_X_OFFSET,getPosition().y);
    node->setAngle(getAngle());
    
    // Animation
    if (pickingUpObject){
        if (current_node==walking_node){
            switchAnimation(pickup_node);
            endFalling(0);
            animateWalking(0);
            animateClimbing(0);
        } else if (current_node==pickup_node){
            animatePickup(1);
        } else {
            pickingUpObject = false;
        }
    } else if (climbingStairs) { //climbing stairs
        if (current_node!=climbing_node){
            switchAnimation(climbing_node);
            if (node->getChildByTag(EXCLAMATION_TAG)!=nullptr){
                node->removeChildByTag(EXCLAMATION_TAG);
            }
            endFalling(0);
            animateWalking(0);
            animatePickup(0);
        }
        animateClimbing(1);
        if (!facingStairs && getLinearVelocity().y<=0){
            notClimbingFrames++;
        } else {
            notClimbingFrames = 0;
        }
        if (!facingStairs && getLinearVelocity().y<=0 && notClimbingFrames>5){
            climbingStairs = false;
            notClimbingFrames = 0;
        }
    } else if (getLinearVelocity().y < -0.15){
        if (current_node!=falling_node){
            switchAnimation(falling_node);
            animateWalking(0);
            animateClimbing(0);
            animatePickup(0);
            beginFallingStart = true;
            fallAnimation = true;
        }
        if (beginFallingStart && !beginFallingEnd){ //begin falling
            if (node->getChildByTag(EXCLAMATION_TAG)==nullptr){
                node->addChildWithTag(exclamation_node, EXCLAMATION_TAG,0);
                if (facingRight){
                    exclamation_node->flipHorizontal(0);
                    exclamation_node->cugl::Node::setAnchor(5.0f, -8.0f);
                } else {
                    exclamation_node->flipHorizontal(1);
                    exclamation_node->cugl::Node::setAnchor(-5.0f, -8.0f);
                }
                exclamation_node->setVisible(true);
            }
            beginFalling(1);
        } else if (beginFallingEnd) {
            if (falling_node->getFrame()<2){
                falling_node->setFrame(2);
                fallingFrames = (FALLING_FACTOR*2)-1;
            }
            beginFallingStart = false;
            beginFallingEnd = false;
            animateFalling(1); //mid-fall
            endFallingStart = true;
        } else {
            animateFalling(1); //mid-fall
        }
    } else if (endFallingStart && getLinearVelocity().y>=0){ //begin to end fall

        if (falling_node->getFrame()<4){
            falling_node->setFrame(4);
            fallingFrames = FALLING_FACTOR*4;
        }
        endFallingStart = false;
    } else if (!endFallingEnd && fallAnimation) { //animate end fall
        if (node->getChildByTag(EXCLAMATION_TAG)!=nullptr){
            node->removeChildByTag(EXCLAMATION_TAG);
        }
        endFalling(1);
        if (endFallingEnd){ //finish falling sequence
            endFallingStart = false;
            endFallingEnd = false;
            fallAnimation = false;
        }
    } else {
        if (current_node!=walking_node){
            switchAnimation(walking_node);
            endFalling(0);
            animateClimbing(0);
            animatePickup(0);
            beginFallingStart = false;
            beginFallingEnd = false;
            endFallingStart = false;
            endFallingEnd = false;
        }
        animateWalking(1);
    }

    // Since they are callbacks they must be placed at the very end.
    world->rayCast([=](b2Fixture *fixture, const Vec2 &point, const Vec2 &normal, float fraction) mutable -> float {
        // In future, we should check whether this is part of the wall or not.
        // Aka if the fixture's tag says its part of level geometry.
        if (fixture->IsSensor()) {
            return -1;
        }
        
        topHit = true;
        
        return 0;
    }, start, start + dir);
    
    start = Vec2(getPosition().x + sign * botRayOffset.x, getPosition().y + botRayOffset.y);
    
    world->rayCast([=](b2Fixture *fixture, const Vec2 &point, const Vec2 &normal, float fraction) mutable -> float {
        if (fixture->IsSensor()) {
            return -1;
        }
        
        botHit = true;
        
        return 0;
    }, start, start + dir);
}


/**
 * to switch animation nodes
 */
void Character::switchAnimation(std::shared_ptr<cugl::AnimationNode> next_node){
    node->removeChild(current_node);
    current_node = next_node;
    node->addChild(current_node);
    
    if (current_node==falling_node){
        if (facingRight){
            current_node->cugl::Node::setAnchor(0.75f, 0.7f);
        } else {
            current_node->cugl::Node::setAnchor(0.25f, 0.7f);
        }
        current_node->setPosition(0.0f, 0.0f);
    }
}

/**
 * Walking animation
 */
void Character::animateWalking(bool on) {
    if (on) {
        //printf("walking\n");
        walkingFrames++;
        if (walkingFrames % WALKING_FACTOR == 0) {
            walkingFrames = 0;
            int nextFrame = (walking_node->getFrame() + 1) % WALKING_FRAMES;
            walking_node->setFrame(nextFrame);
        }
    } else {
        walking_node->setFrame(0);
        walkingFrames = 0;
    }
    if (facingRight) {
        walking_node->flipHorizontal(0);
    } else {
        walking_node->flipHorizontal(1);
    }
}

/**
 * Falling animation
 */
void Character::beginFalling(bool on) {
    if (on) {
        //printf("begin falling\n");
        // begin
        fallingFrames++;
        if (fallingFrames%FALLING_FACTOR==0){
            falling_node->setFrame(1);
        }
        if (fallingFrames>=FALLING_FACTOR*2 && falling_node->getFrame()>=1){
            beginFallingEnd = true; // end falling
        }
    }
    else {
        falling_node->setFrame(2);
        fallingFrames = (FALLING_FACTOR*2)-1;
    }
    if (facingRight) {
        falling_node->flipHorizontal(0);
        falling_node->cugl::Node::setAnchor(0.75f, 0.7f);
    } else {
        falling_node->flipHorizontal(1);
        falling_node->cugl::Node::setAnchor(0.25f, 0.7f);
    }
    current_node->setPosition(0.0f, 0.0f);
}

void Character::animateFalling(bool on) {
    if (on) {
        //printf("falling\n");
        // Increment
        fallingFrames++;
        if (fallingFrames%FALLING_FACTOR==0 && falling_node->getFrame()==2){
            falling_node->setFrame(3);
        }
        else if (fallingFrames%FALLING_FACTOR==0 && falling_node->getFrame()==3){
            falling_node->setFrame(2);
        }
    }
    if (facingRight) {
        falling_node->flipHorizontal(0);
        falling_node->cugl::Node::setAnchor(0.75f, 0.7f);
    } else {
        falling_node->flipHorizontal(1);
        falling_node->cugl::Node::setAnchor(0.25f, 0.7f);
    }
    current_node->setPosition(0.0f, 0.0f);
}

void Character::endFalling(bool on){
    if (on) {
        //printf("end falling %i\n", falling_node->getFrame());
        // begin
        fallingFrames++;
        if (fallingFrames%END_FALLING_FACTOR==0 && fallingFrames>=FALLING_FACTOR*5 && falling_node->getFrame()==5){
            //printf("end falling end = true\n");
            endFallingEnd = true; // end falling
            fallAnimation = false;
        } else if (fallingFrames%END_FALLING_FACTOR==0 && fallingFrames>=FALLING_FACTOR*4){
            falling_node->setFrame(5);
        }
    }
    else {
        falling_node->setFrame(0);
        fallingFrames = 0;
    }
    if (facingRight) {
        falling_node->flipHorizontal(0);
        falling_node->cugl::Node::setAnchor(0.75f, 0.7f);
    } else {
        falling_node->flipHorizontal(1);
        falling_node->cugl::Node::setAnchor(0.25f, 0.7f);
    }
    current_node->setPosition(0.0f, 0.0f);
}

/**
 * Climbing animation
 */
void Character::animateClimbing(bool on) {
    if (on) {
        //printf("climbing\n");
        climbingFrames++;
        if (climbingFrames % CLIMBING_FACTOR == 0) {
            climbingFrames = 0;
            int nextFrame = (climbing_node->getFrame() + 1) % CLIMBING_FRAMES;
            climbing_node->setFrame(nextFrame);
        }
    } else {
        climbing_node->setFrame(0);
        climbingFrames = 0;
    }
    if (facingRight) {
        climbing_node->flipHorizontal(0);
    } else {
        climbing_node->flipHorizontal(1);
    }
}

/**
 * Pickup animation
 */
void Character::animatePickup(bool on) {
    if (on) {
        //printf("picking up\n");
        pickupFrames++;
        if (pickupFrames % PICKUP_FACTOR == 0 && pickup_node->getFrame()==PICKUP_FRAMES-1){
            // end pickup
            pickingUpObject = false;
        }
        if (pickingUpObject && pickupFrames % PICKUP_FACTOR == 0) {
            pickupFrames = 0;
            int nextFrame = (pickup_node->getFrame() + 1) % PICKUP_FRAMES;
            pickup_node->setFrame(nextFrame);
        }
    } else {
        pickup_node->setFrame(0);
        pickupFrames = 0;
    }
    if (facingRight) {
        pickup_node->flipHorizontal(0);
    } else {
        pickup_node->flipHorizontal(1);
    }
}

/**
 * Pause character movement while layer switches
 */
void Character::changeSpeed(float speed){
    moveSpeed = speed;
}




