//
//  AbstractUIController.cpp
//  RocketDemo
//
//  Created by Andy Jiang on 2017/04/16.
//  Copyright © 2017年 Game Design Initiative at Cornell. All rights reserved.
//

#include "App.h"
#include "AbstractUIController.hpp"

void AbstractUIController::update(float timestep) {
    if (App::InputController.heldTapReady()) {
        Vec3 worldPos = uiScene->screenToWorldCoords(App::InputController.peekHeldTap()->position);
        Vec2 coord = Vec2(worldPos.x, worldPos.y);
        
        for (std::shared_ptr<Node> child : this->uiScene->getChildren()) {
            if (!child->isVisible()) {
                continue;
            }
            
            Rect rect = child->getBoundingBox();
            if (rect.contains(coord) && App::InputController.heldTapReady() && child->getTag() != 0) {
                heldTaps.push_back(App::InputController.popHeldTap());
            }
            
            Vec2 pos = child->worldToNodeCoords(coord);
            for (std::shared_ptr<Node> button : child->getChildren()) {
                rect = button->getBoundingBox();
                if (rect.contains(pos) && App::InputController.heldTapReady() && button->getTag() != 0) {
                    heldTaps.push_back(App::InputController.popHeldTap());
                }
            }
        }

    }
    
    int i;
    for (i = 0; i < heldTaps.size(); i++) {
        long id = heldTaps[i]->id;
        if (App::InputController.releasedSwipeReady(id)) {
            break;
        } else if (App::InputController.releasedTapReady(id)) {
            auto tap = App::InputController.consumeReleasedTap(id);
            Vec3 worldPos = uiScene->screenToWorldCoords(tap->position);
            Vec2 coord = Vec2(worldPos.x, worldPos.y);
            
            bool processed = false;
            for (std::shared_ptr<Node> child : this->uiScene->getChildren()) {
                if (!child->isVisible()) {
                    continue;
                }
                
                Rect rect = child->getBoundingBox();
                if (rect.contains(coord)) {
                    if (handleUIEvent(child->getTag())) {
                        break;
                    }
                }
                
                Vec2 pos = child->worldToNodeCoords(coord);
                for (std::shared_ptr<Node> button : child->getChildren()) {
                    rect = button->getBoundingBox();
                    if (rect.contains(pos)) {
                        if (handleUIEvent(button->getTag())) {
                            processed = true;
                            break;
                        }
                    }
                }
                
                if (processed) {
                    break;
                }
            }
            break;
        }
    }
    
    if (i < heldTaps.size()) {
        heldTaps.erase(heldTaps.begin() + i);
    }
};
