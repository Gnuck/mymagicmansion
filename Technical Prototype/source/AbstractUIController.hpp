//
//  AbstractUIController.hpp
//  RocketDemo
//
//  Created by Andy Jiang on 2017/04/16.
//  Copyright © 2017年 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef AbstractUIController_hpp
#define AbstractUIController_hpp

#include <cugl/cugl.h>
#include "AbstractController.hpp"
#include "Tap.hpp"

/**
 A template for a controller, including methods call to update and draw.
 */
class AbstractUIController : public AbstractController {

protected:
    /**
     The root scene object for the UI.
     */
    std::shared_ptr<cugl::Scene> uiScene;
    
    std::vector<std::shared_ptr<Tap>> heldTaps;
    
    virtual bool handleUIEvent(int tag) = 0;
    
public:
    /**
     The update loop which is a core part of all controllers.
     */
    void update(float timestep);
    
    /**
     The draw loop which the controller can tell the models it maintains to draw
     */
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch) {
        uiScene->render(batch);
    };
};

#endif /* AbstractUIController_hpp */
