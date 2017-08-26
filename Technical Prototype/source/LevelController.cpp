//
//  LevelController.cpp
//  RocketDemo
//
//  Created by Jiacong Xu on 3/11/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "LevelController.hpp"
#include "LevelLoader.hpp"
#include "App.h"
#include <Box2D/Dynamics/b2World.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/b2Collision.h>
#include "Constants.h"
#include <unistd.h>

using namespace cugl;

void LevelController::init(int level, std::shared_ptr<GameModel> gameModel) {
    // First, we initialize our worlds
    this->gameModel = gameModel;
    
    levelWorld = ObstacleWorld::alloc(Rect(0, 0, 1, 1));
    levelWorld->activateCollisionCallbacks(true);
    levelWorld->onBeginContact = [this](b2Contact* contact){
        beginContact(contact);
    };
    levelWorld->onEndContact = [this](b2Contact*contact){
        endContact(contact);
    };
    
    // Initialize the root node for drawing.
    auto tileRootNode = Node::alloc();

    
    // We have different layers of stuff to be drawn. E.g. each level layer is
    // drawn separately. There is also a layer for characters. We might want to
    // consider some parallax - foreground/background shifts.
    
    // This should contain all the level layers. The level loader should create
    // a new node for each layer and add the nodes appropriately.
    levelRootNode = LevelView::allocWithBounds(Vec2(20, 10));
    tileRootNode->addChildWithName(levelRootNode, "Level Root");
    
    // This is the physics debug layer.
    auto debugNode = Node::alloc();
    tileRootNode->addChildWithName(debugNode, "Debug");
    
    // This is where the character node should go under.
    auto characterNode = Node::alloc();
    tileRootNode->addChildWithName(characterNode, CHARACTER);
    
    // This is the tile highlight node
    auto select = App::AssetManager->get<cugl::Texture>("tile-highlight");
    mainHighlightNode = TileHighlightView::allocWithTexture(select);
    auxHighlightNode = TileHighlightView::allocWithTexture(select);
    tileRootNode->addChild(mainHighlightNode);
    tileRootNode->addChild(auxHighlightNode);
    
    // Next, we load the given level here.
    Vec2 dimensions = LevelLoader::loadLevel(level, gameModel, levelWorld, tileRootNode);
    
    // Size of this level. As read from level loader.
    numColumns = dimensions.x;
    numRows = dimensions.y;
    
    //Starting layer of this level; modified when switching layers
    toLayer = 0;
    
    characterLayer = 0;
    
    tileSize = 420;
    margins.x = 120;
    margins.y = 120;
    
    float scale = tileSize/METERS_PER_TILE;
    
    Size dimen = Application::get()->getDisplaySize();
    dimen *= (MOCKUP_WIDTH/dimen.width);
    levelScene = Scene::alloc(dimen / scale);
    
    Vec2 referenceSize = Vec2(MOCKUP_WIDTH, MOCKUP_HEIGHT);
    tileRootNode->setAnchor(Vec2::ANCHOR_MIDDLE);
    tileRootNode->setPosition(Vec2(dimen.width, dimen.height) / scale / 2);
    tileRootNode->setContentSize((referenceSize - 2 * margins) / scale);
    levelScene->addChildWithName(tileRootNode, "Tile Root");
    levelRootNode->switchLayers(0, false, nullptr);

    App::InputController.clear();
    animatingLayerSwitch = false;
}

void LevelController::update(float dt) {
    processTap();
    processSwipe();
    levelWorld->garbageCollect();
    levelWorld->update(dt);
    
    if (switchlayers) {
        gameModel->character->changeSpeed(0.0);
        gameModel->character->setBodyType(b2_staticBody);
        switchLayer(toLayer, nullptr);
        gameModel->character->setPosition(jumpTo);
        gameModel->character->changeSpeed(1.0);
        switchlayers = false;
        characterLayer = toLayer;
        gameModel->character->setBodyType(b2_dynamicBody);
    }
    
    gameModel->character->update(levelWorld, dt);
    if (gameModel->character->getPosition().y <= -5) {
        delegate->gameLost();
    }
}


