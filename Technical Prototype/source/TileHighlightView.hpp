//
//  TileHighlightView.hpp
//  RocketDemo
//
//  Created by Jiacong Xu on 4/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef TileHighlightView_hpp
#define TileHighlightView_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "Tile.hpp"

/**
 This class displays a highlight border around the tile that is selected. It
 contains some simple animations.
 */
class TileHighlightView : public cugl::PolygonNode {
private:
    /**
     The default scale for this node. Used to restore it to original size.
     */
    cugl::Vec2 nodeDefaultScale;
    
    /** The initial scale of the highlight animation. */
    const float INIT_SCALE = 1.1;
    
    /** The duration of the highlight animation. */
    const float ANIM_TIME = 0.1;
    
public:
    /**
     Allocate and initialize a new highlight view. A newly initialized highlight
     view is hidden.
     */
    static std::shared_ptr<TileHighlightView> allocWithTexture(std::shared_ptr<cugl::Texture> texture) {
        auto node = std::make_shared<TileHighlightView>();
        
        return node->initWithTexture(texture) ? node : nullptr;
    }
    
    /**
     Initializes this node with the given texture.
     */
    bool initWithTexture(const std::shared_ptr<cugl::Texture>& texture);
        /**
     Moves the selection border to that tile.
     */
    void highlightTile(std::shared_ptr<Tile> tile, bool animated);
    
    /**
     Hides the highlight node.
     */
    void removeHighlight(bool animated);
};

#endif /* TileHighlightView_hpp */
