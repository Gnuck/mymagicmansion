#ifndef __APP_H__
#define __APP_H__
#include <cugl/cugl.h>
#include <string>
#include "GameMode.hpp"
#include "LoadingMode.h"
#include "InputController.hpp"
#include "AnimationController.hpp"
#include "TitleMode.hpp"
#include "AudioController.hpp"
#include <iostream>
#include <fstream>
#include <string>

#define MAX_LEVELS 30

using namespace std;

/**
 * This class represents the application root for the game.
 */
class App : public cugl::Application {
  protected:
    /** The global sprite batch for drawing (only want one of these) */
    std::shared_ptr<cugl::SpriteBatch> _batch;
    
    // Player modes
    /** The game section. */
    GameMode _gameMode;
    
  
    /** The loading screen. */
    LoadingMode _loadingMode;
    
    /** The menu. */
    TitleMode _titleMode;
  
    /** The title screen (to be coded later). */
    // TitleMode _titleMode;
  
    /** Whether or not we have finished loading all assets */
    bool _loaded;
    
    /** Whether or not the player has started playing a level. */
    bool _gameStarted;
    
    /** the save directory for the game progress file */
    string dir;
    
  public:
    // The app has several things that's more convenient when presented as
    // static members, so here they are:
  
    // The global asset manager
    static std::shared_ptr<cugl::AssetManager> AssetManager;
  
    // The global audio controller - to be reconfigured here
    static AudioController AudioController;
  
    // The global input controller - to be reconfigured here
    static InputController InputController;

  
    // The global animation controller
    static AnimationController AnimationController;
    
    // The function to save to the game progress file
    static void saveGame(int level);
    
    // The function to retreive the current level from the game progress file
    static int readSaveFile(void);
    
  
    #pragma mark Constructors
    /**
     * Creates, but does not initialized a new application.
     *
     * This constructor is called by main.cpp.  You will notice that, like
     * most of the classes in CUGL, we do not do any initialization in the
     * constructor.  That is the purpose of the init() method.  Separation
     * of initialization from the constructor allows main.cpp to perform
     * advanced configuration of the application before it starts.
     */
    App() : cugl::Application(), _loaded(false) {}
    
    /**
     * Disposes of this application, releasing all resources.
     *
     * This destructor is called by main.cpp when the application quits.
     * It simply calls the dispose() method in Application.  There is nothing
     * special to do here.
     */
    ~App() {}
    
    
    #pragma mark Application State
    /**
     * The method called after OpenGL is initialized, but before running the application.
     *
     * This is the method in which all user-defined program intialization should
     * take place.  You should not create a new init() method.
     *
     * When overriding this method, you should call the parent method as the
     * very last line.  This ensures that the state will transition to FOREGROUND,
     * causing the application to run.
     */
    virtual void onStartup() override;
    
    /**
     * The method called when the application is ready to quit.
     *
     * This is the method to dispose of all resources allocated by this
     * application.  As a rule of thumb, everything created in onStartup()
     * should be deleted here.
     *
     * When overriding this method, you should call the parent method as the
     * very last line.  This ensures that the state will transition to NONE,
     * causing the application to be deleted.
     */
    virtual void onShutdown() override;
    
    /**
     * The method called when the application is suspended and put in the background.
     *
     * When this method is called, you should store any state that you do not
     * want to be lost.  There is no guarantee that an application will return
     * from the background; it may be terminated instead.
     *
     * If you are using audio, it is critical that you pause it on suspension.
     * Otherwise, the audio thread may persist while the application is in
     * the background.
     */
    virtual void onSuspend() override;
    
    /**
     * The method called when the application resumes and put in the foreground.
     *
     * If you saved any state before going into the background, now is the time
     * to restore it. This guarantees that the application looks the same as
     * when it was suspended.
     *
     * If you are using audio, you should use this method to resume any audio
     * paused before app suspension.
     */
    virtual void onResume()  override;
  
  
    #pragma mark Application Loop
    /**
     * The method called to update the application data.
     *
     * This is your core loop and should be replaced with your custom implementation.
     * This method should contain any code that is not an OpenGL call.
     *
     * When overriding this method, you do not need to call the parent method
     * at all. The default implmentation does nothing.
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
    virtual void update(float timestep) override;
    
    /**
     * The method called to draw the application to the screen.
     *
     * This is your core loop and should be replaced with your custom implementation.
     * This method should OpenGL and related drawing calls.
     *
     * When overriding this method, you do not need to call the parent method
     * at all. The default implmentation does nothing.
     */
    virtual void draw() override;
};

#endif
