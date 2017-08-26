//
//  InputController.cpp
//  RocketDemo
//
//  Created by Andy Jiang on 3/12/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "InputController.hpp"

using namespace cugl;

#define PRESS_LISTENER_KEY      1
#define RELEASE_LISTENER_KEY    2
#define DRAG_LISTENER_KEY     3

#define EVENT_SWIPE_LENGTH 200
#define EVENT_SWIPE_TIME 1000

InputController::InputController()
{}

bool InputController::init() {
    
    bool success = true;
    timestamp.mark();
    
#ifndef CU_TOUCH_SCREEN
    success = Input::activate<Keyboard>();
    success = success && Input::activate<Mouse>();
    Mouse* mouse = Input::get<Mouse>();
    mouse->setPointerAwareness(cugl::Mouse::PointerAwareness::DRAG);
    mouse->addPressListener(PRESS_LISTENER_KEY, [=](const cugl::MouseEvent& event, Uint8 clicks, bool focus) {
        this->mousePressBeganCB(event, clicks, focus);
    });
    mouse->addReleaseListener(RELEASE_LISTENER_KEY, [=](const cugl::MouseEvent& event, Uint8 clicks, bool focus) {
        this->mouseReleasedCB(event, clicks, focus);
    });
    mouse->addDragListener(DRAG_LISTENER_KEY, [=](const cugl::MouseEvent& event, Vec2 previous, bool focus) {
        this->mouseDragCB(event, previous, focus);
    });
#else
    Touchscreen* touch = Input::get<Touchscreen>();
    touch->addBeginListener(PRESS_LISTENER_KEY, [=](const cugl::TouchEvent& event, bool focus) {
        this->touchBeganCB(event, focus);
    });
    touch->addEndListener(RELEASE_LISTENER_KEY, [=](const cugl::TouchEvent& event, bool focus) {
        this->touchEndedCB(event, focus);
    });
    touch->addMotionListener(DRAG_LISTENER_KEY, [=](const cugl::TouchEvent& event, Vec2 previous, bool focus) {
        this->touchDragCB(event, focus);
    });
#endif
    
    return success;
}

void InputController::dispose(){
#ifndef CU_TOUCH_SCREEN
    Input::deactivate<Keyboard>();
    Mouse* mouse = Input::get<Mouse>();
    mouse->removePressListener(PRESS_LISTENER_KEY);
    mouse->removeReleaseListener(RELEASE_LISTENER_KEY);
#else
    Touchscreen* touch = Input::get<Touchscreen>();
    touch->removeBeginListener(PRESS_LISTENER_KEY);
    touch->removeEndListener(RELEASE_LISTENER_KEY);
#endif
}

void InputController::update(float timestep) {

}

/** Clears any buffered inputs so that we may start fresh. */
void InputController::clear() {
    initialTouchLocation = Vec2::ZERO;
    finalTouchLocation = Vec2::ZERO;
    panDelta = Vec2::ZERO;
    timestamp.mark();
    heldTaps.clear();
    releasedTaps.clear();
    heldSwipes.clear();
    releasedSwipes.clear();
    consumedTaps.clear();
    consumedSwipes.clear();
}

#pragma mark -
#pragma mark Input Results
bool InputController::heldTapReady() {
    return heldTaps.size() > 0;
}

bool InputController::releasedTapReady(long id) {
    for (auto tap : releasedTaps) {
        if (tap->id == id) {
            return true;
        }
    }
    return false;
}

bool InputController::heldSwipeReady() {
    return heldSwipes.size() > 0;
}

bool InputController::heldSwipeReady(long id) {
    for (auto swipe : heldSwipes) {
        if (swipe->id == id) {
            return true;
        }
    }
    return false;
}

bool InputController::releasedSwipeReady(long id) {
    for (auto swipe : releasedSwipes) {
        if (swipe->id == id) {
            return true;
        }
    }
    return false;
}

std::shared_ptr<Tap> InputController::peekHeldTap() {
    return heldTaps.back();
}

std::shared_ptr<Swipe> InputController::peekHeldSwipe() {
    return heldSwipes.back();
}

std::shared_ptr<Tap> InputController::popHeldTap() {
    auto back = heldTaps.back();
    consumedTaps.push_back(Tap::alloc(back->id, Vec2(back->position)));
    auto result = Tap::alloc(back->id, Vec2(back->position));
    heldTaps.pop_back();
    return result;
}

std::shared_ptr<Swipe> InputController::popHeldSwipe() {
    auto back = heldSwipes.back();
    consumedSwipes.push_back(Swipe::alloc(back->id, Vec2(back->initialPosition), Vec2(back->finalPosition)));
    auto result = Swipe::alloc(back->id, Vec2(back->initialPosition), Vec2(back->finalPosition));
    heldSwipes.pop_back();
    return result;
}

std::shared_ptr<Tap> InputController::consumeReleasedTap(long id) {
    int i;
    for (i = 0; i < releasedTaps.size(); i++) {
        if (releasedTaps[i]->id == id) {
            break;
        }
    }
    auto result = Tap::alloc(releasedTaps[i]->id, Vec2(releasedTaps[i]->position));
    releasedTaps.erase(releasedTaps.begin() + i);
    return result;
}

std::shared_ptr<Swipe> InputController::consumeReleasedSwipe(long id) {
    int i;
    for (i = 0; i < releasedSwipes.size(); i++) {
        if (releasedSwipes[i]->id == id) {
            break;
        }
    }
    auto result = Swipe::alloc(releasedSwipes[i]->id, Vec2(releasedSwipes[i]->initialPosition), Vec2(releasedSwipes[i]->finalPosition));
    releasedSwipes.erase(releasedSwipes.begin() + i);
    return result;
}

