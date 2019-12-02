// zur Verhinderung doppelter Definition
#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <irrlicht.h>
#include "GUI.h"
#include "Optionen.h"
#include "Szene.h"

// Namespace DunGen : DungeonGenerator
namespace DunGen
{
	// EventReceiver-Klasse, verarbeitet Events, erbt von Irrlicht EventReceiver-Interface
	class CEventReceiver : public irr::IEventReceiver
	{
	public:
		CEventReceiver();
		void Initialisiere(irr::gui::IGUIEnvironment* _GuiEnvironment, CGUI* _GUI, COptionen* _Optionen, CSzene* _Szene);

		virtual bool OnEvent(const irr::SEvent& event);

	private:
		// Zeiger auf andere Objekte
		irr::gui::IGUIEnvironment* GuiEnvironment;
		CGUI* GUI;
		COptionen* Optionen;
		CSzene* Szene;		

		bool Initialisiert;
	};
	
} // END NAMESPACE DunGen

#endif