void LevelController::draw(const std::shared_ptr<cugl::SpriteBatch> &batch) {
    levelScene->render(batch);
}

void LevelController::selectTile(Vec2 pos) {
    /*
     Player tapped on the screen. We have to first grab the tile player tapped
     on, and get it selected. To swap the two, we have to both swap its
     collision (model) and scene graph node (view). Can we do these
     simultaneously? Just put the code in the tile object.
     */
    
    // First, determine which tile is selected by pos.
    // It depends on our scale and margin.
    Size displaySize = Application::get()->getDisplaySize();
    pos.x = (pos.x / displaySize.width) * MOCKUP_WIDTH - margins.x;
    float ratio = displaySize.width / displaySize.height;
    float height = MOCKUP_WIDTH / ratio;
    float diff = height - MOCKUP_HEIGHT;
    pos.y = (height - (pos.y / displaySize.height) * height) - margins.y - diff/2;
    
    if (   pos.x < 0
        || pos.y < 0
        || pos.x >= MOCKUP_WIDTH - 2*margins.x
        || pos.y >= MOCKUP_HEIGHT - 2*margins.y) {
        CULog("ignore tap");
        return;
    }
    
    int x = (int)(pos.x / tileSize);
    int y = (int)(pos.y / tileSize);
    int l = gameModel->layer;
    
    // Next, grab the tile object and character object.
    std::shared_ptr<Tile> tile = gameModel->tiles[l][x][y];
    
    // Check to see if tile is locked before processing
    if (tile != nullptr && tile->isLocked()) {
        tile->tileNode->lockNode->shake();
        return;
    }
    
    // Finally, see if there is already a selected tile object.
    if (selectedTile != nullptr) {
        // swap the character's position if character is on one of the two tiles
        Vec2 offset = selectedTile->getPosition() - tile->getPosition();
        
        if (gameModel->character!=nullptr){
            // See if character is on one of the tiles
            if (selectedTile->contains(gameModel->character->getPosition())) {
                // selected tile contains character
                gameModel->character->setPosition(gameModel->character->getPosition() - offset);
            } else if (tile->contains(gameModel->character->getPosition())) {
                // tile contains character
                gameModel->character->setPosition(gameModel->character->getPosition() + offset);
            }
        }
        
        // Swap the two tiles' positions
        Vec2 cachedPos = selectedTile->getPosition();
        selectedTile->setPosition(tile->getPosition());
        tile->setPosition(cachedPos);
        
        // Swap the two tiles in the model.
        std::shared_ptr<Tile> cachedTile = gameModel->tiles[l][selectedTileX][selectedTileY];
        gameModel->tiles[l][selectedTileX][selectedTileY] = gameModel->tiles[l][x][y];
        gameModel->tiles[l][x][y] = cachedTile;
        
        auto sound = App::AssetManager->get<Sound>(SWITCH_TILE);
        App::AudioController.playSoundEffect(SWITCH_TILE,sound);
        
        // Clear the selected tile.
        // selectedTile->tileNode->setVisibility(TileView::Full, true);
        auxHighlightNode->highlightTile(selectedTile, false);
        auxHighlightNode->removeHighlight(true);
        mainHighlightNode->removeHighlight(true);
        selectedTile = nullptr;
    } else {
        selectedTile = tile;
        selectedTileX = x;
        selectedTileY = y;
        mainHighlightNode->highlightTile(selectedTile, true);
    }
}

bool LevelController::discardIgnoredTap() {
    int i;
    for (i = 0; i < ignoreTaps.size(); i++) {
        long id = ignoreTaps[i]->id;
        if (App::InputController.releasedSwipeReady(id)) {
            break;
        } else if (App::InputController.releasedTapReady(id)) {
            App::InputController.consumeReleasedTap(id);
            break;
        }
    }
    if (i < ignoreTaps.size()) {
        ignoreTaps.erase(ignoreTaps.begin() + i);
        return true;
    }
    return false;
}

