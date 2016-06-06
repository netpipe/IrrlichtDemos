#include "GameGlobals.h"
#include <time.h>

void GameSetup()
{
	GameConfig::Load("config.dat", &Game);
}

const float distances[] = { // array of distances from the Sun
	// app_distance, real_distance_MKm
	0000.0f, 0000.0f, // Sun
	0150.0f, 0057.9f, // Mercury
	0265.0f, 0108.2f, // Venus
	0400.0f, 0149.6f, // Earth ( =1.00 a.u. )
	0600.0f, 0230.0f, // Mars
	0820.0f, 0414.7f, // Ceres
	1200.0f, 0778.3f, // Jupiter
	1800.0f, 1427.0f, // Saturn
	2400.0f, 2871.0f, // Uranus
	3000.0f, 4497.0f, // Neptune
	3700.0f, 5900.0f, // Pluto
	7000.0f, 14960.0f // far away from end of Kuiper Belt ( =100.0 a.u )
	// ...addable and insertable (but must be sorted)...
};
int distances_count = -1;
const int distance_colors[] = {
	0x0, // Sun (not used)
	0xDBB787, // Mercury
	0xCD8C30, // Venus
	0x40CC40, // Earth
	0xB53724, // Mars
	0x859298, // Ceres
	0xDFA468, // Jupiter
	0xF0C382, // Saturn
	0x9CBAC5, // Uranus
	0x5C77CE, // Neptune
	0x412533, // Pluto
	0x0 // Kuiper Belt (not used)
	// ...addable (must match distances[])...
};
const char distance_names[][16] = {
	"Sun", // not used
	"Mercury",
	"Venus",
	"Earth",
	"Mars",
	"Ceres",
	"Jupiter",
	"Saturn",
	"Uranus",
	"Neptune",
	"Pluto",
	"Kuiper Belt" // not used
	// ...addable (must match distances[])...
};

float cam_movespeed = 250.0f; // default camera movement speed
float cam_timespeed = 0.001f * 60.0f; // default time speed (min/s)
bool cam_inputreceive = false;
GameObject* selobject = NULL;
bool dialog_visible = false;
gui::IGUIFont* font = NULL;

gui::IGUIStaticText* status2 = NULL;
gui::IGUIStaticText* status3 = NULL;
gui::IGUIStaticText* status_sysinfo = NULL;

GameMapDescription* mapdesc = NULL;

int cam_lookfrom = 0;
int cam_lookat = 0;
gui::IGUIListBox *listFrom = NULL;
gui::IGUIListBox *listAt = NULL;
int video_modes_list[1024*3]; // width1, height1, bpp1, width2...

bool labelVisibility = false;
void SetLabelsVisibility_r(scene::ISceneNode *node, bool value);

bool rulerVisibility = false;
video::SColorf rulerSunAmbient;

bool galaxy_mode = false;
scene::ICameraSceneNode* galaxy_camera = 0;
scene::ICameraSceneNode* user_camera = 0;

gui::IGUIInOutFader *screen_fader;
bool screen_fader_used = false;
bool screen_fader_want_exit = false;

int user_camera_reset_counter = -1;
core::position2d<int> user_camera_reset_pos;

char monthes_str[12][4] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

GameMeasurer gameMeasurer;

