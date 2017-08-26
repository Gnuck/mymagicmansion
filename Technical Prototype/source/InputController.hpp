//
//  InputController.hpp
//  RocketDemo
//
//  Created by Andy Jiang on 3/12/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef InputController_hpp
#define InputController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "AbstractController.hpp"
#include "LevelController.hpp"
#include "GameUIController.hpp"
#include "GameModel.hpp"
#include "Tap.hpp"
#include "Swipe.hpp"

using namespace cugl;

/**
 The top level controller of the game. In charge of the Box2D world, UI, and
 level controllers.
 */
class InputController : public AbstractController {
    
private:
    
    Timestamp timestamp;
    
    // TOUCH SUPPORT
    Vec2 initialTouchLocation;
    Vec2 finalTouchLocation;
    
    std::vector<std::shared_ptr<Tap>> heldTaps;
    std::vector<std::shared_ptr<Tap>> releasedTaps;
    std::vector<std::shared_ptr<Tap>> consumedTaps;
    
    std::vector<std::shared_ptr<Swipe>> heldSwipes;
    std::vector<std::shared_ptr<Swipe>> releasedSwipes;
    std::vector<std::shared_ptr<Swipe>> consumedSwipes;
    
    Vec2 panDelta;
    
    /** Handles touchBegan and mousePress events using shared logic. */
    void touchBegan(const Timestamp timestamp, long id, const Vec2& pos);
    
    /** Handles touchEnded and mouseReleased events using shared logic. */
    void touchEnded(const Timestamp timestamp, long id, const Vec2& pos);
    
    void touchDrag(const Timestamp timestamp, long id, const Vec2& pos);
    
    bool removeHeldTap(long id);
    bool removeHeldSwipe(long id);
    bool removeConsumedTap(long id);
    
    
protected:
    
public:
    
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new input controller.
     *
     * This constructor does not do any initialzation.  It simply allocates the
     * object. This makes it safe to use this class without a pointer.
     */
    InputController(); // Don't initialize.  Allow stack based
    
    bool init();
    
    void dispose();
    
#pragma mark -
#pragma mark Input Detection
    /** Clears any buffered inputs so that we may start fresh. */
    void clear();
    
    void consumeIgnoredInput();
    
    /** Processes the currently cached inputs. */
    void update(float timestep);
    
#pragma mark -
#pragma mark Input Results
    
    /**
     *  Returns the initial location (in world space) of the touch.
     */
    const Vec2& getInitialTouchLocation() const { return initialTouchLocation; }
    
    /**
     *  Returns the final location (in world space) of the touch. That is, the location
     *  where the touch was released.
     */
    const Vec2& getFinalTouchLocation() const { return finalTouchLocation; }
    
    // tap
    bool heldTapReady();
    bool releasedTapReady(long id);
    std::shared_ptr<Tap> peekHeldTap();
    std::shared_ptr<Tap> popHeldTap();
    std::shared_ptr<Tap> consumeReleasedTap(long id);
    
    // swipe
    bool heldSwipeReady();
    bool heldSwipeReady(long id);
    bool releasedSwipeReady(long id);
    std::shared_ptr<Swipe> peekHeldSwipe();
    std::shared_ptr<Swipe> popHeldSwipe();
    std::shared_ptr<Swipe> consumeReleasedSwipe(long id);
    
#pragma mark -
#pragma mark Touch and Mouse Callbacks
    
    // mouse clicks
    void mousePressBeganCB(const MouseEvent& event, Uint8 clicks, bool focus);
    
    void mouseReleasedCB(const MouseEvent& event, Uint8 clicks, bool focus);
    
    void mouseDragCB(const MouseEvent& event, Vec2 previous, bool focus);
    
    // touchscreen
    void touchBeganCB(const TouchEvent& event, bool focus);
    
    void touchEndedCB(const TouchEvent& event, bool focus);
    
    void touchDragCB(const TouchEvent& event, bool focus);
};

#endif /* InputController_hpp */
