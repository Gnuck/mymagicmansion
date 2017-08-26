//
//  AnimationController.hpp
//
//  Created by Jiacong Xu on 4/29/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef AnimationController_hpp
#define AnimationController_hpp

#include <stdio.h>
#include "Animation.hpp"
#include <vector>
#include <functional>

/**
 The AnimationController class helps you animating properties by maintaining a
 list of Animations. By providing a function pointer that takes exactly one
 argument (the value being animated) you can leave actual updating and timing to
 this class. Individual Animations can also be sped up, reversed, played with a
 custom time-scale, cancelled, completed instantly, and so on. You can also set
 a handler which will be called when the animation is complete.
 
 Animation works on all types T where T * float and T + T are defined.
 */
class AnimationController {
private:
    /** This is the list of on-going animations. */
    std::vector<std::shared_ptr<AbstractAnimation>> animations;
    
public:
    /**
     Creates a new AnimationController.
     */
    AnimationController() {};
    
    /**
     Stops all ongoing animations and disposes all resources used by this
     controller.
     */
    ~AnimationController() {};
    
    /**
     Initializes the controller to use.
     */
    bool init();
    
    /** 
     Updates every animation by dt seconds. This should be regularly updated in 
     the parent controller.
     */
    void update(float dt);
    
    /**
     Creates a new Animation and immediately animates it.
     
     @param setter The setter function pointer. Create your own with lambda
     expressions if there is none available.
     @param start The starting value of the animation.
     @param end The ending value of the animation.
     @param duration The duration of the animation in seconds.
     @return A pointer to the created Animation for additional manipulation.
     */
    template <typename T>
    std::weak_ptr<Animation<T>> animate(std::function<void(T)> setter, T start,
                                        T end, float duration) {
        return animate(setter, start, end, duration, AbstractAnimation::EaseInOut);
    }
    
    /**
     Creates a new Animation and immediately animates it with a custom timing
     function. The timing function should take a float, the current elapsed 
     seconds since start of animation, and return a float in range [0, 1], with
     0 the start of animation and 1 the end of the animation. If the value is
     out of the range, the animation extrapolate a value outside start and end.
     
     @param setter The setter function pointer. Create your own with lambda
     expressions if there is none available.
     @param start The starting value of the animation.
     @param end The ending value of the animation.
     @param duration The duration of the animation in seconds.
     @param curve The curving function described above.
     @return A pointer to the created Animation for additional manipulation.
     */
    template <typename T>
    std::weak_ptr<Animation<T>> animate(std::function<void(T)> setter, T start,
                                        T end, float duration,
                                        std::function<float (float, float)> curve) {
        return animate(setter, start, end, duration, curve, nullptr);
    }
    
    
    /**
     Creates a new Animation and immediately animates it with a custom timing
     function. The timing function should take a float, the current elapsed
     seconds since start of animation, and return a float in range [0, 1], with
     0 the start of animation and 1 the end of the animation. If the value is
     out of the range, the animation extrapolate a value outside start and end.
     
     @param setter The setter function pointer. Create your own with lambda
     expressions if there is none available.
     @param start The starting value of the animation.
     @param end The ending value of the animation.
     @param duration The duration of the animation in seconds.
     @param curve The curving function described above.
     @param callback The function to call after the animation is complete.
     @return A pointer to the created Animation for additional manipulation.
     */
    template <typename T>
    std::weak_ptr<Animation<T>> animate(std::function<void(T)> setter, T start,
                                        T end, float duration,
                                        std::function<float (float, float)> curve,
                                        std::function<void (void)> callback) {
        std::shared_ptr<Animation<T>> anim = std::make_shared<Animation<T>>();
        bool success = anim->init(setter, start, end, duration, curve);
        anim->callback = callback;
        
        if (!success) {
            return std::weak_ptr<Animation<T>>();
        }
        
        anim->beginAnimation();
        animations.push_back(anim);
        
        std::weak_ptr<Animation<T>> weakAnim = anim;
        
        return weakAnim;
    }
};

#endif /* AnimationController_hpp */
