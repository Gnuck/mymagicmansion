//
//  AbstractAnimation.hpp
//  RocketDemo
//
//  Created by Jiacong Xu on 4/29/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef AbstractAnimation_hpp
#define AbstractAnimation_hpp

#include <stdio.h>
#include <algorithm>
#include <math.h>

/**
 This is used to abstract out different types of values being animated. By using
 this class we can store all Animations in a vector.
 */
class AbstractAnimation {
protected:
    /** The scaled, elapsed time since the start of animation. */
    float elapsed;
    
public:
    /** The total duration of the animation. */
    float duration;
    
    /** The speed of the animation. */
    float speed;
    
    /** Whether the animation should loop. */
    bool isLooping;
    
    /** Whether the animation is playing. */
    bool isPlaying;
    
    /**
     Steps the animation.
     */
    virtual void step(float dt) = 0;
    
    /**
     Initializes the animation and begins the animation.
     */
    virtual void beginAnimation() = 0;
    
    /**
     Whether the animation is complete.
     */
    bool isComplete() {
        return elapsed > duration;
    }
    
    /*
     Below are a list of simple animation curves.
     */
    static float Linear(float t, float duration) {
        return t / duration;
    }
    
    static float EaseOut(float t, float duration) {
        return pow(t / duration, 0.2f);
    }
    
    static float EaseIn(float t, float duration) {
        return -pow(1 - t / duration, 0.2f) + 1;
    }

    
    static float EaseInOut(float t, float duration) {
        float s = t / duration * 2;
        
        if (s > 1)
            return pow(s - 1, 0.2f) / 2 + 0.5;
        else
            return -pow(1 - s, 0.2f) / 2 + 0.5;
        
    };
    
    static float Spring(float t, float duration) {
        t /= duration;
        float e = 2.71828;
        
        return 1 - pow(e, -6 * t) * (sin(12 * t) + 2 * cos(12 * t)) / 2;
    }
};

#endif /* AbstractAnimation_hpp */
