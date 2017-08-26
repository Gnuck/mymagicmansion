//
//  LayerView.cpp
//  RocketDemo
//
//  Created by Jiacong Xu on 4/29/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "LayerView.hpp"
#include "App.h"

void LayerView::addTiles(std::vector<std::vector<std::shared_ptr<Tile>>> tiles) {
    tileViews = std::vector<std::shared_ptr<TileView>>();
    
    for (int x = 0; x < tiles.size(); x++) {
        for (int y = 0; y < tiles[x].size(); y++) {
            if (tiles[x][y] != nullptr){
                std::shared_ptr<TileView> t = tiles[x][y]->tileNode;
                addChild(t);
                tileViews.push_back(t);
            
                // Reposition to the correct place
                Rect r = rect4Tile(x, y, false);
                t->setPosition(r.getMidX(), r.getMidY());
            }
        }
    }
}

Rect LayerView::rect4Tile(int x, int y, bool isMinimized) {
    if (!isMinimized)
        return Rect(x * METERS_PER_TILE, y * METERS_PER_TILE, METERS_PER_TILE, METERS_PER_TILE);
    else
        return Rect(x * METERS_PER_TILE,
                    getHeight() / 2 + (y - 1) * MINIMIZED_STRIP_HEIGHT / 2,
                    METERS_PER_TILE, MINIMIZED_STRIP_HEIGHT / 2);
}


int LayerView::getTileY(std::shared_ptr<TileView> t) {
    return t->getPositionY() > getHeight() / 2? 1 : 0;
}

void LayerView::toggleLayerVisibility(bool shouldMinimize, bool isAnimated) {
    // We want all top tiles to shrink to some position, then all bottom tiles
    // immediately below.
    float endBottomY = rect4Tile(0, 0, shouldMinimize).getMidY();
    float endTopY = rect4Tile(0, 1, shouldMinimize).getMidY();
    
    for (int i = 0; i < tileViews.size(); i++) {
        std::shared_ptr<TileView> t = tileViews[i];
        TileView::TileDisplayMode mode;
        
        if (getTileY(t) == 0) {
            App::AnimationController.animate<float>([this, t] (float y) {
                t->setPositionY(y);
            }, t->getPositionY(), endBottomY, isAnimated? ANIM_DURATION : 0);
        } else {
            App::AnimationController.animate<float>([this, t] (float y) {
                t->setPositionY(y);
            }, t->getPositionY(), endTopY, isAnimated? ANIM_DURATION : 0);
        }
        
        if (!shouldMinimize) {
            mode = TileView::Full;
        } else if (getTileY(t) == 0) {
            mode = TileView::MinimizedBottom;
        } else {
            mode = TileView::MinimizedTop;
        }
        
        tileViews[i]->setVisibility(mode, isAnimated);
    }
}
