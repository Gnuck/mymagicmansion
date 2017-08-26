//
//  GameModel.hpp
//  RocketDemo
//
//  Created by Jiacong Xu on 3/11/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef GameModel_hpp
#define GameModel_hpp

#include <stdio.h>
#include "Tile.hpp"
#include "Character.hpp"
#include "Collectible.hpp"
#include "Door.hpp"

using namespace cugl;

/**
 Keeps meta data about the level. Actual level data such as tile positions are
 managed by the level controller in a different model.
 */
class GameModel {
public:
    
    /**
     * Enumeration to identify the game state
     */
    enum GameState {
        /** The playing state */
        PLAYING,
        /** The paused state */
        PAUSED,
        /** The state when a goal is reached */
        LEVEL_COMPLETE,
        /** The state when the character falls off the tiles */
        DEATH
    };
    
    /** The current game state */
    GameState gameState;
    
    /** The level we are playing right now. */
    int level;
  
    /** The elapsed time. */
    float time;
    
    /** Contains all tiles in this level. */
    std::vector<std::vector<std::vector<std::shared_ptr<Tile>>>> tiles;
    
    /** The current layer index the player is on. */
    int layer;
    
    /** A shared pointer to the character */
    std::shared_ptr<Character> character;
    
    std::shared_ptr<Collectible> collectible;
    
    std::vector<std::shared_ptr<Collectible>> collectibles;
    
    std::vector<std::shared_ptr<Door>> doors;
    
    std::shared_ptr<Door> door;
    
    //std::shared_ptr<Collectable> collectable;
    
    /**
     Default constructor.
     */
    GameModel() {};

    /**
     Default destructor.
     */
    ~GameModel() {
        dispose();
    }
    
    /**
     Get rid of this tile.
     */
    void dispose();
    
    /**
     Initializes a new model with the given level. Does not initialize tiles.
     Call that with initTiles.
     */
    void init(int level);
    
    /**
     Returns a newly allocated empty tile.
     */
    static std::shared_ptr<GameModel> alloc(int level) {
        std::shared_ptr<GameModel> result = std::make_shared<GameModel>();
        result->init(level);
        return result;
    }
    
    /**
     Initializes the tile container 3D vector with the right dimensions of null
     pointers.
     */
    void initTiles(int depth, int width, int height);
};

#endif /* GameModel_hpp */
