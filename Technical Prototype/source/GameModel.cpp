//
//  GameModel.cpp
//  RocketDemo
//
//  Created by Jiacong Xu on 3/11/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "GameModel.hpp"

void GameModel::init(int level) {
    this->level = level;
    time = 0;
    layer = 0;
    gameState = GameModel::PLAYING;
}

void GameModel::initTiles(int depth, int width, int height) {
    tiles = std::vector<std::vector<std::vector<std::shared_ptr<Tile>>>>();
    
    for (int l = 0; l < depth; l++) {
        tiles.push_back(std::vector<std::vector<std::shared_ptr<Tile>>>());
        
        for (int x = 0; x < width; x++) {
            auto col = std::vector<std::shared_ptr<Tile>>();
            tiles[l].push_back(col);
            
            for (int y = 0; y < height; y++) {
                tiles[l][x].push_back(nullptr);
            }
        }
    }
}

void GameModel::dispose() {

}

