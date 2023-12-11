/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "peakgraphics/Graphics.hpp"
#include "peakgraphics/scene/RootSceneNode.hpp"
#include "peakgraphics/InputReceiver.hpp"
#include "peakgraphics/menu/Menu.hpp"
#include "peakgraphics/menu/Sprite.hpp"
#include <peakengine/support/Thread.hpp>
#include <peakengine/support/OS.hpp>

#include <lf/Lightfeather.h>
using namespace lf;

namespace peak
{
	class EventReceiver : public input::IMouseListener,
		public input::IMouseMovementListener,
		public input::IMouseWheelListener,
		public input::IKeyListener
	{
		public:
			EventReceiver(Graphics *graphics) : graphics(graphics)
			{
			}
			virtual ~EventReceiver()
			{
			}
			virtual void mousePressed(lf::input::CMouseEvent &event)
			{
				if (event.getButton() == 1)
					graphics->onKeyDown(KEY_LBUTTON);
				if (event.getButton() == 2)
					graphics->onKeyDown(KEY_RBUTTON);
				if (event.getButton() == 3)
					graphics->onKeyDown(KEY_MBUTTON);
			}
			virtual void mouseReleased(lf::input::CMouseEvent &event)
			{
				if (event.getButton() == 1)
					graphics->onKeyUp(KEY_LBUTTON);
				if (event.getButton() == 2)
					graphics->onKeyUp(KEY_RBUTTON);
				if (event.getButton() == 3)
					graphics->onKeyUp(KEY_MBUTTON);
			}
			virtual void mouseMoved(lf::input::CMouseEvent &event)
			{
				if (event.getOldX() != 32000)
					graphics->onMouseMoved(event.getX(), event.getY(),
						event.getXDelta(), event.getYDelta());
				else
					graphics->onMouseMoved(event.getX(), event.getY(), 0, 0);
			}
			virtual void keyPressed(lf::input::CKeyEvent &event)
			{
				graphics->onKeyDown((KeyCode)event.getKey());
			}
			virtual void keyReleased(lf::input::CKeyEvent &event)
			{
				graphics->onKeyUp((KeyCode)event.getKey());
			}

			Graphics *graphics;
	};

	Graphics::Graphics()
	{
		eventreceiver = new EventReceiver(this);
	}
	Graphics::~Graphics()
	{
		delete eventreceiver;
	}

	bool Graphics::init(int width, int height, bool fullscreen)
	{
		this->fullscreen = fullscreen;
		this->width = width;
		this->height = height;
		mouselocked = false;
		// Create server thread
		cond.lock();
		stopping = false;
		thread = new Thread();
		thread->create(new ClassFunctor<Graphics>(this, &Graphics::runThread));
		// Wait for graphics to initialize
		cond.wait();
		cond.unlock();
		// TODO: Check success
		return true;
	}
	bool Graphics::shutdown()
	{
		if (stopped)
			return true;
		cond.lock();
		stopping = true;
		cond.wait();
		cond.unlock();
		return true;
	}

	SceneNode *Graphics::getRootSceneNode()
	{
		return rootscenenode.get();
	}

	void Graphics::loadFile(std::string path)
	{
		loadingmutex.lock();
		loadingfiles.push(path);
		loadingmutex.unlock();
	}

	void Graphics::addInputReceiver(InputReceiver *receiver)
	{
		inputmutex.lock();
		inputreceiver.push_back(receiver);
		inputmutex.unlock();
	}
	void Graphics::removeInputReceiver(InputReceiver *receiver)
	{
		inputmutex.lock();
		for (unsigned int i = 0; i < inputreceiver.size(); i++)
		{
			if (inputreceiver[i] == receiver)
			{
				inputreceiver.erase(inputreceiver.begin() + i);
				break;
			}
		}
		inputmutex.unlock();
	}

	void Graphics::registerLoading(Loadable *loadable)
	{
		loadingmutex.lock();
		loading.push(loadable);
		loadingmutex.unlock();
	}
	void Graphics::registerParentChange(SceneNode *node)
	{
		parentmutex.lock();
		parentchange.push(node);
		parentmutex.unlock();
	}

	lf::render::IRenderWindow *Graphics::getWindow()
	{
		return window;
	}
	Vector2I Graphics::getWindowSize()
	{
		return Vector2I(width, height);
	}

