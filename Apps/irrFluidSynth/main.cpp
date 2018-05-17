/** Example 005 User Interface

This tutorial shows how to use the built in User Interface of
the Irrlicht Engine. It will give a brief overview and show
how to create and use windows, buttons, scroll bars, static
texts, and list boxes.

As always, we include the header files, and use the irrlicht
namespaces. We also store a pointer to the Irrlicht device,
a counter variable for changing the creation position of a window,
and a pointer to a listbox.
*/
#include <irrlicht.h>
//#include "driverChoice.h"
#include "exampleHelper.h"

#include <stdio.h>
#include <stdlib.h>



using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#include "utils/fluidsynth_priv.h"
#if defined(HAVE_GETOPT_H)
#include <getopt.h>
#define GETOPT_SUPPORT 1
#endif

#include "fluidsynth.h"

#include "bindings/fluid_lash.h"

#ifndef WITH_MIDI
#define WITH_MIDI 1
#endif
fluid_cmd_handler_t* cmd_handler = NULL;



	char *psoundfont ="soundfonts/VintageDreamsWaves-v2.sf2";
	char *psong = "BLUES.MID";


// Declare a structure to hold some context for the event receiver so that it
// has it available inside its OnEvent() method.
struct SAppContext
{
	IrrlichtDevice *device;
	s32				counter;
	IGUIListBox*	listbox;
};

// Define some values that we'll use to identify individual GUI controls.
enum
{
	GUI_ID_QUIT_BUTTON = 101,
	GUI_ID_NEW_WINDOW_BUTTON,
	GUI_ID_FILE_OPEN_BUTTON,
	GUI_ID_TRANSPARENCY_SCROLL_BAR
};


/* The structure with the effects data. This example simply applies a
 * linear gain the to synthesizer output. */
struct fx_data_t {
        fluid_synth_t* synth;
        float gain;
} fx_data_t;


int fx_function(void* data, int len,
                int nin, float** in,
                int nout, float** out)
{
        struct fx_data_t* fx_data = (struct fx_data_t*) data;
        int i, k;
        float* out_i;

        /* Call the synthesizer to fill the output buffers with its
         * audio output. */
        if (fluid_synth_process(fx_data->synth, len, nin, in, nout, out) != 0) {
                /* Some error occured. Very unlikely to happen, though. */
                return -1;
        }

        /* Apply your effects here. In this example, the gain is
         * applied to all the output buffers. */
        for (i = 0; i < nout; i++) {
                out_i = out[i];
                for (k = 0; k < len; k++) {
                        out_i[k] *= fx_data->gain;
                }
        }

        return 0;
}


/*
	Set the skin transparency by changing the alpha values of all skin-colors
*/
void setSkinTransparency(s32 alpha, irr::gui::IGUISkin * skin)
{
	for (s32 i=0; i<irr::gui::EGDC_COUNT ; ++i)
	{
		video::SColor col = skin->getColor((EGUI_DEFAULT_COLOR)i);
		col.setAlpha(alpha);
		skin->setColor((EGUI_DEFAULT_COLOR)i, col);
	}
}

/*
The Event Receiver is not only capable of getting keyboard and
mouse input events, but also events of the graphical user interface
(gui). There are events for almost everything: button click,
listbox selection change, events that say that a element was hovered
and so on. To be able to react to some of these events, we create
an event receiver.
We only react to gui events, and if it's such an event, we get the
id of the caller (the gui element which caused the event) and get
the pointer to the gui environment.
*/
class MyEventReceiver : public IEventReceiver
{
public:
	MyEventReceiver(SAppContext & context) : Context(context) { }

	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			IGUIEnvironment* env = Context.device->getGUIEnvironment();

