#include "LoadingMode.h"
#include "App.h"
#include "Constants.h"

using namespace cugl;

#pragma mark -
#pragma mark Loading Assets

/** The reference to the progress bar textire */
#define PROGRESS_TEXTURE "textures/progressbar.png"
#define PROGRESS_KEY     "progress"
#define PROGRESS_VOFF   0.15f

float BACK_RECT[] = {0.0f,  1.0f,  0.0625f, 0.4375f};
float FORE_RECT[] = {0.04f, 0.96f, 0.5625f, 0.9375f};
float LEFT_RECT[] = {0.00f, 0.04f, 0.5625f, 0.9375f};
float RGHT_RECT[] = {0.96f, 1.00f, 0.5625f, 0.9375f};


#pragma mark -
#pragma mark Constructors

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
bool LoadingMode::init() {
  // We need to load the assets we need to draw the loading screen here.
  App::AssetManager->load<Texture>(PROGRESS_KEY,PROGRESS_TEXTURE);
    App::AssetManager->load<Texture>("loading1", "textures/loading1.png");
    App::AssetManager->load<Texture>("loading2", "textures/loading2.png");
    App::AssetManager->load<Texture>("loading3", "textures/loading3.png");
    App::AssetManager->load<Texture>("loading4", "textures/loading4.png");
    App::AssetManager->load<Texture>("loading5", "textures/loading5.png");
    App::AssetManager->load<Texture>("loading6", "textures/loading6.png");
    App::AssetManager->load<Texture>("loading7", "textures/loading7.png");
    App::AssetManager->load<Texture>("loading-background", "textures/loading-background.png");
  
  Size size = Application::get()->getDisplaySize();
  size *= GAME_WIDTH/size.width;
  _scene = Scene::alloc(size);
    
    Size mockupSize(MOCKUP_WIDTH, MOCKUP_HEIGHT);
    float scale = GAME_WIDTH/mockupSize.width;
    
    std::shared_ptr<Node> loading = Node::alloc();
    loading->setContentSize(mockupSize);
    loading->setAnchor(Vec2::ANCHOR_MIDDLE);
    loading->setPosition(Vec2(size.width/2.0, size.height/2.0));
    loading->setScale(scale);
    _scene->addChild(loading);
    
    auto loadingBackground = PolygonNode::allocWithTexture(App::AssetManager->get<Texture>("loading-background"));
    loadingBackground->setAnchor(Vec2::ANCHOR_MIDDLE);
    loadingBackground->setPosition(Vec2(mockupSize.width/2.0, mockupSize.height/2.0));
    loading->addChild(loadingBackground);
    
    int x = 181;
    auto image = PolygonNode::allocWithTexture(App::AssetManager->get<Texture>("loading1"));
    image->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
    image->setPosition(Vec2(x, 485));
    x += image->getWidth();
    _progressImages.push_back(image);
    loading->addChild(image);
    
    auto leafTexture = App::AssetManager->get<Texture>("loading7");
    
    for (int i = 2; i <= 6; i++) {
        auto leaf1 = PolygonNode::allocWithTexture(leafTexture);
        leaf1->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        auto leaf2 = PolygonNode::allocWithTexture(leafTexture);
        leaf2->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        
        x += 36;
        leaf1->setPosition(Vec2(x, 485 + 28));
        x += 29 + 36;
        leaf2->setPosition(Vec2(x, 485 + 28));
        x += 29 + 36;
        
        image = PolygonNode::allocWithTexture(App::AssetManager->get<Texture>("loading" + std::to_string(i)));
        image->setAnchor(Vec2::ANCHOR_BOTTOM_LEFT);
        image->setPosition(Vec2(x, 485));
        x += image->getWidth();
        
        leaf1->setVisible(false);
        leaf2->setVisible(false);
        image->setVisible(false);
        _progressImages.push_back(leaf1);
        _progressImages.push_back(leaf2);
        _progressImages.push_back(image);
        loading->addChild(leaf1);
        loading->addChild(leaf2);
        loading->addChild(image);
    }

  
  // The progress bar
  auto texture = App::AssetManager->get<Texture>(PROGRESS_KEY);
  auto background = texture->getSubTexture(BACK_RECT[0], BACK_RECT[1], BACK_RECT[2], BACK_RECT[3]);
  auto foreground = texture->getSubTexture(FORE_RECT[0], FORE_RECT[1], FORE_RECT[2], FORE_RECT[3]);
  auto begincap   = texture->getSubTexture(LEFT_RECT[0], LEFT_RECT[1], LEFT_RECT[2], LEFT_RECT[3]);
  auto finalcap   = texture->getSubTexture(RGHT_RECT[0], RGHT_RECT[1], RGHT_RECT[2], RGHT_RECT[3]);
  
  _bar = ProgressBar::allocWithCaps(background,foreground,begincap,finalcap);
  _bar->setAnchor(Vec2::ANCHOR_MIDDLE);
  _bar->setPosition(Vec2(size.width/2.0f,size.height*PROGRESS_VOFF));
  
  Application::get()->setClearColor(Color4(192,192,192,255));
  return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void LoadingMode::dispose() {
  // Deactivate the button (platform dependent)
  if (isPending()) {
    _button->deactivate();
  }
  _button = nullptr;
  _bar = nullptr;
  _scene = nullptr;
  _progress = 0.0f;
  _completed = false;
}


#pragma mark -
#pragma mark Progress Monitoring
/**
 * The method called to update the game mode.
 *
 * This method updates the progress bar amount.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void LoadingMode::update(float progress) {
  if (_progress < 1) {
    _progress = App::AssetManager->progress();
    if (_progress >= 1) {
      _progress = 1.0f;
      
      // Activate the button (platform dependent)
#if defined CU_TOUCH_SCREEN
      Input::activate<Touchscreen>();
#else
      Input::activate<Mouse>();
#endif
    }
    _bar->setProgress(_progress);
      for (int i = 0; i < _progressImages.size(); i++) {
          if (_progress >= 1.0f*(i+1)/_progressImages.size()) {
              _progressImages.at(i)->setVisible(true);
              if (i == _progressImages.size() - 1) {
                  this->_completed = true;
              }
          }
      }
  }
}

/**
 * The method called to draw the application to the screen.
 *
 * This method should contain OpenGL and related drawing calls.
 */
void LoadingMode::draw(const std::shared_ptr<cugl::SpriteBatch>& batch) {
  _scene->render(batch);
}

/**
 * Returns true if loading is complete, but the player has not pressed play
 *
 * @return true if loading is complete, but the player has not pressed play
 */
bool LoadingMode::isPending( ) const {
  bool result = _button != nullptr && _button->isVisible();
#if defined CU_TOUCH_SCREEN
  result = result && Input::get<Touchscreen>();
#else
  result = result && Input::get<Mouse>();
#endif
  return result;
}

