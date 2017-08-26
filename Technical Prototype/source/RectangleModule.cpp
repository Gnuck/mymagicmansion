//
//  RectangleModule.cpp
//  RocketDemo
//
//  Created by Jiacong Xu on 3/21/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "RectangleModule.hpp"
#include "App.h"

using namespace cugl;
using namespace std;

std::shared_ptr<Node> RectangleModule::generateNewNode(int i, int j) {
    /*
     Creates a new Node from the current configurations (public attributes). 
     This translates to simply creating an image node.
     */
    Vec2 center = Vec2(container.getMidX(), container.getMidY());
    shared_ptr<Texture> texture = App::AssetManager->get<Texture>(textureName);
    shared_ptr<PolygonNode> result = PolygonNode::allocWithTexture(texture);
    result->setScale(container.size.width / texture->getWidth(),
                     container.size.height / texture->getHeight());
    result->setPosition(center - Vec2(METERS_PER_TILE / 2, METERS_PER_TILE / 2) - Vec2(i*METERS_PER_TILE, j*METERS_PER_TILE));
    
    return result;
}

std::shared_ptr<AnimationNode> RectangleModule::generateNewAnimationNode(int i, int j, bool isAnimation, int frames) {
    /*
     Creates a new Node from the current configurations (public attributes).
     This translates to simply creating an image node.
     */
    Vec2 center = Vec2(container.getMidX(), container.getMidY());
    shared_ptr<Texture> texture = App::AssetManager->get<Texture>(textureName);
    shared_ptr<AnimationNode> result = AnimationNode::alloc(texture, 1, frames, frames);
    result->setScale(container.size.width / (texture->getWidth()/frames),
                     container.size.height / texture->getHeight());
    result->setPosition(center - Vec2(METERS_PER_TILE / 2, METERS_PER_TILE / 2) - Vec2(i*METERS_PER_TILE, j*METERS_PER_TILE));
    
    return result;
}

std::vector<std::shared_ptr<Obstacle>> RectangleModule::generateNewColliders() {
    /*
     This just returns a properly configured box, since that is our floor.
     */
    Vec2 center = Vec2(container.getMidX(), container.getMidY());
    shared_ptr<Obstacle> collider = BoxObstacle::alloc(center, container.size);
    collider->setBodyType(b2_staticBody);
    //collider->setDebugColor(Color4::BLUE);
    
    vector<shared_ptr<Obstacle>> result = vector<shared_ptr<Obstacle>>();
    result.push_back(collider);
    
    return result;
}