	void Graphics::onKeyDown(peak::KeyCode key)
	{
		// Get input receivers
		inputmutex.lock();
		std::vector<InputReceiver*> receivers = inputreceiver;
		inputmutex.unlock();
		// Callback
		for (unsigned int i = 0; i < receivers.size(); i++)
			receivers[i]->onKeyDown(key);
	}
	void Graphics::onKeyUp(peak::KeyCode key)
	{
		// Get input receivers
		inputmutex.lock();
		std::vector<InputReceiver*> receivers = inputreceiver;
		inputmutex.unlock();
		// Callback
		for (unsigned int i = 0; i < receivers.size(); i++)
			receivers[i]->onKeyUp(key);
	}
	void Graphics::onMouseMoved(int x, int y, int dx, int dy)
	{
		if (mouselocked && x == 300 && y == 300)
			return;
		inputmutex.lock();
		// Reset mouse
		if (mouselocked)
		{
			input::ICursorControl *cursorcontrol = window->getCursorControl();
			cursorcontrol->setPosition(300, 300);
		}
		// Restrict mouse position to the window
		mousepos += Vector2I(dx, dy);
		if (mousepos.x < 0)
			mousepos.x = 0;
		if (mousepos.y < 0)
			mousepos.y = 0;
		if (mousepos.x >= width)
			mousepos.x = width - 1;
		if (mousepos.y >= height)
			mousepos.y = height - 1;
		// Get input receivers
		std::vector<InputReceiver*> receivers = inputreceiver;
		inputmutex.unlock();
		// Callback
		for (unsigned int i = 0; i < receivers.size(); i++)
			receivers[i]->onMouseMoved(mousepos.x, mousepos.y, dx, dy);
	}

	void Graphics::setMouseLocked(bool mouselocked)
	{
		this->mouselocked = mouselocked;
	}
	bool Graphics::isMouseLocked()
	{
		return mouselocked;
	}

	void Graphics::runThread()
	{
		// Initialize graphics
		lf::initLF();
		window = CLFRender::getInstance().createRenderWindow(core::vector2di(0,0),
			core::vector2d<s32>(width,height), 32, 32,
			render::EWCF_AUTOCLOSE | (fullscreen ? render::EWCF_FULLSCREEN : 0));
		window->setWindowCaption(L"PeakEngine");
		window->setVisible(true);
		window->addMouseListener(eventreceiver);
		window->addMouseMovementListener(eventreceiver);
		window->addKeyListener(eventreceiver);
		CLFRender::getInstance().setAutoSleep(1);
		resmgr = CResourceManager::getInstancePtr();
		scene = window->getRenderLayer3D()->getScene();
		fps = 0;
		input::ICursorControl *cursorcontrol = window->getCursorControl();
		cursorcontrol->setVisible(false);
		cursorcontrol->setPosition(300, 300);
		mousepos = Vector2I(width / 2, height / 2);
		// Create root scene node
		rootscenenode = new RootSceneNode(this, scene->getRootSceneNode());
		// We have finished initializaition
		cond.lock();
		cond.signal();
		cond.unlock();
		// Render loop
		while (!stopping)
		{
			loadingmutex.lock();
			// Load resource files
			while (loadingfiles.size() > 0)
			{
				std::string file = loadingfiles.front();
				loadingfiles.pop();
				loadingmutex.unlock();
				resmgr->loadResources(file.c_str());
				loadingmutex.lock();
			}
			// Load loadable objects
			while (loading.size() > 0)
			{
				LoadablePointer loadable = loading.front();
				loading.pop();
				loadingmutex.unlock();
				loadable->tryLoading();
				loadingmutex.lock();
			}
			loadingmutex.unlock();
			// Update scene node tree
			parentmutex.lock();
			while (parentchange.size() > 0)
			{
				SceneNodePointer node = parentchange.front();
				parentchange.pop();
				parentmutex.unlock();
				node->updateParent();
				parentmutex.lock();
			}
			parentmutex.unlock();
			// Update scene node positions
			rootscenenode->update(OS::get().getTime());
			// Update active menu
			Menu::updateAll();
			// Update sprites
			Sprite::updateAll();
			// Render
			if (!CLFRender::getInstance().update())
			{
				stopped = true;
				break;
			}
			// Update window caption
			unsigned int currentfps = window->getOneSecondFPS();
			if (currentfps != fps)
			{
				fps = currentfps;
				window->setWindowCaption((core::stringw(L"PeakEngine  FPS: ")
					+ core::stringw(fps)).c_str());
			}
		}
		// Shutdown graphics
		lf::deinitLF();
		// TODO
		cond.lock();
		cond.signal();
		cond.unlock();
	}
}
