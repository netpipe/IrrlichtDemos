// ================================
// "Dungeongenerator"
// written by Maximilian Honig
// ================================

#include <iostream>

#include <irrlicht.h>
#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

#include "Zeitmesser.h"
#include "ZufallsGenerator.h"

#include "FraktalGenerator.h"
#include "VoxelRaum.h"
#include "DreiecksMesh.h"
#include "Architekt.h"

#include "GUI.h"
#include "EventReceiver.h"
#include "Optionen.h"
#include "Szene.h"

//====================
// Statische Objekte
//====================

// Hilfsobjekte
DunGen::CZeitmesser Zeitmesser;
DunGen::CZufallsGenerator ZufallsGeneratorAllgemein;
DunGen::CZufallsGenerator ZufallsGeneratorVerwacklung;

// Dungeongenerator-Objekte: statisch erzeugt, da teilweise zu gross fuer den Stack
DunGen::CFraktalGenerator FraktalGenerator(&ZufallsGeneratorAllgemein, &Zeitmesser);
DunGen::CVoxelRaum VoxelRaum(&ZufallsGeneratorAllgemein, &Zeitmesser);
DunGen::CDreiecksMesh DreiecksMesh(&VoxelRaum, &ZufallsGeneratorVerwacklung, &Zeitmesser);
DunGen::CArchitekt Architekt(&VoxelRaum, &DreiecksMesh, &ZufallsGeneratorAllgemein, &Zeitmesser);

// Verwaltung der Einstellungen, Ansteuerung der Generatoren
DunGen::COptionen Optionen;

//====================
// Hauptroutine
//====================
int main(int argc, char* argv[])
{
	// Aufloesung bestimmen
	irr::core::dimension2du aufloesung = irr::core::dimension2du(1200, 900);
	if (argc>=3)
	{
		aufloesung.Width = atoi(argv[1]);
		aufloesung.Height = atoi(argv[2]);
	}
	// Antialiasfaktor bestimmen (0 = aus)
	int antiAlias = 0;
	if (argc>=4)
		antiAlias = atoi(argv[3]);

	std::cout << "[DunGen]: Dungeongenerator wird gestartet." << std::endl;
	std::cout << "[DunGen]: Aufloesung = " << aufloesung.Width << " * " << aufloesung.Height << " , AntiAlias = " <<  antiAlias << std::endl;

	// Eventreceiver erstellen
	DunGen::CEventReceiver eventReceiver;

	// Parameteruebergabe fuer Irrlicht-Device vorbereiten
	irr::SIrrlichtCreationParameters irrlichtParameter;
	irrlichtParameter.DriverType = irr::video::E_DRIVER_TYPE::EDT_OPENGL;
	irrlichtParameter.WindowSize = aufloesung;
	irrlichtParameter.Bits = 32;
	irrlichtParameter.Fullscreen = false;
	irrlichtParameter.Stencilbuffer = false;
	irrlichtParameter.Vsync = false;
	irrlichtParameter.AntiAlias = antiAlias;
	irrlichtParameter.EventReceiver = &eventReceiver;

	// Irrlicht-Device erstellen: nutze OpenGL-Treiber
	//irr::IrrlichtDevice*  irrDevice = irr::createDevice(irr::video::E_DRIVER_TYPE::EDT_OPENGL,aufloesung, 32, false, false, false,&eventReceiver);
	irr::IrrlichtDevice*  irrDevice = irr::createDeviceEx(irrlichtParameter);
	if(irrDevice == 0)
	{
		std::cout << "Programm wird abgebrochen, da IrrlichtDevice nicht erstellt werden konnte." << std::endl;
		return 1;
	}
	irrDevice->setWindowCaption(L"Dungeongenerator");

	// Sub-Devices merken
	irr::video::IVideoDriver* videoDriver = irrDevice->getVideoDriver();
	irr::scene::ISceneManager* sceneManager = irrDevice->getSceneManager();
	irr::gui::IGUIEnvironment* guiEnvironment = irrDevice->getGUIEnvironment();

	// Weitere Objekte
	DunGen::CGUI gui(guiEnvironment);
	DunGen::CSzene szene(irrDevice, &VoxelRaum, &DreiecksMesh, &Architekt);

	// Weitere Initialisierungen
	Architekt.Initialisiere(irrDevice);
	eventReceiver.Initialisiere(guiEnvironment, &gui, &Optionen, &szene);
	Optionen.Initialisiere(irrDevice, &gui, &szene, &FraktalGenerator, &VoxelRaum, &DreiecksMesh, &Architekt, &ZufallsGeneratorAllgemein, &ZufallsGeneratorVerwacklung, &Zeitmesser);

	// Zufallsseed randomisieren
	ZufallsGeneratorAllgemein.SetzeZufallsSeed(time(NULL));

	// Hauptschleife
	irr::s32 letzteFPS = 0;
	irr::s32 aktuelleFPS = 0;
	while(irrDevice->run() && videoDriver)
    {
		if (irrDevice->isWindowActive())
        {
			// Darstellung beginnen
            videoDriver->beginScene(true, true, Optionen.LeseHintergrundfarbe());

			// Szene und Gui darstellen
			szene.SzeneZeichnen();

			// Darstellung abschliessen
            videoDriver->endScene();

			// FPS aktualisieren, wenn geaendert (werden von Irrlicht ca. alle 1,5 Sek. aktualisiert)
			aktuelleFPS = videoDriver->getFPS();
			if (aktuelleFPS != letzteFPS)
			{
				letzteFPS = aktuelleFPS;
				irr::core::stringw str = L"Dungeongenerator - FPS: ";
				str += aktuelleFPS;
				irrDevice->setWindowCaption(str.c_str());
			}
		}
        else
			// pausieren, wenn Fenster nicht aktiv
			irrDevice->yield();
	}

	// Irr-Device und alles was daran haengt wieder loeschen
	irrDevice->drop();
    return 0;
}

// ===============
// ENDE: main.cpp
// ===============