bool LevelController::discardIgnoredSwipe() {
    int i;
    for (i = 0; i < ignoreSwipes.size(); i++) {
        if (App::InputController.releasedSwipeReady(ignoreSwipes[i]->id)) {
            App::InputController.consumeReleasedSwipe(ignoreSwipes[i]->id);
            break;
        }
    }
    if (i < ignoreSwipes.size()) {
        ignoreSwipes.erase(ignoreSwipes.begin() + i);
        return true;
    }
    return false;
}

void LevelController::processTap() {
    
    if (animatingLayerSwitch) {
        while (App::InputController.heldTapReady()) {
            auto tap = App::InputController.popHeldTap();
            ignoreTaps.push_back(tap);
        }
    }
    
    while (discardIgnoredTap());
    
    if (characterLayer != gameModel->layer) {
        return;
    }
    
    while (App::InputController.heldTapReady()) {
        auto tap = App::InputController.popHeldTap();
        heldTaps.push_back(tap);
    }
    
    Vec2 pos;
    bool isSwipe = false;
    int i;
    for (i = 0; i < heldTaps.size(); i++) {
        long id = heldTaps[i]->id;
        if (App::InputController.releasedSwipeReady(id)) {
            isSwipe = true;
            break;
        } else if (App::InputController.releasedTapReady(id)) {
            auto tap = App::InputController.consumeReleasedTap(id);
            pos = Vec2(tap->position);
            break;
        }
    }
                       
    if (i == heldTaps.size()) {
        return;
    }
    heldTaps.erase(heldTaps.begin() + i);
    if (isSwipe) {
        return;
    }
    
    selectTile(pos);
}

void LevelController::processSwipe() {
    
    if (App::InputController.heldSwipeReady()) {
        if (characterLayer == gameModel->layer && !animatingLayerSwitch) {
            std::shared_ptr<Swipe> swipe = App::InputController.popHeldSwipe();
            
            float y1 = swipe->initialPosition.y;
            float y2 = swipe->finalPosition.y;
            int layer = gameModel->layer;
            
            if (y1 - y2 > 50) {
                if (layer > 0) {
                    gameModel->character->changeSpeed(0.0);
                    gameModel->character->setActive(false);
                    gameModel->character->node->setVisible(false);
                    animatingLayerSwitch = true;
                    switchLayer(layer - 1, nullptr);
                }
                heldSwipes.push_back(swipe);
            } else if (y1 - y2 < -50) {
                if (layer < gameModel->tiles.size() - 1) {
                    gameModel->character->changeSpeed(0.0);
                    gameModel->character->setActive(false);
                    gameModel->character->node->setVisible(false);
                    animatingLayerSwitch = true;
                    switchLayer(layer + 1, nullptr);
                }
                heldSwipes.push_back(swipe);
            } else {
                ignoreSwipes.push_back(swipe);
            }
        } else {
            ignoreSwipes.push_back(App::InputController.popHeldSwipe());
        }
    }
    
    while (discardIgnoredSwipe());
    
    if (heldSwipes.size() > 0 && App::InputController.releasedSwipeReady(heldSwipes[0]->id)) {
        App::InputController.consumeReleasedSwipe(heldSwipes[0]->id);
        heldSwipes.erase(heldSwipes.begin() + 0);
        if (!gameModel->character->isActive()) {
            switchLayer(characterLayer, [this] () {
                this->gameModel->character->setActive(true);
                this->gameModel->character->node->setVisible(true);
                this->gameModel->character->changeSpeed(1.0);
                this->animatingLayerSwitch = false;
            });
        }
    }
}

