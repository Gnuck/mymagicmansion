#ifndef __LOADING_MODE_H__
#define __LOADING_MODE_H__
#include <cugl/cugl.h>

#pragma mark -
#pragma mark Loading Mode
/**
 This is the loading mode that we are going to modify.
 */
class LoadingMode {
protected:
  // NO CONTROLLER (ALL IN SEPARATE THREAD)
  
  // VIEW
  /** The root of our scene graph. */
  std::shared_ptr<cugl::Scene>  _scene;
  /** The animated progress bar */
  std::shared_ptr<cugl::ProgressBar>  _bar;
  /** The "play" button */
  std::shared_ptr<cugl::Button> _button;
  
  // MODEL
  /** The progress displayed on the screen */
  float _progress;
  /** Whether or not the player has pressed play to continue */
  bool _completed;
  
    std::vector<std::shared_ptr<cugl::PolygonNode>> _progressImages;
    
    
public:
#pragma mark -
#pragma mark Constructors
  /**
   * Creates a new loading mode with the default values.
   *
   * This constructor does not allocate any objects or start the game.
   * This allows us to use the object without a heap pointer.
   */
  LoadingMode() : _progress(0.0f), _completed(false) {}
  
  /**
   * Disposes of all (non-static) resources allocated to this mode.
   *
   * This method is different from dispose() in that it ALSO shuts off any
   * static resources, like the input controller.
   */
  ~LoadingMode() { dispose(); }
  
  /**
   * Disposes of all (non-static) resources allocated to this mode.
   */
  void dispose();
  
  /**
   * Initializes the controller contents, making it ready for loading
   *
   * The constructor does not allocate any objects or memory.  This allows
   * us to have a non-pointer reference to this controller, reducing our
   * memory allocation.  Instead, allocation happens in this method.
   *
   * @param assets    The (loaded) assets for this game mode
   *
   * @return true if the controller is initialized properly, false otherwise.
   */
  bool init();
  
  
#pragma mark -
#pragma mark Progress Monitoring
  /**
   * The method called to update the game mode.
   *
   * This method updates the progress bar amount.
   *
   * @param timestep  The amount of time (in seconds) since the last frame
   */
  void update(float timestep);
  
  /**
   * The method called to draw the application to the screen.
   *
   * This method should contain OpenGL and related drawing calls.
   */
  void draw(const std::shared_ptr<cugl::SpriteBatch>& batch);
  
  /**
   * Returns true if loading is complete and the player pressed play
   *
   * @return true if loading is complete and the player pressed play
   */
  bool isComplete( ) const { return _completed; }
  
  /**
   * Returns true if loading is complete, but the player has not pressed play
   *
   * @return true if loading is complete, but the player has not pressed play
   */
  bool isPending( ) const;
};

#endif

