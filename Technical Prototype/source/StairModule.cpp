//
//  StairModule.cpp
//  RocketDemo
//
//  Created by Kathryn Angela Stinebruner on 3/23/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "StairModule.hpp"

#include "App.h"

using namespace cugl;
using namespace std;

std::shared_ptr<Node> StairModule::generateNewNode(int i, int j) {
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

std::vector<std::shared_ptr<Obstacle>> StairModule::generateNewColliders() {
    /*
     This returns a properly configured set of boxes as our stairs.
     */
    
    if (direction==0) {  //right stairs
        int boxes = 0;
        vector<shared_ptr<Obstacle>> result = vector<shared_ptr<Obstacle>>();
        Vec2 block_size = Vec2(container.size.width/11.0, container.size.height/11.0);
        //printf("x: %f to %f by %f\n", container.getMinX()+block_size.x, container.getMaxX()+block_size.x, block_size.x);
        //printf("y: %f to %f by %f\n", container.getMinY()+block_size.y, boxes*block_size.y+block_size.x, block_size.y);
        for (float x=container.getMinX(); x<container.getMaxX()+0.01; x=x+block_size.x){
            for (float y=container.getMinY()+block_size.y; y<container.getMinY()+boxes*block_size.y+0.01; y=y+block_size.y){
                shared_ptr<Obstacle> collider = BoxObstacle::alloc(Vec2(x-block_size.x/2,y-block_size.y/2), block_size);
                collider->setBodyType(b2_staticBody);
                //collider->setDebugColor(Color4::BLUE);
                result.push_back(collider);
            }
            boxes++;
        }
        /** If you want to see the container around the stairs: */
        /*Vec2 center = Vec2(container.getMidX(), container.getMidY());
         shared_ptr<Obstacle> collider = BoxObstacle::alloc(center, container.size);
         collider->setBodyType(b2_staticBody);
         collider->setDebugColor(Color4::BLUE);
         
         result.push_back(collider);*/
        return result;
    }
    else { //left stairs
        int boxes = 11;
        vector<shared_ptr<Obstacle>> result = vector<shared_ptr<Obstacle>>();
        Vec2 block_size = Vec2(container.size.width/11.0, container.size.height/11.0);
        //printf("x: %f to %f by %f\n", container.getMinX()+block_size.x, container.getMaxX()+block_size.x, block_size.x);
        //printf("y: %f to %f by %f\n", container.getMinY()+block_size.y, boxes*block_size.y+block_size.x, block_size.y);
        for (float x=container.getMinX()+block_size.x; x<container.getMaxX()+block_size.x; x=x+block_size.x){
            for (float y=container.getMinY()+block_size.y; y<container.getMinY()+boxes*block_size.y+0.01; y=y+block_size.y){
                shared_ptr<Obstacle> collider = BoxObstacle::alloc(Vec2(x-block_size.x/2,y-block_size.y/2), block_size);
                collider->setBodyType(b2_staticBody);
                //collider->setDebugColor(Color4::BLUE);
                result.push_back(collider);
            }
            boxes--;
        }
        /** If you want to see the container around the stairs: */
        /*Vec2 center = Vec2(container.getMidX(), container.getMidY());
         shared_ptr<Obstacle> collider = BoxObstacle::alloc(center, container.size);
         collider->setBodyType(b2_staticBody);
         collider->setDebugColor(Color4::BLUE);
         
         result.push_back(collider);*/
        return result;
    }
}

void StairModule::setDirection(int dir){
    direction = dir;
}
