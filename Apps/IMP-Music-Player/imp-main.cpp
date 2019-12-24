// IMP - Irrlicht Music Player
// Jorge Rosa (1999 - 2018 Portugal)
// Email: jorge.bigarte@gmail.com
// Portfolio: https://sites.google.com/site/jorgerosaportfolio
// Facebook: https://www.facebook.com/jorge.bigarte
// Youtube: https://www.youtube.com/user/jorgeerosa/videos
// Project: https://sourceforge.net/projects/irrlichtmp

/// ################################################################################################################################################################################
///
/// Compiling in my system: (May be different in yours)
///
/// Windows 7 (64 Bits)
/// Code::Blocks 16.01 (32 Bits) - Installed at: "C:\Program Files (x86)\CodeBlocks"
/// The selected compiler: GNU GCC Compiler - Installed at: "C:\MinGW"
/// The "Toolchain executables" are: (set by this order) "mingw32-gcc.exe","mingw32-g++.exe","mingw32-g++.exe","ar.exe","GDB/CDB Debugger: Default","windres.exe","mingw32-make.exe"
///
/// ################################################################################################################################################################################
/// STATIC LIBRARIES in Code::Blocks: add '-static' in settings --> compiler --> Linker settings --> Other linker options

// - The code is almost all here, in a big loop, so it could be easier to follow and learn
// - Designed to be human readable as it can be... in my way, of course...

// For texts in other idioms...

// For special characters hardcoded in code:
// CODE::BLOCKS --> Settings --> Editor --> Other Settings --> Encoding --->  UTF-8, Then C::B must be restarted.
// Then: text->setText(L"Olééé") will now work fine!

// For special characters from external files:
// std::wfstream, std::wstring, stringw all the way!...
// Example:
// std::locale defaultLocale(""); // from machine config
// std::wfstream infile(lyricsPath.c_str());
// infile.imbue(defaultLocale);   // imbue must be done before opening


/// TO INSTALL NEEDED LIBS, PLEASE TAKE A LOOK AT "header.h" FILE
#include "imp-headers.h"


/// Define Irrlicht Global Variables:
// scene::ICameraSceneNode* camera[2] = {0,0};
IrrlichtDevice* device           = NULL;  // Create Device
irr::video::IVideoDriver* driver = NULL;  // Create Driver
irr::scene::ISceneManager* smgr  = NULL;  // Create Scene Manager
irr::io::IFileSystem* filesys    = NULL;  // getFileSystem();

gui::IGUIEnvironment* guienv               = NULL;  // For GUI texts
gui::IGUIListBox* musicListBox             = NULL;  // For GUI ListBox
gui::IGUIFileOpenDialog* musicFileDialog   = NULL;  // For GUI FileOpenDialog
gui::IGUIScrollBar* musicPositionScrollbar = NULL;  // For GUI ScrollBar
gui::IGUIScrollBar* musicVolumeScrollbar   = NULL;  // For GUI ScrollBar
gui::IGUIScrollBar* musicPitchScrollbar    = NULL;  // For GUI ScrollBar
gui::IGUISkin* skin                        = NULL;
gui::IGUIWindow* win1                      = NULL;
gui::IGUIWindow* win2                      = NULL;

/// Fonts: Main
gui::IGUIFont* fontMain = NULL; // Get Texts Font

/// Fonts: Lyrics:
gui::IGUIFont* font_lyrics[10];
gui::IGUIStaticText* texts[50]={};

/// Effects globals (needed for Loading Bars too)
IPostProc* ppRenderer = NULL;
CEffectPostProc* ppEffects[8]={};
std::string ppEffectName[8]={};
int ppeCount = 8; // To be used in main loop (for select a random effect)

/// Take screenshot and convert to texture - For Cameras transition effects (Background image)
irr::video::ITexture* camBackgroundImageTexture = NULL;
irr::video::IImage* camScreenshot = NULL;

/// Take screenshot and convert to texture - For Loading Bars (Background image)
irr::video::ITexture* backgroundImageTexture = NULL;
irr::video::IImage* screenshot = NULL;

/// Loading Bars - Progress
CLoadingBar* LoadBar           = NULL;
CLoadingBar* LoadBarForMusic   = NULL;
CLoadingBar* LoadBarForLyrics  = NULL;

/// LASER LIGHTS: (Global defined: Avoids crashes on exit)
scene::IVolumeLightSceneNode* nodeLaserA[10]={};
scene::IVolumeLightSceneNode* nodeLaserB[10]={};

/// Courtains:
scene::IAnimatedMeshSceneNode* nodeCourtainBackground;
scene::IAnimatedMeshSceneNode* nodeCourtain2;
scene::IAnimatedMeshSceneNode* nodeCourtain3;

/// Main Text:
stringw windowtexts     = L"IMP - Irrlicht Music Player V.1.5.012 | Jorge Rosa | jorge.bigarte@gmail.com | ASCR | Portugal";
wstring fullLoadedTexts = L""; // The holder for the lyrics file full content


/// Define App Global Variables:
DIR* dir;
struct dirent* ent;
std::vector<std::string> musicList;            // Array, for songs full path, that will grow...
std::vector<std::string> musicListNamesOnly;   // Array, for songs names only, that will grow...
std::vector<std::string> musicExtensionsOnly;  // Array, for songs extensions only, that will grow...
std::vector<std::string> musicLyricsListPath;  // Array, for lyrics files, that will grow...



/// Define Audiere Global Variables:
audiere::AudioDevicePtr  audiereAudioDevice = OpenDevice(); // Must be globals! (else audiere will remove it).
/// audiere::MIDIDevicePtr audiereMidiDevice = OpenMIDIDevice(0); // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr music         = NULL;              // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr soundIntro    = NULL;              // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr soundClick    = NULL;              // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr soundOoo      = NULL;              // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr soundClaps    = NULL;              // Must be globals! (else audiere will remove it).
std::vector<std::string> tags;                              // Declare array to fill with music tags.


/// Other Global Variables:
irr::u32 SCREEN_WIDTH  = 820;    // Will be overrided by settings file.
irr::u32 SCREEN_HEIGHT = 465;    // Will be overrided by settings file.
bool FULLSCREEN        = false;  // Will be overrided by settings file.
bool PPEFFECTS         = true;   // Will be overrided by settings file.
bool INTERNALS_SOUNDS  = true;   // Will be overrided by settings file.
bool STENCIL_SHADOWS   = true;   // NOT set in the settings file, yet.
bool VSYNC             = true;   // Limit rendering to the screen refresh rate (true == to avoid crashes and hardware damages)
int FRAMES_PER_SECOND  = 0;      // FOR: If FPS < 60 then do not do "this effect", etc...
int CAMERA_SELECTED    = 1;

/// Initialize some Variables:
/// For keyboard/mouse/other events (to avoid too fast repetitive press/clicks loops):
int canClickAgain[100] = {};              // Array to hold key/mouse status (UP)
bool mouseIsOutsideThisArea = true;

irr::f32 al                  = 0.0f;             // for Ambient Light
bool fadeIn                  = true;             // for Ambient Light
int columns_light_speed      = 0;                // Columns Lights speed
irr::f32 flashes_light_speed = 0;                // Photo Flashes Lights speed
irr::f32 delay               = 0.0f;             // Create a delay
irr::f32 SPEED_LIGHTS        = 0.35f;            // 1.00 = fast, 0.01 = very slow

/// This is the main light responsible for all the stage colors:
scene::ILightSceneNode* light_ML1;
SLight data_ML1;

/// This is the light responsible for white strobe effects:
scene::ILightSceneNode* light_FL1;
SLight data_FL1;

/// This is the spot (focused) light:
scene::ILightSceneNode* light_SP1;
SLight data_SP1;

/// Cameras target:
vector3df cameraTarget  = core::vector3df(0,0,0); // Will be set after...

irr::f32 flashwait      = 0.0f;            // flash photo is visible during a time
int randomflash         = 0;               // from (0 to 15)

/// For FAST random zooms:
bool doFOV          = true;                // For random FOV
irr::f32 newFOV1    = 0.5f;                // For random FOV
bool doFOVrand      = true;                // For random FOV
bool doZoomingIn    = true;                // For zooming in FOV
bool dozoomrand     = true;
irr::f32 zoomwait   = 0.0f;                 // Time until do the next FOV
int zoomwaited      = 5000;

/// For SLOW constant zooms:
bool doFOV2         = true;                // For random FOV
irr::f32 newFOV2    = 0.5f;                // For random FOV
bool doFOVrand2     = true;                // For random FOV
bool doZoomingIn2   = true;                // For zooming in FOV
bool dozoomrand2    = true;
irr::f32 zoomwait2  = 0.0f;                 // Time until do the next FOV

/// Effects:
bool doEFF          = true;                // For random effects
irr::f32 newEFF     = 0.5f;                // For random effects
bool doEFFrand      = true;                // For random effects
int waitEFF         = 0;

/// Music and sounds:
float GLOBAL_VOLUME = 0.5;                 // Maximum is 1.0
int musicID         = 0;
float MUSIC_PITCH   = 1.0;
int playMode        = 1;                   // Play Modes: 1 Single, 2 Single Loop, 3 List loop, 4 List Random
bool paused         = false;
bool playNextSong   = false;
bool doAnimations   = false;


/// Balloons:
bool doingOneBalloon = false;
int ballon_id = 0;

/// Disco balls:
bool doBall        = true;
int ballDirection  = 1;

irr::f32 camRatio1 = 4.0f;      // Default irrlicht camera ratio is: 4.0/3.0
irr::f32 camRatio2 = 3.0f;      // Default irrlicht camera ratio is: 4.0/3.0
bool randomCamera  = true;

irr::f32 rotcamera_X = 0.0f;  // Rotating cameras effect
irr::f32 rotcamera_Y = 0.0f;  // Rotating cameras effect
irr::f32 rotcamera_Z = 0.0f;  // Rotating cameras effect
bool rotcameratoleft = true;  // Rotating cameras effect

float uu  = 0.0,   vv = 0.0;
float uuu = 0.0,  vvv = 0.0;
float zx  = 0.001, zy = 0.001; // zoom in (scale)
float rot = 0.0;
float radians = 0.0;

/// Changing nodes for Band Style:
stringw bandaAll          = "";
wstring BAND_STYLE        = L"Man";   // Band Style
int BAND_SELECTED         = 0;        // Band Style ( 0 == Pivot, 1 == Pop, 2 == Traditional, 3 == Rock, etc...)
int OLD_BAND_SELECTED     = 0;        // Band Style
bool enterNextBand        = true;
bool changingBands        = true;
irr::f32 MotionSpeedBands = 2.0f;
int infos                 = 5;        // 0 = Music Infos, 1= Lyrics file content, 2 = Help, 3 = Keys, 4 = About, 5 = Nothing
int SHOW_TYPE             = 1;        // 0 = Slow, 1 = Normal, 2 = Fast

/// 3D Clock:
bool CLOCK_VISIBLE = false;

/// Lyrics text colors:
int FONT_COLOR_SELECTED = 5;
wstring FONT_COLOR      = L"Purple";
int FONT_SIZE_SELECTED  = 1;
wstring FONT_SIZE       = L"1";
int FONT_COLOR_RED      = 255;
int FONT_COLOR_GREEN    = 255;
int FONT_COLOR_BLUE     = 255;

/// When display the lyrics?...
wstring fulllines = L"";
int linesCounter  = 0;

int stagelights  = 0;

/// Initializing Timers: (Part #1)
int lastFPS             =  -1; // Will be set in the main loop...
irr::u32 then           = 0.0; // Will be set in the main loop...
irr::f32 frameDeltaTime = 0.0f; // Will be set in the main loop...

/// IDs for our scrollbars: (Already defined in "myEvents.h")
/*
enum {
ID_SCROLLBAR_POSITION,
ID_SCROLLBAR_VOLUME,
ID_SCROLLBAR_PITCH
};
*/

