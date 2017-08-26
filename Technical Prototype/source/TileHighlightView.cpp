//
//  TileHighlightView.cpp
//  RocketDemo
//
//  Created by Jiacong Xu on 4/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "TileHighlightView.hpp"
#include "App.h"
#include "AbstractAnimation.hpp"

using namespace cugl;

bool TileHighlightView::initWithTexture(const std::shared_ptr<Texture> &texture) {
    bool success = TexturedNode::initWithTexture(texture);
    
    if (success) {
        // Initially not visible
        setVisible(false);
        
        // Set scale
        nodeDefaultScale = Vec2(5.12 / texture->getWidth(), 5.12 / texture->getHeight());
        setScale(nodeDefaultScale);
    }
    
    return success;
}

void TileHighlightView::highlightTile(std::shared_ptr<Tile> tile, bool animated) {
    setVisible(true);
    setPosition(tile->tileNode->getPosition());
    
    App::AnimationController.animate<Color4>([this] (Color4 color) {
        this->setColor(color);
    }, Color4::CLEAR, Color4::WHITE, animated? ANIM_TIME : 0, AbstractAnimation::EaseOut);
    
    App::AnimationController.animate<Vec2>([this] (Vec2 scale) {
        this->setScale(scale);
    }, INIT_SCALE * nodeDefaultScale, nodeDefaultScale, animated? ANIM_TIME : 0, AbstractAnimation::EaseOut);
}

void TileHighlightView::removeHighlight(bool animated) {
    App::AnimationController.animate<Color4>([this] (Color4 color) {
        this->setColor(color);
    }, Color4::WHITE, Color4::CLEAR, animated? ANIM_TIME : 0, AbstractAnimation::EaseIn);
    
    App::AnimationController.animate<Vec2>([this] (Vec2 scale) {
        this->setScale(scale);
    }, nodeDefaultScale, INIT_SCALE * nodeDefaultScale, animated? ANIM_TIME : 0,
                                           AbstractAnimation::EaseIn, [this] {
                                               setVisible(false);
                                           });
}
