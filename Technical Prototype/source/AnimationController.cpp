//
//  AnimationController.cpp
//
//  Created by Jiacong Xu on 4/29/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "AnimationController.hpp"

bool AnimationController::init() {
    animations = std::vector<std::shared_ptr<AbstractAnimation>>();
    
    return true;
}

void AnimationController::update(float dt) {
    auto expired = std::vector<std::shared_ptr<AbstractAnimation>>();
    
    for (std::shared_ptr<AbstractAnimation> anim : animations) {
        anim->step(dt);
        
        if (anim->isComplete()) {
            // Remove this animation from the loop.
            expired.push_back(anim);
        }
    }
    
    // Delete expired animations from the list.
    for (std::shared_ptr<AbstractAnimation> anim : expired) {
        animations.erase(std::remove(animations.begin(), animations.end(), anim), animations.end());
    }
}
