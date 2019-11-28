#include <irrlicht.h>
#include <iostream>
#include <stdlib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace irr;
using namespace std;

// The image is splitted into numTiles.X tiles horizontally and
// numTiles.Y tiles vertically.
core::vector2d<s32> numTiles = core::vector2d<s32>(5, 5);
core::vector2d<s32> oldNumTiles(numTiles);

// Tile size of the original image.
core::vector2d<s32> orgTileSize = core::vector2d<s32>(0, 0);

// Tile size of the scaled image.
core::vector2d<s32> destTileSize = core::vector2d<s32>(0, 0);

IrrlichtDevice* device = 0;
video::IVideoDriver* driver = 0;
video::ITexture* image = 0;
gui::IGUIEnvironment* env = 0;
gui::IGUIContextMenu* menu = 0;


core::dimension2d<s32> orgImageSize;
core::dimension2d<s32> destImageSize;

// The scale factor between original image and destination image.
f32 scale = 1.0;

bool scaleImage = true;

// The whole image is shown for this number of milliseconds.
u32 showTill = 0;

core::stringc currentFilename = "./media/example.png";

void loadImage(const c8* filename);
void createTileNumberBox();
void createSettingsBox();


// A Tile is one piece of an image. It has a position in the grid,
// a position in the window and a flag indicating that the current
// position is the original one.
class Tile
{
public:

  // Constructor
  Tile(core::vector2d<s32> gpos, core::vector2d<s32> wpos)
    : gridPos(gpos), windowPos(wpos), isfix(false) {}

  // Copy constructor
  Tile(Tile& t)
    : gridPos(t.gridPos), windowPos(t.windowPos), isfix(t.isfix) {}

  // Returns true if the tile has the correct position and should
  // not me moved anymore.
  bool isFix() { return isfix; }

  // Called when the tile is put down. It checks if the position
  // is correct with a tolerance of 15 pixels.
  void putDown()
  {
    if ((gridPos * destTileSize - windowPos).getLength() < 15) {
      windowPos = gridPos * destTileSize;
      isfix = true;
    }
  }

  // The position in the grid. Values are in the range 0 to numTiles.
  core::vector2d<s32> gridPos;

  // The position in the window.
  core::vector2d<s32> windowPos;

  // Flag indicating that the position in the window is correct and
  // the tile can not be moved anymore.
  bool isfix;
};

// List of all tiles. The last created or moved tiles are at the end
// of the list.
core::list<Tile*> tiles;


// ---------------------------------------------------------------------


// A StraightMovingTile moves a tile straight to a destination position.
// It's implemented as a singleton.
class StraightMovingTile
{
public:
  static void create(Tile* t)
  {
    if (0 == instance) instance = new StraightMovingTile(t);
  }

  static bool exists()
  {
    if (0 == instance) return false;
    else return true;
  }

  static void move()
  {
    if (0 != instance) instance->_move();
  }

private:
  static StraightMovingTile* instance;

  StraightMovingTile(Tile* t)
  {
    myTile = t;
    startPos = t->windowPos;
    startTime = device->getTimer()->getTime();
    vector = t->gridPos * destTileSize - startPos;
    timeForWay = (u32)vector.getLength();
  }

  void _move()
  {
    u32 t = device->getTimer()->getTime() - startTime;
    if (t>=timeForWay) {
      myTile->windowPos = myTile->gridPos * destTileSize;
      myTile->putDown();
      delete instance;
      instance = 0;
    } else if (t>0) {
      core::vector2d<s32> pos = startPos;
      pos += vector * (f32)t/(f32)timeForWay;
      myTile->windowPos = pos;
    }
  }

  Tile* myTile;
  core::vector2d<s32> startPos;
  core::vector2d<s32> vector;
  u32 startTime;
  u32 timeForWay;
};

StraightMovingTile* StraightMovingTile::instance = 0;


// ---------------------------------------------------------------------


class MyEventReceiver : public IEventReceiver
{
public:
  MyEventReceiver() : carryTile(0), delta(core::vector2d<s32>(0,0)) {}

