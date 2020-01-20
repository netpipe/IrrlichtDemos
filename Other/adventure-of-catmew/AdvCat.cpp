#include "AdvCat.h"


AdvCat::AdvCat()
{
		core = new Game("settings.cfg");

		core->lock();
		core->getRoot()->lock();
	
		core->getRoot()->setWindowTitle("The Adventures of CatMewMew");	

		gfx::ITexture* skyTexture = core->createSkyGradientTexture(Color(0.4f,0.4f,0.9f),Color(0.2f,0.2f,0.6f));
		sky = core->getRoot()->getSceneManager()->createSprite(skyTexture);
		sky->setPosition(Vector(512,88));

		sky->setLayer(100);

		conf::Configuration levelConf("media/levels/levels.cfg");

		std::vector<std::string> levelStrArray = conf::toArray(levelConf.getSection("levels")->getKeyData("maps"));

		IFont* font = core->getRoot()->getVideoDriver()->createFont("media/fonts/BlueStone.ttf", 18);

		std::vector<IText*> levelTxtArray;

		for(int i = 0;i < levelStrArray.size();++i)
		{
			IText* lvlTxt = core->getRoot()->getSceneManager()->createText(levelStrArray[i]);
			lvlTxt->setFont(font);
			lvlTxt->setPosition(Vector(400,core->getRoot()->getDimension().y - 100 - i * 60));
			lvlTxt->getMaterial()->setColor(Color(1, 1, 0, 1));
			levelTxtArray.push_back(lvlTxt);
		}
		
		signed int currSelection = levelTxtArray.size() - 1;

		bool switch1 = false;
		bool switch2 = false;

		while(true)
		{
			// Drawing time.		
			core->getRoot()->getVideoDriver()->beginFrame();
			core->getRoot()->getSceneManager()->update();		
			core->getRoot()->getVideoDriver()->endFrame();

			if(core->getRoot()->getKeyReleased(gfx::EKC_UP))
			{
				if(!switch1)
				currSelection--;
				switch1 = true;
			}
			else
			{
				switch1 = false;
			}
			
			if(core->getRoot()->getKeyReleased(gfx::EKC_DOWN))
			{
				if(!switch2)
				currSelection++;
				switch2 = true;
			}
			else
			{
				switch2 = false;
			}

			if(currSelection >= levelTxtArray.size())
			{
				currSelection = levelTxtArray.size() - 1;
			}
			if(currSelection < 0)
			{
				currSelection = 0;
			}

			for(int i = 0;i < levelTxtArray.size();++i)
				levelTxtArray[i]->getMaterial()->setColor(Color(1, 1, 0, 1));

			levelTxtArray[currSelection]->getMaterial()->setColor(Color(1, 0, 0, 1));

			core->getRoot()->sleep(5);

			if(core->getRoot()->getKeyPressed(gfx::EKC_ESCAPE) || !core->update())
				exit(0);

			if(core->getRoot()->getKeyPressed(gfx::EKC_RETURN))
				break;
		}

		for(int i = 0;i < levelTxtArray.size();++i)
			levelTxtArray[i]->remove();

		catMewMew = new Player(core);
		
		cloudManage = new CloudManager(core);
		enemies = new Enemies(core);
		levelManage = new LevelManager(core, enemies);
		mewShooter = new MewMewShooter(core,catMewMew->getCatDude(),levelManage);
		collisionManage = new CollisionManager(core,levelManage,catMewMew);
		
		levelManage->loadLevel(levelStrArray[currSelection]);
		
		skyTexture = core->createSkyGradientTexture(levelManage->SkyCol1,levelManage->SkyCol2);
		sky->getMaterial()->setTexture(0,skyTexture);
		
		

		
		tFPS = core->getRoot()->getSceneManager()->createText("FPS: 0");
		tFPS->setFont(font);
		tFPS->setCentered(false);
		tFPS->setPosition(Vector(10, core->getRoot()->getDimension().y - (font->getSize()*2 + 20)));	
		tFPS->getMaterial()->setColor(Color(1, 1, 0, 1));

}

void AdvCat::update()
{

		std::stringstream stream;
		stream << "FPS: ";
		stream << core->getRoot()->getVideoDriver()->getFrameCount();
		tFPS->setText(stream.str());	
		
		// Update sky position.
		sky->setPosition(core->wPos + Vector(512,88));
		
		// Things behave weirdly under very low fps, so don't update then.
		if(core->getRoot()->getVideoDriver()->getFrameCount() > 10.0f)
		{
			// Update logic-specific stuff.
			mewShooter->update();
			cloudManage->update();
			enemies->update();
		}
		
		// Drawing time.		
		core->getRoot()->getVideoDriver()->beginFrame();
		core->getRoot()->getSceneManager()->update();		
		core->getRoot()->getVideoDriver()->endFrame();
		
		// Ditto here.
		if(core->getRoot()->getVideoDriver()->getFrameCount() > 10.0f)
		{
			// Update catMewMew AFTER drawing.
			catMewMew->update();
			collisionManage->update();
		}
				
		// Sleep a bit as to not be greedy.
		core->getRoot()->sleep(1);
		
		if(core->getRoot()->getKeyPressed(gfx::EKC_ESCAPE))
			core->close();
}

AdvCat::~AdvCat()
{
		core->getRoot()->unlock();
		core->getRoot()->remove();	

		core->unlock();
		core->remove();	
}
