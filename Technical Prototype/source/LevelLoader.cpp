//
//  LevelLoader.cpp
//  RocketDemo
//
//  Created by Andy Jiang on 3/12/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "LevelLoader.hpp"
#include <fstream>
#include <unistd.h>

#include <string.h>
#include "RectangleModule.hpp"
#include "StairModule.hpp"
#include "Collectible.hpp"
#include "Door.hpp"
#include "Constants.h"
#include "LevelView.hpp"

using namespace cugl;
using namespace std;

void init() {
    
}

Vec2 LevelLoader::loadLevel(int level, shared_ptr<GameModel> gameModel,
                            shared_ptr<ObstacleWorld> levelWorld,
                            shared_ptr<Node> tileRootNode) {
    
    // For initializing views, we have the scene graph nodes as follows:
    
    // This node is where all physics debug views should go to
    std::shared_ptr<Node> debugNode = tileRootNode->getChildByName("Debug");
    
    // This node is where the character's views should go to.
    std::shared_ptr<Node> characterNode = tileRootNode->getChildByName(CHARACTER);
    
    // This node is where all tile's views should go to. For each tile layer,
    // create a new node with that tile's index (0, 1, 2...) and add all tiles
    // under that.
    std::shared_ptr<LevelView> levelRoot = std::static_pointer_cast<LevelView>(tileRootNode->getChildByName("Level Root"));
    
    float bottomLeftX = METERS_PER_TILE/2;
    float bottomLeftY = METERS_PER_TILE/2;
    int i, j;
    float x, y, w, h;
    
    shared_ptr<Tile> currentTile = nullptr;
    
    // We create the factories that will produce various modules.
    RectangleModule rectModule = RectangleModule();
    rectModule.init();
    
    StairModule stairModule = StairModule();
    stairModule.init();
    
    std::string file = PATH_TO_LEVELS + LEVEL_FILE_PREFIX + std::to_string(level) + ".json";
    std::shared_ptr<JsonReader> reader = JsonReader::allocWithAsset(file);
    const std::shared_ptr<cugl::JsonValue>& json = reader->readJson();
    if (json == nullptr) {
        CUAssertLog(false, "Failed to load level file");
        return NULL;
    }
    
    // Constants
    int width = json->getInt("width");
    int height = json->getInt("height");
    int depth = json->getInt("depth");
    
    // Initializing tiles in game model
    gameModel->initTiles(depth, width, height);
    
    // Reusable variables
    Rect container;
    auto layers = json->get("layers");
    
    for (int l = 0; l < depth; l++) {
        auto tiles = layers->get(l);
        
        // Add tiles to layer
        for (int idx = 0; idx < tiles->size(); idx++) {
            // Create a new horizontal row
            // Create tile
            i = tiles->get(idx)->getInt("x");
            j = tiles->get(idx)->getInt("y");
            Vec2 center = Vec2(bottomLeftX + i * METERS_PER_TILE, bottomLeftY + j * METERS_PER_TILE);
            auto color_vals = tiles->get(idx)->get("color");
            Color4f color = Color4f(color_vals->get(0)->asFloat(),
                                    color_vals->get(1)->asFloat(),
                                    color_vals->get(2)->asFloat(),
                                    color_vals->get(3)->asFloat());
            string lockedBy = tiles->get(idx)->get("lockedBy")->asString();
            currentTile = Tile::alloc(center, color);
            currentTile->setLocked(lockedBy, false);
            
            // Load decorations
            auto decorations = tiles->get(idx)->get("decoration");
            for (int d = 0; d<decorations->size(); d++){
                x = decorations->get(d)->getFloat("x");
                y = decorations->get(d)->getFloat("y");
                w = decorations->get(d)->getFloat("w");
                h = decorations->get(d)->getFloat("h");
                rectModule.container = Rect(x, y, w, h);
                rectModule.textureName = decorations->get(d)->getString("texture");
                auto color_vals = decorations->get(d)->get("color");
                Color4f color = Color4f(color_vals->get(0)->asFloat(),
                                        color_vals->get(1)->asFloat(),
                                        color_vals->get(2)->asFloat(),
                                        color_vals->get(3)->asFloat());
                std::shared_ptr<Node> colorNode = rectModule.generateNewNode(i,j);
                colorNode->setColor(color);
                currentTile->backgroundNode->addChild(colorNode, 0);
            }
            
            // Load geometry
            int numObjects = (int)tiles->get(idx)->get("geometry")->size();
            for(int idx_2 = 0; idx_2 < numObjects; idx_2++) {
                
                auto obj = tiles->get(idx)->get("geometry")->get(idx_2);
                x = obj->getFloat("x");
                y = obj->getFloat("y");
                w = obj->getFloat("w");
                h = obj->getFloat("h");
                container = Rect(x, y, w, h);
                
                if (obj->getString("type") == "rectangle") {
                    /** FLOORS AND WALLS */
                    rectModule.textureName = obj->getString("texture");
                    rectModule.container = container;
                    currentTile->foregroundNode->addChild(rectModule.generateNewNode(i,j));
                    vector<shared_ptr<Obstacle>> list = rectModule.generateNewColliders();
                    for (auto collider : list) {
                        collider->setName(FLOOR);
                        currentTile->colliders.push_back(collider);
                    }
                    //May want to add value in JSON to differentiate from wall
                    
                } else if (obj->getString("type") == STAIRS) {
                    /** STAIRS */
                    string textureName = obj->getString("texture");
                    int direction = obj->getInt("direction");
                    if (direction == 1) {
                        textureName += LEFT_STAIR_SUFFIX;
                    }
                    
                    stairModule.textureName = textureName;
                    stairModule.setDirection(direction);
                    stairModule.container = container;
                    currentTile->foregroundNode->addChild(stairModule.generateNewNode(i,j));
                    vector<shared_ptr<Obstacle>> list = stairModule.generateNewColliders();
                    for (auto collider : list) {
                        collider->setName(STAIRS);
                        currentTile->colliders.push_back(collider);
                    }
                    
                } else if (obj->getString("type") == DOOR) {
                    /** DOORS */
                    string id = obj->getString("id");
                    string connecting = obj->getString("connecting");
                    string textureName = obj->getString("texture");
                    Vec2 pos = Vec2(x, y);
                    Vec2 size = Vec2(w, h);
                    Vec2 offset = Vec2(i*METERS_PER_TILE + METERS_PER_TILE/2, j*METERS_PER_TILE + METERS_PER_TILE/2);
                    std::shared_ptr<Door> door = Door::alloc(pos, size, offset, id, connecting, l, textureName);
                    gameModel->door = door;
                    gameModel->doors.push_back(door);
                    currentTile->foregroundNode->addChild(door->node);
                    currentTile->colliders.push_back(door);
                    // TODO: set to goal door or other layer door
                    
                } else if (obj->getString("type") == COLLECTIBLE) {
                    /** COLLECTIBLES */
                    Vec2 pos = Vec2(x, y);
                    Vec2 offset = Vec2(i*METERS_PER_TILE + METERS_PER_TILE/2, j*METERS_PER_TILE + METERS_PER_TILE/2);
                    std::shared_ptr<Collectible> collectible = Collectible::alloc(pos, offset, obj->getString("tag"));
                    gameModel->collectibles.push_back(collectible);
                    currentTile->foregroundNode->addChildWithName(collectible->node, obj->getString("tag"));
                    currentTile->colliders.push_back(collectible);
                    // TODO: set collectible attributes
                }
            }
            
            gameModel->tiles[l][i][j] = currentTile;
        }
    }
    
    
    // Character
    if (json->get(CHARACTER) == nullptr) {
        CUAssertLog(false, "Failed to load character");
        return NULL;
    }
    
    Vec2 pos = Vec2(json->get(CHARACTER)->getFloat("x"), json->get(CHARACTER)->getFloat("y"));
    bool facingRight = json->get(CHARACTER)->getInt("facingRight") == 1;
    std::shared_ptr<Character> character = Character::alloc(pos, facingRight);
    character->setDebugScene(debugNode);
    levelWorld->addObstacle(character);
    gameModel->character = character;
    characterNode->addChildWithName(character->node, "char");
    
    // Fill in hole tiles with blanks
    for (int l = 0; l < gameModel->tiles.size(); l++) {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                auto t = gameModel->tiles[l][x][y];
                if (t==nullptr){
                    Vec2 center = Vec2(bottomLeftX + x * METERS_PER_TILE, bottomLeftY + y * METERS_PER_TILE);
                    Color4 color = Color4(0,0,0,0);
                    gameModel->tiles[l][x][y] = Tile::alloc(center, color);
                    gameModel->tiles[l][x][y]->setLocked("always", false);
                    gameModel->tiles[l][x][y]->tileNode->setLockVisibility(false);
                }
            }
        }
    }
    
    // we are done configuring the tile. We should now add all tile to the world
    levelRoot->addTiles(gameModel->tiles);
    
    for (int l = 0; l < gameModel->tiles.size(); l++) {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                auto t = gameModel->tiles[l][x][y];
                if (t) {
                    for (auto collider : t->colliders) {
                        levelWorld->addObstacle(collider);
                        collider->setDebugScene(debugNode);
                        collider->setActive(l == 0);
                    }
                }
            }
        }
    }
    
    return Vec2(width, height);
}
