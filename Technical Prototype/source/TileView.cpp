//
//  TileView.cpp
//  RocketDemo
//
//  Created by Jiacong Xu on 4/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "TileView.hpp"
#include "App.h"
#include "AbstractAnimation.hpp"

using namespace cugl;

bool TileView::initWithPosition(const Vec2 &pos) {
    bool status = Node::initWithPosition(pos);
    
    if (status) {
        mainNode = Node::alloc();
        
        backgroundNode = Node::alloc();
        foregroundNode = Node::alloc();
        
        // The strip needs to be slightly smaller (same gap as mask) to look good
        stripContainerNode = Node::alloc();
        Rect stripRect = Rect(METERS_PER_TILE * 12 / 512 / 2, 0,
                              METERS_PER_TILE * 500 / 512, 5);
        minimizedStripNode = PolygonNode::alloc(stripRect);
        stripMaskNode = Node::alloc();
        auto stripColorMaskNode = PolygonNode::alloc(stripRect);
        stripColorMaskNode->setColor(Color4(0, 0, 0, 128));
        stripMaskNode->addChild(stripColorMaskNode);
        stripContainerNode->addChild(minimizedStripNode);
        stripContainerNode->addChild(stripMaskNode);
        
        lockNode = LockView::allocWithPosition(Vec2(0, 0));
        bgColorNode = PolygonNode::alloc(Rect(0, 0, 5, 5));
        
        
        std::shared_ptr<Texture> mask = App::AssetManager->get<Texture>("tile-mask");
        maskNode = PolygonNode::allocWithTexture(mask);
        
        // We want to display 5 by 5, but the image is some other scale.
        maskNode->setScale(METERS_PER_TILE / mask->getWidth(), METERS_PER_TILE / mask->getHeight());
        
        backgroundNode->addChildWithName(bgColorNode, "Background Color Node");
        mainNode->addChildWithName(backgroundNode, "Background Node");
        mainNode->addChildWithName(foregroundNode, "Foreground Node");
        mainNode->addChildWithName(lockNode, "Lock Node");
        mainNode->addChildWithName(maskNode, "Mask");
        addChild(mainNode);
        addChild(stripContainerNode);
    }
    
    return status;
}

bool TileView::init() {
    return initWithPosition(cugl::Vec2::ZERO);
}

void TileView::setNodeBackgroundColor(Color4 color) {
    bgColorNode->setColor(color);
    minimizedStripNode->setColor(Color4(color.r, color.g, color.b, 0));
    themeColor = color;
}

void TileView::setVisibility(TileDisplayMode mode, bool animated) {
    /*
     We do the following animation:
     Scale foreground + background to the desired size
     Toggle alpha for foreground and background (minimized = 0, full = 1)
     Toggle alpha for minimized strip.
     Scale + translate minimized strip properly.
     
     On scaling: nodes are scaled to 1 by default. So if we have 512 pixels and
     wants to shrink it to 24, we just take the ratio.
     */
    
    float endScale = mode == Full? 1 : (MINIMIZED_STRIP_HEIGHT / METERS_PER_TILE / 2);
    
    Color4 startColor = mode == Full? Color4(255, 255, 255, 0) : Color4::WHITE;
    Color4 endColor = mode == Full? Color4::WHITE : Color4(255, 255, 255, 0);
    
    Color4 endStripColor = mode == Full? Color4::CLEAR : bgColorNode->getColor();
    
    App::AnimationController.animate<Color4>([this] (Color4 c) {
        mainNode->setColor(c);
    }, mainNode->getColor(), endColor, animated? ANIM_DURATION : 0);
    
    App::AnimationController.animate<float>([this] (float s) {
        setScale(1, s);
    }, getScaleY(), endScale, animated? ANIM_DURATION : 0);
    
    App::AnimationController.animate<Color4>([this] (Color4 c) {
        stripContainerNode->setColor(c);
    }, stripContainerNode->getColor(), startColor, animated? ANIM_DURATION : 0);
    
    App::AnimationController.animate<Color4>([this] (Color4 c) {
        minimizedStripNode->setColor(c);
    }, minimizedStripNode->getColor(), endStripColor, animated? ANIM_DURATION : 0);
}

void TileView::setLocked(bool isLocked, bool animated) {
    Color4 endLockColor = isLocked? Color4::WHITE : Color4::CLEAR;
    Color4 startLockColor = isLocked? Color4::CLEAR : Color4::WHITE;
    
    if (isLocked) {
        App::AnimationController.animate<Color4>([this] (Color4 color) {
            lockNode->setColor(color);
            stripMaskNode->setColor(color);
        }, startLockColor, endLockColor, animated? ANIM_DURATION : 0);
    }
    
    // Tell the lock to animate itself
    if (!isLocked && animated)
        lockNode->unlock([this, startLockColor, endLockColor] {
            App::AnimationController.animate<Color4>([this] (Color4 color) {
                lockNode->setColor(color);
                stripMaskNode->setColor(color);
            }, startLockColor, endLockColor, ANIM_DURATION);
        });
    else {
        stripMaskNode->setColor(endLockColor);
        lockNode->setColor(endLockColor);
    }
}

void TileView::setLockVisibility(bool isVisible) {
    isLockVisible = isVisible;
    lockNode->setVisible(isVisible);
}

void TileView::setLockColorOnce(Color4 color) {
    if (didSetLockColor)
        return;
    
    didSetLockColor = true;
    lockNode->setLockColor(color);
}
