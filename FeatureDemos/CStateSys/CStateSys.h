/**
	@file CStateSys.h
	@author Jookia
	@date 6-24-09
**/

#ifndef __C_STATESYS_H_
#define __C_STATESYS_H_

#include "irrArray.h"
#include "IEventReceiver.h"
#include "IGUIElement.h"
#include "IrrlichtDevice.h"
#include "SColor.h"

class CStateManager;

/**
	CState is a base class for states.
**/

class CState
{
	public:
		//! Constructs the state.
		/**
			When the state gets created it assigns man
			to be it's manager and if a par is assigned,
			it'll set par as it's parent too and use
			the parent's manager instead of the assigned one.
			@param man The manager.
			@param par The possible parent.
		**/
		CState(CStateManager* man, CState* par = NULL)
		{
			manager = man;
			
			if(par != NULL)
			{
				parent = par;
				parent->addChild(this);
				
				if(parent->getStateManager() != NULL)
					manager = parent->getStateManager();
			}
			else
				parent = NULL;
		}
		
		//! Deconstructs the state.
		/**
			When the state gets destroyed it'll remove and kill
			all it's children and then destroy all it's elements
			in the element array.
		**/
		~CState(void)
		{
			if(parent != NULL)
				parent->removeChild(this);
			
			int childSize = children.size() - 1;
			
			for(int i = childSize; i >= 0; i--)
				if(children[i]->getParent() == this)
					children[i]->kill(false);
			
			children.clear();
			
			int elementSize = elements.size() - 1;
			
			for(int j = elementSize; j >= 0; j--)
				elements[j]->remove();
			
			elements.clear();
		}
		
		//! Runs the state.
		/**
			When the state runs, it'll run all of its children too.
		**/
		virtual void run(void)
		{
			int size = children.size() - 1;
			
			for(int i = size; i >= 0; i--)
				children[i]->run();
		}
		
		//! Handles events.
		/**
			When an event is called, it gets passed from the
			manager to the state. Then it passes it to all its children.
			@param event The event.
			@return Whether the event was processed or not.
		**/
		virtual bool onEvent(const SEvent& event)
		{
			int size = children.size() - 1;
			
			for(int i = size; i >= 0; i--)
				if(children[i]->onEvent(event) == true)
					return true;
			
			return false;
		}
		
		//! Adds a child to the state.
		/**
			@param child The state we want to add.
			@return If it had an error.
		**/
		virtual bool addChild(CState* child)
		{
			bool errored = false;
			
			if(errored == false)
			{
				int size = children.size() - 1;
				
				for(int i = size; i >= 0; i--)
					if(children[i] == child)
						errored = true;
				
				if(errored == false)
					children.push_back(child);
			}
			
			return errored;
		}
		
		//! Removes a child from the state.
		/**
			Removes a child from the state. It also kills the child.
			@param child The state we want to remove and kill.
			@return If it had an error.
		**/
		virtual bool removeChild(CState* child)
		{
			int size = children.size() - 1;
			
			for(int i = size; i >= 0; i--)
				if(children[i] == child)
				{
					children.erase(i);
					children[i]->kill(false);
					return false;
				}
			
			return true;
		}
		
		//! Kills the state.
		/**
			Deletes the state and possibly removes the state rom the parent's children.
			@param removeParent Whether we should remove the state from the parent's children or not. Default value is true.
		**/
		void kill(bool removeParent = true)
		{
			if(removeParent == true)
				if(parent->removeChild(this) == false)
					return;
			
			parent = NULL;
			
			delete this;
		}
		
		//! Returns the parent assigned to the state.
		/**
			@return The parent.
		**/
		virtual CState* getParent(void)
		{
			return parent;
		}
		
		//! Returns the manager assigned to the state.
		/**
			@return The manager.
		**/
		virtual CStateManager* getStateManager(void)
		{
			return manager;
		}
	
	protected:
		CStateManager* manager; ///< manager is a pointer to a CStateManager object.
		CState* parent;         ///< parent is a pointer the CState object that is the state's parent.
		
		irr::core::array<CState*> children;                ///< children is an array of CStates that have this state as its parent.
		irr::core::array<irr::gui::IGUIElement*> elements; ///< elements is an array of IGUIElements that will be deleted once this state is deleted.
};