/// For development only:
int DEBUG_MODE = 0; /// <-- | 0 = No debug | 1 = debug |



    /*
    int applyICON(){
    HINSTANCE hInstance = (HINSTANCE)GetModuleHandle(NULL);
    HICON hSmallIcon = (HICON) LoadImage ( hInstance, MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR );
    irr::video::SExposedVideoData exposedData = driver->getExposedVideoData();
    HWND hWnd = reinterpret_cast<HWND>(exposedData.D3D9.HWnd);
    SendMessage ( hWnd, WM_SETICON, ICON_SMALL, (long)hSmallIcon );
    };
    */



    /// Small, Handy, Functions:

    // Convert INT to String:
    std::string convertIntToString(int number_in){
    stringstream ss;   // Create a stringstream
    ss << number_in;   // Add number to the stream
    return ss.str();   // Return a string with the contents of the stream
    };

    // Convert String to Upper Case:
    std::string convertToUpperInString(std::string string_in){
    for (size_t i = 0; i < string_in.length(); i++){
    string_in[i]=toupper(string_in[i]);
    };
    return string_in.c_str();
    };

    // Convert String to Upper Case:
    std::wstring convertToUpperInWString(std::wstring string_in){
    for (size_t i = 0; i < string_in.length(); i++){
    string_in[i]=toupper(string_in[i]);
    };
    return string_in.c_str();
    };

    // Filter these chars:
    bool my_filter1(char c){ return c==' ' || c==';' || c=='-' || c=='\n'; }
    // Function to remove above filtered chars:
    std::string& removeCaracteresInString(std::string& input_string){
    char* new_end;
    new_end=&*std::remove_if(input_string.begin(), input_string.end(), my_filter1);
    unsigned new_string_len = new_end - &*input_string.begin();
    input_string.resize(new_string_len);
    return input_string;
    };

    // Filter these chars:
    bool my_filter2(char c){ return c==' ' || c==';' || c=='-' || c=='\n'; }
    // Function to remove above filtered chars:
    wstring& removeCaracteresInWString(wstring& input_string){
    // char * new_end;
    wchar_t* new_end;
    new_end=&*std::remove_if(input_string.begin(), input_string.end(), my_filter2);
    unsigned new_string_len = new_end - &*input_string.begin();
    input_string.resize(new_string_len);
    return input_string;
    };

    // Function to remove the duplicates in an array:
    template<class Iterator>
    Iterator makeUnique(Iterator first, Iterator last){
    while (first != last){
    Iterator next(first);
    last = std::remove(++next, last, *first);
    first = next;
    };
    return last;
    };

    // Function to remove one white space from a string:
    std::string removeWhite(std::string& str){
    std::string temp;
    for(unsigned int i=0; i<str.length(); i++){
    if(str[i] == ' ' && str[i-1] == ' '){ } else { temp += str[i]; };
    };
    return temp;
    };

    // Function to removes all white spaces from a string:
    std::string removeAllWhitesAndBreakLinesInString(std::string& str){
    std::string temp;
    for(unsigned int i=0; i<str.length(); i++){
    if(str[i] != ' ' && str[i] != '\n'){ temp += str[i]; };
    };
    return temp;
    };

    // Function to removes all white spaces from a string:
    wstring removeAllWhitesAndBreakLinesInWstring(wstring& str){
    wstring temp;
    for(unsigned int i=0; i<str.length(); i++){
    if(str[i] != L' ' && str[i] != L'\n'){ temp += str[i]; };
    };
    return temp;
    };

    // Function to split string into lines, using a delimiter: (for example the <br> tag)
    std::vector<std::string> splitThisString(const std::string &str, const std::string &delim){
    std::vector<std::string> ret;
    std::string::const_iterator prev = str.begin();
    for (std::string::const_iterator i = str.begin(); i<str.end()-delim.length()+1; i++){
    if (equal(delim.begin(), delim.end(), i)){
    ret.push_back(std::string(prev,i));
    i += delim.length()-1;
    prev = i+1;
    };
    };
    ret.push_back(std::string(prev,str.end()));
    return ret;
    };

    // Function to split string into lines, using a delimiter: (for example the <br> tag)
    std::vector<wstring> splitThisWideString(const wstring &str, const wstring &delim){
    std::vector<wstring> ret;
    wstring::const_iterator prev = str.begin();
    for (wstring::const_iterator i = str.begin(); i < str.end() - delim.length()+1; i++){
    if (equal(delim.begin(), delim.end(), i)){
    ret.push_back(wstring(prev,i));
    i += delim.length()-1;
    prev = i+1;
    };
    };
    ret.push_back(wstring(prev,str.end()));
    return ret;
    };

    // Function to remove words from wide string: (for example the <code> tag)
    bool wordToRemoveFound = false;
    template<typename T> T removeWordsFromWideString(T SrcStart, T SrcEnd, const T TargetStart, const T TargetEnd){
    wordToRemoveFound = false;
    size_t TargetLen = TargetEnd-TargetStart;
    T i;
    while((i=std::search(SrcStart,SrcEnd,TargetStart,TargetEnd)) != SrcEnd){
    std::copy(i + TargetLen,SrcEnd+1,i);
    wordToRemoveFound = true;
    };
    return SrcStart;
    };

    // Round up a float type and show ONLY one decimal place:
    float roundUpThisFloat(float n){
    float t;
    t = n-floor(n);
    if(t>=0.5){n*=10; ceil(n); n/=10;
    } else {
    n*=10; floor(n); n/=10;
    };
    return n;
    };

    // Check if wstring is a number:
    bool isNumber(const wstring &str){
    wstring::const_iterator it = str.begin();
    while (it != str.end() && std::isdigit(*it)){ it++; };
    return !str.empty() && it == str.end();
    };


    // Replace chars in wide string:
    wstring replaceCharInWString(wstring str, char ch1, char ch2){
    for (unsigned int i=0; i<str.length(); i++) {
    if(str[i] == ch1){ str[i] = ch2; };
    };
    return str;
    };





    /// TODO: Read many lyrics file formats... (srt, sub, kar, ...)
    void testing_FILES() {
    /*
    std::ifstream in("demo.srt", ios::in | ios::binary);
    if(!in){ std::cout << "Cannot open the file"; }

    std::ofstream out("output.txt", ios::out | ios::binary);
    if(!out){ std::cout << "Cannot save to file"; }

    std::string str;
    char rep[80]; // replace array

    getline(in,str);
    while(in) {
    for(int i=0; i<80; i++) {
    if(str[i] == ','){ rep[i] = ' '; } else { rep[i] = str[i]; };
    out.put(rep[i]);
    };
    out << std::endl;
    getline(in,str);
    };

    in.close();
    out.close();
    */
    };








    /// ##########################################################################################
    /// ############# READ SETTINGS FROM  FILE ###################################################
    /// ##########################################################################################

    // READ data from text file:
    void readSettingsFromFile(){

    try{
    int iLines = 10;                     // Set the number of lines to read
    std::string arraySETT[iLines][2];    // Declares an array of X rows and 2 columns.
    int irow = 0;
    // int icol = 0;
	ifstream iinFile;
    iinFile.open("settings.txt");
    if(iinFile.is_open()){
    // while (! myfile.eof()){
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Reading \"settings.txt\" file..." << endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    for(irow=0; irow<iLines; irow++){
    std::string temp1="";
    std::string temp2="";
    getline(iinFile, temp1, '=');
    getline(iinFile, temp2, ';');
    arraySETT[irow][0] = removeAllWhitesAndBreakLinesInString(temp1);
    arraySETT[irow][1] = removeAllWhitesAndBreakLinesInString(temp2);
    // Debug line:
    std::cout << "Line " << irow << " -> " << arraySETT[irow][0] << ": " << arraySETT[irow][1] << std::endl;
    };
    // }
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    iinFile.close();
    // All done, now lets apply read settings to vars:
    SCREEN_WIDTH  = atoi(arraySETT[0][1].c_str());                      // String to int
    SCREEN_HEIGHT = atoi(arraySETT[1][1].c_str());                      // String to int
    std::string fs = removeCaracteresInString(arraySETT[2][1]).c_str(); // Text to bool (workaround)
    if(fs == "true"){ FULLSCREEN = true; };                             // Text to bool (workaround)
    std::string ppe = removeCaracteresInString(arraySETT[3][1]);        // Text to bool (workaround)
    if(ppe == "false"){ PPEFFECTS = false; };                           // Text to bool (workaround)
    std::string aud = removeCaracteresInString(arraySETT[4][1]);        // Text to bool (workaround)
    if(aud == "false"){ INTERNALS_SOUNDS = false; };                    // Text to bool (workaround)
    std::string ssw = removeCaracteresInString(arraySETT[5][1]);        // Text to bool (workaround)
    if(ssw == "false"){ STENCIL_SHADOWS = false; };                     // Text to bool (workaround)
    } else { printf("ERROR: Unable to read custom settings from \"settings.txt\" file! \n"); }

    std::cout << "\n " << std::endl;

    } catch (int e) { std::cout << "ERROR in readSettingsFromFile(): " << std::endl; std::cout << e << std::endl; };

    };



    /// ##########################################################################################
    /// ############# READ SETTINGS FROM FILE - END ##############################################
    /// ##########################################################################################





    /*
    /// ##########################################################################################
    /// ############# AUDIO: (USING SMFL LIBRARY) ################################################
    /// ##########################################################################################

    // ogg, wav, flac, aiff, au, raw, paf, svx, nist, voc, ircam, w64, mat4, mat5 pvf, htk, sds, avr, sd2, caf, wve, mpc2k, rf64


    sf::SoundBuffer buffer;
    sf::Sound music;
    float sduration = 0.0;
    float sposition = 0.0;

    int loadMyMusic(std::string musicPath){
    music = sf::Sound();
    buffer.LoadFromFile(musicPath.c_str());
    music.SetBuffer(buffer);

    // sduration = buffer.GetDuration();
    // sposition = music.GetPlayingOffset();



    // music.GetStatus() == sf::Sound::Stopped;
    // music.GetStatus() == sf::Sound::Playing;

    // music.SetPitch(3);
    // music.Play();
    // sf::Sleep(2);
    // music.Stop();
    // music.Volume = 50;
    // music.SetLoop(true);



    // while (music.GetStatus() == sf::Sound::Playing){
    // std::cout << buffer.GetSamples()[int(music.GetPlayingOffset()*buffer.GetSampleRate())] << std::endl;
    // sf::Sleep(0.01);
    // }

    }

    /// ##########################################################################################
    /// ############# AUDIO: (USING SMFL LIBRARY) ################################################
    /// ##########################################################################################
    */







    /// ##########################################################################################
    /// ############# SPECTRUM: (USING KissFFT LIBRARY) ##########################################
    /// ##########################################################################################

    /*
    void TestFft(const char* title, const kiss_fft_cpx in[N], kiss_fft_cpx out[N]) {
    kiss_fft_cfg cfg;

    printf("%s\n", title);

    if ((cfg = kiss_fft_alloc(N, 0, NULL, NULL)) != NULL) {
    size_t i;

    kiss_fft(cfg, in, out);
    free(cfg);

    for (i = 0; i < N; i++){
    printf(" in[%2zu] = %+f , %+f    "
    "out[%2zu] = %+f , %+f\n", i, in[i].r, in[i].i, i, out[i].r, out[i].i);
    }
    } else {
    printf("not enough memory?\n");
    exit(-1);
    }
    }

    inline float scale(kiss_fft_scalar val) {
    int g = 0;
    return val < 0 ? val*(1/32768.0f ) : val*(1/32767.0f);
    };
    */


    /// ##########################################################################################
    /// ############# SPECTRUM: (USING KissFFT LIBRARY) ##########################################
    /// ##########################################################################################



    void drawMoreStuff(){
    /// driver->draw2DRectangleOutline(core::rect<s32>(5,5,180,SCREEN_HEIGHT-5), video::SColor(100,180,180,255)) ;
    /// driver->draw2DLine(position2di(10,10), position2di(200,200), SColor(255,255,0,0));
    };


    bool doCameraTransition = false;
    bool doingCamTransOnce[30]; /// Only needed for the fist and last song cameras
    // bool fillvar = true;
    irr::f32 camfadeout = 150; /// 255 == opaque | 0 = full transparent

    void drawCameraTransition(){
    /// Fill vars only once:
    // if(fillvar==true){ for(int i=0; i<5; i++){ doingCamTransOnce[i]=true; }; fillvar=false; };
    /// Take screenshot:
    if(doCameraTransition==true){
    camScreenshot = driver->createScreenShot();
    if(camScreenshot!=NULL){ /// Voiding crashes, when menu window is hidden
    if(camBackgroundImageTexture != NULL){ driver->removeTexture(camBackgroundImageTexture); camBackgroundImageTexture = NULL; };
    if(camBackgroundImageTexture == NULL){ camBackgroundImageTexture = driver->addTexture("camShot", camScreenshot); };
    camScreenshot->drop();
    camScreenshot = NULL;
    };
    camfadeout = 150;
    doCameraTransition = false;
    };
    /// Make it dissapear:
    camfadeout -= 0.29*frameDeltaTime;
    if(camfadeout<=0){ camfadeout = 0; };
    /// Render Background Image:
    int sw = driver->getScreenSize().Width;
    int sh = driver->getScreenSize().Height;
    driver->draw2DImage(camBackgroundImageTexture, position2d<s32>(0,0), core::rect<s32>(0,0,sw,sh), 0, video::SColor(camfadeout,200,200,200), false);
    };


    void closed_COURTAINS(){
    if(nodeCourtain2 && nodeCourtain3){
    nodeCourtain2->setScale(core::vector3df(1.0,1.0,1.0));
    nodeCourtain2->setVisible(true);
    nodeCourtain3->setScale(core::vector3df(1.0,1.0,1.0));
    nodeCourtain3->setVisible(true);
    };
    };


    void open_or_close_COURTAINS(){
    if(nodeCourtain2 && nodeCourtain3){
    /// Anim - Both Courtains Open:
    if(doAnimations){
    /// Courtain left:
    vector3df sc2 = nodeCourtain2->getScale();
    sc2.X -= 0.00020f*frameDeltaTime; if(sc2.X<=0.0f){ sc2.X = 0.0f; nodeCourtain2->setVisible(false); };
    nodeCourtain2->setScale(core::vector3df(sc2));
    /// Courtain right:
    vector3df sc3 = nodeCourtain3->getScale();
    sc3.X -= 0.00020f*frameDeltaTime; if(sc3.X<=0.0f){ sc3.X = 0.0f; nodeCourtain3->setVisible(false); };
    nodeCourtain3->setScale(core::vector3df(sc3));
    };
    /// Anim - Both Courtains Close:
    if(!doAnimations){
    /// Courtain left:
    nodeCourtain2->setVisible(true);
    vector3df sc2 = nodeCourtain2->getScale();
    sc2.X += 0.00005f*frameDeltaTime; if(sc2.X>=1.0f){ sc2.X = 1.0f; };
    nodeCourtain2->setScale(core::vector3df(sc2));
    /// Courtain right:
    nodeCourtain3->setVisible(true);
    vector3df sc3 = nodeCourtain3->getScale();
    sc3.X += 0.00005f*frameDeltaTime; if(sc3.X>=1.0f){ sc3.X = 1.0f; };
    nodeCourtain3->setScale(core::vector3df(sc3));
    };
    };
    };


    /// ##########################################################################################
    /// ############# AUDIO: (USING AUDIERE LIBRARY) #############################################
    /// ##########################################################################################

    int fchannels = 0;
    int frate = 0;
    audiere::SampleFormat fformat;
    audiere::SampleSource* src;

    /// Changing from frames, to seconds:
    int songFullTime = 0; // Retrieves number in seconds, ex: 1756785 --> Music Total Length
    int songCurrentTime = 0; // Retrieves number in seconds, ex: 1756785 --> Music Position (Now its 0)


    void load_MUSIC(std::string musicPath){

    if(texts[20]!=NULL){ texts[20]->setText(L"Please Standby\nThe show will continue in a moment..."); };
    CAMERA_SELECTED = 1; // 1
    fulllines = L"";
    for(int i=0; i<5; i++){ doingCamTransOnce[i]=true; };
    // closed_COURTAINS();

    /// Prepare the render with an GREYSCALE effect:
    // driver->beginScene(false, false, 0);
    driver->beginScene(true, true, 0);
    texts[25]->setText(L"Loading Music...");
    texts[7]->setText(L"");
    if(ppEffects[0]){ ppEffects[0]->render(NULL); };
    driver->setMaterial(driver->getMaterial2D()); // In case buffer is distorted
    guienv->drawAll();  /// Draw GUI
    drawMoreStuff();
    driver->endScene();
    texts[25]->setText(L"");
    texts[7]->setText(L"");

    /// Take screenshot and convert to texture:
    screenshot = driver->createScreenShot();
    if(screenshot!=NULL){ /// Voiding crashes, when menu window is hidden
    if(backgroundImageTexture != NULL){ driver->removeTexture(backgroundImageTexture); backgroundImageTexture = NULL; };
    if(backgroundImageTexture == NULL){ backgroundImageTexture = driver->addTexture("screenshot", screenshot); };
    screenshot->drop();
    screenshot = NULL;
    };

    /// Loading Bar for Lyrics Only: #1 (Initialize)
	LoadBarForMusic = new CLoadingBar(device, driver, guienv, backgroundImageTexture, rect<s32>(300, SCREEN_HEIGHT-50, SCREEN_WIDTH-300, SCREEN_HEIGHT-50+15), true, 7);  // posX, posY, bar width, bar height
    LoadBarForMusic->setPercent(5); // 0 to 100
    LoadBarForMusic->draw();
    // LoadBarForMusic->setVisible(true); // for the last "LoadBar" only

    // audiereAudioDevice = NULL; // clear device
    // music = NULL;         // clear music
    tags.clear();            // clears all items in array:  std::vector<std::string> tags
    if(music){ if(music->isPlaying()){ music->stop(); }; }; // clear music
    music = NULL;            // clear music
    bool error = false;

    // blah = OpenSampleSource(...)
    // Calculate length in seconds here ....
    // music = OpenSound(device, blah);

    // audiereAudioDevice = audiere::OpenDevice();
    if(!audiereAudioDevice){ std::cout << "ERROR: Audiere: Cant open audio device! \n" << std::endl; };
    music = audiere::OpenSound(audiereAudioDevice.get(), musicPath.c_str(), true); // Load music into memory? true / false

    /// May be a midi file: (TODO)
    // if(music == NULL){ audiere::OpenFile(audiereMidiDevice.get(), musicPath.c_str(), false); }
    // std::vector<FileFormatDesc> fff = audiere::GetSupportedFileFormats();

    if(!music){ error = true; std::cout << "ERROR: Audiere - Cant open the audio file!" << std::endl; };  /// Void CRASH for many reasons!
    if(music && music->getLength()<=0){ error = true; music = NULL; std::cout << "ERROR: Audiere - File not supported!" << std::endl; }; /// Void CRASH when file format is supported, but not the codec!

    /// Coloring ListBox:
    if(musicListBox && error){
    int c = musicListBox->getItemCount();
    for(int i=0; i<c; i++){ musicListBox->setItemOverrideColor(i, video::SColor(200,150,150,200)); }; // Colors to default
    musicListBox->setItemOverrideColor(musicListBox->getSelected(), video::SColor(255,255,0,0)); /// ERROR -> paint this item in red
    };

    LoadBarForMusic->setPercent(50); // 0 to 100
    LoadBarForMusic->draw();

    // All is ready, lets go...
    if(audiereAudioDevice && music && music->getLength()>0){

    // music->setRepeat(false);        // true, false or int.
    music->setVolume(GLOBAL_VOLUME);   // volume 0.0 = silence, 1.0 = maximum. Default is 1.0.
    music->setPitchShift(MUSIC_PITCH); // shift can range from 0.5 to 2.0. Default is 1.0.
    music->setPan(0.0);                // pan -1.0 = left, 0.0 = center (default), 1.0 = right
    // music->play();
    /// music->setReverb(x); <-- Would be GREAT for a real live concert, right!?... ;)
    // music->reset();
    // music->error();
    // music->isMuted();
    // music->isPlaying();
    // music->getPosition();
    // music->getLength();
    // music->read(64, buffer);
    // music->read();
    // music->openStream();
    // music = audiereAudioDevice->openBuffer(outSound, largest, sfinfo.channels, sfinfo.samplerate * 2, SF_ML16);

    LoadBarForMusic->setPercent(60); // 0 to 100
    LoadBarForMusic->draw();

    // audiere::MIDIDevice *audiereMIDIDevice = NULL;
    std::cout << "" << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Loaded a new Music" << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    src = audiere::OpenSampleSource(musicPath.c_str());

    LoadBarForMusic->setPercent(90); // 0 to 100
    LoadBarForMusic->draw();

    std::cout << " " << std::endl;
    std::cout << "Audiere Device: " << audiereAudioDevice->getName() << std::endl;

    if(src){
    if(src->getTagCount()>0){
    std::cout << " " << std::endl;
    std::cout << "Music Tags: " << std::endl;
    tags.push_back("Music Tags: \n");
    };
    for(int i=0; i<src->getTagCount(); i++){
    std::cout << "Tag: " << i << ") Key: " << src->getTagKey(i) << " -> " << src->getTagValue(i) << std::endl;
    std::string tg = src->getTagKey(i); tg += ": "; tg += src->getTagValue(i); tg += "\n";
    tags.push_back(tg); // Add to array: Songs tags
    };
    if(src->getTagCount()>0){ tags.push_back("\n"); };
    };

    // Needed to get song length to seconds:
    if(src->isSeekable()){
    src->getFormat(fchannels, frate, fformat);
    std::cout << " " << std::endl;
    std::cout << "Music Info: " << std::endl;
    std::cout << "Channels: " << fchannels << " - Frame Rate: " << frate << " - Format: " << fformat << std::endl;
    };

    std::cout << "Music total frames: " << music->getLength() << std::endl;
    std::cout << "Music path: " << musicPath << std::endl;
    std::cout << " " << std::endl;

    /// Changing from frames, to seconds:
    songFullTime = music->getLength()/frate;      // Retrieves number in seconds, ex: 1756785 --> Music Total Length
    songCurrentTime = music->getPosition()/frate; // Retrieves number in seconds, ex: 1756785 --> Music Position (Now its 0)

    /// Music Scrollbar Controller:
    if(musicPositionScrollbar){
    musicPositionScrollbar->setMin(0);
    musicPositionScrollbar->setMax(music->getLength());
    musicPositionScrollbar->setPos(0);
    musicPositionScrollbar->setSmallStep(music->getLength()/50);
    musicPositionScrollbar->setLargeStep(music->getLength());
    };

    /// if(INTERNALS_SOUNDS){ soundClaps->play(); };

    LoadBarForMusic->setPercent(99); // 0 to 100
    LoadBarForMusic->draw();

    };

    /// Loading Bar:
    LoadBarForMusic->setPercent(100);   // Last One: Must be 100
    LoadBarForMusic->draw();
    LoadBarForMusic->setVisible(false); // Last One: Must hide LoadBar
    delete LoadBarForMusic; // Clear
    LoadBarForMusic = NULL; // Clear

    if(texts[20]!=NULL){ texts[20]->setText(L""); };

    };



    /// ##########################################################################################
    /// ############# AUDIO: (USING AUDIERE LIBRARY) - END #######################################
    /// ##########################################################################################









    /// ------------------------------------------------------------------------------------------

    /// RENDER TEXT TO TEXTURE:

    ITexture* rtt;
    IGUIFont* font_texture;

    void doTextToTexture(scene::ISceneNode* node, stringw text){
    /// Initialize:
    // rtt = driver->addRenderTargetTexture(core::dimension2d<u32>(512, 512), "rtt", ECF_R8G8B8);
    // font_texture = guienv->getFont("font2.bmp"); // Get my text font
    /// Set texture stuff:
    driver->setRenderTarget(rtt, true, true, video::SColor(255,255,255,255));
    driver->draw2DImage(driver->getTexture("plackard.jpg"), position2d<s32>(0,0));
    /// Add custom texts:
    // text += L" - Live";
    font_texture->draw(text.c_str(), core::rect<s32>(0,0,512,512), SColor(255,255,0,0), true, true, 0); // bool hcenter = true, bool vcenter = true
    driver->setRenderTarget(0);
    node->setMaterialTexture(0, rtt); // set material of cube to render target
    };


    /// ------------------------------------------------------------------------------------------
    /// ------------------------------------------------------------------------------------------
    /// ------------------------------------------------------------------------------------------


    /// ANIMATION: Spectators dancing:
    bool anim_forward4 = true;
    f32 ANIMATION_SPEED4 = 0.060f;
    void animation4(IAnimatedMeshSceneNode* node){

    if(SHOW_TYPE == 0){ ANIMATION_SPEED4 = 0.030f; };
    if(SHOW_TYPE == 1){ ANIMATION_SPEED4 = 0.060f; };
    if(SHOW_TYPE == 2){ ANIMATION_SPEED4 = 0.090f; };

    /// Set animation frames limits:
    f32 fstart = 40; // node->getStartFrame();
    f32 fend = node->getEndFrame();
    f32 frame = node->getFrameNr();
    if(SHOW_TYPE == 0){ fstart = 0; fend = 0; frame = 0; }; /// Freezes dancing

    /// Next frame calculations:
    if(frame<=fstart){ frame = fstart; anim_forward4 = true;  };
    if(frame>=fend  ){ frame = fend;   anim_forward4 = false; };
    if(anim_forward4 == true  ){ frame += ANIMATION_SPEED4 * frameDeltaTime; };
    if(anim_forward4 == false ){ frame -= ANIMATION_SPEED4 * frameDeltaTime; };

    /// Apply frame:
    node->setCurrentFrame((f32)frame);
    };

    /// ANIMATION: Spectators clapping
    bool anim_forward5 = true;
    f32 ANIMATION_SPEED5 = 0.185f;
    void animation5(IAnimatedMeshSceneNode* node){

    /// Set animation frames limits:
    f32 fstart = 80; // node->getStartFrame();
    f32 fend = node->getEndFrame();
    f32 frame = node->getFrameNr();

    /// Next frame calculations:
    if(frame<=fstart){ frame = fstart; anim_forward5 = true;  };
    if(frame>=fend  ){ frame = fend;   anim_forward5 = false; };
    if(anim_forward5 == true  ){ frame += ANIMATION_SPEED5 * frameDeltaTime; };
    if(anim_forward5 == false ){ frame -= ANIMATION_SPEED5 * frameDeltaTime; };

    /// Apply frame:
    node->setCurrentFrame((f32)frame);
    };

    /// ANIMATION: Spectators standby animation
    bool anim_forward6 = true;
    f32 ANIMATION_SPEED6 = 0.000f;
    void animation6(IAnimatedMeshSceneNode* node){

    /// Set animation frames limits:
    s32 fstart = node->getStartFrame();
    // s32 fend = node->getEndFrame();
    // s32 frame = node->getFrameNr();

    /*
    /// Next frame calculations:
    if(frame<=fstart){ frame = fstart; anim_forward6 = true;  };
    if(frame>=fend  ){ frame = fend;   anim_forward6 = false; };
    if(anim_forward6 == true  ){  frame += ANIMATION_SPEED6 * frameDeltaTime; };
    if(anim_forward6 == false ){  frame -= ANIMATION_SPEED6 * frameDeltaTime; };
    */

    /// Apply frame:
    node->setCurrentFrame((f32)fstart);
    };

    /// ANIMATION: Spectators hands up to take a photo (NOT USED ATM)
    bool anim_forward8 = true; // reverse animation
    f32 ANIMATION_SPEED8 = 0.185; // 0.200f;
    void animation8(IAnimatedMeshSceneNode* node){

    /// Set animation frames limits:
    // f32 fstart = node->getStartFrame();
    // f32 fend = node->getEndFrame();
    f32 frame = 80; // node->getFrameNr();

    /// Next frame calculations:
    /*
    if(frame<=fstart){ frame = fstart; anim_forward8 = true;  };
    if(frame>=fend  ){ frame = fend;   anim_forward8 = false; };
    if(anim_forward8 == true  ){  frame += ANIMATION_SPEED8 * frameDeltaTime; };
    if(anim_forward8 == false ){  frame -= ANIMATION_SPEED8 * frameDeltaTime; };
    */

    /// Apply frame:
    node->setCurrentFrame((f32)frame);
    };

    /// ANIMATION: BAND standby:
    bool anim_forward_BS = true;
    f32 ANIMATION_SPEED_BS = 0.000f;
    void animation_BS(IAnimatedMeshSceneNode* node){

    /// Set animation frames limits:
    s32 fstart = node->getStartFrame();
    // s32 fend = node->getEndFrame();
    // s32 frame = node->getFrameNr();

    /*
    /// Next frame calculations:
    if(frame<=fstart){ frame = fstart; anim_forward_BS = true;  };
    if(frame>=fend  ){ frame = fend;   anim_forward_BS = false; };
    if(anim_forward_BS == true  ){ frame += ANIMATION_SPEED_BS * frameDeltaTime; };
    if(anim_forward_BS == false ){ frame -= ANIMATION_SPEED_BS * frameDeltaTime; };
    */

    /// Apply frame:
    node->setCurrentFrame((f32)fstart);
    };

    /// ANIMATION: BAND playing:
    bool anim_forward_BP = true;
    f32 ANIMATION_SPEED_BP = 0.050f;
    void animation_BP(IAnimatedMeshSceneNode* node){

    if(SHOW_TYPE == 0){ ANIMATION_SPEED_BP = 0.030f; };
    if(SHOW_TYPE == 1){ ANIMATION_SPEED_BP = 0.050f; };
    if(SHOW_TYPE == 2){ ANIMATION_SPEED_BP = 0.070f; };

    /// Set animation frames limits:
    f32 fstart = node->getStartFrame();
    f32 fend = node->getEndFrame();
    f32 frame = node->getFrameNr();

    /// Next frame calculations:
    if(frame<=fstart){ frame = fstart; anim_forward_BP = true;  };
    if(frame>=fend  ){ frame = fend;   anim_forward_BP = false; };
    if(anim_forward_BP == true  ){ frame += ANIMATION_SPEED_BP * frameDeltaTime; };
    if(anim_forward_BP == false ){ frame -= ANIMATION_SPEED_BP * frameDeltaTime; };

    /// Apply frame:
    node->setCurrentFrame((f32)frame);
    };

    /// ANIMATION: Security:
    f32 ANIMATION_SPEED_SEC = 0.020f;
    void animation_SEC(IAnimatedMeshSceneNode* node){

    /// Set animation frames limits:
    f32 fstart = node->getStartFrame();
    f32 fend   = node->getEndFrame();
    f32 frame  = node->getFrameNr();

    /// Next frame calculations:
    frame += ANIMATION_SPEED_SEC * frameDeltaTime;
    if(frame>fend){ frame = fstart; };

    /// Apply frame:
    node->setCurrentFrame((f32)frame);
    };



    /// ANIMATION: Ballons:
    f32 ANIMATION_ROTSPEED_BALL = 0.035f;
    void animation_BALLOONS_UP(IAnimatedMeshSceneNode* node){

    /// Set new position:
    core::vector3df pos = core::vector3df(node->getPosition().X, node->getPosition().Y + 0.095*frameDeltaTime, node->getPosition().Z);
    // cout << "pos.Y: " << pos.Y << endl;

    /// Set new scale:
    core::vector3df sca = core::vector3df(node->getScale().X + 0.0020*frameDeltaTime, node->getScale().Y + 0.0020*frameDeltaTime, node->getScale().Z + 0.0020*frameDeltaTime);
    if(sca.X >= 1.5){ sca.X = 1.5; }; if(sca.Y >= 1.5){ sca.Y = 1.5; }; if(sca.Z >= 1.5){ sca.Z = 1.5; }; // Max scale allowed

    /// Set animation frames limits:
    f32 fstart = node->getStartFrame();
    f32 fend   = node->getEndFrame();
    f32 frame  = node->getFrameNr();

    /// Next frame calculations:
    frame += ANIMATION_ROTSPEED_BALL * frameDeltaTime;
    if(frame > fend){ frame = fstart; };

    /// Apply frame:
    node->setCurrentFrame((f32)frame);
    node->setPosition(core::vector3df(pos));
    node->setScale(core::vector3df(sca));
    node->setVisible(true);
    };


    /// ANIMATION: Ballons Reset:
    void animation_BALLOONS_RESET(IAnimatedMeshSceneNode *node){
    core::vector3df pos = core::vector3df(0,80,-3);     // RESET to Position relative to spectator
    core::vector3df sca = core::vector3df(0.0,0.0,0.0); // RESET to initial scale
    /// Set animation frames limits:
    f32 fstart = node->getStartFrame();
    /// Apply frame:
    node->setCurrentFrame((f32)fstart);
    node->setPosition(core::vector3df(pos));
    node->setScale(core::vector3df(sca));
    node->setVisible(false);
    };


    /// ------------------------------------------------------------------------------------------
    /// ------------------------------------------------------------------------------------------
    /// ----------------------------------------------------------------------------------	nodeBand->getMaterial(i).setTexture(0, driver->getTexture(material)); // Apply texture to my specified material


    /// LOAD BAND MESHES ON RUNTIME:

    scene::IAnimatedMeshSceneNode* nodeBand = NULL;
    scene::IMeshBuffer* bufferBand = NULL;
    stringw meshName;
    stringw material;
    // bool bandLoaded          = false;
    // bool bandCenteredInStage = false;

    void load_BANDMESH(){

    std::cout << "Band Info:" << std::endl;

    // Clear any previously loaded band
    if(nodeBand){ nodeBand->remove(); nodeBand = NULL; };
    bufferBand = NULL;
    // bandLoaded = false;
    // bandCenteredInStage = false;

	/// LOAD THE BAND:
	if(BAND_SELECTED == 0){ meshName = L"band_man.md3";         material = L"band_man.jpg";         };
	if(BAND_SELECTED == 1){ meshName = L"band_woman.md3";       material = L"band_woman.jpg";       };
	if(BAND_SELECTED == 2){ meshName = L"band_pop.md3";         material = L"band_pop.jpg";         };
	if(BAND_SELECTED == 3){ meshName = L"band_traditional.md3"; material = L"band_traditional.jpg"; };
	if(BAND_SELECTED == 4){ meshName = L"band_rock.md3";        material = L"band_rock.jpg";        };
	if(BAND_SELECTED == 5){ meshName = L"band_dj.md3";          material = L"band_dj.jpg";          };
	if(BAND_SELECTED == 6){ meshName = L"band_drummers.md3";    material = L"band_drummers.jpg";    };
	if(BAND_SELECTED == 7){ meshName = L"band_santa.md3";       material = L"band_santa.jpg";       };

	nodeBand = smgr->addAnimatedMeshSceneNode(smgr->getMesh(meshName));
	bufferBand = smgr->getMesh(meshName)->getMesh(0)->getMeshBuffer(0);
    int vcb = bufferBand->getVertexCount();
    std::cout << "Band vertex count: " << vcb << std::endl;
	nodeBand->setPosition(vector3df(600, 96, -520));  /// Out of stage
	nodeBand->setRotation(vector3df(0, 180, 0));
	nodeBand->setScale(core::vector3df(0.8,0.8,0.8));
	nodeBand->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	nodeBand->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeBand->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeBand->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    nodeBand->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER
    nodeBand->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)



	for(u32 i=0; i<nodeBand->getMaterialCount(); i++){
	nodeBand->getMaterial(i).setTexture(0, driver->getTexture(material)); // Apply texture to my specified material
	nodeBand->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeBand->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeBand->getMaterial(i).getTextureMatrix(1).setTextureScale(0.1f,0.1f); /// Repeat (tile) the texture

    /// Start Colors:
	/// Set size of specular highlights
	nodeBand->getMaterial(i).Lighting = true;
	nodeBand->getMaterial(i).Shininess = 5.0f; /// Reflex type: 20.0f == Metallic
    /// Affect the colour of the reflected light:
    nodeBand->getMaterial(i).SpecularColor = video::SColor(200,20,20,20); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeBand->getMaterial(i).AmbientColor  = video::SColor(200,200,200,200); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    nodeBand->getMaterial(i).DiffuseColor  = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    nodeBand->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeBand->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeBand->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	nodeBand->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
	};

    /// Set animation frames limits:
    f32 framestartb = nodeBand->getStartFrame();
    f32 frameendb = nodeBand->getEndFrame()-1;
    nodeBand->setFrameLoop(framestartb, frameendb);
    nodeBand->setCurrentFrame(framestartb);
    nodeBand->setAnimationSpeed(0);
    nodeBand->setLoopMode(false);
    nodeBand->setVisible(true);
    // nodeBand->addShadowVolumeSceneNode(); /// Activates STENCIL_SHADOWS only on this node (BAND)
    };








    /// ##########################################################################################
    /// ############# LYRICS: READ FILE ##########################################################
    /// ##########################################################################################

    wstring arrayLYRICS[1000][10];     // Declares an array of 1000 rows and 3 columns: <start time> <end time> <lyrics> <code> <code> <code> <code> <code> <code> <code>

    // stringc arrayLYRICS[1000][3];         // Declares an array of 1000 rows and 3 columns: <start time> <end time> <lyrics>
    // std::vector<std::array> arrayLYRICS;  // Array, for songs full path, that will grow...
    bool thisMusicHasLyrics = false;         // Display the (L) alongside music name or not.
    bool lyricsFadeout      = false;
    f32 lirFadeOut          = 255;

    bool file_has_band_setting   = false;
    bool file_has_show_setting   = false;
    bool file_has_color_setting  = false;
    bool file_has_size_setting   = false;
    bool file_has_clock_setting  = false;
    bool file_has_sounds_setting = false;

    // std::vector<wstring> removecodetags;





    void load_LYRICS(std::string lyricsPath){

    if(texts[20]!=NULL){ texts[20]->setText(L"Please Standby\nThe show will continue in a moment..."); };
    CAMERA_SELECTED = 1;
    fulllines = L"";

    /// Temporary disable device VSYNC, else will take loooong time to read the text files:
    // device->getDevice

    // Clear items (if already exist) in lyrics array:
    for(int r = 0; r<1000; r++){
    arrayLYRICS[r][0] = L""; // Start time
    arrayLYRICS[r][1] = L""; // End time
    arrayLYRICS[r][2] = L""; // lines of lyrics text
    };


    // Reset Color:
    /// FONT_COLOR_RED = 255; FONT_COLOR_GREEN = 255; FONT_COLOR_BLUE = 255;
    // Reset Lyrics:
    thisMusicHasLyrics = false;
    // reset full texts from file:
    fullLoadedTexts = L"";
    // reset:
    file_has_band_setting   = false;
    file_has_show_setting   = false;
    file_has_color_setting  = false;
    file_has_size_setting   = false;
    file_has_clock_setting  = false;
    file_has_sounds_setting = false;

    // removecodetags.clear();

    /// Initializing some vars...
    // std::vector<wstring> fullLoadedTexts; // The holder for the file full content
    wstring joiningTextLines = L"";
    std::locale defaultLocale(""); // Allowing special characters
    std::string foundFileFormat = "";

    /// Remove any file extension that path may have:
    size_t lastindex1 = lyricsPath.find_last_of(".");
    std::string LP1 = lyricsPath.substr(0, lastindex1);

    /// TXT exists?...
    std::string ext1 = (LP1 += ".txt");
    std::wfstream infile1(ext1.c_str()); /// <-- NOT ifstream !!!
    infile1.imbue(defaultLocale); // imbue must be done before opening
    wstring line1;
    if(infile1.is_open()){
    fullLoadedTexts = L"";
    while(std::getline(infile1, line1)){ fullLoadedTexts+=(line1+L'\n'); /* std::wcout << line1 << std::endl; */ }; foundFileFormat = ".txt"; };
    if(infile1.is_open()){ infile1.close(); };

    /// Remove any file extension that path may have:
    size_t lastindex2 = lyricsPath.find_last_of(".");
    std::string LP2 = lyricsPath.substr(0, lastindex2);

    /// SRT exists?...
    std::string ext2 = (LP2 += ".srt");
    std::wfstream infile2(ext2.c_str()); /// <-- NOT ifstream !!!
    infile2.imbue(defaultLocale); // imbue must be done before opening
    wstring line2;
    if(infile2.is_open()){
    fullLoadedTexts = L"";
    while(std::getline(infile2, line2)){ fullLoadedTexts+=(line2+L'\n'); /* std::wcout << line2 << std::endl; */ }; foundFileFormat = ".srt"; };
    if(infile2.is_open()){ infile2.close(); };

    std::cout << std::endl;
    std::cout << "Lyrics path and format: " << lyricsPath.c_str() << foundFileFormat << std::endl << std::endl;

    /// Prepare the render with a GREYSCALE effect:
    driver->beginScene(true, true, 0);
    /// driver->beginScene(false, false, 0);
    texts[25]->setText(L"Loading Lyrics...");
    texts[7]->setText(L"");
    if(ppEffects[0]){ ppEffects[0]->render(NULL); }
    driver->setMaterial(driver->getMaterial2D()); // In case buffer is distorted
    guienv->drawAll();  /// Draw GUI
    drawMoreStuff();
    driver->endScene();
    texts[25]->setText(L"");
    texts[7]->setText(L"");

    /// Take screenshot and convert to texture:
    screenshot = driver->createScreenShot();
    if(screenshot!=NULL){ /// Voiding crashes, when menu window is hidden
    if(backgroundImageTexture != NULL){ driver->removeTexture(backgroundImageTexture); backgroundImageTexture = NULL; };
    if(backgroundImageTexture == NULL){ backgroundImageTexture = driver->addTexture("screenshot", screenshot); };
    screenshot->drop();
    screenshot = NULL;
    };

    /// Loading Bar for Lyrics Only: #1 (Initialize)
	LoadBarForLyrics = new CLoadingBar(device, driver, guienv, backgroundImageTexture, rect<s32>(300, SCREEN_HEIGHT-50, SCREEN_WIDTH-300, SCREEN_HEIGHT-50+15), true, 7);  // posX, posY, bar width, bar height
    LoadBarForLyrics->setPercent(10); // 0 to 100
    LoadBarForLyrics->draw();
    // LoadBarForLyrics->setVisible(true); // for the last "LoadBar" only




    /// (TXT and SRT) Parsing timings and texts lyrics...
    std::wcout << "Parsing lyrics file... ";
    // std::wcout << "1st) Removing everything until find first time line..." << std::endl << std::endl;
    wstringstream prestream(fullLoadedTexts);
    wstring preline = L"";
    wstring clearing = L"";
    bool gotFirstTime = false;
    /// Removes everything until find first time line:
    while(getline(prestream, preline)){
    if(preline.size()>=29 && preline.find_first_not_of(L"0123456789") != std::wstring::npos && preline.find(L"-->") != std::wstring::npos && preline.find(L":") != std::wstring::npos){
    gotFirstTime = true; /// Found the first time line!
    };
    if(gotFirstTime==true && !isNumber(preline)){ clearing += (preline+L'\n'); };
    };

    // std::wcout << clearing << std::endl;

    // std::wcout << "2nd) Store times and lyrics..." << std::endl << std::endl;
    wstringstream lstream(clearing);
    wstring lline = L"";
    linesCounter = 0; /// Reset!
    while(std::getline(lstream, lline) && linesCounter<1000) {
    /// Got a time line?... Store it...
    if(lline.size()>=29 && lline.find_first_not_of(L"0123456789") != wstring::npos && lline.find(L"-->") != std::wstring::npos && lline.find(L":") != std::wstring::npos){
    lline.erase(std::remove(lline.begin(), lline.end(), '\n'), lline.end()); /// Remove ALL line breaks!
    lline.erase(std::remove(lline.begin(), lline.end(), '\r'), lline.end()); /// Remove ALL line breaks!
    lline.erase(std::remove(lline.begin(), lline.end(),  ' '), lline.end()); /// Remove ALL line spaces!
    if(lline.size()>=27){
    arrayLYRICS[linesCounter][0] = lline.substr(0,  lline.find(L"-->")-4);  /// Start time! -4 is to avoid decimals! (For now...)
    arrayLYRICS[linesCounter][1] = lline.substr(15, lline.size()-19);       /// End time! -21 is to avoid decimals!  (For now...)
    linesCounter++;
    };
    } else {
    /// Got a lyrics line?... Store it...
    if(linesCounter>0){ arrayLYRICS[linesCounter-1][2] += (lline+L'\n'); };
    };
    };
    /// Removes the last "\n", since will be duplicated... Except in the last line!
    for(int i=0; i<linesCounter-1; i++){
    wstring temp = arrayLYRICS[i][2];
    wstring withoutlastchar = temp.substr(0, temp.size()-1);
    arrayLYRICS[i][2] = withoutlastchar;
    };





    std::wcout << "Done!" << std::endl << std::endl;


    std::wcout << "Parsed lyrics from file:" << std::endl << std::endl;
    for(int i=0; i<linesCounter; i++){
    std::wcout << arrayLYRICS[i][0] << "-" << arrayLYRICS[i][1] << "-" << arrayLYRICS[i][2] << std::endl;
    };



    LoadBarForLyrics->setPercent(20); // 0 to 100
    LoadBarForLyrics->draw();



    /// (TXT and SRT) Parsing code in lyrics...
    std::wcout << std::endl;
    std::wcout << "Parsing code from lyrics file... ";
    wstringstream cstream(fullLoadedTexts);
    wstring cline = L"";

    while(getline(cstream, cline)) {

    std::wstring code = L"";
    code = cline;
    code = convertToUpperInWString(code);   // All to Caps
    code = removeCaracteresInWString(code); // Remove all the Spaces

    /// A Band was set?...
    if(code == L"<BAND:MAN>"          ){ BAND_STYLE = L"Man";         file_has_band_setting = true; };
    if(code == L"<BAND:WOMAN>"        ){ BAND_STYLE = L"Woman";       file_has_band_setting = true; };
    if(code == L"<BAND:POP>"          ){ BAND_STYLE = L"Pop";         file_has_band_setting = true; };
    if(code == L"<BAND:TRADITIONAL>"  ){ BAND_STYLE = L"Traditional"; file_has_band_setting = true; };
    if(code == L"<BAND:ROCK>"         ){ BAND_STYLE = L"Rock";        file_has_band_setting = true; };
    if(code == L"<BAND:DJ>"           ){ BAND_STYLE = L"Dj";          file_has_band_setting = true; };
    if(code == L"<BAND:DRUMMERS>"     ){ BAND_STYLE = L"Drummers";    file_has_band_setting = true; };
    if(code == L"<BAND:SANTA>"        ){ BAND_STYLE = L"Santa";       file_has_band_setting = true; };

    /// A Show Type was set?...
    if(code == L"<SHOW:SLOW>"  ){ SHOW_TYPE = 0; file_has_show_setting = true; };
    if(code == L"<SHOW:NORMAL>"){ SHOW_TYPE = 1; file_has_show_setting = true; };
    if(code == L"<SHOW:FAST>"  ){ SHOW_TYPE = 2; file_has_show_setting = true; };

    /// A Color was set?...
    if(code == L"<LYRICSCOLOR:WHITE>"  ){ FONT_COLOR_SELECTED = 0; file_has_color_setting = true; };
    if(code == L"<LYRICSCOLOR:RED>"    ){ FONT_COLOR_SELECTED = 1; file_has_color_setting = true; };
    if(code == L"<LYRICSCOLOR:GREEN>"  ){ FONT_COLOR_SELECTED = 2; file_has_color_setting = true; };
    if(code == L"<LYRICSCOLOR:BLUE>"   ){ FONT_COLOR_SELECTED = 3; file_has_color_setting = true; };
    if(code == L"<LYRICSCOLOR:YELLOW>" ){ FONT_COLOR_SELECTED = 4; file_has_color_setting = true; };
    if(code == L"<LYRICSCOLOR:PURPLE>" ){ FONT_COLOR_SELECTED = 5; file_has_color_setting = true; };

    /// A Size was set?...
    if(code == L"<LYRICSSIZE:1>"){ FONT_SIZE = L"1"; FONT_SIZE_SELECTED = 1; file_has_size_setting = true; };
    if(code == L"<LYRICSSIZE:2>"){ FONT_SIZE = L"2"; FONT_SIZE_SELECTED = 2; file_has_size_setting = true; };
    if(code == L"<LYRICSSIZE:3>"){ FONT_SIZE = L"3"; FONT_SIZE_SELECTED = 3; file_has_size_setting = true; };
    if(code == L"<LYRICSSIZE:4>"){ FONT_SIZE = L"4"; FONT_SIZE_SELECTED = 4; file_has_size_setting = true; };
    if(code == L"<LYRICSSIZE:5>"){ FONT_SIZE = L"5"; FONT_SIZE_SELECTED = 5; file_has_size_setting = true; };
    if(code == L"<LYRICSSIZE:6>"){ FONT_SIZE = L"6"; FONT_SIZE_SELECTED = 6; file_has_size_setting = true; };

    /// Clock was set?...
    if(code == L"<CLOCK:ON>" ){ CLOCK_VISIBLE = true;  file_has_clock_setting = true; };
    if(code == L"<CLOCK:OFF>"){ CLOCK_VISIBLE = false; file_has_clock_setting = true; };

    /// Sounds were set?...
    if(code == L"<SOUNDS:ON>" ){ INTERNALS_SOUNDS = true;  file_has_sounds_setting = true; };
    if(code == L"<SOUNDS:OFF>"){ INTERNALS_SOUNDS = false; file_has_sounds_setting = true; };

    }; // End: string was fully parsed.

    std::wcout << "Done!" << std::endl << std::endl;


    /// Set band:
    if(BAND_STYLE == L"Man"         ){ BAND_SELECTED = 0; };
    if(BAND_STYLE == L"Woman"       ){ BAND_SELECTED = 1; };
    if(BAND_STYLE == L"Pop"         ){ BAND_SELECTED = 2; };
    if(BAND_STYLE == L"Traditional" ){ BAND_SELECTED = 3; };
    if(BAND_STYLE == L"Rock"        ){ BAND_SELECTED = 4; };
    if(BAND_STYLE == L"Dj"          ){ BAND_SELECTED = 5; };
    if(BAND_STYLE == L"Drummers"    ){ BAND_SELECTED = 6; };
    if(BAND_STYLE == L"Santa"       ){ BAND_SELECTED = 7; };


    LoadBarForLyrics->setPercent(40); // 0 to 100
    LoadBarForLyrics->draw();
    /// float perc = 15;

    thisMusicHasLyrics = true;
    // infile.close();

    /// Loading Bar:
    LoadBarForLyrics->setPercent(60);   // Last One: Must be 100
    LoadBarForLyrics->draw();
    LoadBarForLyrics->setVisible(false); // Last One: Must hide LoadBar
    delete LoadBarForLyrics; // Clear
    LoadBarForLyrics = NULL; // Clear


    /// Fadeout from black...
    /*
    gui::IGUIInOutFader* fader = device->getGUIEnvironment()->addInOutFader();
    fader->setColor(video::SColor(0,0,0,0));	nodeStageFloor->getMaterial(i).setTexture(0, driver->getTexture("stage_floor.jpg"));
	nodeStageFloor->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeStageFloor->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeStageFloor->getMaterial(i).getTextureMatrix(1).setTextureScale(0.1f,0.1f); /// Repeat (tile) the texture
    fader->fadeIn(5);
    */


    /// There are settings set in the lyrics file?... Then change these texts colors to blue:
    if(file_has_band_setting   ){ texts[14]->setOverrideColor(video::SColor(255,150,190,255)); } else { texts[14]->setOverrideColor(video::SColor(255,210,235,255)); }; // opacity, red, green, blue
    if(file_has_show_setting   ){ texts[22]->setOverrideColor(video::SColor(255,150,190,255)); } else { texts[22]->setOverrideColor(video::SColor(255,210,235,255)); }; // opacity, red, green, blue
    if(file_has_color_setting  ){ texts[19]->setOverrideColor(video::SColor(255,150,190,255)); } else { texts[19]->setOverrideColor(video::SColor(255,210,235,255)); }; // opacity, red, green, blue
    if(file_has_size_setting   ){ texts[18]->setOverrideColor(video::SColor(255,150,190,255)); } else { texts[18]->setOverrideColor(video::SColor(255,210,235,255)); }; // opacity, red, green, blue
    if(file_has_clock_setting  ){ texts[24]->setOverrideColor(video::SColor(255,150,190,255)); } else { texts[24]->setOverrideColor(video::SColor(255,210,235,255)); }; // opacity, red, green, blue
    if(file_has_sounds_setting ){ texts[23]->setOverrideColor(video::SColor(255,150,190,255)); } else { texts[23]->setOverrideColor(video::SColor(255,210,235,255)); }; // opacity, red, green, blue

    /// Default texts:
    if(texts[20]!=NULL){ texts[20]->setText(L""); };

    /// Default ambient Light:
    smgr->setAmbientLight(video::SColorf(0.50,0.50,0.50,0.0)); // Ambient Light. SColorf == r,g,b,a

    /// Default Animated light: ( Change color to white )
    data_ML1.AmbientColor  = video::SColorf(1.0,1.0,1.0,0.0);
    data_ML1.SpecularColor = video::SColorf(1.0,1.0,1.0,0.0);
    data_ML1.DiffuseColor  = video::SColorf(1.0,1.0,1.0,0.0);
    light_ML1->setLightData(data_ML1); // Apply data to the light

    /// Default strobe light:
    light_FL1->setVisible(false);

    /// Loading Band...
    load_BANDMESH(); /// Loading Band...

    };


    /// ##########################################################################################
    /// ############# LYRICS: READ FILE - END ####################################################
    /// ##########################################################################################







    /// ##########################################################################################
    /// ############# RETRIEVE MUSIC + LYRICS FILES, FROM FOLDER #################################
    /// ##########################################################################################




    void findMusicsAndLyrics(){

    /// Notify user:
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Searching for music and lyrics files in directory \"" << fileopendialog_path.c_str() << "\"" << std::endl;

    // Reset vectors:
    musicList.clear();
    musicListNamesOnly.clear();
    musicExtensionsOnly.clear();
    musicLyricsListPath.clear();

    // std::string ft[23] = {"ogg","wav","flac","aiff","au","raw","paf","svx","nist","voc","ircam","w64","mat4","mat5","pvf","htk","sds","avr","sd2","caf","wve","mpc2k","rf64"}; // SMFL - Custom Array (Supported Audiere file formats)
    // std::string ft[9] = {"wav","aiff","ogg","flac","mp3","mod","s3m","it","xm"}; // Audiere - Custom Array (Supported Audiere file formats)
    std::string ft[11] = {"wav","aiff","ogg","flac","mp3","mod","s3m","it","xm","mid","kar"}; // Array with all supported Audiere audio file formats
    int ftz = 11; // Custom Array total size
    // cout << "Music File Formats: " << ftz << "\n" << endl;

    /// dir = opendir("music"); // Path to the music directory
    dir = opendir(fileopendialog_path.c_str()); // Path to the music directory

    if(dir != NULL){
    while((ent = readdir(dir)) != NULL){

    for(int i=0; i<ftz; i++){
    std::string fn;
    fn = std::string(ent->d_name);
    std::string fext;
    fext = fn.substr(fn.find_last_of(".")+1); // Get file extension from found file

    if(fext == std::string(ft[i])){           // Has this file an valid audio extension?
    // Adding song (full path) to array:
    /// fileopendialog_path --> was previously defined at myEvents.h !
    std::string fullPath = fileopendialog_path; fullPath += ent->d_name;
    /// std::string fullPath = "music/"; fullPath += ent->d_name;
    musicList.push_back(fullPath);  // Add to array: Songs full path

    musicExtensionsOnly.push_back(convertToUpperInString(fext)); // Add to array: Songs extension only, to upper case

    // Adding song (song name) to array:
    std::string sn;
    sn = ent->d_name;                        // Get file name
    int start = sn.find(fext);               // Remove file extension
    int len = std::string(fext).size();      // Remove file extension
    sn.replace(start-1, len+1, "");          /// Remove the MUSIC file extension
    musicListNamesOnly.push_back(sn);        // Now, just add to array: Songs name only (withouth file extension)

    // Adding lyrics (full path) to array:
    std::string ly;
    ly = fullPath;                        // Get file name
    int starty = ly.find(fext);           // Remove file extension
    int leny = std::string(fext).size();  // Remove file extension
    ly.replace(starty-1, leny+1, "");     /// Remove any file extension TXT
    musicLyricsListPath.push_back(ly);    // Now, just add to array: Lyrics file, with full path

    } else {
    // printf("Skipped file: %s\n", ent->d_name);
    };
    };
    };

    closedir(dir);

    } else { }; // Error!...

    std::cout << "------------------------------------------------------------------------------" << std::endl;
    for(unsigned int i=0; i<musicList.size(); i++) { std::cout << i+1 << ") Music files: " << musicList[i]  << std::endl; }; // Write log
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    for(unsigned int i=0; i<musicLyricsListPath.size(); i++) { std::cout << i+1 << ") Lyrics files: " << musicLyricsListPath[i] << std::endl; }; // Write log
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Search for musics and lyrics... Done" << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;

    };


    /// ##########################################################################################
    /// ############# RETRIEVE MUSIC + LYRICS FILES, FROM FOLDER - END ###########################
    /// ##########################################################################################






    /// ##########################################################################################
    /// ############# LISTBOX: FILL ##############################################################
    /// ##########################################################################################


    void fillListBox(){
    findMusicsAndLyrics(); /// First, lets call the above function to fill all the next needed arrays.
    // Reset:
    if(musicListBox) { musicListBox->clear(); };
    // Ok, we may go now...
    if(musicListBox && musicList.size()>0){ // void nulls, else crashes!
    /// Add buttons, one for each found song: (loop)
    int mc = musicList.size()-1;   // Get total songs count
    // int sPos = 185;             // For Buttons starting Y position
    // IGUIButton* button[10]={};  // For Buttons
    for(int i=0; i<=mc; i++){
    stringw songPath = musicList[i].c_str(); // eg: stringw text("Hello");
    stringw songNameOnly = musicListNamesOnly[i].c_str();
    stringw songExtensionOnly = musicExtensionsOnly[i].c_str();
    /// stringw allWords = L""; allWords += L"("; allWords += songExtensionOnly; allWords += L") "; allWords += songNameOnly;
    /// stringw allWords = L""; allWords += i+1; allWords += L") "; allWords += songNameOnly;
    stringw allWords = L""; allWords += songNameOnly;
    musicListBox->addItem(allWords.c_str());
    // button[i] = guienv->addButton(rect<s32>(position2di(10,sPos), dimension2di(155,20)), win, -1, allWords.c_str()); // For Buttons
    // button[i]->setID(i+100); // For Buttons -> set IDs, IDs counting from 100, in this case
    // sPos += 20; // For Buttons
    };
    if(musicList.size()-1 >= 0){
    musicListBox->setSelected(0);  // Auto select first item from ListBox
    /// -----------------------------------------------------------------------------------
    /// Coloring ListBox:
    int c = musicListBox->getItemCount();
    for(int i=0; i<c; i++){ musicListBox->setItemOverrideColor(i, video::SColor(200,150,150,200)); }; // Colors to default
    musicListBox->setItemOverrideColor(musicListBox->getSelected(), video::SColor(255,210,235,255)); // Color for selected item
    /// -----------------------------------------------------------------------------------
    };
    };
    };

    /// ##########################################################################################
    /// ############# LISTBOX: FILL - END ########################################################
    /// ##########################################################################################









    /// ##########################################################################################
    /// ############# SPECTRUM: (USING KissFFT LIBRARY) ##########################################
    /// ##########################################################################################

    /*
    static double xAxis = 0;
    static int sampleCount = 0;
    static double pieSteps;
    static double fullSinWave = 3.14159265*2;
    static double sampleRate = 44100;
    static double wantedHz = 0;
    int octiveOffset;
    int dataStream1[1024] = {0};
    kiss_fft_cpx dataStream2[1024] = {0};


    void test_FFT() {

    // for (int i=0; i<1024; i++){ dataStream1[i].i = dataStream1[i].r = 0; };

    for (int i=0; i<1024; i++) {
    wantedHz = 1000;
    pieSteps = fullSinWave/(sampleRate/wantedHz);
    xAxis += pieSteps;
    dataStream1[i] = (int)(sin(xAxis)*32768.0);
    // std::cout << "data: " << dataStream1[j] << std::endl;
    };

    kiss_fft_cfg mycfg = kiss_fft_alloc(1024, 0, NULL, NULL);
    kiss_fft_cpx* in_buf  = (kiss_fft_cpx*)malloc(sizeof(kiss_fft_cpx)*1024);
    kiss_fft_cpx* out_buf = (kiss_fft_cpx*)malloc(sizeof(kiss_fft_cpx)*1024);
    for (int i=0; i<1024; i++){
    in_buf[i].r = dataStream1[i];
    in_buf[i].i = dataStream1[i];
    };

    kiss_fft(mycfg, in_buf, out_buf);

    for (int i=0; i<256; i++){
    int ix = out_buf[i].i;
    int rx = out_buf[i].r;
    // printfbar(sqrt(ix*ix+rx*rx)););
    std::string mybar = "";
    for(int i=0; i<((sqrt(ix*ix+rx*rx))/2000); i++){ mybar += "*"; };
    /// std::cout << "Bar: " << mybar << '\r';
    std::cout << "Bar: " << mybar << std::endl;
    /// usleep(100);
    };

    };
    */



    /*
    int file_channels = 0;
    int file_rate = 0;
    int file_number_of_frames = 0; /// ?...
    audiere::SampleFormat file_format;
    audiere::SampleSource* source;
    size_t done = 0;

    int dataStream1[1024] = {0};
    */



    void testing_KissFFT() {


    /*

    std::string pa = "C://BACKUPS - IMPORTANTES - 01/- IRRLICHT/Irrlicht-Music-Player-V.1.5.006/bin/music/aateste.wav";
    source = audiere::OpenSampleSource(pa.c_str());



    /// short int* buffer = (short int*)malloc(source->getLength());
    // Get 2048 samples
    // const int TIME = 2048;
    unsigned char* buffer = new unsigned char[FFT_LEN];

    source->getFormat(file_channels, file_rate, file_format);
    SampleBuffer* sample_buffer = CreateSampleBuffer(buffer, file_number_of_frames, file_channels, file_rate, SF_ML16);

    // SampleSource* sample_source = audiereAudioDevice.get().openBuffer();
    // unsigned char* buffer = audiereAudioDevice->openBuffer();

    // unsigned char* buffer = CreateSampleBuffer(buffer, file_number_of_frames, file_channels, file_rate, SF_ML16);







    char *music_file = "C:/BACKUPS - IMPORTANTES - 01/- IRRLICHT/Irrlicht-Music-Player-V.1.5.006/bin/music/Gentle Moments.mp3";
    //int          readcount;

    short buf[WIN * 2];
    int nfft = WIN;
    double intensity = 0;
    kiss_fft_cfg cfg;
    kiss_fft_cpx cx_in[WIN];
    kiss_fft_cpx cx_out[WIN];
    short *sh;

    cfg = kiss_fft_alloc(nfft, 0, 0, 0);


    if (sf_open(music_file, SFM_READ, &sfinfo))
    {   // Open failed so print an error message.
        printf("Not able to open input file %s.\n", "input.wav");
        // Print the error message fron libsndfile.
        sf_perror(NULL);
        return  1;
    }

    while ((sf_read_short(infile, buf, WIN)))//fread(buf, 1, WIN * 2, in)
    {
        //system("cls");

        for (int i = 0;i<WIN;i++) {
            sh = (short *)&buf[i * 2];
            cx_in[i].r = (float) (((double)*sh) / 32768.0);
            cx_in[i].i = 0.0;
        }

        kiss_fft(cfg, cx_in, cx_out);
        //Display the value of a position
        int position = 511;
        intensity = sqrt(pow(cx_out[position].r, 2) + pow(cx_out[position].i, 2));
        printf("%9.4f\n", intensity);

        //Display all values
        for (i = 0;i<WIN;i++) {
            //printf("Joe: cx_out[i].r:%f\n", cx_out[i].r);
            //printf("Joe: cx_out[i].i:%f\n", cx_out[i].i);
            intensity = sqrt(pow(cx_out[i].r,2) + pow(cx_out[i].i,2));
            printf("%d - %9.4f\n", i, intensity);
        }

    }
    sf_close(infile);
    free(cfg);
    int temp;
    scanf_s("%d", &temp);

    */





    /*
    for (int i=0; i<FFT_LEN; i++) {
    std::cout << "L BUFFER: " << buffer[i] << std::endl;
    };
    */



    /*

    // 16-bit integer encoded in bytes, hence x2 size

     // Get 1024 samples
    const int TIME = 1024;


    short* samples = new short[done/2];
    int index = 0;

    // Iterate 2 bytes at a time
    for (int i=0; i<done; i+=2) {
    unsigned char first  = buffer[i];
    unsigned char second = buffer[i+1];
    samples[index++] = (first|(second<<8));
    };


    std::cout << "L BUFFER: " << buffer[0] << " - " << "R BUFFER: " << buffer[0+1] << std::endl;






    kiss_fft_cfg ComplexCfg;

    float        RealIn[FFT_LEN];
    kiss_fft_cpx ComplexIn[FFT_LEN];

    kiss_fft_cpx RealOut[FFT_LEN];
    kiss_fft_cpx ComplexOut[FFT_LEN];

    ComplexCfg = kiss_fft_alloc(FFT_LEN, 0, 0, 0);

    for (int i=0;i<FFT_LEN;i++) {
    RealIn[i]=cos( NUM*i*2*M_PI/FFT_LEN);
    ComplexIn[i].r=RealIn[i];
    ComplexIn[i].i=0;
    };

    kiss_fft(ComplexCfg, ComplexIn, ComplexOut);

    std::cout.precision(0);
    cout << "Real values:    " << std::fixed;
    for (int i=0;i<16;i++){ cout << sqrt(SQ(RealOut[i].r)+SQ(RealOut[i].i)) << " "; };
    cout << endl;

    cout << "Complex values: ";
    for (int i=0;i<16;i++){ cout << sqrt(SQ(ComplexOut[i].r)+SQ(ComplexOut[i].i)) << " "; };
    cout << endl;

    float* output = new float[FFT_LEN/2+1];

    for (int i=0; i<(FFT_LEN/2+1); i++) {
    float re = ComplexOut[i].r * FFT_LEN;
    float im = ComplexOut[i].i * FFT_LEN;
    output[i] = sqrtf(re*re + im*im);
    };


    for (int i=0; i<200; i++){ cout << output[i] << endl; };
    */







    /*

    // Array to store the calculated data
    int speclen = TIME/2+1;
    float* output = new float[speclen];

    // kiss_fftr_cfg config;
    kiss_fft_cfg config = kiss_fft_alloc(TIME, 0, NULL, NULL);
    /// kiss_fft_cpx* spectrum = (kiss_fft_cpx*) malloc(sizeof(kiss_fft_cpx) * TIME);


    kiss_fft_cpx* in_buf  = (kiss_fft_cpx*)malloc(sizeof(kiss_fft_cpx)*TIME);
    kiss_fft_cpx* out_buf = (kiss_fft_cpx*)malloc(sizeof(kiss_fft_cpx)*TIME);


    for (int i=0; i<TIME; i++){
    in_buf[i].r = buffer[i];
    in_buf[i].i = buffer[i];
    std::cout << "BUFFER: " << in_buf[i].r << std::endl;
    };


    kiss_fft(config, in_buf, out_buf);

    for (int i=0; i<256; i++){
    int ix = out_buf[i].i;
    int rx = out_buf[i].r;
    // printfbar(sqrt(ix*ix+rx*rx)););
    std::string mybar = "";
    for(int i=0; i<((sqrt(ix*ix+rx*rx))/2000); i++){ mybar += "*"; };
    /// std::cout << "Bar: " << mybar << '\r';
    std::cout << "Bar: " << mybar << std::endl;
    /// usleep(100);
    };
    */

    /*
    // Right here...
    kiss_fft(config, (kiss_fft_cpx*)samples, spectrum);
    for (int i=0; i<speclen; i++) {
    float re = scale(spectrum[i].r) * TIME;
    float im = scale(spectrum[i].i) * TIME;
    output[i] = sqrtf(re*re + im*im);
    };

    for (int i=0; i<256; i++){
    int ix = spectrum[i].i;
    int rx = spectrum[i].r;
    // printfbar(sqrt(ix*ix+rx*rx)););
    std::string mybar = "";
    for(int i=0; i<((sqrt(ix*ix+rx*rx))/2000); i++){ mybar += "*"; };
    /// std::cout << "Bar: " << mybar << '\r';
    std::cout << "Bar: " << mybar << std::endl;
    /// usleep(100);
    };
    */


    };




    /// ##########################################################################################
    /// ############# SPECTRUM: (USING KissFFT LIBRARY) ##########################################
    /// ##########################################################################################