void GameInit()
{
	// Init distinct values

	distances_count = (sizeof(distances)/sizeof(*distances)) / 2;

	srand(Game.Timer->getRealTime());

	Game.Video->setFog(video::SColor(0,8,8,8), true, 500.0f, 1200.0f);

	// Maps setup

	Game.FS->addZipFileArchive("data.dat", true, false);

	Game.Map->Load("maps/sun.xml");
	Game.Map->Load("maps/mercury.xml");
	Game.Map->Load("maps/venus.xml");
	Game.Map->Load("maps/earth.xml");
	Game.Map->Load("maps/mars.xml");
	Game.Map->Load("maps/jupiter.xml");
	Game.Map->Load("maps/saturn.xml");
	Game.Map->Load("maps/uranus.xml");
	Game.Map->Load("maps/neptune.xml");
	Game.Map->Load("maps/pluto.xml");

	Game.Map->Load("maps/asteroid_belt.xml");
	Game.Map->Load("maps/kuiper_belt.xml");
	Game.Map->Load("maps/stars.xml");

	Game.Map->Load("maps/galaxy.xml");

	// User camera setup

	Game.Timer->setSpeed(cam_timespeed);

	user_camera = GameFPSCamera(
		core::vector3df(-400.0f, 0.0f, 0.0f), 50.0f,
		cam_movespeed/(cam_timespeed/(0.001f*60.0f)));

	user_camera->setFarValue(10000.0f);

	// Map description setup

	mapdesc = new GameMapDescription();
	mapdesc->Load("maps/_descriptions.xml");

	scene::ISceneNode* n;
	gui::IGUIFont* fnt = Game.GUI->getFont("media_fonts/SegoeUI_10px_bold.xml");
	for (int i=0; i < mapdesc->ItemsCount; i++)
	{
		if (mapdesc->Items[i].title == L"Help") continue;
		if (mapdesc->Items[i].title == L"Milky Way") continue;

		n = Game.Map->GetObjectByName(mapdesc->Items[i].key[0])->pNode;

		Game.Scene->addTextSceneNode(
			fnt, core::stringw(mapdesc->Items[i].title.c_str()).c_str(),
			video::SColor(100,255,255,255), n);
	}
	
	SetLabelsVisibility_r(Game.Scene->getRootSceneNode(), labelVisibility);

	// GUI setup

	Game.GUI->addImage(Game.Video->getTexture("media_gui/gui_panel_distance.png"), core::position2d<int>(10,Game.Height-40));
	status3 = Game.GUI->addStaticText(L"x", core::rect<int>(52,Game.Height-36,264,Game.Height-10));
	status3->setOverrideFont(Game.GUI->getFont("media_fonts/SansSerif_14px.xml"));

	Game.GUI->addImage(Game.Video->getTexture("media_gui/gui_panel_time.png"), core::position2d<int>(Game.Width-266,Game.Height-40));
	status2 = Game.GUI->addStaticText( L"x", core::rect<int>(Game.Width-224,Game.Height-36,Game.Width-10,Game.Height-8));
	status2->setOverrideFont(Game.GUI->getFont("media_fonts/SansSerif_14px.xml"));

	Game.GUI->addImage(Game.Video->getTexture("media_gui/gui_banner.png"), core::position2d<s32>(Game.Width-256-10, 10));

	Game.Skin->setFont(Game.GUI->getFont("media_fonts/SegoeUI_11px.xml"));
	Game.Skin->setColor( gui::EGDC_3D_FACE, video::SColor( 200, 20,60,100 ) );
	Game.Skin->setColor( gui::EGDC_3D_SHADOW, video::SColor( 100, 0,0,0 ) );
	Game.Skin->setColor( gui::EGDC_3D_HIGH_LIGHT, video::SColor( 200, 40,120,240 ) );
	Game.Skin->setColor( gui::EGDC_3D_DARK_SHADOW, video::SColor( 150, 20,50,80 ) );
	Game.Skin->setColor( gui::EGDC_HIGH_LIGHT, video::SColor( 200, 20,40,80 ) );
	Game.Skin->setColor( gui::EGDC_ACTIVE_CAPTION, video::SColor( 255, 255,255,50 ) );
	Game.Skin->setColor( gui::EGDC_BUTTON_TEXT, video::SColor( 255, 255,255,255 ) );
	Game.Skin->setColor( gui::EGDC_TOOLTIP, video::SColor( 255, 0,0,0 ) );
	Game.Skin->setColor( gui::EGDC_TOOLTIP_BACKGROUND, video::SColor( 220, 240,220,200 ) );
	Game.Skin->setDefaultText( gui::EGDT_MSG_BOX_CANCEL, L"Close" );

	Game.GUI->addButton(core::rect<int>(  6, 4, 86,23), 0, 111, L"Exit");
	Game.GUI->addButton(core::rect<int>(  6,26, 86,45), 0, 222, L"Help");
	Game.GUI->addButton(core::rect<int>( 90, 4,170,23), 0, 667, L"Options");
	Game.GUI->addButton(core::rect<int>( 90,26,148,45), 0, 555, L"SysInfo")->setIsPushButton(true);
	Game.GUI->addButton(core::rect<int>(150,26,170,45), 0, 837, L"...")->setToolTipText(L"Show advanced SysInfo");
	Game.GUI->addButton(core::rect<int>(174, 4,254,23), 0, 444, L"Labels")->setIsPushButton(true);
	Game.GUI->addButton(core::rect<int>(174,26,254,45), 0, 333, L"Camera");
	Game.GUI->addButton(core::rect<int>(258, 4,338,23), 0, 888, L"Screenshot");
	Game.GUI->addButton(core::rect<int>(258,26,338,45), 0, 778, L"Galaxy")->setIsPushButton(true);
	Game.GUI->addButton(core::rect<int>(342, 4,422,23), 0, 780, L"Measurer");
	Game.GUI->addButton(core::rect<int>(342,26,422,45), 0, 779, L"Ruler")->setIsPushButton(true);

	int ys = (Game.Height/2)-(200/2)-80;

	Game.GUI->addImage(Game.Video->getTexture("media_gui/gui_panel_speedcontrols.png"), core::position2d<int>(4,50+ys));

	Game.GUI->addButton(core::rect<int>( 10, 98+ys, 30,120+ys), 0, 921, L"Z", L"Set camera speed to slow")->
		setOverrideFont(Game.GUI->getFont("media_fonts/Impact_11px.xml"));
	Game.GUI->addButton(core::rect<int>( 10,125+ys, 30,167+ys), 0, 922, L"X", L"Set camera speed to normal")->
		setOverrideFont(Game.GUI->getFont("media_fonts/Impact_11px.xml"));
	Game.GUI->addButton(core::rect<int>( 10,172+ys, 30,255+ys), 0, 923, L"C", L"Set camera speed to fast")->
		setOverrideFont(Game.GUI->getFont("media_fonts/Impact_11px.xml"));

	Game.GUI->addImage(Game.Video->getTexture("media_gui/gui_panel_timecontrols.png"), core::position2d<int>(Game.Width-32-4,50+ys));

	Game.GUI->addButton(core::rect<int>( Game.Width-30, 98+ys, Game.Width-10,120+ys), 0, 911, L"1", L"Set time speed to min/sec")->
		setOverrideFont(Game.GUI->getFont("media_fonts/Impact_11px.xml"));
	Game.GUI->addButton(core::rect<int>( Game.Width-30,125+ys, Game.Width-10,147+ys), 0, 912, L"2", L"Set time speed to hour/sec")->
		setOverrideFont(Game.GUI->getFont("media_fonts/Impact_11px.xml"));
	Game.GUI->addButton(core::rect<int>( Game.Width-30,152+ys, Game.Width-10,174+ys), 0, 913, L"3", L"Set time speed to day/sec")->
		setOverrideFont(Game.GUI->getFont("media_fonts/Impact_11px.xml"));
	Game.GUI->addButton(core::rect<int>( Game.Width-30,179+ys, Game.Width-10,211+ys), 0, 914, L"4", L"Set time speed to week/sec")->
		setOverrideFont(Game.GUI->getFont("media_fonts/Impact_11px.xml"));
	Game.GUI->addButton(core::rect<int>( Game.Width-30,216+ys, Game.Width-10,255+ys), 0, 915, L"5", L"Set time speed to month/sec")->
		setOverrideFont(Game.GUI->getFont("media_fonts/Impact_11px.xml"));

	screen_fader = Game.GUI->addInOutFader();

	Game.Device->setWindowCaption(L"Solar Model");

	Game.Cursor->setVisible(true);
	if (cam_inputreceive) Game.Cursor->setPosition(0.5f, 0.5f);
	user_camera->setInputReceiverEnabled(cam_inputreceive);

	// Galaxy feature initialization

	galaxy_camera =
		Game.Scene->addCameraSceneNode(
			0,
			core::vector3df(100000.0,100080.0,99880.0),
			core::vector3df(99998.0,100005.0,99970.0));

	Game.Scene->setActiveCamera(user_camera);

	// Setting up time (now)
	// FIXME: If program will be executed in past (before 2007-Jun-07)
	// than it will work wrong until 2007-Jun-07 will come.

	time_t tstart, tnow;
	tm t; // 2007-06-07 00:00:00
	t.tm_year = 107; // 2007-1900
	t.tm_mon = 05; // 06-1
	t.tm_mday = 07;
	t.tm_hour = 0;
	t.tm_min = 0;
	t.tm_sec = 0;
	t.tm_wday = 4;
	t.tm_yday = 157;
	t.tm_isdst = -1;
	tstart = mktime(&t);
	time(&tnow);
	Game.Timer->setTime((u32)(tnow-tstart));

	// Initializing GameComparer module

	gameMeasurer.Init();
}

