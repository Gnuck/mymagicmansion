//
//  AbstractTileModule.hpp
//  RocketDemo
//
//  Created by Jiacong Xu on 3/21/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef AbstractTileModule_hpp
#define AbstractTileModule_hpp

#include <stdio.h>
#include <cugl/cugl.h>

/**
 An abstract class defining what all pieces making up a tile should provide to
 the level loader. A module in a tile should know how its collision geometry and
 how it is drawn (scene graph node). All module classes act as factories, in
 that only ONE instance is enough to create the whole level. Just reconfigure
 and keep calling generateNew.
 
 Implementation notes:
 
 - make sure in the main get methods, you are CREATING new things and returning
 
 - you need to make sure the textures you mention are properly loaded, take a
   look at assets.json file under Assets/json. For now just chuck the image in
   there, we will come back to it later.
 
 - finally, to test whether the componnet you created works, go to Andy's
   LevelController, find where he creates the component you are doing, and
   replace his code. See if things work out.
 */
class AbstractTileModule {
public:
    /**
     This is the container of the shape. Most of the time it is suffice to only
     define this to create the shape. Some times additional information is
     needed by a subclass in order to create the geometry, such as facing of the
     stairs.
     */
    cugl::Rect container;
    
    /**
     This is the main texture of the shape which will be fetched from the asset
     manager.
     */
    std::string textureName;
    
    /**
     Creates a new scene graph node that contains everything necessary to render
     this object. (i,j) is the location of the tile.
     */
    virtual std::shared_ptr<cugl::Node> generateNewNode(int i, int j) = 0;
    
    /**
     Creates and returns a list of colliders that make up its physics.
     */
    virtual std::vector<std::shared_ptr<cugl::Obstacle>> generateNewColliders() = 0;
    
    /**
     Initializes the module with an empty rect as its container.
     */
    virtual bool init() {
        container = cugl::Rect();
        return true;
    }
    
    /**
     Initializes the module with the given rect as its container.
     */
    virtual bool initWithRect(cugl::Rect rect) {
        container = rect;
        return true;
    }
};

#endif /* AbstractTileModule_hpp */
