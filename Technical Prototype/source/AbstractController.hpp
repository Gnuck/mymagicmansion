//
//  AbstractController.hpp
//  RocketDemo
//
//  Created by Jiacong Xu on 3/11/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef AbstractController_hpp
#define AbstractController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Constants.h"

/**
 A template for a controller, including methods call to update and draw.
 */
class AbstractController {
public:
    /**
     The update loop which is a core part of all controllers.
     */
    virtual void update(float timestep) {};
  
    /**
     The draw loop which the controller can tell the models it maintains to draw
     */
    virtual void draw(const std::shared_ptr<cugl::SpriteBatch>& batch) {};
};

#endif /* AbstractController_hpp */
