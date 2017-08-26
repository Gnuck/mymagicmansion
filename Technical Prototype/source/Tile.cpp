//
//  Tile.cpp
//  RocketDemo
//
//  Created by Kathryn Angela Stinebruner on 3/11/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "Tile.hpp"
#include "App.h"
#include "LockColor.hpp"

using namespace cugl;

void Tile::init(cugl::Vec2 position, Color4f color) {
    this->position = position;
    this->lockedBy = "";
    colliders = std::vector<std::shared_ptr<Obstacle>>();
    tileNode = TileView::allocWithPosition(position);
    tileNode->setNodeBackgroundColor(color);
    foregroundNode = tileNode->foregroundNode;
    backgroundNode = tileNode->backgroundNode;
}

void Tile::dispose() {
    // Tile node is a smart pointer, so we just have to set it to null. The node
    // is created by level loader and transferred to the scene graph.
    tileNode = nullptr;
    
    // The list of colliders is more annoying. All the colliders are kept by
    // box2d world so we are just keeping a list of weak references to them. In
    // addition they are smart pointers. So we just clear our references.
    colliders.clear();
}

Vec2 Tile::getPosition() {
    return position;
}


void Tile::setPosition(Vec2 position) {
    Vec2 offset = position - this->position;
    this->position = position;
    
    // Now we need to move everything in scene graph node and physics. Start
    // with scene graph.
    if (tileNode != nullptr) {
        tileNode->setPosition(tileNode->getPosition() + offset);
    }
    
    for (std::shared_ptr<Obstacle> o : colliders) {
        o->setPosition(o->getPosition() + offset);
    }
}

bool Tile::contains(cugl::Vec2 pos) {
    return position.x - METERS_PER_TILE / 2 < pos.x && position.x + METERS_PER_TILE / 2 > pos.x
        && position.y - METERS_PER_TILE / 2 < pos.y && position.y + METERS_PER_TILE / 2 > pos.y;
}

void Tile::setActive(bool isActive) {
    // We might want an animation on this. In that case the visibility should
    // not be instantly turned off, but some kind of transition is allowed. We
    // should let the tileNode, the view, decide how to animate this.
    // This is handled by another view, so we don't worry about it here.
    // tileNode->setVisible(isActive);
    
    // The physics should be toggled immediately though. If we have animation,
    // this means the character might be shifting while we are still animating.
    // This can potentially be a problem.
    for (std::shared_ptr<Obstacle> collider : colliders) {
        collider->setActive(isActive);
    }
}

bool Tile::isLocked() {
    return !lockedBy.empty();
}

std::string Tile::getLockKey() {
    return lockedBy;
}

void Tile::setLocked(std::string lockedBy, bool animated) {
    this->lockedBy = lockedBy;
    Color4 lockColor = LockColor::getLockColor(lockedBy);
    tileNode->setLockColorOnce(lockColor);
    tileNode->setLocked(isLocked(), animated);
}