			switch(event.GUIEvent.EventType)
			{

			/*
			If a scrollbar changed its scroll position, and it is
			'our' scrollbar (the one with id GUI_ID_TRANSPARENCY_SCROLL_BAR),
			then we change the transparency of all gui elements. This is an
			easy task: There is a skin object, in which all color
			settings are stored. We simply go through all colors
			stored in the skin and change their alpha value.
			*/
			case EGET_SCROLL_BAR_CHANGED:
				if (id == GUI_ID_TRANSPARENCY_SCROLL_BAR)
				{
					s32 pos = ((IGUIScrollBar*)event.GUIEvent.Caller)->getPos();
					setSkinTransparency(pos, env->getSkin());
				}
				break;

			/*
			If a button was clicked, it could be one of 'our'
			three buttons. If it is the first, we shut down the engine.
			If it is the second, we create a little window with some
			text on it. We also add a string to the list box to log
			what happened. And if it is the third button, we create
			a file open dialog, and add also this as string to the list box.
			That's all for the event receiver.
			*/
			case EGET_BUTTON_CLICKED:
				switch(id)
				{
				case GUI_ID_QUIT_BUTTON:
					Context.device->closeDevice();
					return true;

				case GUI_ID_NEW_WINDOW_BUTTON:
					{
					Context.listbox->addItem(L"Window created");
					Context.counter += 30;
					if (Context.counter > 200)
						Context.counter = 0;

					IGUIWindow* window = env->addWindow(
						rect<s32>(100 + Context.counter, 100 + Context.counter, 300 + Context.counter, 200 + Context.counter),
						false, // modal?
						L"Test window");

					env->addStaticText(L"Please close me",
						rect<s32>(35,35,140,50),
						true, // border?
						false, // wordwrap?
						window);
					}
					return true;

				case GUI_ID_FILE_OPEN_BUTTON:
					Context.listbox->addItem(L"File open");
					// There are some options for the file open dialog
					// We set the title, make it a modal window, and make sure
					// that the working directory is restored after the dialog
					// is finished.
					env->addFileOpenDialog(L"Please choose a file.", true, 0, -1, true);
					return true;

				default:
					return false;
				}
				break;

			case EGET_FILE_SELECTED:
				{
					// show the event and the selected model filename from the file dialog
					IGUIFileOpenDialog* dialog =
						(IGUIFileOpenDialog*)event.GUIEvent.Caller;
					Context.listbox->addItem(L"EGET_FILE_SELECTED");
					Context.listbox->addItem(dialog->getFileName());
				}
				break;

			case EGET_DIRECTORY_SELECTED:
				{
					// show the event and the selected directory name from the file dialog
					IGUIFileOpenDialog* dialog =
						(IGUIFileOpenDialog*)event.GUIEvent.Caller;
					Context.listbox->addItem(L"EGET_DIRECTORY_SELECTED");
					Context.listbox->addItem(dialog->getDirectoryNameW());
				}
				break;

			default:
				break;
			}
		}

		return false;
	}

private:
	SAppContext & Context;
};

static void
fast_render_loop(fluid_settings_t* settings, fluid_synth_t* synth, fluid_player_t* player)
{
  fluid_file_renderer_t* renderer;

  renderer = new_fluid_file_renderer (synth);
  if (!renderer) return;

  while (fluid_player_get_status(player) == FLUID_PLAYER_PLAYING) {
    if (fluid_file_renderer_process_block(renderer) != FLUID_OK) {
      break;
    }
  }
  delete_fluid_file_renderer(renderer);
}
/*
OK, now for the more interesting part. First, create the Irrlicht device. As in
some examples before, we ask the user which driver he wants to use for this
example.
*/
int main()
{


	// create device and exit if creation failed
	IrrlichtDevice * device = createDevice(EDT_OPENGL,core::dimension2d<u32>(640, 480));

	if (device == 0)
		return 1; // could not create selected driver.

	/* The creation was successful, now we set the event receiver and
		store pointers to the driver and to the gui environment. */

	device->setWindowCaption(L"Irrlicht Engine - User Interface Demo");
	device->setResizable(true);

	video::IVideoDriver* driver = device->getVideoDriver();
	IGUIEnvironment* env = device->getGUIEnvironment();

	const io::path mediaPath = getExampleMediaPath();

	/*
	To make the font a little bit nicer, we load an external font
	and set it as the new default font in the skin.
	To keep the standard font for tool tip text, we set it to
	the built-in font.
	*/

	IGUISkin* skin = env->getSkin();
	IGUIFont* font = env->getFont(mediaPath + "fonthaettenschweiler.bmp");
	if (font)
		skin->setFont(font);

	skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);

	/*
	We add three buttons. The first one closes the engine. The second
	creates a window and the third opens a file open dialog. The third
	parameter is the id of the button, with which we can easily identify
	the button in the event receiver.
	*/

	env->addButton(rect<s32>(10,240,110,240 + 32), 0, GUI_ID_QUIT_BUTTON,
			L"Quit", L"Exits Program");
	env->addButton(rect<s32>(10,280,110,280 + 32), 0, GUI_ID_NEW_WINDOW_BUTTON,
			L"New Window", L"Launches a new Window");
	env->addButton(rect<s32>(10,320,110,320 + 32), 0, GUI_ID_FILE_OPEN_BUTTON,
			L"File Open", L"Opens a file");

	/*
	Now, we add a static text and a scrollbar, which modifies the
	transparency of all gui elements. We set the maximum value of
	the scrollbar to 255, because that's the maximal value for
	a color value.
	Then we create an other static text and a list box.
	*/

	env->addStaticText(L"Transparent Control:", rect<s32>(150,20,350,40), true);
	IGUIScrollBar* scrollbar = env->addScrollBar(true,
			rect<s32>(150, 45, 350, 60), 0, GUI_ID_TRANSPARENCY_SCROLL_BAR);
	scrollbar->setMax(255);
	scrollbar->setPos(255);
	setSkinTransparency( scrollbar->getPos(), env->getSkin());

	// set scrollbar position to alpha value of an arbitrary element
	scrollbar->setPos(env->getSkin()->getColor(EGDC_WINDOW).getAlpha());

	env->addStaticText(L"Logging ListBox:", rect<s32>(10,110,350,130), true);
	IGUIListBox * listbox = env->addListBox(rect<s32>(10, 140, 350, 210));
	env->addEditBox(L"Editable Text", rect<s32>(350, 80, 550, 100));

	// Store the appropriate data in a context structure.
	SAppContext context;
	context.device = device;
	context.counter = 0;
	context.listbox = listbox;

	// Then create the event receiver, giving it that context structure.
	MyEventReceiver receiver(context);

	// And tell the device to use our custom event receiver.
	device->setEventReceiver(&receiver);


	/*
	And at last, we create a nice Irrlicht Engine logo in the top left corner.
	*/
	env->addImage(driver->getTexture(mediaPath + "irrlichtlogo2.png"),
			position2d<int>(10,10));


	/*
	That's all, we only have to draw everything.
	*/
  fluid_settings_t* settings;
