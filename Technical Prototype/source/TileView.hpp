//
//  TileView.hpp
//  RocketDemo
//
//  Created by Jiacong Xu on 4/28/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef TileView_hpp
#define TileView_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "LockView.hpp"

/**
 This class controls the visual aspects of the tile in the game.
 */
class TileView : public cugl::Node {
private:
    /** The background color node for the tile. */
    std::shared_ptr<cugl::Node> bgColorNode;
    
    /** The main node that contains everything else but minimized color strip. */
    std::shared_ptr<cugl::Node> mainNode;
    
    /** The minimized color strip. */
    std::shared_ptr<cugl::Node> minimizedStripNode;
    
    /** The mask for the strip, symbolizing the tile is locked. */
    std::shared_ptr<cugl::Node> stripMaskNode;
    
    /** The container for the minimized color strip. */
    std::shared_ptr<cugl::Node> stripContainerNode;
    
    /** The mask for rounded corners. */
    std::shared_ptr<cugl::Node> maskNode;
    
    /** The background color and minimized bar color of this tile. */
    cugl::Color4 themeColor;
    
    /** The duration of layer switching animation. */
    const float ANIM_DURATION = 0.15;
    
    /** 
     The height of a thin strip in minimized layers. This is in meters. Note we
     divide this into two.
     */
    const float MINIMIZED_STRIP_HEIGHT = 0.25;
    
    /**
     The lock status of the tile.
     */
    bool isLocked = false;
    
    /**
     Whether the lock color has already been set.
     */
    bool didSetLockColor = false;
    
    /**
     Whether the lock is visible when active.
     */
    bool isLockVisible = true;
    
public:
    /** The lock node. */
    std::shared_ptr<LockView> lockNode;
    
    /** The foreground layer node for the tile. */
    std::shared_ptr<cugl::Node> foregroundNode;
    
    /** The background layer node for the tile. */
    std::shared_ptr<cugl::Node> backgroundNode;
    
    /**
     Returns a new copy of TileView with the given position.
     */
    static std::shared_ptr<TileView> allocWithPosition(cugl::Vec2 pos) {
        std::shared_ptr<TileView> t = std::make_shared<TileView>();
        
        return t->initWithPosition(pos) ? t : nullptr;
    }
    
    /**
     We do some custom initializations in this function.
     */
    virtual bool initWithPosition(const cugl::Vec2& pos);
    
    /**
     Initializing with the origin as our position.
     */
    virtual bool init();
    
    /**
     Sets the background color for this tile view. This also becomes the color
     for the minimized strips.
     */
    void setNodeBackgroundColor(cugl::Color4 color);
    
    /**
     The three display modes for a tile node. Full is the original tile,
     MinimizedTop and MinimizedBottom are two thin strips that when put on top
     of each other form a thin rounded rectangle.
     */
    enum TileDisplayMode {
        Full, MinimizedTop, MinimizedBottom
    };
    
    /**
     Sets the visibility of this node. There are three states, fully visible,
     minimized top half, and minimized bottom half.
     */
    void setVisibility(TileDisplayMode mode, bool animated);
    
    /**
     Sets whether the tile is locked. Locked tiles cannot be moved.
     */
    void setLocked(bool isLocked, bool animated);
    
    /**
     Sets whether the lock is visible.
     */
    void setLockVisibility(bool isVisible);
    
    /**
     Sets the color for the lock on this tile. Can only be set once.
     */
    void setLockColorOnce(cugl::Color4 color);
    
};



#endif /* TileView_hpp */
