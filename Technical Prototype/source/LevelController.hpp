//
//  LevelController.hpp
//  RocketDemo
//
//  Created by Jiacong Xu on 3/11/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef LevelController_hpp
#define LevelController_hpp

#include <stdio.h>
#include "AbstractController.hpp"
#include "LevelControllerDelegate.hpp"
#include "GameModel.hpp"
#include "TileHighlightView.hpp"
#include <cugl/cugl.h>
#include <string.h>
#include "LevelView.hpp"
#include "Tap.hpp"
#include "Swipe.hpp"

using namespace cugl;

/**
 Level controller actually manages the level itself. That is, moving the player,
 loading the level, swapping the tiles, etc.
 */
class LevelController : public AbstractController {
private:
    /**
     The length of each tile in screen coordinates.
     */
    float tileSize;
    
    /**
     The margin in screenspace coordinates.
     */
    Vec2 margins;
    
    int numColumns;
    
    int numRows;
    
    std::vector<std::shared_ptr<Tap>> heldTaps;
    std::vector<std::shared_ptr<Tap>> ignoreTaps;
    std::vector<std::shared_ptr<Swipe>> heldSwipes;
    std::vector<std::shared_ptr<Swipe>> ignoreSwipes;
    
    bool animatingLayerSwitch;
    
    /**
     The selected tile. Null if none selected.
     */
    std::shared_ptr<Tile> selectedTile;
    
    int selectedTileX;
    
    int selectedTileY;
    
    int characterLayer;
    
    /**
     The main selection border view, displayed on the first tapped object.
     */
    std::shared_ptr<TileHighlightView> mainHighlightNode;
    
    /**
     The secondary selection border view, displayed on the tile being switched.
     */
    std::shared_ptr<TileHighlightView> auxHighlightNode;
    
    /**
     String id for the door that the character is currently exiting. Empty string if not exiting a door. This is done for layer-switching purposes to prevent the character 
         from entering the door it just exited.
     */
    std::string exitDoor = "";
    
    
    /**
     Changes the current active layer to the given layer index, updating both
     model and view.
     */
    void switchLayer(int activeLayer, std::function<void (void)> callback);
    
    /**
     Layer to switch to during a layer switch
     */
    int toLayer;
    
    /**
     Checks and processes a tap on the screen.
     */
    void processTap();
    
    /**
     Checks and processes a tap on the screen.
     */
    void processSwipe();
    
    bool discardIgnoredTap();
    bool discardIgnoredSwipe();
    
    void selectTile(Vec2 pos);
    
    /**
     Callback for receiving contact information in the physics world.
     */
    void beginContact(b2Contact* contact);
    
    /**
     Callback for receiving end contact information in the physics world.
     */
    void endContact(b2Contact* contact);
    

    
public:
    /**
     The delegate responding to level events.
     */
    LevelControllerDelegate *delegate;
    
    /**
     The shared game model pointer.
     */
    std::shared_ptr<GameModel> gameModel;
    
    /**
     The scene object for the layers.
     */
    std::shared_ptr<LevelView> levelRootNode;
    
    /**
     The root scene object for the levels.
     */
    std::shared_ptr<cugl::Scene> levelScene;
    
    /**
     The root box2D world object.
     */
    std::shared_ptr<cugl::ObstacleWorld> levelWorld;
    
    /**
     The constructor for the level controller.
     */
    LevelController() {};
    
    /**
     The destructor for the level controller.
     */
    ~LevelController() {};
    
    /**
     Flag to begin switching layers. Layer switch in process if true, false otherwise.
     */
    bool switchlayers = false;
    
    /**
     Position for the character to jump to during a layer switch
     */
    Vec2 jumpTo = Vec2(2.5,2.5);
    
    /**Temporary flag to check for collectable collision to test speedup function*/
    bool speedUp = false;
    
    /**
     The initializer for the level controller. The game model is shared in the
     game but level controller does not own it.
     */
    void init(int level, std::shared_ptr<GameModel> gameModel);
    
    void update(float dt);

    /**
     Overriden draw method.
     */
    void draw(const std::shared_ptr<cugl::SpriteBatch>& batch);
};


#endif /* LevelController_hpp */