void BeginExitApplication()
{
	screen_fader->setVisible(true);
	screen_fader->fadeOut((int)(cam_timespeed*1000.0f));
	screen_fader_want_exit = true;
}

int visual_button_clicked;

void ProcessEvent_GUI_ButtonClicked_Exit()
{
	BeginExitApplication();
}

void ProcessEvent_GUI_ButtonClicked_Help()
{
	core::stringc s, s2;

	mapdesc->GetDescription("GUI_Help", s, s2);

	if (s2.size() > 0)
	{
		Game.Skin->setSize(
			gui::EGDS_MESSAGE_BOX_WIDTH,
			Game.Skin->getFont()->getDimension(core::stringw(s2.c_str()).c_str()).Width + 92);

		Game.GUI->addMessageBox(
			core::stringw(s.c_str()).c_str(),
			core::stringw(s2.c_str()).c_str(),
			true, gui::EMBF_CANCEL);

		dialog_visible = true;
	}
}

void ProcessEvent_GUI_ButtonClicked_Camera()
{
	gui::IGUIWindow* win =
		Game.GUI->addWindow(core::rect<s32>(150,100,650,500), true, L"Camera", NULL, 9999);
	win->getCloseButton()->remove();

	Game.GUI->addStaticText(L"Look From", core::rect<s32>(20,30,240,50), false, false, win);
	listFrom = Game.GUI->addListBox(core::rect<s32>(20,50,240,320), win, -1, true);

	Game.GUI->addStaticText(L"Look At", core::rect<s32>(260,30,480,50), false, false, win);
	listAt = Game.GUI->addListBox(core::rect<s32>(260,50,480,320), win, -1, true);

	listFrom->addItem(L"- Not used -");
	listAt->addItem(L"- Not used -");
	for (int i=0; i < mapdesc->ItemsCount; i++)
	{
		if (mapdesc->Items[i].title == L"Help") continue;
		if (mapdesc->Items[i].title == L"Milky Way") continue;
		core::stringc s = mapdesc->Items[i].title;
		listFrom->addItem(core::stringw(s.c_str()).c_str());
		listAt->addItem(core::stringw(s.c_str()).c_str());
	}

	listFrom->setSelected(cam_lookfrom);
	listAt->setSelected(cam_lookat);

	Game.GUI->addButton(core::rect<s32>(300, 355, 380, 380), win, gui::EMBF_OK, L"OK");
	Game.GUI->addButton(core::rect<s32>(400, 355, 480, 380), win, gui::EMBF_CANCEL, L"Cancel");

	dialog_visible = true;
}

void ProcessEvent_GUI_ButtonClicked_Galaxy()
{
	galaxy_mode = !galaxy_mode;
	Game.Scene->setActiveCamera(galaxy_mode ? galaxy_camera : user_camera);

	Game.GUI->setFocus(0);
}

void ProcessEvent_GUI_ButtonClicked_SysInfo()
{
	if (status_sysinfo == NULL)
	{
		gui::IGUIWindow* win =
			Game.GUI->addWindow(
				core::rect<s32>(6, 48, 580, 140), false, L"System Information", NULL, 8888);

		win->getCloseButton()->remove();

		int w = win->getAbsolutePosition().getWidth();
		int h = win->getAbsolutePosition().getHeight();

		status_sysinfo =
			Game.GUI->addStaticText(
				L"x", core::rect<int>(10, 24, w-10, h-10), false, true, win);
	}
	else
	{
		status_sysinfo->getParent()->remove();
		status_sysinfo = NULL;
	}

	Game.GUI->setFocus(0);
}

void ProcessEvent_GUI_ButtonClicked_SysInfoAdvanced()
{
	core::stringw s;
	
	s = L"Detected video device features:";
	s += Game.Video->queryFeature(video::EVDF_RENDER_TO_TARGET) ? L"\n+ Render to target" : L"";
	s += Game.Video->queryFeature(video::EVDF_HARDWARE_TL) ? L"\n+ Hardware transform and lighting" : L"";
	s += Game.Video->queryFeature(video::EVDF_MULTITEXTURE) ? L"\n+ Multiple textures per material" : L"";
	s += Game.Video->queryFeature(video::EVDF_BILINEAR_FILTER) ? L"\n+ Bilinear filtering" : L"";
	s += Game.Video->queryFeature(video::EVDF_MIP_MAP) ? L"\n+ Mip maps" : L"";
	s += Game.Video->queryFeature(video::EVDF_MIP_MAP_AUTO_UPDATE) ? L"\n+ Driver updates mip maps automatically" : L"";
	s += Game.Video->queryFeature(video::EVDF_VERTEX_SHADER_1_1) ? L"\n+ Vertex Shader 1.1" : L"";
	s += Game.Video->queryFeature(video::EVDF_VERTEX_SHADER_2_0) ? L"\n+ Vertex Shader 2.0" : L"";
	s += Game.Video->queryFeature(video::EVDF_VERTEX_SHADER_3_0) ? L"\n+ Vertex Shader 3.0" : L"";
	s += Game.Video->queryFeature(video::EVDF_PIXEL_SHADER_1_1) ? L"\n+ Pixel Shader 1.1" : L"";
	s += Game.Video->queryFeature(video::EVDF_PIXEL_SHADER_1_2) ? L"\n+ Pixel Shader 1.2" : L"";
	s += Game.Video->queryFeature(video::EVDF_PIXEL_SHADER_1_3) ? L"\n+ Pixel Shader 1.3" : L"";
	s += Game.Video->queryFeature(video::EVDF_PIXEL_SHADER_1_4) ? L"\n+ Pixel Shader 1.4" : L"";
	s += Game.Video->queryFeature(video::EVDF_PIXEL_SHADER_2_0) ? L"\n+ Pixel Shader 2.0" : L"";
	s += Game.Video->queryFeature(video::EVDF_PIXEL_SHADER_3_0) ? L"\n+ Pixel Shader 3.0" : L"";
	s += Game.Video->queryFeature(video::EVDF_ARB_VERTEX_PROGRAM_1) ? L"\n+ ARB vertex programs 1.0" : L"";
	s += Game.Video->queryFeature(video::EVDF_ARB_FRAGMENT_PROGRAM_1) ? L"\n+ ARB fragment programs 1.0" : L"";
	s += Game.Video->queryFeature(video::EVDF_ARB_GLSL) ? L"\n+ GLSL" : L"";
	s += Game.Video->queryFeature(video::EVDF_HLSL) ? L"\n+ HLSL" : L"";
	s += Game.Video->queryFeature(video::EVDF_TEXTURE_NPOT) ? L"\n+ Non-power-of-two textures support" : L"";
	s += Game.Video->queryFeature(video::EVDF_FRAMEBUFFER_OBJECT) ? L"\n+ Framebuffer objects" : L"";
	s += Game.Video->queryFeature(video::EVDF_VERTEX_BUFFER_OBJECT) ? L"\n+ Vertex buffer objects" : L"";

	Game.Skin->setSize(
		gui::EGDS_MESSAGE_BOX_WIDTH,
		Game.Skin->getFont()->getDimension(s.c_str()).Width + 92);

	Game.GUI->addMessageBox(
		L"Advanced SysInfo", s.c_str(),
		true, gui::EMBF_CANCEL);

	dialog_visible = true;
}

