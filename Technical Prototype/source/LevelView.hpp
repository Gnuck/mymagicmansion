//
//  LevelView.hpp
//  RocketDemo
//
//  Created by Jiacong Xu on 4/29/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef LevelView_hpp
#define LevelView_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "LayerView.hpp"

/**
 This class is responsible in correctly arranging the layers of tiles in the 
 game. Its most important function is handling animation between layer switches.
 */
class LevelView : public cugl::Node {
private:
    /** The layers in this view. */
    std::vector<std::shared_ptr<LayerView>> layerViews;
    
    /** The currently active layer */
    int activeLayer = -1;
    
    /** Calculate the ith layer's y position considering the active layer. */
    float getLayerY(int i);
    
    /** The duration of layer switching animation. */
    const float ANIM_DURATION = 0.15;
    
    
public:
    LevelView() {}
    
    ~LevelView() {}
    
    /**
     The default allocator. Since we will be moving layers vertically, it is the
     easiest to have the origin at the center of the screen.
     */
    static std::shared_ptr<LevelView> allocWithBounds(Vec2 size) {
        std::shared_ptr<LevelView> obj = std::make_shared<LevelView>();
        return obj->initWithBounds(size)? obj : nullptr;
    }
    
    /**
     This method traverses through all the tiles and add the nodes to the view
     hierarchy.
     */
    void addTiles(std::vector<std::vector<std::vector<std::shared_ptr<Tile>>>> tiles);
    
    /**
     Shows the active layer and minimizes the rest above or below it.
     */
    void switchLayers(int active, bool animated, std::function<void (void)> callback);
};

#endif /* LevelView_hpp */
