//
//  Tap.cpp
//  RocketDemo
//
//  Created by Andy Jiang on 2017/03/24.
//  Copyright © 2017年 Game Design Initiative at Cornell. All rights reserved.
//

#include "Tap.hpp"

using namespace cugl;

void Tap::init(long id, Vec2 position) {
    this->id = id;
    this->position = position;
}

void Tap::dispose() {
    this->id = -1;
    this->position = Vec2::ZERO;
}
