#include <irrlicht.h>


#include <stdio.h>
#include <iostream>
using namespace std;
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

IGUIFont *font; // blah
u32 mask = 0xF00000;

class MineSweeperTileNode : public ISceneNode
{
public:
	MineSweeperTileNode(ISceneManager* smgr, s32 xpos, s32 zpos, IMesh* floor, IMesh* mine, IMesh* flag)
		: ISceneNode(smgr->getRootSceneNode(), smgr, mask | (zpos*100 +xpos) ),
	  HasMine(false), Reveal(false), Flagged(false), Highlight(false), XPos(xpos), ZPos(zpos),
	  Count(0)
	{
		setDebugName("Tile");

		Box = floor->getBoundingBox();
		Box.addInternalPoint(Box.MinEdge + vector3df(0,1,0));
		vector3df p = floor->getBoundingBox().getExtent();
		p.X *= xpos;
		p.Z *= zpos;
		setPosition(p);
	//	setAutomaticCulling(EAC_OFF);
		// add floor
		FloorNode = smgr->addMeshSceneNode(floor, this);
		// add mine
		MineNode = smgr->addMeshSceneNode(mine, this);
		MineNode->setVisible(false);
		// add mine count / flag
		TextNode = smgr->addBillboardTextSceneNode(font, L"?", this, dimension2d<f32>(1.5,1.5));
		TextNode->setVisible(false);
	}

	virtual ESCENE_NODE_TYPE getType() const { return (ESCENE_NODE_TYPE) MAKE_IRR_ID('t','i','l','e'); }

	virtual void OnRegisterSceneNode()
	{
		SceneManager->registerNodeForRendering(this);
		if (!Reveal)
		{
			if (Flagged)
			{
				FloorNode->getMaterial(0).AmbientColor.set(255,255,10,10);
			}
			else
			{
				FloorNode->getMaterial(0).AmbientColor.set(255,100,100,100);
			}
		}
		else
		{
			if (HasMine)
			{
				FloorNode->getMaterial(0).AmbientColor.set(255,255,10,10);
			}
			else
			{
				FloorNode->getMaterial(0).AmbientColor.set(255,10,255,10);
			}
		}
		if (Flagged)
		{
			u32 poop=1;
		}

		MineNode->setVisible( ( Reveal && HasMine ) || (!Reveal && Flagged) );
		TextNode->setVisible( (Reveal && Count>0 && !HasMine) || (!Reveal && Flagged) );
TextNode->setMaterialFlag(EMF_ZBUFFER,0);
		ISceneNode::OnRegisterSceneNode();
	}

	void setCount(s32 c)
	{
		Count = c;
		if (c != 0)
        {
            // NUMBER TEXT FOR MINES
		//    printf ("c is: %i  ", c);
			NumberText = (stringw)c;
			TextNode->setText(NumberText.c_str());
		//	TextNode->
        //    printf ("NumberText is: %s \n", NumberText.c_str());
		}
	}
	void toggleFlag()
	{
		if (Reveal)
			return;

		Flagged = !Flagged;
		if (!Flagged)
			setCount(Count);
		else
			TextNode->setText(L"?");
	}
	void reveal()
	{
		if (Flagged)
			toggleFlag();
		Reveal = true;
	}

	virtual void render()
	{
		if (Highlight)
		{
			SMaterial m;
			SceneManager->getVideoDriver()->setMaterial(m);
			SceneManager->getVideoDriver()->setTransform(ETS_WORLD, AbsoluteTransformation);
			SceneManager->getVideoDriver()->draw3DBox(Box);
			Highlight = false;
		}
	}

	virtual const core::aabbox3d<f32>& getBoundingBox() const { return Box; }

	aabbox3d<f32> Box;
	ISceneNode *FloorNode, *MineNode, *FlagNode;
	irr::scene::IBillboardTextSceneNode *TextNode;

	bool Highlight;
	stringw NumberText;
	bool HasMine;
	bool Reveal;
	bool Flagged;
	s32 Count, XPos, ZPos;
};

class MineSweeper : public IEventReceiver
{
public:

	MineSweeper()
	{
		s32 w=800, h=600;
		device =
			createDevice( video::EDT_OPENGL, dimension2du(w, h), 32,
				false, false, false, 0);

		device->setWindowCaption(L"MINES!");

		device->getFileSystem()->addZipFileArchive("sweeper.dat");

		driver = device->getVideoDriver();
		smgr = device->getSceneManager();
		guienv = device->getGUIEnvironment();

		// we are our own event receiver
		device->setEventReceiver(this);
		smgr->setAmbientLight(SColorf(1.0, 1.0, 1.0, 1.0));

		// load up the graphics.
		Floor = smgr->getMesh("ground.obj")->getMesh(0);
		Mine = smgr->getMesh("mine.obj")->getMesh(0);

		//font = guienv->getFont("fonthaettenschweiler.bmp");
		font = guienv->getSkin()->getFont();

		// add camera
		Camera = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(1,1,1), 0);

		// add losing text
		GameOverIcon = guienv->addImage(driver->getTexture("game over.png"), core::position2di(w/2-128,h/2-128) );
		GameOverIcon->setVisible(false);
		WinnerIcon = guienv->addImage(driver->getTexture("winner.png"), core::position2di(w/2-128,h/2-128) );
		WinnerIcon->setVisible(false);