void ProcessEvent_GUI_ButtonClicked_Ruler()
{
	rulerVisibility = !rulerVisibility;

	// hiding skybox
	Game.ClearBackBuffer = rulerVisibility;
	Game.ClearColor = video::SColor(255,0,0,0);
	for (int i=0; i<Game.Map->ObjectsCount; i++)
	{
		if (Game.Map->Objects[i]->Type == GAME_OBJECT_TYPE_SKYBOX)
		{
			Game.Map->Objects[i]->pNode->setVisible(!rulerVisibility);
		}
	}

	// modifying scene' ambient color

	GameObject *o = Game.Map->GetObjectByName("Sun");
	assert(o->Type == GAME_OBJECT_TYPE_LIGHT);
	GameLightObject *l = (GameLightObject*)o;
	video::SLight ld = l->pLightNode->getLightData();

	if (rulerVisibility)
	{
		// making scene more brighter
		rulerSunAmbient = ld.AmbientColor;
		ld.AmbientColor = video::SColorf(0.25f, 0.25f, 0.25f);
	}
	else
	{
		// resoting scene brightness
		ld.AmbientColor = rulerSunAmbient;
	}

	l->pLightNode->setLightData(ld);

	Game.GUI->setFocus(0);
}

void ProcessEvent_GUI_ButtonClicked_Measurer()
{
	gameMeasurer.Open();
	dialog_visible = gameMeasurer.IsVisible();
}

void ProcessEvent_GUI_ButtonClicked_Labels()
{
	labelVisibility = !labelVisibility;
	SetLabelsVisibility_r(Game.Scene->getRootSceneNode(), labelVisibility);

	Game.GUI->setFocus(0);
}

void ProcessEvent_GUI_ButtonClicked_Screenshot()
{
	core::stringc fn;
	core::stringw w;

	w = GameScreenshot(&Game, fn) ?
		L"Screenshot successfuly saved to\n" :
		L"Error saving screenshot to\n";

	w += fn.c_str();

	Game.Skin->setSize(
		gui::EGDS_MESSAGE_BOX_WIDTH,
		Game.Skin->getFont()->getDimension(w.c_str()).Width + 92);

	Game.GUI->addMessageBox(L"Screenshot", w.c_str(), true, gui::EMBF_CANCEL);

	dialog_visible = true;
}

void ProcessEvent_GUI_ButtonClicked_Options()
{
	// reading options from config file

	GameStruct conf;
	GameConfig::Load("config.dat", &conf);

	// creating window

	gui::IGUIWindow* win =
		Game.GUI->addWindow(core::rect<s32>(138,130,636,510), true, L"Options", NULL, 9944);
	win->getCloseButton()->remove();

	// video drivers list (we will not support Software and BurningsVideo
	// drivers because the rendering result is unacceptable)

	Game.GUI->addStaticText(
		L"Video Driver", core::rect<s32>(20,30,240,50), false, false, win);

	gui::IGUIComboBox *vdrv =
		Game.GUI->addComboBox(core::rect<s32>(20,50,240,70), win, 401);

	int i=0, k=0, sel=0;

#ifdef _IRR_COMPILE_WITH_OPENGL_
	vdrv->addItem(L"OpenGL");
	if (conf.DriverType == video::EDT_OPENGL) sel = i;
	i++;
#endif

#ifdef _IRR_COMPILE_WITH_DIRECT3D_8_
	vdrv->addItem(L"Direct3D 8");
	if (conf.DriverType == video::EDT_DIRECT3D8) sel = i;
	i++;
#endif

#ifdef _IRR_COMPILE_WITH_DIRECT3D_9_
	vdrv->addItem(L"Direct3D 9");
	if (conf.DriverType == video::EDT_DIRECT3D9) sel = i;
	i++;
#endif

	vdrv->setSelected(sel);

	// resolutions list

	Game.GUI->addStaticText(
		L"Resolution", core::rect<s32>(260,30,480,50), false, false, win);

	gui::IGUIComboBox *res =
		Game.GUI->addComboBox(core::rect<s32>(260,50,480,70), win, 402);
	
	k = 0;
	sel = 0;

	video::IVideoModeList *lst = Game.Device->getVideoModeList();
	for (int i=0; i < lst->getVideoModeCount(); i++)
	{
		int w = lst->getVideoModeResolution(i).Width;
		int h = lst->getVideoModeResolution(i).Height;
		int b = lst->getVideoModeDepth(i);

		if (w<800 || h<600 || b<16) continue; // we will not accept
						// resolution lower than 800 x 600 x 16 bpp

		core::stringc s = "";
		s += w;
		s += " x ";
		s += h;
		s += " x ";
		s += b;
		res->addItem(core::stringw(s.c_str()).c_str());

		if (conf.Width==w && conf.Height==h && conf.BitsPerPixel==b) sel = k;
		video_modes_list[k*3+0] = w;
		video_modes_list[k*3+1] = h;
		video_modes_list[k*3+2] = b;
		k++;
	}
	res->setSelected(sel);

	// fullscreen checkbox
	
	Game.GUI->addCheckBox(
		conf.Fullscreen,
		core::rect<s32>(260,90,480,110), win, 403, L"Use fullscreen mode");

	// antialiasing checkbox

	Game.GUI->addCheckBox(
		conf.AntiAlias,
		core::rect<s32>(20,90,240,110), win, 404, L"Use antialiasing");

	// backgroundrendering checkbox

	gui::IGUICheckBox *cb = Game.GUI->addCheckBox(
		conf.BackgroundRendering,
		core::rect<s32>(20,130,240,150), win, 406, L"Render in background");

	cb->setToolTipText(L"Render scene when Solar Model is not active application");

	// vsync checkbox

	Game.GUI->addCheckBox(
		conf.Vsync,
		core::rect<s32>(260,130,480,150), win, 405, L"Use VSync");

	// screenshotwithoutgui checkbox

	Game.GUI->addCheckBox(
		conf.ScreenshotWithoutGUI,
		core::rect<s32>(20,170,240,190), win, 419, L"Screenshot without GUI");

	// scheenshotquality scroll

	core::stringw w = "Use Screenshot Quality: ";
	w += conf.ScreenshotQuality;
	w += "%";
	Game.GUI->addStaticText(
		w.c_str(), core::rect<s32>(20,210,240,230), false, false, win, 408);

	gui::IGUIScrollBar *sb =
		Game.GUI->addScrollBar(true, core::rect<s32>(20,230,240,250), win, 407);

	sb->setMax(99);
	sb->setPos(conf.ScreenshotQuality-1);

	// hints

	Game.GUI->addStaticText(
		L"[i] Antialiasing and VSync ignores if running not in fullscreen mode.",
		core::rect<s32>(20,280,480,300), false, true, win);
	Game.GUI->addStaticText(
		L"[i] You have to restart application for certain settings to take effect.",
		core::rect<s32>(20,300,480,320), false, true, win);

	// SAVE & Cancel

	gui::IGUIButton *bSave = Game.GUI->addButton(
		core::rect<s32>(300,340,380,365), win, gui::EMBF_OK, L"SAVE");
	bSave->setToolTipText(L"Save changes to disk");

	gui::IGUIButton *bCancel = Game.GUI->addButton(
		core::rect<s32>(400,340,480,365), win, gui::EMBF_CANCEL, L"Cancel");

	dialog_visible = true;
}

