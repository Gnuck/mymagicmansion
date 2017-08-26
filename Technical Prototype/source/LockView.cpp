//
//  LockView.cpp
//  RocketDemo
//
//  Created by Jiacong Xu on 5/5/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "LockView.hpp"
#include "App.h"

using namespace cugl;

bool LockView::initWithPosition(const Vec2 &pos) {
    bool success = cugl::Node::initWithPosition(pos);
    
    if (success) {
        std::shared_ptr<Texture> arch = App::AssetManager->get<Texture>("lock-arch");
        archNode = PolygonNode::allocWithTexture(arch);
        archNode->setScale(0.01, 0.01);
        archNode->setPosition(0, 0.125);
        
        std::shared_ptr<Texture> body = App::AssetManager->get<Texture>("lock-body");
        bodyNode = PolygonNode::allocWithTexture(body);
        bodyNode->setAnchor(0, 1);
        bodyNode->setScale(0.01, 0.01);
        bodyNode->setPosition(-0.225, 0.08);
        
        lockImageNode = Node::allocWithPosition(0, 1.5);
        lockImageNode->addChild(archNode);
        lockImageNode->addChild(bodyNode);
        
        std::shared_ptr<Texture> lockDecor = App::AssetManager->get<Texture>("tile-border");
        auto borderNode = PolygonNode::allocWithTexture(lockDecor);
        borderNode->setScale(METERS_PER_TILE / lockDecor->getWidth(), METERS_PER_TILE / lockDecor->getHeight());
        
        auto bgNode = PolygonNode::alloc(Rect(0, 0, 5, 5));
        bgNode->setColor(Color4(0, 0, 0, 128));
        
        addChild(bgNode);
        addChild(borderNode);
        addChild(lockImageNode);
    }
    
    return success;
}

Color4 LockView::getLockColor() {
    return archNode->getColor();
}

void LockView::setLockColor(Color4 color) {
    archNode->setColor(color);
    bodyNode->setColor(color);
}

void LockView::unlock(std::function<void(void)> callback) {
    App::AnimationController.animate<float>([this] (float v) {
        bodyNode->setPositionY(v);
    }, bodyNode->getPositionY(), bodyNode->getPositionY() + 0.02, 0.15, AbstractAnimation::Linear, [this, callback] {
        App::AnimationController.animate<float>([this] (float v) {
            bodyNode->setPositionY(v);
        }, bodyNode->getPositionY(), bodyNode->getPositionY() - 0.06, 0.2, AbstractAnimation::Linear, callback);
        
        App::AnimationController.animate<float>([this] (float v) {
            bodyNode->setAngle(v);
        }, bodyNode->getAngle(), -0.2, 0.3, AbstractAnimation::Linear);
        
    });
    
    App::AnimationController.animate<float>([this] (float v) {
        archNode->setPositionY(v);
    }, archNode->getPositionY(), archNode->getPositionY() - 0.02, 0.15, AbstractAnimation::Linear, [this] {
        App::AnimationController.animate<float>([this] (float v) {
            archNode->setPositionY(v);
        }, archNode->getPositionY(), archNode->getPositionY() + 0.06, 0.2, AbstractAnimation::Linear);
    });
}

void LockView::shake() {
    // Vibrate for a bit.
    App::AnimationController.animate<float>([this] (float v) {
        lockImageNode->setPositionX(v);
    }, lockImageNode->getPositionX(), 0.1, 0.05, AbstractAnimation::EaseOut, [this] {
        App::AnimationController.animate<float>([this] (float v) {
            lockImageNode->setPositionX(v);
        }, lockImageNode->getPositionX(), 0, 0.2, AbstractAnimation::Spring);
    });
}