		QuitGame = false;
		Playing = false;
	}

	~MineSweeper()
	{
		device->drop();
	}

	void checkMouse()
	{
		HighlightX = -1;
		HighlightZ = -1;
		if (!Playing)
			return;

		ISceneNode* node = smgr->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB(device->getCursorControl()->getPosition(), mask);

		if (node)
		{
			while (node && node->getType() != (ESCENE_NODE_TYPE) MAKE_IRR_ID('t','i','l','e'))
				node = node->getParent();

			if (node)
			{

				MineSweeperTileNode * n2 = (MineSweeperTileNode*) node;
				n2->Highlight = true;
				HighlightX = n2->XPos;
				HighlightZ = n2->ZPos;
			}
		}
	}

	void run()
	{
		while(!QuitGame)
		{
			QuitGame = !device->run();
			checkMouse();
			draw();
		}
	}

	bool OnEvent(const SEvent& e)
	{

		if (e.EventType == EET_MOUSE_INPUT_EVENT)
		{
			if (e.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
			{
				if (Playing && HighlightX > -1 && HighlightZ > -1)
				{
					if (reveal(HighlightX, HighlightZ, true))
					{
						// game over
						GameOverIcon->setVisible(true);
						for (u32 i=0; i<Tiles.size(); ++i)
							Tiles[i]->reveal();
						Playing = false;
					}
					else
					{
						if ( SquaresLeft == MineCount)
						{
							WinnerIcon->setVisible(true);
							for (u32 i=0; i<Tiles.size(); ++i)
								Tiles[i]->reveal();
							Playing = false;
						}
					}
					return true;
				}
				else if (!Playing)
				{
					// start game
					GameOverIcon->setVisible(false);
					WinnerIcon->setVisible(false);
					newGame(20,20,50);
					Playing = true;
				}

			}
			else if (e.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN && HighlightX > -1 && HighlightZ > -1)
			{
				Tiles[HighlightX + HighlightZ*Width]->toggleFlag();
				return true;
			}
		}

		return false;
	}

	// is there a mine in this position?
	bool reveal(s32 x, s32 z, bool manual=false)
	{
		s32 p = z*Width + x;

		if (Tiles[p]->Reveal)
			return Tiles[p]->HasMine;

		if (manual && Tiles[p]->Flagged)
			return false;

		Tiles[p]->reveal();
		SquaresLeft--;

		if (Tiles[p]->HasMine)	// BOOM! yes it does.
			return true;
		if (Tiles[p]->Count > 0) // no it doesn't, but neighbours do
			return false;

		// we haven't got a mine, flood fill

		if (x != 0)
		{
			reveal(x-1, z);
			if (z != 0) reveal(x-1, z-1);
			if (z != Height-1) reveal(x-1, z+1);
		}
		if (x != Width-1)
		{
			reveal(x+1,z);
			if (z != 0) reveal(x+1, z-1);
			if (z != Height-1) reveal(x+1, z+1);
		}

		if (z != 0) reveal(x, z-1);
		if (z != Height-1) reveal(x, z+1);
	}
	void setMineCounts()
	{
		for (u32 p=0; p<Width*Height; ++p)
			Tiles[p]->setCount(getMineCount(p));
	}
	s32 getMineCount(s32 p)
	{
		s32 ret = 0;

		s32 x = p % Width;
		s32 z = p / Width;

		if (x != 0)
		{
			if (Tiles[p-1]->HasMine) ret++;
			if (z != 0 && Tiles[p-Width-1]->HasMine) ret++;
			if (z != Height-1 && Tiles[p+Width-1]->HasMine) ret++;
		}
		if (x != Width-1)
		{
			if (Tiles[p+1]->HasMine) ret++;
			if (z != 0 && Tiles[p-Width+1]->HasMine) ret++;
			if (z != Height-1 && Tiles[p+Width+1]->HasMine) ret++;
		}

		if (z != 0 && Tiles[p-Width]->HasMine) ret++;
		if (z != Height-1 && Tiles[p+Width]->HasMine) ret++;

		return ret;
	}

	// creates a new game
	void newGame(s32 w, s32 h, s32 mc)
	{
		Width = w; Height = h; MineCount = mc;
		SquaresLeft = w*h;
		StartTime = device->getTimer()->getTime();

		// clear everything
		for (u32 i=0; i<Tiles.size(); ++i)
		{
			Tiles[i]->remove();
		}
		Tiles.clear();

		// create all tiles
		for (s32 z=0; z < Height; ++z)
		{
			for (s32 x=0; x<Width; ++x)
			{
				MineSweeperTileNode* t = new MineSweeperTileNode(smgr, x, z, Floor, Mine, Flag);

				Tiles.push_back(t);
				t->drop();
			}
		}

		// add mines
		for (u32 i=0; i<MineCount; ++i)
		{
			u32 p;
			do
			{
				p = rand() % (Width*Height);
			} while (Tiles[p]->HasMine);

			Tiles[p]->HasMine=true;
		}
		setMineCounts();

		Camera->setTarget(Tiles[(Width/2) + (Height/2*Width)]->getPosition());
		Camera->setPosition(Tiles[(Width/2) + ((Height-1)*Width)]->getPosition() + core::vector3df(0,Height+5,10) );

		GameOver = false;
	}

	// draws the game
	void draw()
	{
		driver->beginScene(true, true, SColor(0,0,0,0));
		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();
	}

	s32 Width, Height, MineCount, SquaresLeft;
	array<MineSweeperTileNode*> Tiles;

	IMesh* Floor;
	IMesh* Mine;
	IMesh* Flag;

	IrrlichtDevice *device;
	IVideoDriver* driver;
	ISceneManager* smgr;
	IGUIEnvironment* guienv;
	ICameraSceneNode* Camera;
	bool GameOver, QuitGame, Playing;
	IGUIImage *GameOverIcon, *WinnerIcon;
	s32 HighlightX, HighlightZ;
	u32 StartTime;
};
