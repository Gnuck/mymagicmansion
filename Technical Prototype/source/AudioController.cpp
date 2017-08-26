//
//  AudioController.cpp
//  RocketDemo
//
//  Created by Nicholas Treat Milner-Gonzales on 4/25/17.
//  Copyright © 2017 Game Design Initiative at Cornell. All rights reserved.
//

#include "AudioController.hpp"


AudioController::AudioController(){
}

void AudioController::playBackgroundMusic(std::shared_ptr<Music> bg_music){
        AudioEngine::get()->queueMusic(bg_music,true, 1, 2.2);
}



void AudioController::stopBackgroundMusic(){
    AudioEngine::get()->stopMusic(2.2);
    
}

void AudioController::playSoundEffect(std::string key, std::shared_ptr<Sound> sound){
    AudioEngine::get()->playEffect(key, sound);
}

void AudioController::stopSoundEffect(std::string key){
    AudioEngine::get()->stopEffect(key);
}
