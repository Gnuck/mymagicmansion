//
//  AudioController.hpp
//  RocketDemo
//
//  Created by Nicholas Treat Milner-Gonzales on 4/25/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef AudioController_hpp
#define AudioController_hpp

#include <stdio.h>
#include <cugl/cugl.h>
#include "AbstractController.hpp"
#include "LevelController.hpp"
#include "GameModel.hpp"



class AudioController : public AbstractController {
    
    
public:
#pragma mark -
#pragma mark Constructors
    
    AudioController();
    
    void init();
    
    void dispose();
    
    void playBackgroundMusic(std::shared_ptr<Music> bg_music);
    
    void playBackgroundMusic(std::string music);
    
    void stopBackgroundMusic();
    
    void playSoundEffect(std::string key, std::shared_ptr<Sound> sound);
    
    void stopSoundEffect(std::string key);
};
#endif /* AudioController_hpp */