bool ProcessEvent_GUI_ButtonClicked(SEvent event)
{
	if (event.EventType!=EET_GUI_EVENT ||
		event.GUIEvent.EventType!=gui::EGET_BUTTON_CLICKED)
	{
		return false;
	}

	switch (event.GUIEvent.Caller->getID())
	{
	case 111: // Exit
		ProcessEvent_GUI_ButtonClicked_Exit();
		return true;

	case 222: // Help
		ProcessEvent_GUI_ButtonClicked_Help();
		return true;

	case 333: // Camera
		ProcessEvent_GUI_ButtonClicked_Camera();
		return true;

	case 778: // Galaxy
		ProcessEvent_GUI_ButtonClicked_Galaxy();
		return true;

	case 779: // Ruler
		ProcessEvent_GUI_ButtonClicked_Ruler();
		return true;

	case 780: // Measurer
		ProcessEvent_GUI_ButtonClicked_Measurer();
		return true;

	case 444: // Labels
		ProcessEvent_GUI_ButtonClicked_Labels();
		return true;

	case 555: // SysInfo
		ProcessEvent_GUI_ButtonClicked_SysInfo();
		return true;

	case 837: // SysInfo Advanced
		ProcessEvent_GUI_ButtonClicked_SysInfoAdvanced();
		return true;

	case 888: // Screenshot
		ProcessEvent_GUI_ButtonClicked_Screenshot();
		return true;

	case 667: // Options
		ProcessEvent_GUI_ButtonClicked_Options();
		return true;

	case 911: visual_button_clicked = KEY_KEY_1; return false;
	case 912: visual_button_clicked = KEY_KEY_2; return false;
	case 913: visual_button_clicked = KEY_KEY_3; return false;
	case 914: visual_button_clicked = KEY_KEY_4; return false;
	case 915: visual_button_clicked = KEY_KEY_5; return false;

	case 921: visual_button_clicked = KEY_KEY_Z; return false;
	case 922: visual_button_clicked = KEY_KEY_X; return false;
	case 923: visual_button_clicked = KEY_KEY_C; return false;
	}

	return false;
}