//  int arg1 = 1;
  char buf[512];
//  int c, i;
  int interactive = 1;
  int midi_in = 1;
  fluid_player_t* player = NULL;
  fluid_midi_router_t* router = NULL;
  //fluid_sequencer_t* sequencer = NULL;
  fluid_midi_driver_t* mdriver = NULL;
  fluid_audio_driver_t* adriver = NULL;
  fluid_synth_t* synth = NULL;
#ifdef NETWORK_SUPPORT
  fluid_server_t* server = NULL;
  int with_server = 0;
#endif
  char* config_file = NULL;
  int audio_groups = 0;
  int audio_channels = 0;
  int dump = 0;
  int fast_render = 0;
  static const char optchars[] = "a:C:c:dE:f:F:G:g:hijK:L:lm:nO:o:p:R:r:sT:Vvz:";
#ifdef LASH_ENABLED
  int connect_lash = 1;
  int enabled_lash = 0;		/* set to TRUE if lash gets enabled */
  fluid_lash_args_t *lash_args;

  lash_args = fluid_lash_extract_args (&argc, &argv);
#endif



  settings = new_fluid_settings();


 /* The 'groups' setting is relevant for LADSPA operation and channel mapping
   * in rvoice_mixer.
   * If not given, set number groups to number of audio channels, because
   * they are the same (there is nothing between synth output and 'sound card')
   */
  if ((audio_groups == 0) && (audio_channels != 0)) {
      audio_groups = audio_channels;
  }
  if (audio_groups != 0)
  {
      fluid_settings_setint(settings, "synth.audio-groups", audio_groups);
  }

  if (fast_render) {
    midi_in = 0;
    interactive = 0;
#ifdef NETWORK_SUPPORT
    with_server = 0;
#endif
    fluid_settings_setstr(settings, "player.timing-source", "sample");
    fluid_settings_setint(settings, "synth.lock-memory", 0);
  }

  /* create the synthesizer */
  synth = new_fluid_synth(settings);
  if (synth == NULL) {
    fprintf(stderr, "Failed to create the synthesizer\n");
    exit(-1);
  }


  /* load the soundfonts (check that all non options are SoundFont or MIDI files) */