void LevelController::beginContact(b2Contact* contact) {
    // Detect door here
    
    b2Body* body1 = contact->GetFixtureA()->GetBody();
    b2Body* body2 = contact->GetFixtureB()->GetBody();

    std::string name1 = ((Obstacle*)(body1->GetUserData()))->getName();
    std::string name2 = ((Obstacle*)(body2->GetUserData()))->getName();
    
    if (name1 == exitDoor || name2 == exitDoor) {
        // We just exited from this door! DON'T go through.
    } else if (((name1.find(DOOR) != std::string::npos) && name2 == CHARACTER)
        || (name1 == CHARACTER && (name2.find(DOOR) != std::string::npos))) {
        switchlayers = true;
        
        // find the coordinates of the door that the character is jumping to on
        // the next layer, update the exitDoor to prevent reentering the door
        // the character jumps to, and update toLayer to prepare for a layer
        // switch
        std::string character = name1;
        std::string door = name2;
        
        if (name2 == CHARACTER) {
            character = name2;
            door = name1;
        }
        
        std::string connect;
        
        //find the name/id of the connecting door
        for (std::shared_ptr<Door> d : gameModel->doors) {
            if (d->getName() == door) {
                connect = d->getConnecting();
            }
        }
        
        //find the coordinates of the connecting door
        for (std::shared_ptr<Door> conn : gameModel->doors) {
            if (conn->getName() == connect) {
                exitDoor = conn->getName();
                toLayer = conn->layer;
                jumpTo = conn->getPosition() + conn->node->getParent()->getPosition() - Vec2(0, conn->getSize().y / 4);
            }
        }
        
        auto sound = App::AssetManager->get<Sound>(DOOR_OPEN);
        App::AudioController.playSoundEffect(DOOR_OPEN,sound);
    } else if (((name1 == "goal") && name2 == CHARACTER)
        || (name1 == CHARACTER && name2 == "goal")) {
        delegate->gameWon();
    } else if (((name1.find(COLLECTIBLE) != std::string::npos) && name2 == CHARACTER) ||
               ((name2.find(COLLECTIBLE) != std::string::npos) && name1 == CHARACTER)) {
        // Check for that name contains "collectable" tag, then we can remove a
        // specific collectable by its full name.
        
        std::string character = name1;
        std::string collectible = name2;
        
        if (name2 == CHARACTER) {
            character = name2;
            collectible = name1;
        }
        
        //set flag for pickup animation
        gameModel->character->pickingUpObject = true;
        
        // unlock tile
        std::string tag = collectible.substr(4); // length of "key-"
        
        for (int l = 0; l < gameModel->tiles.size(); l++){
            for (int x = 0; x < gameModel->tiles[l].size(); x++){
                for (int y = 0; y < gameModel->tiles[l][x].size(); y++){
                    if (gameModel->tiles[l][x][y]->getLockKey() == tag) {
                        gameModel->tiles[l][x][y]->setLocked("", true);
                        gameModel->tiles[l][x][y]->foregroundNode->removeChildByName(tag);
                    }
                }
            }
        }
        
        gameModel->character->collected.push_front(collectible);
        
        for (std::shared_ptr<Collectible> c : gameModel->collectibles) {
            if (c->getName() == collectible) {
                c->destroy();
            }
        }
        
        auto sound = App::AssetManager->get<Sound>(GRAB_COLLECTABLE);
        App::AudioController.playSoundEffect(GRAB_COLLECTABLE,sound);
    }
}

void LevelController::endContact(b2Contact* contact){
    
    b2Body* body1 = contact->GetFixtureA()->GetBody();
    b2Body* body2 = contact->GetFixtureB()->GetBody();
    
    std::string name1 = ((Obstacle*)(body1->GetUserData()))->getName();
    std::string name2 = ((Obstacle*)(body2->GetUserData()))->getName();
    
    if ( (name1 == exitDoor && name2 == CHARACTER) ||
            (name2 == exitDoor && name1 == CHARACTER)){
        exitDoor = "";
    }
}

void LevelController::switchLayer(int activeLayer, std::function<void (void)> callback) {
    // First we change the active layer in game model
    gameModel->layer = activeLayer;
    levelRootNode->switchLayers(activeLayer, true, callback);
    
    // Then we change the models, physics, etc.
    for (int l = 0; l < gameModel->tiles.size(); l++) {
        for (int x = 0; x < gameModel->tiles[l].size(); x++) {
            for (int y = 0; y < gameModel->tiles[l][x].size(); y++) {
                gameModel->tiles[l][x][y]->setActive(activeLayer == l);
            }
        }
    }
    
    // Finally we need to clear the selected tile.
    selectedTile = nullptr;
    mainHighlightNode->removeHighlight(false);
}