bool ProcessEvent_GUI_Dialog_Options(SEvent event)
{
	if (event.EventType != EET_GUI_EVENT) return false;

	gui::IGUIElement *parent = event.GUIEvent.Caller->getParent();

	if (parent == 0) return false;
	if (parent->getID() != 9944) return false;

	if (event.GUIEvent.EventType == gui::EGET_SCROLL_BAR_CHANGED)
	{
		gui::IGUIWindow *win = (gui::IGUIWindow*) parent;
		int sq = ((gui::IGUIScrollBar*)win->getElementFromId(407))->getPos()+1;

		core::stringw w = "Use Screenshot Quality: ";
		w += sq;
		w += "%";
		((gui::IGUIStaticText*)win->getElementFromId(408))->setText(w.c_str());

		return true;
	}

	if (event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED)
	{
		gui::IGUIWindow *w;
		gui::IGUIComboBox* drv;
		gui::IGUIComboBox* res;
		io::IWriteFile *f;
		char drvbuf[256];
		int i, sel;

		switch (event.GUIEvent.Caller->getID())
		{
		case gui::EMBF_OK:
			w = (gui::IGUIWindow*) parent; // <-- dialog window;

			f = Game.FS->createAndWriteFile("config.dat");

			// video driver
			drv = (gui::IGUIComboBox*) w->getElementFromId(401, true);
			assert(drv != NULL);

			i=0;
			sel=0;

#ifdef _IRR_COMPILE_WITH_OPENGL_
			if (drv->getSelected() == i) strcpy(drvbuf, "drivertype opengl\r\n");
			i++;
#endif
#ifdef _IRR_COMPILE_WITH_DIRECT3D_8_
			if (drv->getSelected() == i) strcpy(drvbuf, "drivertype direct3d8\r\n");
			i++;
#endif
#ifdef _IRR_COMPILE_WITH_DIRECT3D_9_
			if (drv->getSelected() == i) strcpy(drvbuf, "drivertype direct3d9\r\n");
			i++;
#endif

			f->write(drvbuf, (s32)strlen(drvbuf));

			// resolution

			char resbuf[128];
			res = (gui::IGUIComboBox*) w->getElementFromId(402, true);
			assert(res != NULL);
			sprintf(
				resbuf,
				"width %u\r\nheight %u\r\nbitsperpixel %u\r\n",
				video_modes_list[res->getSelected()*3+0],
				video_modes_list[res->getSelected()*3+1],
				video_modes_list[res->getSelected()*3+2]);
			f->write(resbuf, (s32)strlen(resbuf));

			// fullscreen

			char fsbuf[128];
			sprintf(fsbuf, "fullscreen %s\r\n",
				((gui::IGUICheckBox*)w->getElementFromId(403,true))->isChecked() ?
				"1" : "0");
			f->write(fsbuf, (s32)strlen(fsbuf));

			// antialias

			char aabuf[128];
			sprintf(aabuf, "antialias %s\r\n",
				((gui::IGUICheckBox*)w->getElementFromId(404,true))->isChecked() ?
				"1" : "0");
			f->write(aabuf, (s32)strlen(aabuf));

			// vsync

			char vsbuf[128];
			sprintf(vsbuf, "vsync %s\r\n",
				((gui::IGUICheckBox*)w->getElementFromId(405,true))->isChecked() ?
				"1" : "0");
			f->write(vsbuf, (s32)strlen(vsbuf));

			// backgroundrendering

			char brbuf[128];
			sprintf(brbuf, "backgroundrendering %s\r\n",
				((gui::IGUICheckBox*)w->getElementFromId(406,true))->isChecked() ?
				"1" : "0");
			f->write(brbuf, (s32)strlen(brbuf));

			Game.BackgroundRendering =
				((gui::IGUICheckBox*)w->getElementFromId(406,true))->isChecked();

			// screenshotwithoutgui

			char sgbuf[128];
			sprintf(sgbuf, "screenshotwithoutgui %s\r\n",
				((gui::IGUICheckBox*)w->getElementFromId(419,true))->isChecked() ?
				"1" : "0");
			f->write(sgbuf, (s32)strlen(sgbuf));

			Game.ScreenshotWithoutGUI =
				((gui::IGUICheckBox*)w->getElementFromId(419,true))->isChecked();

			// screenshotquality

			char ssqbuf[128];
			sprintf(ssqbuf, "screenshotquality %u\r\n",
				((gui::IGUIScrollBar*)w->getElementFromId(407,true))->getPos()+1);
			f->write(ssqbuf, (s32)strlen(ssqbuf));
			Game.ScreenshotQuality =
				((gui::IGUIScrollBar*)w->getElementFromId(407,true))->getPos()+1;

			// closing file
			f->drop();

			break;

		case gui::EMBF_CANCEL:
			// nothing
			break;
		}

		parent->remove();
		dialog_visible = false;

		return true;
	}

	return false;
}

bool ProcessEvent_GUI_Dialog_Camera(SEvent event)
{
	if (event.EventType != EET_GUI_EVENT) return false;
	if (event.GUIEvent.EventType != gui::EGET_BUTTON_CLICKED) return false;

	gui::IGUIElement *parent = event.GUIEvent.Caller->getParent();

	if (parent == 0) return false;
	if (parent->getID() != 9999) return false;

	switch (event.GUIEvent.Caller->getID())
	{
	case gui::EMBF_OK:
		assert(listFrom != NULL);
		assert(listAt != NULL);

		cam_lookfrom = listFrom->getSelected();
		cam_lookat = listAt->getSelected();

		break;

	case gui::EMBF_CANCEL:
		// nothing
		break;
	}

	listFrom = NULL;
	listAt = NULL;

	parent->remove();
	dialog_visible = false;

	return true;
}

bool ProcessEvent_GUI_Dialog_Measurer(SEvent event)
{
	bool r = false;

	if (gameMeasurer.IsVisible())
	{
		r = gameMeasurer.Event(event);
		dialog_visible = gameMeasurer.IsVisible();
	}

	return r;
}

bool ProcessEvent_Mouse_ObjectPicking(SEvent event)
{
	if (event.EventType != EET_MOUSE_INPUT_EVENT ||
		event.MouseInput.Event != EMIE_LMOUSE_PRESSED_DOWN ||
		cam_inputreceive != false ||
		selobject == NULL)
	{
		return false;
	}

	core::stringc s, s2;

	mapdesc->GetDescription(selobject->Name, s, s2);

	if (s2.size() > 0)
	{
		Game.Skin->setSize(
			gui::EGDS_MESSAGE_BOX_WIDTH,
			Game.Skin->getFont()->getDimension(core::stringw(s2.c_str()).c_str()).Width + 92);

		Game.GUI->addMessageBox(
			core::stringw(s.c_str()).c_str(),
			core::stringw(s2.c_str()).c_str(),
			true, gui::EMBF_CANCEL);

		dialog_visible = true;
	}

	return true;
}