//  for (i = arg1; i < argc; i++) {
    if (fluid_is_soundfont(psoundfont))
    {
      if (fluid_synth_sfload(synth, psoundfont, 1) == -1)
	fprintf(stderr, "Failed to load the SoundFont %s\n", psoundfont);
    }
    else if (!fluid_is_midifile(psoundfont))
      fprintf (stderr, "Parameter '%s' not a SoundFont or MIDI file or error occurred identifying it.\n",
	       psoundfont);


  /* start the synthesis thread */
  if (!fast_render) {
		fluid_settings_setstr(settings, "audio.driver", "alsa");
    adriver = new_fluid_audio_driver(settings, synth);
    if (adriver == NULL) {
      fprintf(stderr, "Failed to create the audio driver\n");
//      goto cleanup;
    }
  }


  /* start the midi router and link it to the synth */
#if WITH_MIDI
  if (midi_in) {
    /* In dump mode, text output is generated for events going into and out of the router.
     * The example dump functions are put into the chain before and after the router..
     */
    //sequencer = new_fluid_sequencer2(0);

    router = new_fluid_midi_router(
      settings,
      dump ? fluid_midi_dump_postrouter : fluid_synth_handle_midi_event,
      (void*)synth);

    if (router == NULL) {
      fprintf(stderr, "Failed to create the MIDI input router; no MIDI input\n"
	      "will be available. You can access the synthesizer \n"
	      "through the console.\n");
    } else {
      mdriver = new_fluid_midi_driver(
	settings,
	dump ? fluid_midi_dump_prerouter : fluid_midi_router_handle_midi_event,
	(void*) router);
      if (mdriver == NULL) {
	fprintf(stderr, "Failed to create the MIDI thread; no MIDI input\n"
		"will be available. You can access the synthesizer \n"
		"through the console.\n");
      }
    }
  }
#endif


  /* play the midi fildes, if any */
//  for (i = arg1; i < argc; i++) {
    if  (fluid_is_midifile(psong)) {

      if (player == NULL) {
	player = new_fluid_player(synth);
	if (player == NULL) {
	  fprintf(stderr, "Failed to create the midifile player.\n"
		  "Continuing without a player.\n");
	//  break;
	}
      }

      fluid_player_add(player, psong);
    }
 // }

  if (player != NULL) {

    if (fluid_synth_get_sfont(synth, 0) == NULL) {
      /* Try to load the default soundfont if no soundfont specified */
      char *s;
      if (fluid_settings_dupstr(settings, "synth.default-soundfont", &s) != FLUID_OK)
        s = NULL;
      if ((s != NULL) && (s[0] != '\0'))
        fluid_synth_sfload(synth, s, 1);

      FLUID_FREE(s);
    }

    fluid_player_play(player);
  }

  cmd_handler = new_fluid_cmd_handler(synth, router);
  if (cmd_handler == NULL) {
    fprintf(stderr, "Failed to create the command handler\n");
 //   goto cleanup;
  }

  /* try to load the user or system configuration */
  if (config_file != NULL) {
    fluid_source(cmd_handler, config_file);
  } else if (fluid_get_userconf(buf, sizeof(buf)) != NULL) {
    fluid_source(cmd_handler, buf);
  } else if (fluid_get_sysconf(buf, sizeof(buf)) != NULL) {
    fluid_source(cmd_handler, buf);
  }

  /* run the server, if requested */
#ifdef NETWORK_SUPPORT
  if (with_server) {
    server = new_fluid_server(settings, synth, router);
    if (server == NULL) {
      fprintf(stderr, "Failed to create the server.\n"
	     "Continuing without it.\n");
    }
  }
#endif

#ifdef LASH_ENABLED
  if (enabled_lash)
    fluid_lash_create_thread (synth);
#endif




	while(device->run() && driver)
	if (device->isWindowActive())
	{
		driver->beginScene(video::ECBF_COLOR | video::ECBF_DEPTH, SColor(0,200,200,200));

		env->drawAll();



 if (fast_render) {
    char *filename;
    if (player == NULL) {
      fprintf(stderr, "No midi file specified!\n");
//      goto cleanup;
    }

    fluid_settings_dupstr (settings, "audio.file.name", &filename);
    printf ("Rendering audio to file '%s'..\n", filename);
    if (filename) FLUID_FREE (filename);

    fast_render_loop(settings, synth, player);
  }

        /* Play a note */
  //      fluid_synth_noteon(synth, 0, 60, 100);

//        printf("Press \"Enter\" to stop: ");
//        fgetc(stdin);
//        printf("done\n");





		driver->endScene();
	}

        if (adriver) {
                delete_fluid_audio_driver(adriver);
        }
        if (synth) {
                delete_fluid_synth(synth);
        }
        if (settings) {
                delete_fluid_settings(settings);
        }

	device->drop();

	return 0;
}

/*
**/
