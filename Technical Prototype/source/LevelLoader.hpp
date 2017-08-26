//
//  LevelLoader.hpp
//  RocketDemo
//
//  Created by Andy Jiang on 3/12/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef LevelLoader_hpp
#define LevelLoader_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "GameModel.hpp"
#include "Tile.hpp"
#include "RectangleModule.hpp"
#include "StairModule.hpp"
//#include "json.hpp"

using namespace cugl;

class LevelLoader {

protected:
    
    /** Reference to the player avatar */
    std::shared_ptr<Character>			  _avatar;
    
    /** The asset manager for this game mode. */
    std::shared_ptr<cugl::AssetManager> _assets;
    
public:
#pragma mark -
#pragma mark Constructors
    LevelLoader();
    
    virtual ~LevelLoader() { dispose(); }
    
    void dispose();
    
    bool init();
    
#pragma mark -
#pragma mark Loading
    static Vec2 loadLevel(int level, std::shared_ptr<GameModel> gameModel, std::shared_ptr<ObstacleWorld> levelWorld, std::shared_ptr<Node> tileRootNode);
    
    
};

#endif /* LevelLoader_hpp */
