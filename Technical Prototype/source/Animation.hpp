//
//  Animation.hpp
//  RocketDemo
//
//  Created by Jiacong Xu on 4/29/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Animation_hpp
#define Animation_hpp

#include <stdio.h>
#include <math.h>
#include <memory>
#include <functional>
#include "AbstractAnimation.hpp"

/**
 This class represents an animation maintained by the AnimationController.
 Individual animations can be manipulated at any time while still being
 animated.
 
 This class can be used by itself without the AnimationController, but it has to
 be manually updated.
 */
template <typename T>
class Animation : public AbstractAnimation {
public:
    /** The starting value of the animation. */
    T start;
    
    /** The ending value of the animation. */
    T end;
    
    /** The setter function pointer. */
    std::function<void (T)> setter;
    
    /**
     The curving function pointer from elapsed time to interpolation amount.
     */
    std::function<float (float, float)> curve;
    
    /**
     The call back function pointer to invoke after the animation is finished.
     */
    std::function<void (void)> callback;
    
    /**
     Default allocator.
     */
    static std::shared_ptr<Animation> alloc() {
        std::shared_ptr<Animation> anim = std::make_shared<Animation>();
        
        return anim->init()? anim : nullptr;
    }
    
    /**
     Initializes a new Animation container.
     */
    bool init(std::function<void (T)> setter, T start, T end, float duration,
              std::function<float (float, float)> curve = Linear) {
        // Check if multiplication and addition work
        try {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-value"
            0.5 * start + 0.5 * end;
#pragma clang diagnostic pop
        } catch (...) {
            printf("Type cannot be animated - check both float scaling and addition for that type are overriden.");
            return false;
        }
        
        this->setter = setter;
        this->start = start;
        this->end = end;
        this->duration = duration;
        this->curve = curve;
        
        elapsed = 0;
        speed = 1;
        isPlaying = false;
        
        return true;
    }
    
    
    void step(float dt) {
        if (!isPlaying || elapsed > duration) {
            return;
        }
        
        elapsed += speed * dt;
        
        if (elapsed > duration) {
            if (isLooping) {
                elapsed = 0;
                setter(start);
            } else {
                complete();
            }
        } else {
            float a = curve(elapsed, duration);
            T value = start * (1 - a) + end * a;
            setter(value);
        }
    }
    
    void beginAnimation() {
        isPlaying = true;
        setter(start);
    }
    
    /** Immediately completes this animation. */
    void complete() {
        elapsed = duration + 0.01;
        setter(end);
        isPlaying = false;
        
        if (callback != nullptr) {
            callback();
        }
    }
    
};

#endif /* Animation_hpp */
