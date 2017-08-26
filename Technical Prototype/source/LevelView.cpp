//
//  LevelView.cpp
//  RocketDemo
//
//  Created by Jiacong Xu on 4/29/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "LevelView.hpp"
#include "App.h"

void LevelView::addTiles(std::vector<std::vector<std::vector<std::shared_ptr<Tile>>>> tiles) {
    // Start from a new layer view vector.
    layerViews = std::vector<std::shared_ptr<LayerView>>();
    
    for (std::vector<std::vector<std::shared_ptr<Tile>>> layer : tiles) {
        std::shared_ptr<LayerView> layerView = LayerView::allocWithBounds(getSize());
        addChild(layerView);
        
        layerView->addTiles(layer);
        layerViews.push_back(layerView);
    }
}

void LevelView::switchLayers(int target, bool animated, std::function<void (void)> callback) {
    if (target == activeLayer)
        return;
    
    activeLayer = target;
    
    // We do the following:
    // 1. Calculate how many layers we need to shift between currently active
    //    and target.
    // 2. Move target to the center.
    // 3. Shrink current and rearrange each.
    for (int i = 0; i < layerViews.size(); i++) {
        std::shared_ptr<LayerView> layer = layerViews[i];
        layer->toggleLayerVisibility(i != target, animated);
        
        // We now need to calculate exactly where the layer should be.
        App::AnimationController.animate<float>([layer] (float y) {
            layer->setPositionY(y);
        }, layer->getPositionY(), getLayerY(i), animated? ANIM_DURATION : 0, AbstractAnimation::EaseInOut, callback);
        callback = nullptr;
    }
}

float LevelView::getLayerY(int i) {
    float center = getHeight() / 2;
    const float LAYER_GAP = 0.1;
    const float LAYER_HEIGHT = 0.25;
    
    int sign = std::max(std::min(i - activeLayer, 1), -1);
    
    return center + (i - activeLayer) * (LAYER_GAP + LAYER_HEIGHT) + sign * getHeight() / 2;
}