bool ProcessEvent_Keys_SpeedAndTime(SEvent event)
{
	if ((event.EventType != EET_KEY_INPUT_EVENT || !event.KeyInput.PressedDown) &&
		(!visual_button_clicked))
	{
		return false;
	}

	f32 t = -1.0f;
	f32 s = -1.0f;

	int key = visual_button_clicked;
	if (key==0) key = event.KeyInput.Key;

	switch (key)
	{
	case KEY_KEY_1: t = 0.001f * 60.0f; break;
	case KEY_KEY_2: t = 0.001f * 60.0f * 60.0f; break;
	case KEY_KEY_3: t = 0.001f * 60.0f * 60.0f * 24.0f; break;
	case KEY_KEY_4: t = 0.001f * 60.0f * 60.0f * 24.0f * 7.0f; break;
	case KEY_KEY_5: t = 0.001f * 60.0f * 60.0f * 24.0f * 30.0f; break;

	case KEY_KEY_Z: s = 250.0f; break;
	case KEY_KEY_X: s = 1000.0f; break;
	case KEY_KEY_C: s = 2500.0f; break;

	case KEY_SPACE:
		cam_inputreceive = !cam_inputreceive;
		Game.Cursor->setVisible(!cam_inputreceive);
		if (cam_inputreceive) Game.Cursor->setPosition(0.5f, 0.5f);
		user_camera->setInputReceiverEnabled(cam_inputreceive);

		{{{
			// This code fixes Camera Key State bug:
			// http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=27742
			// The fix provided by rogerborg (see link above).
			// Note: when bug will be fixed this code will not be needed.
			// {{{

			core::list<scene::ISceneNodeAnimator*>::ConstIterator ait =
				user_camera->getAnimators().begin();
			
			SEvent ekey;
			ekey.EventType = EET_KEY_INPUT_EVENT;
			ekey.KeyInput.Char = 0;
			ekey.KeyInput.Control = false;
			ekey.KeyInput.Shift = false;
			ekey.KeyInput.PressedDown = false;

			for (; ait != user_camera->getAnimators().end(); ++ait)
			{
				if ((*ait)->isEventReceiverEnabled())
				{
					for(int keyCode = 0; keyCode < KEY_KEY_CODES_COUNT; ++keyCode)
					{
						ekey.KeyInput.Key = (EKEY_CODE)keyCode;
						(void)((*ait)->OnEvent(ekey));
					}
				}
			}

			// }}}
		}}}

		return true;

	case KEY_ESCAPE:
		BeginExitApplication();
		return true;
	}

	if (t>0.0f) cam_timespeed = t;
	if (s>0.0f)	cam_movespeed = s;

	if (t>0.0f || s>0.0f)
	{
		Game.Timer->setSpeed(cam_timespeed);

		user_camera_reset_pos = Game.Cursor->getPosition();
		user_camera_reset_counter = 2;

		scene::ICameraSceneNode* newcam =
			GameFPSCamera(
				core::vector3df(-500.0f, 0.0f, -200.0f), 50.0f,
				cam_movespeed/(cam_timespeed/(0.001f*60.0f)));

		if (galaxy_mode) Game.Scene->setActiveCamera(galaxy_camera);

		newcam->setInputReceiverEnabled(cam_inputreceive);

		newcam->setTarget(user_camera->getTarget()-user_camera->getPosition());
		newcam->setFarValue(user_camera->getFarValue());
		newcam->setPosition(user_camera->getPosition());
		newcam->setRotation(user_camera->getRotation());
		newcam->setUpVector(user_camera->getUpVector());

		user_camera->remove();
		user_camera = newcam;

		return true;
	}

	return false;
}

bool GameEvent(SEvent event)
{
	bool r;

	if (gameMeasurer.IsVisible())
	{
		return ProcessEvent_GUI_Dialog_Measurer(event);
	}

	if (event.EventType == EET_GUI_EVENT &&
		event.GUIEvent.EventType == gui::EGET_MESSAGEBOX_CANCEL)
	{
		dialog_visible = false;
		return true;
	}

	visual_button_clicked = 0;
	r = ProcessEvent_GUI_ButtonClicked(event); // may set >> visual_button_clicked
	if (r) return r;

	r = ProcessEvent_GUI_Dialog_Options(event);
	if (r) return r;

	r = ProcessEvent_GUI_Dialog_Camera(event);
	if (r) return r;

	if (dialog_visible) return false;

	r = ProcessEvent_Mouse_ObjectPicking(event);
	if (r) return r;

	r = ProcessEvent_Keys_SpeedAndTime(event); // reads << visual_button_clicked
	if (r) return r;

	return false;
}

void SetLabelsVisibility_r(scene::ISceneNode *node, bool value)
{
	if (node->getType() == scene::ESNT_TEXT) node->setVisible(value);

	core::list<scene::ISceneNode*>::ConstIterator begin = node->getChildren().begin(); 
	core::list<scene::ISceneNode*>::ConstIterator end = node->getChildren().end(); 
	for (; begin != end; ++begin) SetLabelsVisibility_r(*begin, value);
}

void ProcessMousePosition()
{
	// restoring mouse position after camera reset

	if (user_camera_reset_counter>0) user_camera_reset_counter--;
	if (user_camera_reset_counter==0)
	{
		user_camera_reset_counter = -1;
		Game.Cursor->setPosition(user_camera_reset_pos);
	}
}

void ProcessScreenFader()
{
	if (!screen_fader_used && screen_fader->isReady())
	{
		screen_fader->fadeIn((int)(cam_timespeed*1000.0f));
		screen_fader_used = true;
	}

	if (screen_fader_used && screen_fader->isVisible() && screen_fader->isReady())
	{
		if (screen_fader_want_exit) Game.Device->closeDevice();
		else screen_fader->setVisible(false);
	}
}

void ProcessSpaceTimer()
{
	char tm[64];

	if (galaxy_mode)
	{
		strcpy(tm, "225M-250M years / cycle");
	}
	else
	{
		u32 t = Game.Timer->getTime();
		t += (u32) (5.0f*(365.25f/12.0f)*86400.0f + 6.0f*86400.0f);

		u32 year,mon,day,hr,min,sec;
		year = (u32) ( t / (365.25*86400) );
		t -= (u32) ( year*(365.25*86400) );
		mon = (u32) ( t / ((365.25/12)*86400) );
		t -= (u32) ( mon*((365.25/12)*86400) );
		day = t / 86400;
		t -= day*86400;
		hr = t / 3600;
		t -= hr*3600;
		min = t / 60;
		t -= min*60;
		sec = t;

		sprintf(
			tm, "%04u-%s-%02u %02u:%02u:%02u",
			year+2007, monthes_str[mon], day+1, hr, min, sec
		);
	}

	core::stringw s = tm;
	status2->setText(s.c_str());
}

void ProcessSpaceDistance()
{
	char tm[64];

	if (galaxy_mode)
	{
		strcpy( tm, "175000 light-years" );
	}
	else
	{
		core::vector3df campos = user_camera->getAbsolutePosition();

		float x = (float) campos.getDistanceFrom(
			Game.Map->GetObjectByName("Sun_Volume")->pNode->getAbsolutePosition());

		int i;

		for (i=1; i<distances_count; i++) if (x < distances[i*2]) break;
		if (i < distances_count)
		{
			float app_1 = ( distances[i*2+1] - distances[(i-1)*2+1] ) / ( distances[i*2] - distances[(i-1)*2] );
			x = distances[(i-1)*2+1] + ( x - distances[(i-1)*2] ) * app_1;
			sprintf(tm, "%.0lf Mkm = %.2lf a.u.", x, x/distances[3*2+1]);
		}
		else
		{
			strcpy(tm, ">15000 Mkm = 100 a.u.");
		}
	}

	status3->setText(core::stringw(tm).c_str());
}

