//
//  TitleUIController.hpp
//  RocketDemo
//
//  Created by Andy Jiang on 2017/04/16.
//  Copyright © 2017年 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef TitleUIController_hpp
#define TitleUIController_hpp

#include "AbstractUIController.hpp"
#include "TitleUIControllerDelegate.hpp"

/**
 This controller handles the display and player interaction with the UI.
 */
class TitleUIController : public AbstractUIController {
    
private:
    void initLevelSelect();
    
    void initLevelSelect(int index);
    
protected:
    std::vector<std::shared_ptr<cugl::Node>> levelSelectNodes;
    
    std::shared_ptr<cugl::Node> mainMenuNode;
    
    std::shared_ptr<cugl::Node> creditsNode;
    
    bool handleUIEvent(int tag);
    
    int current_level;
    
public:
    /**
     The delegate that will respond to events.
     */
    TitleUIControllerDelegate *delegate;
    
    void activateLevelSelectUI();
    
    void switchLevelSelect(int index);
    
    void updateWindows();
    
    void activateMainMenuUI();
    
    void activateCreditsUI();
    
    void deactivate();
    
    /**
     Initializes this instance. This lays out the game's UI elements.
     */
    bool init();
    
    TitleUIController() {}
    
    ~TitleUIController() {}
    
};

#endif /* TitleUIController_hpp */