#pragma mark -
#pragma mark Touch and Mouse Callbacks
void InputController::mousePressBeganCB(const MouseEvent& event, Uint8 clicks, bool focus) {
    touchBegan(event.timestamp, 0l, event.position);
}

void InputController::mouseReleasedCB(const MouseEvent& event, Uint8 clicks, bool focus) {
    touchEnded(event.timestamp, 0l, event.position);
}

void InputController::mouseDragCB(const MouseEvent& event, Vec2 previous, bool focus) {
    touchDrag(event.timestamp, 0l, event.position);
}

void InputController::touchBeganCB(const cugl::TouchEvent& event, bool focus) {
    touchBegan(event.timestamp, event.touch, event.position);
}

void InputController::touchEndedCB(const cugl::TouchEvent& event, bool focus) {
    touchEnded(event.timestamp, event.touch, event.position);
}

void InputController::touchDragCB(const cugl::TouchEvent& event, bool focus) {
    touchDrag(event.timestamp, event.touch, event.position);
}

void InputController::touchBegan(const Timestamp timestamp, long id, const Vec2& pos) {
    heldTaps.push_back(Tap::alloc(id, Vec2(pos)));
}

bool InputController::removeHeldTap(long id) {
    int i;
    for (i = 0; i < heldTaps.size(); i++) {
        if (heldTaps[i]->id == id) {
            break;
        }
    }
    if (i < heldTaps.size()) {
        heldTaps.erase(heldTaps.begin() + i);
        return true;
    }
    return false;
}

bool InputController::removeHeldSwipe(long id) {
    int i;
    for (i = 0; i < heldSwipes.size(); i++) {
        if (heldSwipes[i]->id == id) {
            break;
        }
    }
    if (i < heldSwipes.size()) {
        heldSwipes.erase(heldSwipes.begin() + i);
        return true;
    }
    return false;
}

bool InputController::removeConsumedTap(long id) {
    int i;
    for (i = 0; i < consumedTaps.size(); i++) {
        if (consumedTaps[i]->id == id) {
            break;
        }
    }
    if (i < consumedTaps.size()) {
        consumedTaps.erase(consumedTaps.begin() + i);
        return true;
    }
    return false;
}

void InputController::touchEnded(const Timestamp timestamp, long id, const Vec2& pos) {

    if (removeHeldTap(id)) {
        return;
    }
    
    if (removeHeldSwipe(id)) {
        return;
    }
    
    Vec2 initialPos;
    bool isSwipe = false;
    int i;
    for (i = 0; i < consumedSwipes.size(); i++) {
        if (consumedSwipes[i]->id == id) {
            isSwipe = true;
            initialPos = Vec2(consumedSwipes[i]->initialPosition);
            break;
        }
    }
    if (i < consumedSwipes.size()) {
        consumedSwipes.erase(consumedSwipes.begin() + i);
    }
    
    if (isSwipe) {
        releasedSwipes.push_back(Swipe::alloc(id, initialPos, Vec2(pos)));
    } else {
        removeConsumedTap(id);
        releasedTaps.push_back(Tap::alloc(id, Vec2(pos)));
    }
}

void InputController::touchDrag(const Timestamp timestamp, long id, const Vec2& pos) {
    for (auto swipe : heldSwipes) {
        if (swipe->id == id) {
            swipe->finalPosition.set(pos);
            return;
        }
    }
    for (auto swipe : consumedSwipes) {
        if (swipe->id == id) {
            swipe->finalPosition.set(pos);
            return;
        }
    }
    
    Vec2 initialPos;
    
    for (auto tap : heldTaps) {
        if (tap->id == id) {
            initialPos = Vec2(tap->position);
            break;
        }
    }
    
    for (auto tap : consumedTaps) {
        if (tap->id == id) {
            initialPos = Vec2(tap->position);
            break;
        }
    }
    
    panDelta = pos - initialPos;
    
    if (panDelta.length() > EVENT_SWIPE_LENGTH) {
        removeHeldTap(id);
        removeConsumedTap(id);
        heldSwipes.push_back(Swipe::alloc(id, initialPos, pos));
    }
}

void InputController::consumeIgnoredInput() {
    if (heldTaps.size() > 0 && heldTaps.size() == releasedTaps.size()) {
        int i;
        int j;
        for (i = 0; i < heldTaps.size(); i++) {
            for (j = 0; j < releasedTaps.size(); j++) {
                if (releasedTaps[j]->id == heldTaps[i]->id) {
                    break;
                }
            }
            if (j < releasedTaps.size()) {
                heldTaps.erase(heldTaps.begin() + i);
                releasedTaps.erase(releasedTaps.begin() + j);
                break;
            }
        }
    }
    
    if (heldSwipes.size() > 0 && heldSwipes.size() == releasedSwipes.size()) {
        int i;
        int j;
        for (i = 0; i < heldSwipes.size(); i++) {
            for (j = 0; j < releasedSwipes.size(); j++) {
                if (releasedSwipes[j]->id == heldSwipes[i]->id) {
                    break;
                }
            }
            if (j < releasedSwipes.size()) {
                heldSwipes.erase(heldSwipes.begin() + i);
                releasedSwipes.erase(releasedSwipes.begin() + j);
            }
        }
    }
}
