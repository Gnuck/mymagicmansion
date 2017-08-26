#include "App.h"

using namespace cugl;


#pragma mark -
#pragma mark Application State

std::shared_ptr<cugl::AssetManager> App::AssetManager = nullptr;
InputController App::InputController;
AnimationController App::AnimationController;
AudioController App:: AudioController;

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
void App::onStartup() {
  // Initialize the asset manager static object.
  AssetManager = AssetManager::alloc();
  
  _batch  = SpriteBatch::alloc();
  
  // Start-up basic input
  #ifdef CU_TOUCH_SCREEN
    Input::activate<Touchscreen>();
  #else
    Input::activate<Mouse>();
    Input::activate<Keyboard>();
  #endif
    
  AssetManager->attach<Font>(FontLoader::alloc()->getHook());
  AssetManager->attach<Texture>(TextureLoader::alloc()->getHook());
  AssetManager->attach<Sound>(SoundLoader::alloc()->getHook());
  AssetManager->attach<Music>(MusicLoader::alloc()->getHook());
  
  // Create a "loading" screen
  _loaded = false;
  _loadingMode.init();
    

  _gameStarted = false;
  
  Application::get()->setClearColor(Color4f::BLACK);
    
  // Queue up the other assets
  AudioEngine::start();
  AssetManager->loadDirectoryAsync("json/assets.json",nullptr);
    
  AnimationController.init();
    
  
  Application::onStartup(); // YOU MUST END with call to parent
}

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
void App::onShutdown() {
  _loadingMode.dispose();
  _gameMode.dispose();
  _titleMode.dispose();
  AssetManager = nullptr;
  _batch = nullptr;
  
  // Shutdown input
  #ifdef CU_TOUCH_SCREEN
    Input::deactivate<Touchscreen>();
  #else
    Input::deactivate<Mouse>();
  #endif
  
  AudioEngine::stop();
  Application::onShutdown();  // YOU MUST END with call to parent
}

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
void App::onSuspend() {
  // The audio isn't going to persist on iPhones at least. Although it'd be nice
  // to pause it and continue where the player left off. However I doubt players
  // are going to remember the audio when they suspended the app...
  AudioEngine::get()->pauseAll();
}

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
void App::onResume() {
  AudioEngine::get()->resumeAll();
}


#pragma mark -
#pragma mark Application Loop

/**
 The root update loop that we have control over.
 */
void App::update(float timestep) {
  // This is the main update loop. We need to update whichever mode is active
  // here. Since we probably only going to have three modes (loading, title and
  // game), we probably don't need to abstract all the modes out in an abstract
  // class. So...just a simple switching will suffice here.
  
  // We also might want to let the loading mode to first load the assets for the
  // title, which should take a lot less time, then when selecting a particular
  // level to play, pull up the loading mode again and load the assets for that
  // level.
  
  // We WON'T be having a title mode for the technical prototype, so the code
  // below will suffice for now.
    
    AnimationController.update(timestep);
  
  if (!_loaded && !_loadingMode.isComplete()) {
      _loadingMode.update(timestep);
  } else if (!_loaded) {
      _loadingMode.dispose(); // Disables the input listeners in this mode
    
      InputController.init();
      auto bg_music = AssetManager->get<Music>(BG_MUSIC2);
      AudioController.playBackgroundMusic(bg_music);
      
      _titleMode.init();
      _loaded = true;
  } else if (!_gameStarted && !_titleMode.levelSelected()) {
      InputController.update(timestep);
      _titleMode.update(timestep);
  } else if (!_gameStarted) {
      auto sound = AssetManager->get<Sound>(DOOR_OPEN);
      AudioController.playSoundEffect(DOOR_OPEN,sound);
      AudioController.stopBackgroundMusic();
      auto bg_music = AssetManager->get<Music>(BG_MUSIC1);
      AudioController.playBackgroundMusic(bg_music);
      _gameMode.init(_titleMode.getLevel());
      _gameStarted = true;
      _titleMode.exit();
  } else if (_gameMode.returnToLevelSelect()) {
      AudioController.stopBackgroundMusic();
      auto bg_music = AssetManager->get<Music>(BG_MUSIC2);
      AudioController.playBackgroundMusic(bg_music);
      _gameStarted = false;
      _gameMode.dispose();
      _titleMode.enterLevelSelect();
  } else {
      InputController.update(timestep);
      _gameMode.update(timestep);
  }
}

/**
 * The method called to draw the application to the screen.
 *
 * This is your core loop and should be replaced with your custom implementation.
 * This method should OpenGL and related drawing calls.
 *
 * When overriding this method, you do not need to call the parent method
 * at all. The default implmentation does nothing.
 */
void App::draw() {
  // Again, here we are just switching between drawing for modes.
  if (!_loaded) {
    _loadingMode.draw(_batch);
  } else if (!_gameStarted) {
    _titleMode.draw(_batch);
  } else {
    _gameMode.draw(_batch);
  }
}

// The function to save to the game progress file
void App::saveGame(int level){
    // save file
    int current_level = 1;
    
    App a;
    string save_file = a.getSaveDirectory() + "/save.txt";
    a.dispose();

    
    // get the current level in save file
    std::string line;
    std::ifstream myfile(save_file.c_str());
    if (myfile.is_open()) {
        while ( getline(myfile, line) ) {
            current_level = std::atoi(line.c_str());
        }
        myfile.close();
    }
    
    // write to save file if the level is greater
    if (level+1>current_level && level+1<=MAX_LEVELS){
        FILE *f = fopen(save_file.c_str(),"w");
        fprintf(f, "%i", level+1);
        fclose(f);
    }
}

// The function to retreive the current level from the game progress file
int App::readSaveFile(){
    int current_level=1;
    
    App a;
    string save_file = a.getSaveDirectory() + "/save.txt";
    a.dispose();
    
    std::string line;
    std::ifstream myfile(save_file.c_str());
    if (myfile.is_open()) {
        while ( getline(myfile, line) ) {
            current_level = std::atoi(line.c_str());
        }
        myfile.close();
    } else { // file has not yet been created, start at level 1
        FILE *f = fopen(save_file.c_str(),"w");
        fprintf(f, "%i", current_level);
        fclose(f);
    }
    return current_level;
}