  bool OnEvent(const SEvent &event)
  {
    if (EET_MOUSE_INPUT_EVENT == event.EventType) {
      switch (event.MouseInput.Event) {

      case EMIE_LMOUSE_PRESSED_DOWN:  // Left mouse button was pressed down.
	{
	  // Iterate from top to down through the tiles.
	  core::list<Tile*>::Iterator itr = tiles.getLast();
	  while (itr != tiles.end()) {
	    if (((*itr)->windowPos.X < event.MouseInput.X) &&
		((*itr)->windowPos.Y < event.MouseInput.Y) &&
		((*itr)->windowPos.X + destTileSize.X > event.MouseInput.X) &&
		((*itr)->windowPos.Y + destTileSize.Y > event.MouseInput.Y) &&
		(false == (*itr)->isFix())) {
	      // Found the highest tile under the mouse.
	      // Move that tile to the end of the list.
	      carryTile = new Tile(**itr);
	      tiles.push_back(carryTile);
	      tiles.erase(itr);

	      delta.X = event.MouseInput.X - carryTile->windowPos.X;
	      delta.Y = event.MouseInput.Y - carryTile->windowPos.Y;

	      break;
	    }
	    itr--;
	  }
	  break;
	}

      case EMIE_LMOUSE_LEFT_UP:       // Left mouse button was left up.
	{
	  if (0 != carryTile) {
	    carryTile->putDown();
	    carryTile = 0;
	  }
	  break;
	}

      case EMIE_MOUSE_MOVED:          // The mouse cursor changed its position.
	{
	  if (0 != carryTile) {
	    carryTile->windowPos.X = event.MouseInput.X - delta.X;
	    carryTile->windowPos.Y = event.MouseInput.Y - delta.Y;
	  }
	  if (event.MouseInput.Y < 50 && false == menu->isVisible()) menu->setVisible(true);
	  if (event.MouseInput.Y > 100 && true == menu->isVisible()) menu->setVisible(false);

	  break;
	}

      }

    }
    if (event.EventType == EET_GUI_EVENT) {
      s32 id = event.GUIEvent.Caller->getID();

      switch(event.GUIEvent.EventType) {
      case gui::EGET_CHECKBOX_CHANGED:
	{
	  if (321 == id) scaleImage = ((gui::IGUICheckBox*)event.GUIEvent.Caller)->isChecked();
	  break;
	}

      case gui::EGET_MENU_ITEM_SELECTED:
	{
	  gui::IGUIContextMenu* menu = (gui::IGUIContextMenu*)event.GUIEvent.Caller;
	  s32 id = menu->getItemCommandId(menu->getSelectedItem());

	  if (101 == id) { // Open
	    env->addFileOpenDialog(L"Please select an image to open.");
	  }
	  else if (102 == id) { // Restart
	    loadImage(currentFilename.c_str());
	  }
	  else if (103 == id) { // Number tiles
	    createTileNumberBox();
	  }
	  else if (104 == id) { // Settings
	    createSettingsBox();
	  }
	  else if (150 == id) { // Quit
	    device->closeDevice();
	    return true;
	  }
	  else if (201 == id) { // Show image for 1 second
	    showTill = 1000 + device->getTimer()->getTime();
	  }
	  else if (202 == id) { // Hint
	    // Iterate over all tiles and write the not fixed tiles into a temporary array.
	    // From this array choose randomly one tile and move it to its correct position.
	    core::list<Tile*>::Iterator itr = tiles.begin();
	    core::array<Tile*> notFixedTiles;
	    while (itr != tiles.end()) {
	      if (false == (*itr)->isFix()) notFixedTiles.push_back(*itr);
	      itr++;
	    }
	    if (false == notFixedTiles.empty()) {
	      s32 hint = (f32)notFixedTiles.size()*rand()/(RAND_MAX+1.0);
	      StraightMovingTile::create(notFixedTiles[hint]);
	      notFixedTiles.clear();
	    }
	  }
	  else if (203 == id) { // About
	    core::dimension2d<u32> dim = device->getVideoDriver()->getScreenSize();
	    core::stringw str = L"ScreenSize=";
	    str += dim.Width;
	    str += " x ";
	    str += dim.Height;
	    env->addMessageBox(L"IrrPhotoPuzzle", L"Created by Thomas Wernecke\n\nEmail: irrphotopuzzle@gmail.com");
	  }
	  break;
	}
      case gui::EGET_FILE_SELECTED:
	{
	  gui::IGUIFileOpenDialog* dialog =(gui::IGUIFileOpenDialog*)event.GUIEvent.Caller;
	//  currentFilename = core::stringc(dialog->getFilename());
	//  loadImage(currentFilename.c_str());
	  break;
	}
      case gui::EGET_SCROLL_BAR_CHANGED:
	{
	  if (301 == id) {
	    numTiles.X = 1 + ((gui::IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
	    tiles.clear();
	    orgTileSize.X = orgImageSize.Width/numTiles.X;
	    orgTileSize.Y = orgImageSize.Height/numTiles.Y;
	    destTileSize.X = (s32)(scale * orgTileSize.X);
	    destTileSize.Y = (s32)(scale * orgTileSize.Y);
	  }
	  if (302 == id) {
	    numTiles.Y = 1+ ((gui::IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
	    tiles.clear();
	    orgTileSize.X = orgImageSize.Width/numTiles.X;
	    orgTileSize.Y = orgImageSize.Height/numTiles.Y;
	    destTileSize.X = (s32)(scale * orgTileSize.X);
	    destTileSize.Y = (s32)(scale * orgTileSize.Y);
	  }
	  break;
	}
      case gui::EGET_BUTTON_CLICKED:
	{
	  if (310 == id) { // TileNumerBox: ok
	    event.GUIEvent.Caller->getParent()->remove();
	    loadImage(currentFilename.c_str());
	    oldNumTiles = numTiles;
	  }
	  if (311 == id) { // TileNumerBox: cancel
	    event.GUIEvent.Caller->getParent()->remove();
	    numTiles = oldNumTiles;
	    loadImage(currentFilename.c_str());
	  }
	  if (320 == id) { // SettingsBox: ok
	    event.GUIEvent.Caller->getParent()->remove();
	  }

	  break;
	}

      }
    }
    return false;
  }

private:
  // Tile carried as long as the left mouse button is pressed.
  Tile* carryTile;

  // Delta between the mouse position and the tile position.
  core::vector2d<s32> delta;
};


// ---------------------------------------------------------------------


// Returns a random initial position for a tile in the area between image and
// window border. Depending on the image and window size this position is right
// to the image or under it.
core::vector2d<s32> getRandomTilePos() {
  core::dimension2d<u32> dim = device->getVideoDriver()->getScreenSize();
  core::vector2d<s32> vec;
  f32 varX, varY;
  if (dim.Width - destImageSize.Width > dim.Height - destImageSize.Height) {
    varX = dim.Width - destImageSize.Width - destTileSize.X;
    varY = dim.Height - destTileSize.Y;
    vec = core::vector2d<s32>(destImageSize.Width+(int) (varX*rand()/(RAND_MAX+1.0)), 1+(int) (varY*rand()/(RAND_MAX+1.0)));
  } else {
    varX = dim.Width - destTileSize.X;
    varY = dim.Height - destImageSize.Height - destTileSize.Y;
    vec = core::vector2d<s32>(1+(int) (varX*rand()/(RAND_MAX+1.0)), destImageSize.Height+(int) (varY*rand()/(RAND_MAX+1.0)));
  }

  return vec;
}


// ---------------------------------------------------------------------


// Loads an image, calculates the dependent parameters, creates the tiles and
// places them to their initial position.
void loadImage(const c8* filename)
{
  image = driver->getTexture(filename);

  orgImageSize = image->getOriginalSize();
  destImageSize = orgImageSize;

  core::dimension2d<u32> dim = device->getVideoDriver()->getScreenSize();

  if (scaleImage) {
    f32 widthScale = (f32)dim.Width / orgImageSize.Width;
    f32 heightScale = (f32)dim.Height / orgImageSize.Height;
    if (widthScale < heightScale) scale = widthScale; else scale = heightScale;
  } else scale = 1.0;

  destImageSize.Width = (s32)(scale * orgImageSize.Width);
  destImageSize.Height = (s32)(scale * orgImageSize.Height);

  orgTileSize.X = orgImageSize.Width/numTiles.X;
  orgTileSize.Y = orgImageSize.Height/numTiles.Y;
  destTileSize.X = (s32)(scale * orgTileSize.X);
  destTileSize.Y = (s32)(scale * orgTileSize.Y);

  // Create the tiles.
  tiles.clear();
  for (s32 x=0; x<numTiles.X; x++) {
    for (s32 y=0; y<numTiles.Y; y++) {
      core::vector2d<s32> pos = getRandomTilePos();
      Tile* tile = new Tile(core::vector2d<s32>(x, y), pos);
      tiles.push_back(tile);
    }
  }
}


// ---------------------------------------------------------------------


void createTileNumberBox()
{
  // remove tileSize box if already there
  //IGUIEnvironment* env = device->getGUIEnvironment();
  gui::IGUIElement* root = env->getRootGUIElement();
  gui::IGUIElement* e = root->getElementFromId(5000, true);
  if (e) e->remove();

  gui::IGUIWindow* box = env->addWindow(core::rect<s32>(200,200,400,400),
					false, L"TileNumberBox", 0, 5000);

  env->addStaticText(L"Horizonal tile number:", core::rect<s32>(5,30,150,50), false, true, box, false);
  gui::IGUIScrollBar* scrollbar = env->addScrollBar(true, core::rect<s32>(5,55,190,70), box, 301);
  scrollbar->setPos(numTiles.X);
  scrollbar->setMax(15);

  env->addStaticText(L"Vertical tile number:", core::rect<s32>(5,90,150,110), false, true, box, false);
  scrollbar = env->addScrollBar(true, core::rect<s32>(5,115,190,130), box, 302);
  scrollbar->setPos(numTiles.Y);
  scrollbar->setMax(15);

  env->addButton(core::rect<s32>(20,150,70,180), box, 310, L"set");
  env->addButton(core::rect<s32>(130,150,180,180), box, 311, L"cancel");
}


// ---------------------------------------------------------------------


void createSettingsBox()
{
  // remove settings box if already there
  gui::IGUIElement* root = env->getRootGUIElement();
  gui::IGUIElement* e = root->getElementFromId(5010, true);
  if (e) e->remove();

  gui::IGUIWindow* box = env->addWindow(core::rect<s32>(200,200,380,300),
					false, L"SettingsBox", 0, 5010);

  env->addCheckBox(scaleImage, core::rect<s32>(10,20,200,50), box, 321, L"Scale image to window");
  env->addButton(core::rect<s32>(60,50,120,80), box, 320, L"set");
}


// ---------------------------------------------------------------------
void main_loop(){


  device->run();
   // if (device->isWindowActive()) {
      u32 now = device->getTimer()->getTime();
      //driver->beginScene(true, true, video::SColor(0,120,102,136));
      driver->beginScene(true, true, video::SColor(0,50,50,255));

      if (true == StraightMovingTile::exists()) StraightMovingTile::move();

      // Draw the grid.
      for (s32 x=0; x<numTiles.X * destTileSize.X; x+= destTileSize.X) driver->draw2DLine(core::position2d<s32>(x, 0), core::position2d<s32>(x, numTiles.Y * destTileSize.Y));
      for (s32 y=0; y<numTiles.Y * destTileSize.Y; y+= destTileSize.Y) driver->draw2DLine(core::position2d<s32>(0, y), core::position2d<s32>(numTiles.X * destTileSize.X,  y));

      // Draw the tiles.
      core::list<Tile*>::Iterator itr = tiles.begin();
      while (itr != tiles.end()) {
	driver->draw2DImage(image,
			    core::rect<s32>((*itr)->windowPos.X, (*itr)->windowPos.Y, destTileSize.X + (*itr)->windowPos.X, destTileSize.Y + (*itr)->windowPos.Y), // destRect
			    core::rect<s32>(orgTileSize.X * (*itr)->gridPos.X, orgTileSize.Y * (*itr)->gridPos.Y, orgTileSize.X * (1+(*itr)->gridPos.X) , orgTileSize.Y * (1+(*itr)->gridPos.Y) )); // sourceRect

	itr++;
      }

      // Draw the whole image
      if (now < showTill) driver->draw2DImage(image, core::rect<s32>(0, 0, destImageSize.Width, destImageSize.Height), core::rect<s32>(0, 0, orgImageSize.Width, orgImageSize.Height));

      env->drawAll();

      driver->endScene();
  //  }


}

int main()
{
  MyEventReceiver receiver;

  // let user select driver type

  video::E_DRIVER_TYPE driverType;

//  printf("Please select the driver you want for this example:\n"	\
//	 " (a) Direct3D 9.0c\n (b) Direct3D 8.1\n (c) OpenGL 1.5\n"	\
//	 " (otherKey) exit\n\n");

//  char i;
//  std::cin >> i;
//
//  switch(i) {
//  case 'a': driverType = video::EDT_DIRECT3D9;break;
//  case 'b': driverType = video::EDT_DIRECT3D8;break;
//  case 'c': driverType = video::EDT_OPENGL;   break;
//  default: return 0;
//  }

#ifdef __EMSCRIPTEN__
	driverType = video::EDT_OGLES2;
  device = createDevice(driverType, core::dimension2d<u32>(600, 600), 32, false, false, false, &receiver);
  if (device == 0) return 1; // could not create selected driver.
#else
driverType = video::EDT_OPENGL;
  device = createDevice(driverType, core::dimension2d<u32>(600, 600), 32, false, false, false, &receiver);
  if (device == 0) return 1; // could not create selected driver.
#endif
  driver = device->getVideoDriver();

  env = device->getGUIEnvironment();

  srand(device->getTimer()->getRealTime());

  loadImage(currentFilename.c_str());

  // gui
  gui::IGUISkin* skin = env->getSkin();
  gui::IGUIFont* font = env->getFont("./media/fonthaettenschweiler.bmp");
  if (font) skin->setFont(font);

  menu = env->addMenu();
  menu->addItem(L"File", -1, true, true);
  menu->addItem(L"Help", -1, true, true);
  menu->setVisible(false);

  gui::IGUIContextMenu* submenu;
  submenu = menu->getSubMenu(0);
  submenu->addItem(L"Open", 101);
  submenu->addItem(L"Restart", 102);
  submenu->addItem(L"Number tiles", 103);
  submenu->addItem(L"Settings", 104);
  submenu->addSeparator();
  submenu->addItem(L"Quit", 150);

  submenu = menu->getSubMenu(1);
  submenu->addItem(L"Show", 201);
  submenu->addItem(L"Hint", 202);
  submenu->addItem(L"About", 203);



  // Make the skin intransparent.
  for (s32 i=0; i<gui::EGDC_COUNT ; ++i) {
    video::SColor col = env->getSkin()->getColor((gui::EGUI_DEFAULT_COLOR)i);
    col.setAlpha(255);
    env->getSkin()->setColor((gui::EGUI_DEFAULT_COLOR)i, col);
  }

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop,0,1);
#else

  while(device->run() && driver) {
    if (device->isWindowActive()) {
      u32 now = device->getTimer()->getTime();
      //driver->beginScene(true, true, video::SColor(0,120,102,136));
      driver->beginScene(true, true, video::SColor(0,50,50,255));

      if (true == StraightMovingTile::exists()) StraightMovingTile::move();

      // Draw the grid.
      for (s32 x=0; x<numTiles.X * destTileSize.X; x+= destTileSize.X) driver->draw2DLine(core::position2d<s32>(x, 0), core::position2d<s32>(x, numTiles.Y * destTileSize.Y));
      for (s32 y=0; y<numTiles.Y * destTileSize.Y; y+= destTileSize.Y) driver->draw2DLine(core::position2d<s32>(0, y), core::position2d<s32>(numTiles.X * destTileSize.X,  y));

      // Draw the tiles.
      core::list<Tile*>::Iterator itr = tiles.begin();
      while (itr != tiles.end()) {
	driver->draw2DImage(image,
			    core::rect<s32>((*itr)->windowPos.X, (*itr)->windowPos.Y, destTileSize.X + (*itr)->windowPos.X, destTileSize.Y + (*itr)->windowPos.Y), // destRect
			    core::rect<s32>(orgTileSize.X * (*itr)->gridPos.X, orgTileSize.Y * (*itr)->gridPos.Y, orgTileSize.X * (1+(*itr)->gridPos.X) , orgTileSize.Y * (1+(*itr)->gridPos.Y) )); // sourceRect

	itr++;
      }

      // Draw the whole image
      if (now < showTill) driver->draw2DImage(image, core::rect<s32>(0, 0, destImageSize.Width, destImageSize.Height), core::rect<s32>(0, 0, orgImageSize.Width, orgImageSize.Height));

      env->drawAll();

      driver->endScene();
    }
  }
  #endif
  device->drop();
  return 0;
}

