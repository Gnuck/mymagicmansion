//
//  Door.cpp
//  RocketDemo
//
//  Created by Nicholas Treat Milner-Gonzales on 3/27/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "Door.hpp"
#include "App.h"
#include "Constants.h"

bool Door::init(Vec2 pos, Vec2 size, Vec2 offset, std::string id, std::string connecting, int layer, std::string textureName) {
    // The size of the door should be a constant.
    doorSize = size;
    if (BoxObstacle::init(Vec2(pos.x + doorSize.x/2, pos.y + doorSize.y/4), Size(doorSize.x/8, doorSize.y/4))) {
        setName(id);
        setSensor(true);
        setBodyType(b2_staticBody);
        //setDebugColor(Color4::ORANGE);
        Door::connecting = connecting;
        
        // For scene graph node
        std::shared_ptr<Texture> texture = App::AssetManager->get<Texture>(textureName);
        node = PolygonNode::allocWithTexture(texture);
        node->setScale(doorSize.x / texture->getWidth(),
                       doorSize.y / texture->getHeight());
        node->setPosition(pos - offset + doorSize/2);
        doorLoc=node->getPosition();
        Door::layer = layer;
        
        
        return true;
    }
    
    return false;
}


std::string Door::getConnecting(){
    return connecting;
}

Vec2 Door::getSize(){
    return doorSize;
}

void Door::destroy() {
    node->dispose();
}
