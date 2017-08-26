//
//  Collectable.cpp
//  RocketDemo
//
//  Created by Nicholas Treat Milner-Gonzales on 3/26/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "Collectible.hpp"
#include "App.h"
#include "Constants.h"

bool Collectible::init(Vec2 pos, Vec2 offset, std::string name) {
    // The size of the door should be a constant.
    if (BoxObstacle::init(pos + collSize/2, collSize)) {
        std::string dash = "-";
        setName(COLLECTIBLE+dash+name); // Name of collectible should be "key-tag" where "tag" should be a color
        setSensor(true);
        setBodyType(b2_staticBody);
        
        // For scene graph node
        textureName = COLLECTIBLE+dash+name;
        std::shared_ptr<Texture> texture = App::AssetManager->get<Texture>(textureName);
        node = PolygonNode::allocWithTexture(texture);
        node->setScale(collSize.x / texture->getWidth(),
                       collSize.y / texture->getHeight());
        node->setPosition(pos - offset + collSize/2);
        return true;
    }
    
    return false;
}

void Collectible::destroy() {
    node->removeFromParent();
    node->dispose();
    markRemoved(true);
}
