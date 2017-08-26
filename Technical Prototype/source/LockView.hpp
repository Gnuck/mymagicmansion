//
//  LockView.hpp
//  RocketDemo
//
//  Created by Jiacong Xu on 5/5/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef LockView_hpp
#define LockView_hpp

#include <stdio.h>
#include <cugl/cugl.h>

class LockView : public cugl::Node {
private:
    /**
     The arch of the lock.
     */
    std::shared_ptr<cugl::Node> archNode;
    
    /**
     The body of the lock.
     */
    std::shared_ptr<cugl::Node> bodyNode;
    
    /**
     Container for those two.
     */
    std::shared_ptr<cugl::Node> lockImageNode;
    
public:
    /**
     Default allocator. Configures a new lock with given position.
     */
    static std::shared_ptr<LockView> allocWithPosition(cugl::Vec2 pos) {
        std::shared_ptr<LockView> obj = std::make_shared<LockView>();
        
        return obj->initWithPosition(pos)? obj : nullptr;
    }
    
    /**
     Default initializer.
     */
    virtual bool initWithPosition(const cugl::Vec2& pos);
    
    /**
     Returns the currently set lock color.
     */
    cugl::Color4 getLockColor();
    
    /**
     Sets the color of the lock. This differs from setting this node directly to
     allow the two color settings to multiply with each other.
     */
    void setLockColor(cugl::Color4 color);
    
    /**
     Plays the unlock animation. Currently can only be played once.
     */
    void unlock(std::function<void(void)> callback);
    
    /**
     Plays the shaking animation.
     */
    void shake();
};

#endif /* LockView_hpp */