/**
	The CStateManager handles states.
	Here's an example of using it with Irrlicht.
	
	@code
		#include <irrlicht.h>
			using namespace irr;
		
		#include "CStateSys.h"
		
		int main(void)
		{
			// Create the device.
			IrrlichtDevice *device = createDevice(video::EDT_OPENGL,
				core::dimension2d<s32>(640, 480));
			
			if(device == NULL)
				return EXIT_FAILURE; // Could not create selected driver.
			
			device->setWindowCaption(L"Hello World");
			video::IVideoDriver* driver = device->getVideoDriver();
			
			CStateManager stateSys(device);
			CState* testState = new CState(&stateSys);
			stateSys.setState(menuState);
			
			while(device->run() == true && driver != NULL)
			{
				stateSys.run();
				
				driver->beginScene(true, true, stateSys.getColor());
				
				if(stateSys.getRenderScene() == true)
					scene->drawAll();
				
				if(stateSys.getRenderGUI() == true)
					env->drawAll();
				
				driver->endScene();
			}
			
			device->drop();
			
			return EXIT_SUCCESS;
		}
	@endcode
**/

class CStateManager : public IEventReceiver
{
	public:
		//! Constructs the manager.
		/**
			When the manager is constructed it will use itself
			as am event reciever and set its inital color to black.
			@param dev The device that the manager will use.
		**/
		CStateManager(irr::IrrlichtDevice* dev)
		{
			device = dev;
			currentState = NULL;
			
			device->setEventReceiver(this);
			
			color = irr::video::SColor(255, 0, 0, 0);
			
			renderGUI = true;
			renderScene = true;
		}
		
		//! Deconstructs the manager.
		/**
			When the manager is destroyed, it
			deletes the current state and the
			current state will delete all of
			its children.
		**/
		~CStateManager(void)
		{
			delete currentState;
		}
		
		//! Runs the manager.
		/**
			Run runs the current state's run function
			which will run all its children.
			@return If it had an error.
		**/
		bool run(void)
		{
			bool errored = false;
			
			if(device == NULL || currentState == NULL)
				errored = true;
			
			if(errored == false)
				currentState->run();
			
			return errored;
		}
		
		//! Handles events.
		/**
			When an event is called, it gets passed to the
			current state's onEvent function. Then it gets
			passed on to all its children.
			@param event The event.
			@return Whether the event was processed or not.
		**/
		bool OnEvent(const SEvent& event)
		{
			if(currentState != NULL)
				return currentState->onEvent(event);
			else
				return false;
		}
		
		//! Sets the state.
		/**
			Changes the manager's state.
			The current state gets deleted.
			@param state The state you want to set it to.
			@return If it had an error.
		**/
		bool setState(CState* state)
		{
			bool errored = false;
			
			if(state == NULL || state == currentState)
				errored = true;
			
			if(errored == false)
			{
				delete currentState;
				currentState = state;
			}
			
			return errored;
		}
		
		//! Returns the state.
		/**
			Returns the current state assigned to the manager.
			@return The device.
		**/
		CState* getState(void)
		{
			return currentState;
		}
		
		//! Returns the device.
		/**
			Returns the device assigned to the manager.
			@return The device.
		**/
		irr::IrrlichtDevice* getDevice(void)
		{
			return device;
		}
		
		//! Returns the color.
		/**
			Returns the current color.
			@return Current color.
		**/
		irr::video::SColor getColor(void)
		{
			return color;
		}
		
		//! Returns whether we should render the GUI.
		/**
			@return Current state of renderGUI.
		**/
		bool getRenderGUI(void)
		{
			return renderGUI;
		}
		
		//! Returns whether we should render the scene.
		/**
			@return Current state of renderScene.
		**/
		bool getRenderScene(void)
		{
			return renderScene;
		}
		
		//! Sets whether we should render the GUI.
		/**
			@param setting Sets renderGUI to this.
		**/
		void setRenderGUI(bool setting)
		{
			renderGUI = setting;
		}
		
		//! Sets whether we should render the scene.
		/**
			@param setting Sets renderScene to this.
		**/
		void setRenderScene(bool setting)
		{
			renderScene = setting;
		}
		
		//! Sets the color.
		/**
			Takes a SColor and sets color to it.
			@param col The color to set the current color to.
		**/
		void setColor(irr::video::SColor col)
		{
			color = col;
		}
	
	protected:
		CState* currentState;        ///< currentState is a pointer to the current state that's being run.
		irr::IrrlichtDevice* device; ///< device is a pointer to the IrrlichtDevice that manager uses.
		irr::video::SColor color;    ///< color is a SColor defined by the current state. Usually used for drawing the scene.
		
		bool renderGUI;   ///< renderGUI is a bool defined by states.
		bool renderScene; ///< renderScene is a bool defined by states.
};

#endif