/// Main:
int main(int argc, char** argv){


#include "init.h" // contains c++ code for initialization



    /// Loading Bar: #10
    LoadBar->setPercent(90); // 0 to 100
    LoadBar->draw();
    // LoadBar->setVisible(false);


    int gpcd = driver->getPrimitiveCountDrawn();
    std::cout << "Primitives count: " << gpcd << std::endl;


    /// Loading Bar: #11
    LoadBar->setPercent(100);   // Last One: Must be 100
    LoadBar->draw();
    LoadBar->setVisible(false); // Last One: Must hide LoadBar
    delete LoadBar; // Clear
    LoadBar = NULL; // Clear

    if(INTERNALS_SOUNDS){ soundIntro->play(); };  // Audiere

    std::cout << "IMP Initialized... Ok" << std::endl;




    /// UNDER DEVELOPMENT YET: *******************************************************************


    /*
    irr::io::IReadFile* file = device->getFileSystem()->createAndOpenFile("music/An Intro.ogg");

    // Get the file size, so we can pre-allocate the buffer
    long length = file->getSize();

    cout << "FILE: " << length << endl;

    char* bufferKiss2 = new char[length]; // (When reading  ascii-files instead of binary files you would use length+1 and add a 0 at the end)
    file->read(bufferKiss2, length);

    // At this point you have the whole file in memory in the buf variable - do something with it!

    // cleanup
    delete[] bufferKiss2;
    file->drop();
    */

    // -------------------------------------------------------------------------------------------

    /*
    kiss_fft_cpx in[N], out[N];
    size_t i;

    for (i = 0; i < N; i++){ in[i].r = in[i].i = 0; };
    TestFft("Zeroes (complex)", in, out);

    for (i = 0; i < N; i++){ in[i].r = 1, in[i].i = 0; };
    TestFft("Ones (complex)", in, out);

    for (i = 0; i < N; i++){ in[i].r = sin(2 * M_PI * 4 * i / N), in[i].i = 0; };
    TestFft("SineWave (complex)", in, out);

    return 0;
    */


    /*
    static double xAxis       = 0;
    static int sampleCount    = 0;
    static double pieSteps;
    static double fullSinWave = 3.14159265*2;
    static double sampleRate  = 44100;
    static double wantedHz    = 12*44100.0/512;
    int octiveOffset;
    char * globalString = ""; // stringToSend;
    int16_t dataStream[512];

    int ix = 0;
    int rx = 0;
    */

    /// http://stackoverflow.com/questions/1679974/converting-an-fft-to-a-spectogram/10643179

    /*

    KissFFT in an "image":

    mag
     /\
     |
     |      !         !
     |      !    !    !
     +--!---!----!----!---!--> freq
     0          Fs/2      Fs

     */


    /// ******************************************************************************************


    /*
    char *music_file = "C:/BACKUPS - IMPORTANTES - 01/- IRRLICHT/Irrlicht-Music-Player-V.1.5.006/bin/music/aateste.wav";
    std::string musicPath = "C:/BACKUPS - IMPORTANTES - 01/- IRRLICHT/Irrlicht-Music-Player-V.1.5.006/bin/music/aateste.wav";
    // music = audiere::OpenSound(audiereAudioDevice.get(), musicPath.c_str(), true); // Load music into memory? true / false

    // mpg123_handle *m = NULL;
    // int  channels = 0, encoding = 0;
    // long rate = 0;
    // int err = MPG123_OK;

    // err = mpg123_init();
    // m = mpg123_new(NULL, &err);
    // mpg123_open(m, "L:\\audio-io\\audio-analysis\\samples\\zero.mp3");
    // mpg123_getformat(m, &rate, &channels, &encoding);

    // err = mpg123_format_none(m);
    // err = mpg123_format(m, rate, channels, encoding);

    // Get 2048 samples
    const int TIME = 2048;
    unsigned char* buffer = new unsigned char[TIME*2];


    std::string pa = "C:/BACKUPS - IMPORTANTES - 01/- IRRLICHT/Irrlicht-Music-Player-V.1.5.006/bin/music/aateste.wav";

    audiere::OutputStreamPtr musica = audiere::OpenSound(audiereAudioDevice.get(), pa.c_str(), true); // Load music into memory? true / false
    musica->play();

    // 16-bit integer encoded in bytes, hence x2 size

    // size_t done = 0;
    // err = mpg123_read(m, buffer, TIME*2, &done);

    /// Load .OGG sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileSPEC = device->getFileSystem()->createAndOpenFile("audio_applause.ogg");
    void* bufferSPEC = malloc(readFileSPEC->getSize());
    readFileSPEC->read(bufferSPEC, readFileSPEC->getSize());
    // Create memory file:
    FilePtr targetFileSPEC = CreateMemoryFile(bufferSPEC, readFileSPEC->getSize()); // targetFile22 is the file (path) that we want !!!
    /// free(bufferSPEC);
    /// readFileSPEC->drop();


    audiere::OutputStreamPtr audioSPEC = NULL;
    audioSPEC = audiere::OpenSound(audiereAudioDevice.get(), targetFileSPEC, true); // Load music into memory? true / false
    audioSPEC->setVolume(GLOBAL_VOLUME/2);
    audioSPEC->setPan(0);    // Left, 1 Right
    audioSPEC->setRepeat(0); // 1 loop, 0 don't loop
    audioSPEC->play();






    short* samples = new short[musica->getSize()/2];
    int index = 0;

    // Iterate 2 bytes at a time
    for (int i = 0; i < musica->getSize(); i += 2) {
        unsigned char first = buffer[i];
        unsigned char second = buffer[i + 1];
        samples[index++] = (first | (second << 8));
    }

    // Array to store the calculated data
    int speclen = TIME / 2 + 1;
    float* output = new float[speclen];

    kiss_fftr_cfg config;
    kiss_fft_cpx* spectrum;

    config = kiss_fftr_alloc(TIME, 0, NULL, NULL);
    spectrum = (kiss_fft_cpx*) malloc(sizeof(kiss_fft_cpx) * TIME);

    // Right here...
    kiss_fftr(config, (kiss_fft_scalar*) samples, spectrum);

    for (int i = 0; i < speclen; i++) {
        float re = scale(spectrum[i].r) * TIME;
        float im = scale(spectrum[i].i) * TIME;

        output[i] = sqrtf(re*re + im*im);
    }

    return 0;

    */





    /// TODO:
    testing_KissFFT();

    /// TODO:
    testing_FILES();




    /// IMPORTANT FOR DEVELOPERS:
    /// Saves all meshes to a file:
    /// smgr->saveScene("fullscene.irr",0,0);




    /// Initializing Timers: (Part #2)
    lastFPS = -1;
    then = device->getTimer()->getTime();
    frameDeltaTime = 0.0;
    bool quit = false;




    /// Main Loop:
	while(device->run() && quit == false){

    // driver->beginScene(true, true, video::SColor(0,0,0,0));
    driver->beginScene(true, true, 0);

    // Work out a frame delta time.
    const u32 now = device->getTimer()->getTime();
    /// frameDeltaTime = (f32)(now - then)/1000.f; // Time in seconds
    frameDeltaTime = (f32)(now - then);
    then = now;

    /// Delay loop - Keep frame rate constant... as possible! (Not used by now)
    /// createDelay();

    /// Force to create a new random seed:
    srand(time(NULL));


#include "events.h"



















            /// All done, so now get the selected Camera:
            smgr->setActiveCamera(camera[CAMERA_SELECTED]);


            /// #1 Update all cameras target... Cameras target always is: Band position ...
            if(nodeBand){
            cameraTarget = core::vector3df(nodeBand->getPosition().X, nodeBand->getPosition().Y+65, nodeBand->getPosition().Z);
            camera[CAMERA_SELECTED]->setTarget(cameraTarget); // Set Camera Target (band)
            };

            /// #2 ...Except for camera N.7 (positioned at back of the band)
            if(CAMERA_SELECTED == 7){
            cameraTarget = core::vector3df(nodeBand->getPosition().X, nodeBand->getPosition().Y+50, nodeBand->getPosition().Z);
            camera[CAMERA_SELECTED]->setTarget(cameraTarget);
            camera[CAMERA_SELECTED]->setFOV(1.60f);
            }; // Set Camera Target (spectators)


            if(CAMERA_SELECTED == 10){  /// Follow path camera
            cameraTarget = core::vector3df(nodeBand->getPosition().X, nodeBand->getPosition().Y+50, nodeBand->getPosition().Z);
            camera[CAMERA_SELECTED]->setPosition(CameraPositionNode->getPosition());
            camera[CAMERA_SELECTED]->setTarget(cameraTarget);
            }; // Set Camera Target (spectators)



            /// Only for debug mode:
            if(DEBUG_MODE==1) {
            CAMERA_SELECTED = 20;
            // camera[CAMERA_SELECTED]->setTarget(camera[CAMERA_SELECTED]->getTarget());
            camera[CAMERA_SELECTED]->setFOV(1.25f);
            };



            /*
            ITexture * fireballTexture = driver->getTexture("courtain.jpg");
            recti fireballTextureFrame(0,0, SCREEN_HEIGHT, SCREEN_WIDTH);
            driver->draw2DImage(fireballTexture, vector2di(0,0), fireballTextureFrame);
            */

            // if( keepcam < 1000){
            /// Prepare the render with an effect:
            // driver->beginScene(false, false, 0);

            /*
            /// Take screenshot and convert to texture:
            screenshot = driver->createScreenShot();
            if(screenshot!=NULL){ /// Voiding crashes, when menu window is hidden
            if(backgroundImageTexture != NULL){ driver->removeTexture(backgroundImageTexture); backgroundImageTexture = NULL; };
            if(backgroundImageTexture == NULL){ backgroundImageTexture = driver->getTexture("courtain.jpg"); }; // backgroundImageTexture = driver->addTexture("screenshot", screenshot); };
            screenshot->drop();
            screenshot = NULL;
            };
            */

            /*
            if(backgroundImageTexture == NULL){ backgroundImageTexture = driver->getTexture("courtain.jpg"); };

            /// Render Background Image:
            if(backgroundImageTexture != NULL){
            // driver->beginScene(true, true, SColor(100,0,0,0));
            driver->enableMaterial2D();
            driver->draw2DImage(backgroundImageTexture, position2d<s32>(0,0), core::rect<s32>(0,0,sw,sh), 0, video::SColor(155,255,255,255), true);
            driver->enableMaterial2D(false);
            // driver->draw2DImage(backgroundImageTexture, position2d<s32>(0,0), core::rect<s32>(0,0,sw,sh));
            // driver->setMaterial(driver->getMaterial2D()); // In case buffer is distorted
            // driver->endScene();
            };

            // };
            */



            /// Rectangle:
            // driver->setRenderTarget(10, true, true, video::SColor(255,255,255,255));
            // driver->draw2DRectangle(video::SColor(255,255,0,0), core::rect<s32>(0,0,1000,1000)); // opacity, red, green, blue
            // driver->draw2DRectangle(irr::video::SColor(50,0,0,0), irr::core::rect<irr::s32>(0,0 ,driver->getScreenSize().Width, driver->getScreenSize().Height));
            // driver->draw2DRectangle(SColor(255,0,0,0,), rect<s32>(0,0,640,480));
            // driver->draw2DRectangleOutline(rect<s32>(1,1,639,479), SColor(255,255,255,255));
            // driver->setRenderTarget(0);









            /// EFFECTS (2/2): ( From: "libraries\PostProcessor\..." )
            // With effects enabled: Excludes effect N.1(water) - and - N.6(saturate)
            if(ppeffectsEnable && PPEFFECTS){
            /// driver->beginScene(false, false, 0);
            ppEffects[ppEffectsRand]->render(NULL); // Render effects
            driver->setMaterial(driver->getMaterial2D()); // Updates all 2D materials (else some will be inverted/distorted when using effects)
            drawCameraTransition(); /// Take screenshot (Before draw the GUI)
            guienv->drawAll(); /// Draw GUI
            drawMoreStuff();
            driver->endScene(); /// ENDS RENDER
            } else {
            // With effects disabled:
            /// driver->beginScene(false, false, 0);
            smgr->drawAll(); /// Draw scene
            driver->setMaterial(driver->getMaterial2D()); // Updates all 2D materials (else some will be inverted/distorted when using effects)
            drawCameraTransition(); /// Take screenshot (Before draw the GUI)
            guienv->drawAll(); /// Draw GUI
            drawMoreStuff();
            driver->endScene(); /// ENDS RENDER
            };





	};


    /// Closing time...

    /// Free and close Kiss FFT:
    // fclose (pFile);
    // kiss_fft_cleanup();
    // free(fft.forwardConfig);
    // free(fft.inverseConfig);
    // free(fft.spectrum);
    /// Free and close Audiere:
	if(music      ){ if(music->isPlaying()     ){ music->stop();      }; music = NULL;      };
	if(soundClick ){ if(soundClick->isPlaying()){ soundClick->stop(); }; soundClick = NULL; };
	if(soundOoo   ){ if(soundOoo->isPlaying()  ){ soundOoo->stop();   }; soundOoo = NULL;   };
	if(soundIntro ){ if(soundIntro->isPlaying()){ soundIntro->stop(); }; soundIntro = NULL; };
	if(soundClaps ){ if(soundClaps->isPlaying()){ soundClaps->stop(); }; soundClaps = NULL; };
	if(audiereAudioDevice){ audiereAudioDevice = NULL; };
    /// Free and close Irrlicht:
    if(device){ device->closeDevice(); };
	if(device){ device->drop();        };
	/// Goodbye:
	std::cout << "------------------------------------------------------------------------------" << std::endl;
	std::cout << "\nIMP - Irrlicht Music Player Exited... Goodbye!\n" << std::endl;
	std::cout << "------------------------------------------------------------------------------" << std::endl;
	// return 0;

};