void ProcessSysInfo()
{
	if (!status_sysinfo) return;

	u32 proc_speed;
	Game.OS->getProcessorSpeedMHz(&proc_speed);

	u32 mem_total, mem_avail;
	Game.OS->getSystemMemory(&mem_total, &mem_avail);

	wchar_t ss[1024];
/*
	_snwprintf(ss, sizeof(ss),
		L"FPS:  %03u            "
		L"POLYGONS DRAWN:  %06u            "
		L"RENDERER:  %ls\n"
		L"PROCESSOR SPEED:  %u MHz            "
		L"MEMORY AVAILABLE:  %u Mb / %u Mb\n"
		L"OS VERSION:  %ls",
		Game.Video->getFPS(),
		Game.Video->getPrimitiveCountDrawn(),
		Game.Video->getName(),
		proc_speed,
		mem_avail>>10, mem_total>>10,
		Game.OS->getOperationSystemVersion()
	);
*/
	status_sysinfo->setText(ss);
}

void ProcessObjectSelection()
{
	selobject = NULL;

	// turn off all objects
	for (int i=0; i<Game.Map->ObjectsCount; i++)
	{
		if (Game.Map->Objects[i]->pNode->getMaterialCount()>0)
		{
			Game.Map->Objects[i]->pNode->getMaterial(0).EmissiveColor =
				video::SColor(0,0,0,0);
		}
	}

	if (!cam_inputreceive &&
		!dialog_visible &&
		Game.Cursor->getPosition().Y>45 &&
		Game.Cursor->getPosition().X>30 &&
		Game.Cursor->getPosition().X<Game.Width-30)
	{
		scene::ISceneNode* selnode =
			Game.SceneCollision->getSceneNodeFromScreenCoordinatesBB(
				core::position2d<s32>(
					Game.Cursor->getPosition().X,
					Game.Cursor->getPosition().Y));

		// turn on selected object
		for (int i=0; i<Game.Map->ObjectsCount; i++)
		{
			if (Game.Map->Objects[i]->pNode == selnode)
			{
				if (Game.Map->Objects[i]->pNode->getMaterialCount()>0)
				{
					selobject = Game.Map->Objects[i];
					selobject->pNode->getMaterial(0).EmissiveColor =
						video::SColor(255,255,255,255);
				}
			}
		}
	}
}

void Draw3DCircle(int count, float radius, video::SColor color)
{
	core::line3df line;

	core::position2df first;
	core::position2df a, b;

	for (int i=0; i<count; i++)
	{
		b = a;

		float p = i / (float)count * (core::PI*2);
		a = core::position2df(sin(p)*radius, cos(p)*radius);

		if (i == 0)
		{
			first = a;
		}
		else
		{
			line.start = core::vector3df(a.X, 0.0f, a.Y);
			line.end = core::vector3df(b.X, 0.0f, b.Y);
			Game.Video->draw3DLine(line.start, line.end, color);
		}
	}

	line.start = core::vector3df(a.X, 0.0f, a.Y);
	line.end = core::vector3df(first.X, 0.0f, first.Y);
	Game.Video->draw3DLine(line.start, line.end, color);
}

void ProcessCameraLooks()
{
	if (cam_lookfrom > 0)
	{
		for (int i=0; i < mapdesc->ItemsCount; i++)
		{
			if (mapdesc->Items[i].title == L"Help") continue;
			if (mapdesc->Items[i].title == L"Milky Way") continue;
			if (i == cam_lookfrom-1)
			{
				scene::ISceneNode* n = Game.Map->GetObjectByName(mapdesc->Items[i].key[0])->pNode;
				core::vector3df v = n->getAbsolutePosition();
				user_camera->setPosition(v);
				break;
			}
		}
	}

	if (cam_lookat > 0)
	{
		for (int i=0; i < mapdesc->ItemsCount; i++)
		{
			if (mapdesc->Items[i].title==L"Help") continue;
			if (mapdesc->Items[i].title==L"Milky Way") continue;
			if (i==cam_lookat-1)
			{
				scene::ISceneNode* n = Game.Map->GetObjectByName(mapdesc->Items[i].key[0])->pNode;
				core::vector3df v = n->getAbsolutePosition();
				user_camera->setTarget(v);
				break;
			}
		}
	}
}

bool GameRun()
{
	/*
	static scene::IMeshSceneNode *cometNode = 0;
	static core::vector3df cometSpeed;
	const float G = -6.6742e-11f;

	if (!cometNode)
	{
		cometNode = Game.Scene->addCubeSceneNode(40.0f);
		cometNode->setPosition(core::vector3df(1000.0f, 0.0f, 1000.0f));
		cometSpeed = core::vector3df(0.0f, 0.0f, 2.0f);
	}
	else
	{
		core::vector3df hbPos = Game.Map->GetObjectByName("Sun_Volume")->pNode->getPosition();
		core::vector3df oldPos = cometNode->getPosition();

		float len = core::vector3df(hbPos-oldPos).getLength();
		float force = 100.0f / (len*len);

		cometSpeed += (hbPos-oldPos)*force;
		cometNode->setPosition(oldPos + cometSpeed);

		if (status_sysinfo)
		{
			core::stringw s;
			s += "dsitance : ";
			s += len;
			s += "\nspeed : ";
			s += cometSpeed.getLength();
			s += "\nSun gravity force : ";
			s += force;
			status_sysinfo->setText(s.c_str());
		}
	}
	*/

	ProcessMousePosition();

	ProcessScreenFader();

	ProcessSpaceTimer();

	ProcessSpaceDistance();

	ProcessSysInfo();

	ProcessObjectSelection();

	ProcessCameraLooks();

	return true;
}

void GamePost()
{
	if (rulerVisibility)
	{
		video::SMaterial mtl;
		mtl.Lighting = false;
		Game.Video->setMaterial(mtl);
		Game.Video->setTransform(video::ETS_WORLD, core::matrix4());

		for (int i=1; i<distances_count-1; i++) // skip Sun and Kuiper Belt
		{
			float d = distances[i*2];
			video::SColor c = video::SColor(distance_colors[i]);
			Draw3DCircle(40 + int(d)/400, d, c);

			core::vector3df pos =
				Game.Map->GetObjectByName(distance_names[i])->pNode->getAbsolutePosition();

			Game.Video->draw3DLine(
				core::vector3df(pos.X, pos.Y, pos.Z),
				core::vector3df(0.0f, 0.0f, 0.0f),
				distance_colors[i]);
		}
	}
}

void GameDone()
{
	if (mapdesc) delete mapdesc;
	gameMeasurer.Done();
}
