//
//  Constants.h
//  RocketDemo
//
//  Created by Andy Jiang on 2017/03/31.
//  Copyright © 2017年 Game Design Initiative at Cornell. All rights reserved.
//

#ifndef Constants_h
#define Constants_h

#include <stdio.h>

#define CHARACTER       "character"
#define STAIRS          "stair"
#define COLLECTIBLE     "key"
#define DOOR            "door"
#define FLOOR           "floor"
#define WALL            "wall"

#define WALKING_TEXTURE         "walking"
#define FALLING_TEXTURE         "falling"
#define CLIMBING_TEXTURE        "climbing"
#define PICKUP_TEXTURE          "pickup"
#define DOOR_TEXTURE            "door"
#define COLLECTIBLE_TEXTURE     "key1"
#define UNLOCK_TEXTURE          "lock-blue-glowing"
#define EXCLAMATION_TEXTURE     "exclamation"
#define LEFT_STAIR_SUFFIX       "-left"

#define BG_MUSIC1               "bg-music1"
#define BG_MUSIC2               "bg-music2"
#define DOOR_OPEN               "door_open"
#define GRAB_COLLECTABLE        "grab_collectable"
#define SWITCH_TILE             "switch_tile"
#define UNLOCK_SOUND            "unlock"

#define LEVEL_SELECT_FONT       "felt"

const static std::string PATH_TO_LEVELS = "json/";
const static std::string LEVEL_FILE_PREFIX = "level";


/*** UI numbers ***/

/** This is adjusted by screen aspect ratio to get the height */
#define GAME_WIDTH 1024

#define MOCKUP_WIDTH 1920
#define MOCKUP_HEIGHT 1080

#define LEVEL_LEFT_MARGIN               152
#define LEVEL_TOP_MARGIN                200
#define LEVEL_WINDOW_HORIZONTAL_GAP     165
#define LEVEL_WINDOW_VERTICAL_GAP       144
#define LEVEL_WINDOW_SIDE               206
#define LEVEL_TEXT_TOP_MARGIN           36
#define LEVEL_TEXT_BOTTOM_MARGIN        100

#define PAUSE_BUTTON_TAG        111
#define RESUME_BUTTON_TAG       222
#define RESTART_BUTTON_TAG      333
#define RETURN_MENU_TAG         444
#define LEVEL_SELECT_TAG        555
#define SPEED_BUTTON_TAG        666 
#define CREDITS_TAG             777
#define PLAY_TAG                888
#define EXCLAMATION_TAG         123
#define NEXT_LEVEL_TAG          999


#define FIRST_SCREEN            101
#define SECOND_SCREEN           202
#define THIRD_SCREEN            303

#endif /* Constants_h */
