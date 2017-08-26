//
//  Swipe.cpp
//  RocketDemo
//
//  Created by Andy Jiang on 2017/03/26.
//  Copyright © 2017年 Game Design Initiative at Cornell. All rights reserved.
//

#include "Swipe.hpp"

using namespace cugl;

void Swipe::init(long id, Vec2 initialPosition, Vec2 finalPosition) {
    this->id = id;
    this->initialPosition = initialPosition;
    this->finalPosition = finalPosition;
}

void Swipe::dispose() {
    this->id = -1;
    this->initialPosition = Vec2::ZERO;
    this->finalPosition = Vec2::ZERO;
}
