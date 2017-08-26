//
//  LayerView.hpp
//  RocketDemo
//
//  Created by Jiacong Xu on 4/29/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef LayerView_hpp
#define LayerView_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "TileView.hpp"
#include "Tile.hpp"

/**
 This class is responsible in correctly arranging tiles within a layer. This is
 used in conjunction with the level view to achieve layer switching animations.
 */
class LayerView : public cugl::Node {
private:
    /** The tiles in this layer. */
    std::vector<std::shared_ptr<TileView>> tileViews;
    
    /** Returns the proper rectangle of a tile at index x and y. */
    Rect rect4Tile(int x, int y, bool isMinimized);
    
    /** Returns whether the tile is top or bottom row. */
    int getTileY(std::shared_ptr<TileView> t);
    
    /**
     The height of a thin strip in minimized layers. This is in meters. Note we
     divide this into two.
     */
    const float MINIMIZED_STRIP_HEIGHT = 0.25;
    
    /** The duration of layer switching animation. */
    const float ANIM_DURATION = 0.15;
    
    
public:
    LayerView() {}
    
    ~LayerView() {}
    
    /**
     The default allocator. Since we will be moving tiles vertically, it is the
     easiest to have the origin at the center of the screen.
     */
    static std::shared_ptr<LayerView> allocWithBounds(Vec2 size) {
        std::shared_ptr<LayerView> obj = std::make_shared<LayerView>();
        return obj->initWithBounds(size)? obj : nullptr;
    }
    
    /**
     This method traverses through all the tiles and add the nodes to the view
     hierarchy.
     */
    void addTiles(std::vector<std::vector<std::shared_ptr<Tile>>> tiles);
    
    /** 
     Shrink or expand this layer to or from a thin line.
     */
    void toggleLayerVisibility(bool shouldMinimize, bool isAnimated);
};

#endif /* LayerView_hpp */
