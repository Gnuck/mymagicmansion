//
//  LockColor.hpp
//  Magic Moving Mansion Mania
//
//  Created by Jiacong Xu on 5/18/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef LockColor_hpp
#define LockColor_hpp

#include <stdio.h>
#include <cugl/cugl.h>

/**
 Defines a list of valid lock colors.
 */
class LockColor {
public:
    static cugl::Color4 White;
    static cugl::Color4 Blue;
    static cugl::Color4 Yellow;
    static cugl::Color4 Pink;
    static cugl::Color4 Green;
    static cugl::Color4 Orange;
    
    /**
     Converts string to color.
     */
    static cugl::Color4 getLockColor(std::string colorString) {
        if (colorString == "green") {
            return Green;
        } else if (colorString == "blue") {
            return Blue;
        } else if (colorString == "yellow") {
            return Yellow;
        } else if (colorString == "pink") {
            return Pink;
        } else if (colorString == "orange") {
            return Orange;
        } else {
            return White;
        }
    }
};
#endif /* LockColor_hpp */
