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



    std::cout << " " << std::endl;
    std::cout << " " << std::endl;
    std::cout << "IMP - IRRLICHT MUSIC PLAYER" << std::endl;
    std::cout << " " << std::endl;
    std::cout << " " << std::endl;

    /// Fill this array: (Can click again?)
    int arrCASize = sizeof(canClickAgain)/sizeof(int);       // Count array items
    for(signed int i=0; i<arrCASize; i++){ canClickAgain[i] = 1; }  // Fill array

    /// Ok, now lets get all settings from the external settings file:
    readSettingsFromFile();

	/// Loop to chose driver from all available:
    video::E_DRIVER_TYPE driverType;
    for (int i=0; i<irr::video::EDT_COUNT; i++){
    if (irr::IrrlichtDevice::isDriverSupported(irr::video::E_DRIVER_TYPE(i))){ driverType = irr::video::E_DRIVER_TYPE(i); };
    };


    /// Create device from the above found driver:
	device = createDevice(EDT_OPENGL, core::dimension2d<u32>(SCREEN_WIDTH, SCREEN_HEIGHT), 16, FULLSCREEN, STENCIL_SHADOWS, VSYNC, 0); /// OK!... Or using the next code...
	/// device = createDevice(deviceType = video::EDT_SOFTWARE, core::dimension2d< u32 >(640, 480), u32 bits = 16, bool fullscreen = false, bool stencilbuffer = false, bool vsync = false, IEventReceiver* receiver = 0

    /*
	SIrrlichtCreationParameters params = SIrrlichtCreationParameters();
	params.DriverType  = driverType;
	params.WindowSize  = core::dimension2d<u32>(SCREEN_WIDTH, SCREEN_HEIGHT);
	params.ZBufferBits = 16;
	params.Fullscreen  = FULLSCREEN;
	params.StencilBuffer = false;
	params.Vsync = false;
    params.AntiAlias   = true;
    params.Event = 0;
    device = createDeviceEx(params);
    */

    if (device == 0){ return 1; }; /// If device fails... Do nothing...

	driver  = device->getVideoDriver();     // Create Driver
	filesys = device->getFileSystem();
    guienv  = device->getGUIEnvironment();  // For GUI texts
	smgr    = device->getSceneManager();    // Create Scene Manager
	// video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices(); // Not used, by now...


	/// For STENCIL_SHADOWS:
	/// Set shadows colors:
	smgr->setShadowColor(video::SColor(150,0,0,0)); // a (0 = transparent | 255 = opaque), r, g, b
	/// Some tweaks to accelerate the shadows process:
	// device->IsUseStencilBuffer();
    // scene->setShadowDelayerMax(2);
    // scene->setAutoShadowDelayer(true);


	// Add an events receiver: (keyboard)
	MyEventReceiver receiver;
    device->setEventReceiver(&receiver);

    /// Device: Other Stuff: (Window)
    device->setWindowCaption(windowtexts.c_str());
	device->getCursorControl()->setVisible(true);  // Mouse cursor visibility
	device->setResizable(true);                    // Resizable window

	/// Driver: Other Stuff: (for transparencies)
	driver->setTextureCreationFlag(E_TEXTURE_CREATION_FLAG::ETCF_ALWAYS_32_BIT, true);
	driver->setTextureCreationFlag(E_TEXTURE_CREATION_FLAG::ETCF_CREATE_MIP_MAPS, true);

    /// Materials:
	driver->getMaterialRenderer(video::EMT_PARALLAX_MAP_SOLID);

    /// Driver: Other Stuff: (for smoothness)
	driver->getMaterial2D().TextureLayer[0].BilinearFilter = true;
	driver->getMaterial2D().TextureLayer[1].BilinearFilter = true;
	driver->getMaterial2D().TextureLayer[2].BilinearFilter = true;
	driver->getMaterial2D().TextureLayer[3].BilinearFilter = true;
    driver->getMaterial2D().AntiAliasing = video::EAAM_FULL_BASIC;



    std::cout << "\n\nInternal Datas:" << std::endl;
	/// Load all needed files: (package is a ZIP file, in encryption level: AES-128)
	// http://irrlicht.sourceforge.net/docu/classirr_1_1io_1_1_i_file_system.html#a05de4e45b4d36c779b9b8f7cb1dccf7c
    // device->getFileSystem()->addFileArchive("data/world.dll", true, false, EFAT_ZIP, ""); // String filename, ignoreCase, ignorePaths, fileType, password
    device->getFileSystem()->addFileArchive("world/world.zip", true, true, EFAT_ZIP, "none"); // String filename, ignoreCase, ignorePaths, fileType, password
    std::cout << "file \"world.zip\"... Ok" << std::endl;
	// Load all from... (deprecated: addZipFileArchive)
	// device->getFileSystem()->addZipFileArchive("data/world.zip");

	// applyICON();

    /// Loading Bar: #1 (Initialize)
    /// driver->removeTexture(backgroundImageTexture);
    /// backgroundImageTexture = driver->getTexture("courtain.jpg");
    LoadBar = new CLoadingBar(device, driver, guienv, NULL, rect<s32>(300, SCREEN_HEIGHT/2, SCREEN_WIDTH-300, SCREEN_HEIGHT/2+15), true, 7);  // posX, posY, bar width, bar height
    LoadBar->setPercent(0); // 0 to 100
    LoadBar->draw();

    /// ##########################################################################################
    /// ############# AUDIO: (USING AUDIERE LIBRARY) - END #######################################
    /// ##########################################################################################

    std::cout << "\n\nInternal Audios:" << std::endl;

    /// Load .OGG sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFile00 = device->getFileSystem()->createAndOpenFile("audio_intro.ogg");
    void* buffer00 = malloc(readFile00->getSize());
    readFile00->read(buffer00, readFile00->getSize());
    // Create memory file:
    FilePtr targetFile00 = CreateMemoryFile(buffer00, readFile00->getSize()); // targetFile11 is the file (path) that we want !!!
    free(buffer00);
    readFile00->drop();

    /// Load .OGG sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFile11 = device->getFileSystem()->createAndOpenFile("audio_ooo.ogg");
    void* buffer11 = malloc(readFile11->getSize());
    readFile11->read(buffer11, readFile11->getSize());
    // Create memory file:
    FilePtr targetFile11 = CreateMemoryFile(buffer11, readFile11->getSize()); // targetFile11 is the file (path) that we want !!!
    free(buffer11);
    readFile11->drop();

    /// Load .OGG sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFile22 = device->getFileSystem()->createAndOpenFile("audio_applause.ogg");
    void* buffer22 = malloc(readFile22->getSize());
    readFile22->read(buffer22, readFile22->getSize());
    // Create memory file:
    FilePtr targetFile22 = CreateMemoryFile(buffer22, readFile22->getSize()); // targetFile22 is the file (path) that we want !!!
    free(buffer22);
    readFile22->drop();

    /// Load .OGG sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFile33 = device->getFileSystem()->createAndOpenFile("audio_click.ogg");
    void* buffer33 = malloc(readFile33->getSize());
    readFile33->read(buffer33, readFile33->getSize());
    // Create memory file:
    FilePtr targetFile33 = CreateMemoryFile(buffer33, readFile33->getSize()); // targetFile33 is the file (path) that we want !!!
    free(buffer33);
    readFile33->drop();

    /// OK!... Now lets load Sounds to AUDIERE:
    if(!audiereAudioDevice){ std::cout << "ERROR: Audiere: Cant open audio device! \n" << std::endl; exit(1); };
    // if(!audiereMidiDevice ){ std::cout << "ERROR: Audiere: Cant open midi device! \n"  << std::endl; exit(1); };

    soundIntro = audiere::OpenSound(audiereAudioDevice.get(), targetFile00, true); // Load music into memory? true / false
    soundIntro->setVolume(GLOBAL_VOLUME/2);
    soundIntro->setPan(0);    // Left, 1 Right
    soundIntro->setRepeat(0); // 1 loop, 0 don't loop
    // soundIntro->play();
    std::cout << "Audio \"audio_intro.ogg\"... Ok" << std::endl;

    soundOoo = audiere::OpenSound(audiereAudioDevice.get(), targetFile11, true); // Load music into memory? true / false
    soundOoo->setVolume(GLOBAL_VOLUME/3);
    soundOoo->setPan(0);    // Left, 1 Right
    soundOoo->setRepeat(0); // 1 loop, 0 don't loop
    // soundOoo->play();
    std::cout << "Audio \"audio_booo.ogg\"... Ok" << std::endl;

    soundClaps = audiere::OpenSound(audiereAudioDevice.get(), targetFile22, true); // Load music into memory? true / false
    soundClaps->setVolume(GLOBAL_VOLUME/2);
    soundClaps->setPan(0);    // Left, 1 Right
    soundClaps->setRepeat(0); // 1 loop, 0 don't loop
    // soundClaps->play();
    std::cout << "Audio \"audio_applause.ogg\"... Ok" << std::endl;

    soundClick = audiere::OpenSound(audiereAudioDevice.get(), targetFile33, true); // Load music into memory? true / false
    soundClick->setVolume(GLOBAL_VOLUME/2);
    soundClick->setPan(0);    // Left, 1 Right
    soundClick->setRepeat(0); // 1 loop, 0 don't loop
    // soundClick->play();    // 1 loop, 0 don't loop
    std::cout << "Audio \"audio_click.ogg\"... Ok" << std::endl;


    /// ##########################################################################################
    /// ############# AUDIO: (USING AUDIERE LIBRARY) - END #######################################
    /// ##########################################################################################




    /// Loading Bar: #2
    LoadBar->setPercent(10); // 0 to 100
    LoadBar->draw();
    // LoadBar->setVisible(false);





    /// ############# DEVELOPER NOTES ################################################################

    /// .3DS files exported from 3DMax 2017 loads OK. Don't support animations
    /// .blend files cant load
    /// .dae files (Collada) handles lights very well. Don't support animations

    /*
    /// Create (Load Animated Object) for INTERNAL TESTS PURPOSES only:
	scene::IAnimatedMeshSceneNode* nodeTests = smgr->addAnimatedMeshSceneNode(smgr->getMesh("stage.obj"));
	/// if(nodeTests){
	scene::IMeshBuffer* buffer92 = smgr->getMesh("stage.obj")->getMesh(0)->getMeshBuffer(0);
    int vc92 = buffer92->getVertexCount();
    std::cout << "Vertex count: " << vc92 << std::endl;
	nodeTests->setPosition(vector3df(0, 100, 0));
	/// nodeTests->setRotation(vector3df(0, 180, 0));
	nodeTests->setScale(core::vector3df(1.0,1.0,1.0));
	nodeTests->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeTests->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeTests->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    // nodeTests->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER
    nodeTests->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeTests->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    /// Set animation frames limits:
    f32 framestartb = nodeTests->getStartFrame();
    f32 frameendb = nodeTests->getEndFrame()-1;
    /// nodeTests->setFrameLoop(framestartb, frameendb);
    /// nodeTests->setCurrentFrame(framestartb);
    nodeTests->setAnimationSpeed(1);
    nodeTests->setLoopMode(true);
    nodeTests->setVisible(true);
	/// };
	*/

    /*
    for(u32 i=0; i<nodeTests->getMaterialCount(); i++){ // nodeTests->getMaterialCount(); // 40
	nodeTests->getMaterial(i).setTexture(0, driver->getTexture("stage_floor.jpg"));
	nodeTests->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeTests->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeTests->getMaterial(i).getTextureMatrix(1).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeTests->getMaterial(i).Lighting = true;
	nodeTests->getMaterial(i).Shininess = 10; // Set size of specular highlights (20.0f == Metallic)
	nodeTests->getMaterial(i).SpecularColor.set(10,10,10,10); /// The lights reflexes
    // nodeTests->getMaterial(i).Shininess = 0.0f; // Set size of specular highlightss (20.0f == Metallic)
	// nodeTests->getMaterial(i).SpecularColor.set(155,155,155,155);
	// nodeTests->getMaterial(i).DiffuseColor.set(255,255,255,255);
	// nodeTests->getMaterial(i).EmissiveColor.set(0,0,0,0);
	// nodeTests->getMaterial(i).getTextureMatrix(1).buildTextureTransform(1, core::vector2df(-0.5f, -0.5f*5), core::vector2df(0.5f,0.5f), core::vector2df(5, 5));
	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeTests->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeTests->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	nodeTests->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
	};
	*/







    /// #############################################################################################







    /// ############# LOAD NODES ################################################################

    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Load MESHES:" << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;

    /// Create (Loads Object) STAGE:
	scene::IAnimatedMeshSceneNode* nodeStage = smgr->addAnimatedMeshSceneNode(smgr->getMesh("stage.obj")); // For an animated node
	// scene::ISceneNode* nodeStage = smgr->addMeshSceneNode(smgr->getMesh("stage.obj")->getMesh(0));

	scene::IMeshBuffer* buffer1 = smgr->getMesh("stage.obj")->getMesh(0)->getMeshBuffer(0);
    int vc1 = buffer1->getVertexCount();
    std::cout << "Stage vertex count: " << vc1 << std::endl;
	// scene::IMeshSceneNode* nodeStage = smgr->getMeshManipulator()->createMeshWithTangents(smgr->getMesh("cena.obj")->getMesh(0));
	// scene::IMeshSceneNode* nodeStage = smgr->addMeshSceneNode(smgr->getMesh("cena.obj")->getMesh(0));
	nodeStage->setPosition(vector3df(0, 100, 0));
	nodeStage->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeStage->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeStage->setMaterialFlag(video::EMF_FOG_ENABLE, true);         // Node is affected by FOG?
    nodeStage->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    nodeStage->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    for(u32 i=0; i<nodeStage->getMaterialCount(); i++){ // nodeStage->getMaterialCount(); // 40
	nodeStage->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeStage->getMaterial(i).getTextureMatrix(1).setTextureScale(2.0f,1.0f); /// Repeat (tile) the texture

    /// Start Colors:
	/// Set size of specular highlights
	nodeStage->getMaterial(i).Lighting = true;
	nodeStage->getMaterial(i).Shininess = 70.0f; /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
    /// Affect the colour of the reflected light:
    nodeStage->getMaterial(i).SpecularColor = video::SColor(200,100,100,100); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeStage->getMaterial(i).AmbientColor  = video::SColor(200,200,200,200); /// How much ambient light (a global light) is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStage->getMaterial(i).DiffuseColor  = video::SColor(200,200,200,200); /// How much diffuse light coming from a light source is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStage->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// Light emitted by this material. Default is to emit no light (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors


    /*
    /// Start Colors:
	/// Set size of specular highlights
	nodeStage->getMaterial(i).Lighting = true;
	nodeStage->getMaterial(i).Shininess = 70.0f; /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
    /// Affect the colour of the reflected light:
    nodeStage->getMaterial(i).SpecularColor = video::SColor(150,100,100,100); /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
    /// Tweak all the light related aspects of the material:
    nodeStage->getMaterial(i).AmbientColor  = video::SColor(200,150,150,150); /// How much ambient light (a global light) is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStage->getMaterial(i).DiffuseColor  = video::SColor(150,100,100,100); /// How much diffuse light coming from a light source is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStage->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// Light emitted by this material. Default is to emit no light (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors
    */

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeStage->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeStage->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
    nodeStage->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust    nodeStage->getMaterial(i).Shininess = 12.0f; // Set size of specular highlights (20.0f == Metallic)
	};


	// nodeStage->setMaterialType(video::EMT_DETAIL_MAP); // Needed for multitexturing (Apply (mix) texture to my specified material)
	/// Material Type --> http://irrlicht.sourceforge.net/docu/namespaceirr_1_1video.html#ac8e9b6c66f7cebabd1a6d30cbc5430f1
	/// Material Type --> http://webcache.googleusercontent.com/search?q=cache:r8-qBiunQboJ:irrlicht.sourceforge.net/tut011.html+irrlicht+animator+rotation&cd=2&hl=pt-PT&ct=clnk&gl=pt&source=www.google.pt
    nodeStage->setMaterialType(video::EMT_REFLECTION_2_LAYER); // EMT_REFLECTION_2_LAYER
    nodeStage->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);  // Increase view distance quality (similar to sharpness)

    std::cout << "Mesh file \"stage.obj\"... Ok" << std::endl;

    std::cout << "\n\nMeshes:" << std::endl;


    /// Create (Loads Object) STAGE:
	/// scene::ISceneNode* nodeStageFloor = smgr->addMeshSceneNode(smgr->getMesh("stage_floor.obj")->getMesh(0));
	scene::IMeshSceneNode* nodeStageFloor = smgr->addMeshSceneNode(smgr->getMesh("stage_floor.obj"));
	scene::IMeshBuffer* buffersf = smgr->getMesh("stage_floor.obj")->getMesh(0)->getMeshBuffer(0);
    int vcsf = buffersf->getVertexCount();
    std::cout << "Stage floor vertex count: " << vcsf << std::endl;
	// scene::IMeshSceneNode* nodeStageFloor = smgr->getMeshManipulator()->createMeshWithTangents(smgr->getMesh("cena.obj")->getMesh(0));
	// scene::IMeshSceneNode* nodeStageFloor = smgr->addMeshSceneNode(smgr->getMesh("cena.obj")->getMesh(0));
	nodeStageFloor->setPosition(vector3df(0, 100, 0));
	nodeStageFloor->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeStageFloor->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeStageFloor->setMaterialFlag(video::EMF_FOG_ENABLE, true);         // Node is affected by FOG?
    nodeStageFloor->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    nodeStageFloor->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeStageFloor->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeStageFloor->setMaterialType(video::EMT_REFLECTION_2_LAYER); // EMT_REFLECTION_2_LAYER --> The reflection map should be set as first texture!!
    // nodeStageFloor->setMaterialType(video::EMT_LIGHTMAP_LIGHTING_M4); /// EMT_SPHERE_MAP: Look like a reflection of the environment around it

    for(u32 i=0; i<nodeStageFloor->getMaterialCount(); i++){ // nodeStageFloor->getMaterialCount(); // 40
	nodeStageFloor->getMaterial(i).setTexture(0, driver->getTexture("stage_floor_default.jpg"));
	nodeStageFloor->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	// nodeStageFloor->getMaterial(i).setTexture(1, driver->getTexture("stage_floor_reflexes.jpg"));
	// nodeStageFloor->getMaterial(i).getTextureMatrix(1).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	/// nodeStageFloor->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeStageFloor->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeStageFloor->getMaterial(i).getTextureMatrix(1).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	/*
	nodeStageFloor->getMaterial(i).BlendOperation = irr::video::EBO_ADD;
	nodeStageFloor->getMaterial(i).Lighting = true;
	nodeStageFloor->getMaterial(i).ZWriteEnable = false;
	nodeStageFloor->getMaterial(i).ZBuffer = false;
	*/
    /// Start Colors:
	nodeStageFloor->getMaterial(i).Lighting = true;
	nodeStageFloor->getMaterial(i).Shininess = 128.0f; /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
    /// Affect the colour of the reflected light:
    nodeStageFloor->getMaterial(i).SpecularColor = video::SColor(250,250,250,250); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeStageFloor->getMaterial(i).AmbientColor  = video::SColor(250,250,250,250); /// How much ambient light (a global light) is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStageFloor->getMaterial(i).DiffuseColor  = video::SColor(250,250,250,250); /// How much diffuse light coming from a light source is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStageFloor->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// Light emitted by this material. Default is to emit no light (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	// nodeStageFloor->getMaterial(i).getTextureMatrix(1).buildTextureTransform(1, core::vector2df(-0.5f, -0.5f*5), core::vector2df(0.5f,0.5f), core::vector2df(5, 5));
	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeStageFloor->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeStageFloor->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	nodeStageFloor->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
	};

    // Create texture animator
    if(nodeStageFloor){
    // Load textures for animation:
    core::array<video::ITexture*> texturesFloor;
    for(s32 n=1; n<23; ++n){ // Total images (minus) - 1 !
    core::stringc tmp = "";
    tmp = "stage_floor_"; tmp += n; tmp += ".jpg"; // Path to images directory
    video::ITexture* tx = driver->getTexture(tmp.c_str());
    if(tx!=NULL){ texturesFloor.push_back(tx); }; // image found?... Add it!
    };
    scene::ISceneNodeAnimator* animatorFloor = smgr->createTextureAnimator(texturesFloor, 550, true); // Time between images (textures)
    nodeStageFloor->addAnimator(animatorFloor);
    animatorFloor->drop();
    };



    /// Create (Loads Object) STAGE PLAKARD: (texts "irrlicht music player")
	scene::IAnimatedMeshSceneNode* nodeStagePlackard = smgr->addAnimatedMeshSceneNode(smgr->getMesh("stage_plackard.obj")); // For an animated node
	// scene::ISceneNode* nodeStagePlackard = smgr->addMeshSceneNode(smgr->getMesh("stage.obj")->getMesh(0));

	scene::IMeshBuffer* buffer101 = smgr->getMesh("stage_plackard.obj")->getMesh(0)->getMeshBuffer(0);
    int vc101 = buffer101->getVertexCount();
    std::cout << "Stage Plackard vertex count: " << vc101 << std::endl;
	// scene::IMeshSceneNode* nodeStagePlackard = smgr->getMeshManipulator()->createMeshWithTangents(smgr->getMesh("cena.obj")->getMesh(0));
	// scene::IMeshSceneNode* nodeStagePlackard = smgr->addMeshSceneNode(smgr->getMesh("cena.obj")->getMesh(0));
	nodeStagePlackard->setPosition(vector3df(0, 100, 0));
	nodeStagePlackard->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeStagePlackard->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeStagePlackard->setMaterialFlag(video::EMF_FOG_ENABLE, true);         // Node is affected by FOG?
    nodeStagePlackard->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    nodeStagePlackard->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    for(u32 i=0; i<nodeStagePlackard->getMaterialCount(); i++){ // nodeStagePlackard->getMaterialCount(); // 40
	nodeStagePlackard->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeStagePlackard->getMaterial(i).getTextureMatrix(1).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture

    /// Start Colors:
	/// Set size of specular highlights
	nodeStagePlackard->getMaterial(i).Lighting = true;
	nodeStagePlackard->getMaterial(i).Shininess = 6.0f; /// Reflex type: 20.0f == Metallic
    /// Affect the colour of the reflected light:
    nodeStagePlackard->getMaterial(i).SpecularColor = video::SColor(150,50,50,50); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeStagePlackard->getMaterial(i).AmbientColor  = video::SColor(150,150,150,150); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStagePlackard->getMaterial(i).DiffuseColor  = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStagePlackard->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	// nodeStagePlackard->getMaterial(i).getTextureMatrix(1).buildTextureTransform(1, core::vector2df(-0.5f, -0.5f*5), core::vector2df(0.5f,0.5f), core::vector2df(5, 5));
	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeStagePlackard->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeStagePlackard->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
    nodeStagePlackard->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust    nodeStagePlackard->getMaterial(i).Shininess = 12.0f; // Set size of specular highlights (20.0f == Metallic)
	};


	// nodeStagePlackard->setMaterialType(video::EMT_DETAIL_MAP); // Needed for multitexturing (Apply (mix) texture to my specified material)
	/// Material Type --> http://irrlicht.sourceforge.net/docu/namespaceirr_1_1video.html#ac8e9b6c66f7cebabd1a6d30cbc5430f1
	/// Material Type --> http://webcache.googleusercontent.com/search?q=cache:r8-qBiunQboJ:irrlicht.sourceforge.net/tut011.html+irrlicht+animator+rotation&cd=2&hl=pt-PT&ct=clnk&gl=pt&source=www.google.pt
    nodeStagePlackard->setMaterialType(video::EMT_REFLECTION_2_LAYER); // EMT_REFLECTION_2_LAYER
    nodeStagePlackard->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);  // Increase view distance quality (similar to sharpness)

    /*
    // Create render target
    video::ITexture* rttr = 0;
    rttr = driver->addRenderTargetTexture(core::dimension2d<u32>(512,512), "RTTR");
    for(u32 i=0; i<nodeStageFloor->getMaterialCount(); i++){
    nodeStageFloor->setMaterialTexture(i, rttr); // set material of cube to render target
    };
    */


    /// ##########################################################################################

	// smgr->getMeshManipulator()->makePlanarTextureMapping(smgr->getMesh("cena.obj")->getMesh(0), 0.001f);

    /// Create disco balls spot light effects: (Needed for multitexturing)
    // ITexture* texture1 = driver->getTexture("reflexes.jpg");
    // rect<s32> sourceRect = rect<s32>(0,0,256,201);


    /// Create (Loads Object) PLACKARD:
	scene::ISceneNode* nodeFlyingPlackard = smgr->addMeshSceneNode(smgr->getMesh("plackard.obj")->getMesh(0));
	scene::IMeshBuffer* buffer12 = smgr->getMesh("plackard.obj")->getMesh(0)->getMeshBuffer(0);
    int vc12 = buffer12->getVertexCount();
    std::cout << "Flying Plackard vertex count: " << vc12 << std::endl;
	nodeFlyingPlackard->setPosition(vector3df(0, 115, 0));
	nodeFlyingPlackard->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeFlyingPlackard->setMaterialFlag(video::EMF_LIGHTING, false);          // Node is affected by LIGHT?
    nodeFlyingPlackard->setMaterialFlag(video::EMF_FOG_ENABLE, true);         // Node is affected by FOG?
    nodeFlyingPlackard->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    nodeFlyingPlackard->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    /*
    for(u32 i=0; i<nodeFlyingPlackard->getMaterialCount(); i++){ // nodeFlyingPlackard->getMaterialCount(); // 40
	nodeFlyingPlackard->getMaterial(i).setTexture(1, driver->getTexture("plackard.jpg"));
	/// nodeFlyingPlackard->getMaterial(i).getTextureMatrix(0).setTextureScale(1,1);
	nodeFlyingPlackard->getMaterial(i).Lighting = true;
	// nodeFlyingPlackard->getMaterial(i).getTextureMatrix(1).buildTextureTransform(1, core::vector2df(-0.5f, -0.5f*5), core::vector2df(0.5f,0.5f), core::vector2df(5, 5));
	};
	*/

    nodeFlyingPlackard->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER
    nodeFlyingPlackard->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)

    /// Start Colors:
	/// Set size of specular highlights
	nodeFlyingPlackard->getMaterial(0).Lighting = true;
	nodeFlyingPlackard->getMaterial(0).Shininess = 8.0f; /// Reflex type: 20.0f == Metallic
    /// Affect the colour of the reflected light:
    nodeFlyingPlackard->getMaterial(0).SpecularColor = video::SColor(150,50,50,50); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeFlyingPlackard->getMaterial(0).AmbientColor  = video::SColor(150,150,150,150); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    nodeFlyingPlackard->getMaterial(0).DiffuseColor  = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    nodeFlyingPlackard->getMaterial(0).EmissiveColor = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

    std::cout << "Mesh file \"plackard.obj\"... Ok" << std::endl;

	/// Since driver + nodeFlyingPlackard are already created, now we can now initialize "Text to texture" stuff:
    rtt = driver->addRenderTargetTexture(core::dimension2d<u32>(512, 512), "rtt", ECF_R8G8B8);
    font_texture = guienv->getFont("font2.bmp"); // Get my text font

    /// And write some text to texture too:
    doTextToTexture(nodeFlyingPlackard, L"Irrlicht Music Player");





	/// Staff: STAFF #1
	scene::IAnimatedMeshSceneNode* nodeStaff1;
	nodeStaff1 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("staff.MD3"));
	scene::IMeshBuffer* bufferS = smgr->getMesh("staff.MD3")->getMesh(0)->getMeshBuffer(0);
    int vcS = bufferS->getVertexCount();
    std::cout << "Staff n.1 vertex count: " << vcS << std::endl;
	nodeStaff1->setPosition(vector3df(180, 57, -379));
	nodeStaff1->setRotation(vector3df(0, 180-40, 0));
	nodeStaff1->setScale(core::vector3df(0.65,0.65,0.65));
	nodeStaff1->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeStaff1->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeStaff1->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    nodeStaff1->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeStaff1->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeStaff1->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER

	for(u32 i=0; i<nodeStaff1->getMaterialCount(); i++){
	nodeStaff1->getMaterial(i).setTexture(0, driver->getTexture("staff.jpg")); // Apply texture to my specified material
	nodeStaff1->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeStaff1->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeStaff1->getMaterial(i).getTextureMatrix(1).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture

	/// Start Colors:
	/// Set size of specular highlights
	nodeStaff1->getMaterial(i).Lighting = true;
	nodeStaff1->getMaterial(i).Shininess = 0.5f; /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
    /// Affect the colour of the reflected light:
    nodeStaff1->getMaterial(i).SpecularColor = video::SColor(1,1,1,1); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeStaff1->getMaterial(i).AmbientColor  = video::SColor(255,40,40,40); /// How much ambient light (a global light) is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStaff1->getMaterial(i).DiffuseColor  = video::SColor(255,15,15,15); /// How much diffuse light coming from a light source is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStaff1->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// Light emitted by this material. Default is to emit no light (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeStaff1->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeStaff1->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	nodeStaff1->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
	};


	/// Set animation frames limits:
    f32 framestart9 = nodeStaff1->getStartFrame();
    f32 frameend9 = nodeStaff1->getEndFrame()-1;
    nodeStaff1->setFrameLoop(framestart9,frameend9);
    nodeStaff1->setCurrentFrame(framestart9);
    nodeStaff1->setAnimationSpeed(0);
    nodeStaff1->setLoopMode(false);

    std::cout << "Mesh file \"staff.MD3\"... Ok" << std::endl;

    /// Staff: STAFF #2
	scene::IAnimatedMeshSceneNode* nodeStaff2;
	nodeStaff2 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("staff.MD3"));
	scene::IMeshBuffer* bufferS2 = smgr->getMesh("staff.MD3")->getMesh(0)->getMeshBuffer(0);
    int vcS2 = bufferS2->getVertexCount();
    std::cout << "Staff n.2 vertex count: " << vcS2 << std::endl;
	nodeStaff2->setPosition(vector3df(-180, 57, -379));
	nodeStaff2->setRotation(vector3df(0, 180+40, 0));
	nodeStaff2->setScale(core::vector3df(0.65,0.65,0.65));
	nodeStaff2->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeStaff2->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeStaff2->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    nodeStaff2->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeStaff2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeStaff2->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER

	for(u32 i=0; i<nodeStaff2->getMaterialCount(); i++){
	nodeStaff2->getMaterial(i).setTexture(0, driver->getTexture("staff.jpg")); // Apply texture to my specified material
	nodeStaff2->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeStaff2->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeStaff2->getMaterial(i).getTextureMatrix(1).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture

	/// Start Colors:
	/// Set size of specular highlights
	nodeStaff2->getMaterial(i).Lighting = true;
	nodeStaff2->getMaterial(i).Shininess = 0.5f; /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
    /// Affect the colour of the reflected light:
    nodeStaff2->getMaterial(i).SpecularColor = video::SColor(1,1,1,1); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeStaff2->getMaterial(i).AmbientColor  = video::SColor(255,40,40,40); /// How much ambient light (a global light) is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStaff2->getMaterial(i).DiffuseColor  = video::SColor(255,15,15,15); /// How much diffuse light coming from a light source is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStaff2->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// Light emitted by this material. Default is to emit no light (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeStaff2->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeStaff2->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	nodeStaff2->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
	};


	/// Set animation frames limits:
    f32 framestart10 = nodeStaff2->getStartFrame();
    f32 frameend10 = nodeStaff2->getEndFrame()-1;
    nodeStaff2->setFrameLoop(framestart10,frameend10);
    nodeStaff2->setCurrentFrame(framestart10);
    nodeStaff2->setAnimationSpeed(0);
    nodeStaff2->setLoopMode(false);

    std::cout << "Mesh file \"staff.md3\"... Ok" << std::endl;


    /// Loading Bar: #3
    LoadBar->setPercent(20); // 0 to 100
    LoadBar->draw();
    // LoadBar->setVisible(false);


    /// ##########################################################################################


    /// Create (Load Animated Object) SPECTATORS:
    /*
    // Fill array with random numbers:
    vector<int> randUniquesChairsRow1;
    vector<int> randUniquesChairsRow2;
    vector<int> randUniquesChairsRow3;
    vector<int> randUniquesChairsRow4;

    for(int i=0; i<10; i++){
    /// Formula is --> int i = (MIN + rand()%(MAX - MIN + 1)) * INTERVAL;
    int x1 = (1+rand()%(10-1))*20;    // From 30 to 200
    int x2 = (1+rand()%(-10-1))*-20;  // From -30 to -200
    randUniquesChairsRow1.push_back(x1);
    randUniquesChairsRow1.push_back(x2);
    };
    for(int i=0; i<10; i++){
    /// Formula is --> int i = (MIN + rand()%(MAX - MIN + 1)) * INTERVAL;
    int x1 = (1+rand()%(10-1))*20;    // From 30 to 200
    int x2 = (1+rand()%(-10-1))*-20;  // From -30 to -200
    randUniquesChairsRow2.push_back(x1);
    randUniquesChairsRow2.push_back(x2);
    };
    for(int i=0; i<10; i++){
    /// Formula is --> int i = (MIN + rand()%(MAX - MIN + 1)) * INTERVAL;
    int x1 = (1+rand()%(10-1))*20;    // From 30 to 200
    int x2 = (1+rand()%(-10-1))*-20;  // From -30 to -200
    randUniquesChairsRow3.push_back(x1);
    randUniquesChairsRow3.push_back(x2);
    };
    for(int i=0; i<10; i++){
    /// Formula is --> int i = (MIN + rand()%(MAX - MIN + 1)) * INTERVAL;
    int x1 = (1+rand()%(10-1))*20;    // From 30 to 200
    int x2 = (1+rand()%(-10-1))*-20;  // From -30 to -200
    randUniquesChairsRow4.push_back(x1);
    randUniquesChairsRow4.push_back(x2);
    };
    // Remove duplicated random numbers in array:
    // sort(randUniques.begin(), randUniques.end());
    // randUniques.erase(makeUnique(randUniques.begin(), randUniques.end()), randUniques.end());
doingCamTransOnce[5]
    int totRandoms = 0;
    cout << endl;
    for(vector<int>::iterator iter = randUniquesChairsRow1.begin(); iter != randUniquesChairsRow1.end(); i++ter){
    totRandoms+= 1; cout << totRandoms <<") Spectator random is: " << *iter << endl;
    };
    cout << endl;
    */


    int chairInX[48]={ // Range: -20 to -340
    -290, -240, -220, -180, -120, -100, -60, -20, // first row right
    -290, -240, -220, -180, -120, -100, -60, -30, // second row right
    -290, -240, -220, -180, -120, -100, -60, -35, // third row right
     290,  240,  220,  180,  120,  100,  60,  20,
     290,  240,  220,  180,  120,  100,  60,  30,
     290,  240,  220,  180,  120,  100,  60,  35
    };

    /// Create (CLONE Objects) SPECTATORS:
    // scene::ISceneNode* nodeSpectators[15]={};
    // scene::IAnimatedMeshSceneNode* nodeSpectators[15]={};

    scene::IAnimatedMeshSceneNode* nodeSpectators[48]={};
    for(int i=0; i<48; i++){
    // Clone node:
    // nodeSpectators[i] = node9->clone(); // Clone only works with ISceneNode
    /// This mesh has 3 animations: 1 to 30 -> stand up + 30 to 60 -> hands up + 60 to 100 -> clapping
    nodeSpectators[i] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("spectators.MD3"));
	// nodeSpectators[i]->setPosition(vector3df(50, 70, -320));
	// nodeSpectators[i]->setRotation(vector3df(0, 0, 0));
	nodeSpectators[i]->setScale(core::vector3df(0.5,0.5,0.5));
	nodeSpectators[i]->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeSpectators[i]->setMaterialFlag(video::EMF_FOG_ENABLE, true);         // Node is affected by FOG?
    nodeSpectators[i]->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    nodeSpectators[i]->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeSpectators[i]->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeSpectators[i]->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER

	/// http://irrlicht.sourceforge.net/docu/classirr_1_1scene_1_1_i_animated_mesh_scene_node.html#aff1c1e2270f4d3d94e58e7c130c575a4
	// nodeSpectators[i]->setCurrentFrame(1);
	// nodeSpectators[i]->setFrameLoop(30,60);     // Settings also set in main loop
    // nodeSpectators[i]->setAnimationSpeed(0);    // Settings also set in main loop (0, starts statuis is: stop)
    // nodeSpectators[i]->setLoopMode(false);      // Settings also set in main loop
    // nodeSpectators[i]->setAnimationEndCallback(0);

    /// Set in right chairs rows (X):
    if(i>=0  && i<8 ){ nodeSpectators[i]->setPosition(vector3df(chairInX[i], 70, -320)); };
    if(i>=8  && i<16){ nodeSpectators[i]->setPosition(vector3df(chairInX[i], 70, -270)); };
    if(i>=16 && i<24){ nodeSpectators[i]->setPosition(vector3df(chairInX[i], 70, -220)); };

    if(i>=24 && i<32){ nodeSpectators[i]->setPosition(vector3df(chairInX[i], 70, -320)); };
    if(i>=32 && i<40){ nodeSpectators[i]->setPosition(vector3df(chairInX[i], 70, -270)); };
    if(i>=40 && i<48){ nodeSpectators[i]->setPosition(vector3df(chairInX[i], 70, -220)); };

    /*
    if(i>=10 && i<20){ nodeSpectators[i]->setPosition(vector3df(randUniquesChairsRow2[i], 70, -265)); };
    if(i>=20 && i<30){ nodeSpectators[i]->setPosition(vector3df(randUniquesChairsRow3[i], 70, -220)); };
    if(i>=30 && i<40){ nodeSpectators[i]->setPosition(vector3df(randUniquesChairsRow4[i], 70, -175)); };
    */

    // Rotate nodes (spectator) to face target Node (band):
    vector3df nodePos = nodeSpectators[i]->getPosition();
    vector3df targetPos = vector3df(0,0,0); // nodeBand->getPosition();
    vector3df diff = -(targetPos - nodePos); // Normal calculation is: vector3df diff = targetPos - nodePos;
    nodeSpectators[i]->setRotation(diff.getHorizontalAngle());
    nodeSpectators[i]->updateAbsolutePosition();
    };


    /// Aplly (random) textures:
    for(int x=0; x<48; x++){
    int k = rand()%4; // random from (0 to 3)
    for(u32 i=0; i<nodeSpectators[x]->getMaterialCount(); i++){
    if(k == 0){ nodeSpectators[x]->getMaterial(i).setTexture(0, driver->getTexture("spectators_1.jpg")); }; // Apply texture to my specified material
    if(k == 1){ nodeSpectators[x]->getMaterial(i).setTexture(0, driver->getTexture("spectators_2.jpg")); }; // Apply texture to my specified material
    if(k == 2){ nodeSpectators[x]->getMaterial(i).setTexture(0, driver->getTexture("spectators_3.jpg")); }; // Apply texture to my specified material
    if(k == 3){ nodeSpectators[x]->getMaterial(i).setTexture(0, driver->getTexture("spectators_4.jpg")); }; // Apply texture to my specified material
	nodeSpectators[x]->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeSpectators[x]->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeSpectators[x]->getMaterial(i).getTextureMatrix(1).setTextureScale(0.1f,0.1f); /// Repeat (tile) the texture

	/// Start Colors:
	/// Set size of specular highlights
	nodeSpectators[x]->getMaterial(i).Lighting = true;
	nodeSpectators[x]->getMaterial(i).Shininess = 0.5f; /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
    /// Affect the colour of the reflected light:
    nodeSpectators[x]->getMaterial(i).SpecularColor = video::SColor(1,1,1,1); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeSpectators[x]->getMaterial(i).AmbientColor  = video::SColor(255,40,40,40); /// How much ambient light (a global light) is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeSpectators[x]->getMaterial(i).DiffuseColor  = video::SColor(255,15,15,15); /// How much diffuse light coming from a light source is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeSpectators[x]->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// Light emitted by this material. Default is to emit no light (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeSpectators[x]->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeSpectators[x]->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	nodeSpectators[x]->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
	};
    };


    /// Aplly animations:
    for(int x=0; x<48; x++){
    /// Set animation frames limits:
    f32 framestart = nodeSpectators[x]->getStartFrame();
    f32 frameend = nodeSpectators[x]->getEndFrame()-1;
    nodeSpectators[x]->setFrameLoop(framestart,frameend);
    nodeSpectators[x]->setCurrentFrame(framestart);
    nodeSpectators[x]->setAnimationSpeed(0);
    nodeSpectators[x]->setLoopMode(false);
    };

    std::cout << "Mesh file \"spectators.MD3\"... Ok" << std::endl;

    /// Create Objects for SPECTATORS: (PHOTO FLASHES placed at spectator´s hand)
    scene::ISceneNode* flash[48]; // Photos Flashes Array
    scene::ISceneNode* billf[48]; // Bills Array

    for(int i=0; i<48; i++){
	flash[i] = smgr->addEmptySceneNode();              // Photo Flashes
	flash[i]->setParent(nodeSpectators[i]);            // Attach to audiance
	flash[i]->setPosition(core::vector3df(0,117,-15)); // Attach to hand
	flash[i]->setVisible(false);                       // Starts hidden
	billf[i] = smgr->addBillboardSceneNode(flash[i], core::dimension2d<f32>(60,30)); // Attach billboard to node.
	billf[i]->setMaterialFlag(video::EMF_LIGHTING, false);   // Bill Node is affected by LIGHT?
	billf[i]->setMaterialFlag(video::EMF_FOG_ENABLE, false); // Bill Node is affected by FOG?
	billf[i]->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	billf[i]->setMaterialTexture(0, driver->getTexture("estrela.png"));
    };

    /// Only 1 REAL (dynamic) light for each flash (and only 1 will be visible at a time)
    scene::ILightSceneNode* fLight[48]={};
    for(int i=0; i<48; i++){
    fLight[i] = smgr->addLightSceneNode(flash[i], vector3df(0,0,0), video::SColorf(1.0,1.0,1.0,1.0), 100); // SColorf: 0.0f to 1.0f full color
    fLight[i]->enableCastShadow(false);
    };

    std::cout << "Photo Flashes... Ok" << std::endl;


    /// Create (Load Animated Object) BALLONS:
    // Fill array with random numbers:
    vector<int> randUniquesBalloons;

    for(int i=0; i<48; i++){
    /// Formula is --> int i = (MIN + rand()%(MAX - MIN + 1)) * INTERVAL;
    int x1 = (1+rand()%(10-1))*20;    // From 30 to 200
    int x2 = (1+rand()%(-10-1))*-20;  // From -30 to -200
    randUniquesBalloons.push_back(x1);
    randUniquesBalloons.push_back(x2);
    };
    // Remove duplicated random numbers in array:
    randUniquesBalloons.erase(makeUnique(randUniquesBalloons.begin(), randUniquesBalloons.end()), randUniquesBalloons.end());
    // for(vector<int>::iterator iter = randUniques.begin(); iter != randUniques.end(); i++ter){ cout << "--> " << *iter << endl; }

    /// Create (CLONE Objects) BALLONS:
    scene::IAnimatedMeshSceneNode* nodeBalloons[48]={};

    for(int i=0; i<48; i++){
    // Clone node:
    // nodeBalloons[i] = node9->clone(); // Clone only works with ISceneNode
    /// This mesh has 3 animations: 1 to 30 -> stand up + 30 to 60 -> hands up + 60 to 100 -> clapping
    nodeBalloons[i] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("balloon.MD3"));
	// nodeBalloons[i]->setPosition(vector3df(50, 70, -320));
	// nodeBalloons[i]->setRotation(vector3df(0, 0, 0));
	nodeBalloons[i]->setScale(core::vector3df(0.0,0.0,0.0));                // Starts too small - ANIM will change this to new scales!
	nodeBalloons[i]->setMaterialFlag(video::EMF_LIGHTING, true);            // Node is affected by LIGHT?
    nodeBalloons[i]->setMaterialFlag(video::EMF_FOG_ENABLE, false);         // Node is affected by FOG?
    nodeBalloons[i]->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);  // Render both sides?
    nodeBalloons[i]->setParent(nodeSpectators[i]);                          // Attach it to spectator
    nodeBalloons[i]->setPosition(core::vector3df(0, 80, -3));               // Position relative to spectator
    nodeBalloons[i]->setVisible(false);                                     // Starts hidden - ANIM will change this!
    nodeBalloons[i]->setMaterialType(video::EMT_REFLECTION_2_LAYER);        // EMT_REFLECTION_2_LAYER
    nodeBalloons[i]->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);  // Increase view distance quality (similar to sharpness)
    nodeBalloons[i]->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    };

    /// Aplly (random) textures:
    for(int x=0; x<48; x++){
    int k = rand()%5; // random from (0 to x)
    for(u32 i=0; i<nodeBalloons[x]->getMaterialCount(); i++){
    if(k == 0){ nodeBalloons[x]->getMaterial(i).setTexture(0, driver->getTexture("balloon_1.jpg")); }; // Apply texture to my specified material
    if(k == 1){ nodeBalloons[x]->getMaterial(i).setTexture(0, driver->getTexture("balloon_2.jpg")); }; // Apply texture to my specified material
    if(k == 2){ nodeBalloons[x]->getMaterial(i).setTexture(0, driver->getTexture("balloon_3.jpg")); }; // Apply texture to my specified material
    if(k == 3){ nodeBalloons[x]->getMaterial(i).setTexture(0, driver->getTexture("balloon_4.jpg")); }; // Apply texture to my specified material
    if(k == 4){ nodeBalloons[x]->getMaterial(i).setTexture(0, driver->getTexture("balloon_5.jpg")); }; // Apply texture to my specified material
    nodeBalloons[x]->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeBalloons[x]->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeBalloons[x]->getMaterial(i).getTextureMatrix(1).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture

    /// Start Colors:
	/// Set size of specular highlights
	nodeBalloons[x]->getMaterial(i).Lighting = true;
	nodeBalloons[x]->getMaterial(i).Shininess = 3.0f; /// Reflex type: 20.0f == Metallic
    /// Affect the colour of the reflected light:
    nodeBalloons[x]->getMaterial(i).SpecularColor = video::SColor(50,50,50,50); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeBalloons[x]->getMaterial(i).AmbientColor  = video::SColor(150,150,150,150); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    nodeBalloons[x]->getMaterial(i).DiffuseColor  = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    nodeBalloons[x]->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeBalloons[x]->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeBalloons[x]->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	nodeBalloons[x]->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
	};
    };

    /// Aplly animations:
    for(int x=0; x<48; x++){
    /// Set animation frames limits:
    f32 framestart = nodeBalloons[x]->getStartFrame();
    f32 frameend = nodeBalloons[x]->getEndFrame()-1;
    nodeBalloons[x]->setFrameLoop(framestart,frameend);
    nodeBalloons[x]->setCurrentFrame(framestart);
    nodeBalloons[x]->setAnimationSpeed(0);
    nodeBalloons[x]->setLoopMode(false);
    };





    /// Loading Bar: #4
    LoadBar->setPercent(30); // 0 to 100
    LoadBar->draw();
    // LoadBar->setVisible(false);


	/// Create (Load Animated Object) COURTAIN BACKGROUND:
	nodeCourtainBackground = smgr->addAnimatedMeshSceneNode(smgr->getMesh("courtain.MD3"));
	scene::IMeshBuffer* buffer8 = smgr->getMesh("courtain.MD3")->getMesh(0)->getMeshBuffer(0);
    int vc8 = buffer8->getVertexCount();
    std::cout << "Courtain n.1 vertex count: " << vc8 << std::endl;
	nodeCourtainBackground->setPosition(vector3df(0, 100, -1190));
	nodeCourtainBackground->setRotation(vector3df(0, 180, 0));
	nodeCourtainBackground->setScale(core::vector3df(1.0,1.0,1.0));
	nodeCourtainBackground->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeCourtainBackground->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeCourtainBackground->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    // nodeCourtainBackground->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER
    nodeCourtainBackground->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeCourtainBackground->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

	for(u32 i=0; i<nodeCourtainBackground->getMaterialCount(); i++){
	nodeCourtainBackground->getMaterial(i).setTexture(0, driver->getTexture("courtain_background.jpg")); /// JPG - Apply texture to my specified material
	nodeCourtainBackground->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeCourtainBackground->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeCourtainBackground->getMaterial(i).getTextureMatrix(1).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture

    /// Start Colors:
	/// Set size of specular highlights
	nodeCourtainBackground->getMaterial(i).Lighting = true;
	nodeCourtainBackground->getMaterial(i).Shininess = 1.0f; /// Reflex type: 20.0f == Metallic
    /// Affect the colour of the reflected light:
    nodeCourtainBackground->getMaterial(i).SpecularColor = video::SColor(5,5,5,5); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeCourtainBackground->getMaterial(i).AmbientColor  = video::SColor(100,100,100,100); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    nodeCourtainBackground->getMaterial(i).DiffuseColor  = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    nodeCourtainBackground->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeCourtainBackground->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeCourtainBackground->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	nodeCourtainBackground->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
	};



	/// Create (Load Animated Object) COURTAIN #2 - HALF-LEFT:
	nodeCourtain2 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("courtain_left.obj"));
	scene::IMeshBuffer* buffer82 = smgr->getMesh("courtain_left.obj")->getMesh(0)->getMeshBuffer(0);
    int vc82 = buffer82->getVertexCount();
    std::cout << "Courtain n.2 vertex count: " << vc82 << std::endl;
	nodeCourtain2->setPosition(vector3df(587, 239, -450));
	nodeCourtain2->setRotation(vector3df(-90, 0, 0));
	nodeCourtain2->setScale(core::vector3df(1.0,1.0,1.0));
	nodeCourtain2->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeCourtain2->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeCourtain2->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    // nodeCourtain2->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER
    nodeCourtain2->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeCourtain2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

	for(u32 i=0; i<nodeCourtain2->getMaterialCount(); i++){
	nodeCourtain2->getMaterial(i).setTexture(0, driver->getTexture("courtain_left.jpg")); /// JPG (Was a PNG) - Apply texture to my specified material
	nodeCourtain2->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeCourtain2->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeCourtain2->getMaterial(i).getTextureMatrix(1).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture

    /// Start Colors:
	/// Set size of specular highlights
	nodeCourtain2->getMaterial(i).Lighting = true;
	nodeCourtain2->getMaterial(i).Shininess = 1.0f; /// Reflex type: 20.0f == Metallic
    /// Affect the colour of the reflected light:
    nodeCourtain2->getMaterial(i).SpecularColor = video::SColor(5,5,5,5); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeCourtain2->getMaterial(i).AmbientColor  = video::SColor(100,100,100,100); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    nodeCourtain2->getMaterial(i).DiffuseColor  = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    nodeCourtain2->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

    nodeCourtain2->getMaterial(i).BackfaceCulling = false;
	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeCourtain2->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeCourtain2->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
    nodeCourtain2->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
    /// nodeCourtain2->getMaterial(i).MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;  /// EMT_TRANSPARENT_ALPHA_CHANNEL_REF
	};


	/// Create (Load Animated Object) COURTAIN #3 - HALF-RIGHT:
	nodeCourtain3 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("courtain_right.obj"));
	scene::IMeshBuffer* buffer83 = smgr->getMesh("courtain_right.obj")->getMesh(0)->getMeshBuffer(0);
    int vc83 = buffer83->getVertexCount();
    std::cout << "Courtain n.3 vertex count: " << vc83 << std::endl;
	nodeCourtain3->setPosition(vector3df(-587, 239, -450));
	nodeCourtain3->setRotation(vector3df(-90, 0, 0));
	nodeCourtain3->setScale(core::vector3df(1.0,1.0,1.0));
	nodeCourtain3->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeCourtain3->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeCourtain3->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    // nodeCourtain3->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER
    nodeCourtain3->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeCourtain3->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

	for(u32 i=0; i<nodeCourtain3->getMaterialCount(); i++){
	nodeCourtain3->getMaterial(i).setTexture(0, driver->getTexture("courtain_right.jpg")); /// JPG (Was a PNG) - Apply texture to my specified material
	nodeCourtain3->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeCourtain3->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeCourtain3->getMaterial(i).getTextureMatrix(1).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture

    /// Start Colors:
	/// Set size of specular highlights
	nodeCourtain3->getMaterial(i).Lighting = true;
	nodeCourtain3->getMaterial(i).Shininess = 1.0f; /// Reflex type: 20.0f == Metallic
    /// Affect the colour of the reflected light:
    nodeCourtain3->getMaterial(i).SpecularColor = video::SColor(5,5,5,5); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeCourtain3->getMaterial(i).AmbientColor  = video::SColor(100,100,100,100); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    nodeCourtain3->getMaterial(i).DiffuseColor  = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    nodeCourtain3->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

    nodeCourtain3->getMaterial(i).BackfaceCulling = false;
	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeCourtain3->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeCourtain3->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
    nodeCourtain3->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
    /// nodeCourtain3->getMaterial(i).MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;  /// EMT_TRANSPARENT_ALPHA_CHANNEL_REF
	};

	// nodeStage->setMaterialType(video::EMT_DETAIL_MAP); // Needed for multitexturing (Apply (mix) texture to my specified material)
    // Create disco balls spot light effects:
	// node8->setMaterialTexture(0, driver->getTexture("chao.png"));  // The default, loaded texture
	// node8->setMaterialTexture(1, driver->getTexture("reflexes.jpg"));
	// node8->setMaterialType(video::EMT_DETAIL_MAP);                    // Needed for multitexturing



	/// Create (Loads Object) DISCO BALL N1:
	scene::IAnimatedMeshSceneNode* nodeDiscoBall1 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("disco_ball.obj"));
	nodeDiscoBall1->setPosition(vector3df(110,280,-601));
	// nodeDiscoBall1->setRotation(vector3df(0, 180, 0));
	nodeDiscoBall1->setScale(core::vector3df(0.7,0.7,0.7));
	nodeDiscoBall1->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeDiscoBall1->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeDiscoBall1->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    nodeDiscoBall1->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeDiscoBall1->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeDiscoBall1->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER

	for(u32 i=0; i<nodeDiscoBall1->getMaterialCount(); i++){
	nodeDiscoBall1->getMaterial(i).setTexture(0, driver->getTexture("disco_ball.jpg")); // Apply texture to my specified material
	nodeDiscoBall1->getMaterial(i).getTextureMatrix(0).setTextureScale(3.0f,1.0f);   /// Repeat (tile) the texture
	nodeDiscoBall1->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeDiscoBall1->getMaterial(i).getTextureMatrix(1).setTextureScale(0.015f,0.015f); /// Repeat (tile) the texture

	/// Start Colors:
	/// Set size of specular highlights
	nodeDiscoBall1->getMaterial(i).Lighting = true;
	nodeDiscoBall1->getMaterial(i).Shininess = 100.0f; /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
    /// Affect the colour of the reflected light:
    nodeDiscoBall1->getMaterial(i).SpecularColor = video::SColor(255,250,250,250); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeDiscoBall1->getMaterial(i).AmbientColor  = video::SColor(200,200,200,200); /// How much ambient light (a global light) is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeDiscoBall1->getMaterial(i).DiffuseColor  = video::SColor(200,200,200,200); /// How much diffuse light coming from a light source is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeDiscoBall1->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// Light emitted by this material. Default is to emit no light (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeDiscoBall1->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeDiscoBall1->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	nodeDiscoBall1->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
	};

    scene::ISceneNodeAnimator* anim11 = smgr->createRotationAnimator(core::vector3df(0,1.0f,0));
    nodeDiscoBall1->addAnimator(anim11);
    anim11->drop();

    /// Create (CLONE Object) DISCO BALL N2:
    scene::ISceneNode* nodeDiscoBall2 = nodeDiscoBall1->clone();
	nodeDiscoBall2->setPosition(vector3df(-114,280,-601));
	nodeDiscoBall2->setScale(core::vector3df(0.7,0.7,0.7));
	nodeDiscoBall2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    /// Create (CLONE Object) DISCO BALL N3:
    scene::ISceneNode* nodeDiscoBall3 = nodeDiscoBall1->clone();
	nodeDiscoBall3->setPosition(vector3df(-344,320,-518));
	nodeDiscoBall3->setScale(core::vector3df(0.9,0.9,0.9));
	nodeDiscoBall3->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    /// Create (CLONE Object) DISCO BALL N4:
    scene::ISceneNode* nodeDiscoBall4 = nodeDiscoBall1->clone();
	nodeDiscoBall4->setPosition(vector3df(354,320,-518));
	nodeDiscoBall4->setScale(core::vector3df(0.9,0.9,0.9));
	nodeDiscoBall4->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);




	/// Create (Loads Object) FOCUS:
	scene::IAnimatedMeshSceneNode* nodeFocus1 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("focus_top.obj"));
	nodeFocus1->setPosition(vector3df(0,300,-495));
	// nodeFocus1->setRotation(vector3df(0, 180, 0));
	nodeFocus1->setScale(core::vector3df(1.0,1.0,1.0));
	nodeFocus1->setMaterialFlag(video::EMF_LIGHTING, true);            // Node is affected by LIGHT?
    nodeFocus1->setMaterialFlag(video::EMF_FOG_ENABLE, false);         // Node is affected by FOG?
    nodeFocus1->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);  // Render both sides?
    nodeFocus1->setMaterialType(video::EMT_REFLECTION_2_LAYER);        // EMT_REFLECTION_2_LAYER
    nodeFocus1->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);  // Increase view distance quality (similar to sharpness)
    nodeFocus1->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    // nodeFocus1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); /// Transparent material (for the focus PNG lights)

	for(u32 i=0; i<nodeFocus1->getMaterialCount(); i++){
	/// nodeFocus1->getMaterial(i).setTexture(0, driver->getTexture("focus.jpg")); // Apply texture to my specified material

	/// Start Colors:
	/// Set size of specular highlights
	nodeFocus1->getMaterial(i).Lighting = true;
	nodeFocus1->getMaterial(i).Shininess = 100.0f; /// Reflex type: 20.0f == Metallic
    /// Affect the colour of the reflected light:
    nodeFocus1->getMaterial(i).SpecularColor = video::SColor(255,200,200,200); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeFocus1->getMaterial(i).AmbientColor  = video::SColor(255,255,255,255); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    nodeFocus1->getMaterial(i).DiffuseColor  = video::SColor(255,255,255,255); /// (a (0 = transparent | 255 = opaque), r, g, b)
    nodeFocus1->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeFocus1->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeFocus1->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	};

    scene::ISceneNodeAnimator* anim20 = smgr->createRotationAnimator(core::vector3df(0,1.5f,0));
    nodeFocus1->addAnimator(anim20);
    anim20->drop();

    /// Create (CLONE Object) FOCUS:
    scene::ISceneNode* nodeFocus2 = nodeFocus1->clone();
	nodeFocus2->setPosition(vector3df(-200,300,-495));
	nodeFocus2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    /// Create (CLONE Object) FOCUS:
    scene::ISceneNode* nodeFocus3 = nodeFocus1->clone();
	nodeFocus3->setPosition(vector3df(200,300,-495));
	nodeFocus3->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);




	/// Create (Loads Object) FOCUS SMOKE LIGHT:
	scene::IAnimatedMeshSceneNode* nodeFocusSmokeLight1 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("focus_top_light.obj"));
	nodeFocusSmokeLight1->setPosition(vector3df(0,300,-495));
	// nodeFocus1->setRotation(vector3df(0, 180, 0));
	nodeFocusSmokeLight1->setScale(core::vector3df(1.0,1.0,1.0));
	nodeFocusSmokeLight1->setMaterialFlag(video::EMF_LIGHTING, true);            // Node is affected by LIGHT?
    nodeFocusSmokeLight1->setMaterialFlag(video::EMF_FOG_ENABLE, false);         // Node is affected by FOG?
    nodeFocusSmokeLight1->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);  // Render both sides?
    // nodeFocusSmokeLight1->setMaterialType(video::EMT_REFLECTION_2_LAYER);        // EMT_REFLECTION_2_LAYER
    nodeFocusSmokeLight1->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);  // Increase view distance quality (similar to sharpness)
    nodeFocusSmokeLight1->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeFocusSmokeLight1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); /// Transparent material (for the focus PNG lights)

	for(u32 i=0; i<nodeFocusSmokeLight1->getMaterialCount(); i++){
	/// nodeFocusSmokeLight1->getMaterial(i).setTexture(0, driver->getTexture("focus.jpg")); // Apply texture to my specified material

	/// Start Colors:
	/// Set size of specular highlights
	nodeFocusSmokeLight1->getMaterial(i).Lighting = true;
	nodeFocusSmokeLight1->getMaterial(i).Shininess = 0.5; /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
    /// Affect the colour of the reflected light:
    nodeFocusSmokeLight1->getMaterial(i).SpecularColor = video::SColor(200,200,200,200); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeFocusSmokeLight1->getMaterial(i).AmbientColor  = video::SColor(200,200,200,200); /// How much ambient light (a global light) is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeFocusSmokeLight1->getMaterial(i).DiffuseColor  = video::SColor(200,200,200,200); /// How much diffuse light coming from a light source is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeFocusSmokeLight1->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// Light emitted by this material. Default is to emit no light (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeFocusSmokeLight1->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeFocusSmokeLight1->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	};

    scene::ISceneNodeAnimator* anim22 = smgr->createRotationAnimator(core::vector3df(0,1.5f,0));
    nodeFocusSmokeLight1->addAnimator(anim22);
    anim22->drop();

    /// Create (CLONE Object) FOCUS:
    scene::ISceneNode* nodeFocusSmokeLight2 = nodeFocusSmokeLight1->clone();
	nodeFocusSmokeLight2->setPosition(vector3df(-200,300,-495));
	nodeFocusSmokeLight2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    /// Create (CLONE Object) FOCUS:
    scene::ISceneNode* nodeFocusSmokeLight3 = nodeFocusSmokeLight1->clone();
	nodeFocusSmokeLight3->setPosition(vector3df(200,300,-495));
	nodeFocusSmokeLight3->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);






	/// Create (Loads Object) FOCUS FLOOR:
	scene::IAnimatedMeshSceneNode* nodeFocusFloor1 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("focus_floor.obj"));
	nodeFocusFloor1->setPosition(vector3df(0,300,-495));
	// nodeFocusFloor1->setRotation(vector3df(0, 180, 0));
	nodeFocusFloor1->setScale(core::vector3df(1.0,1.0,1.0));
	nodeFocusFloor1->setMaterialFlag(video::EMF_LIGHTING, true);            // Node is affected by LIGHT?
    nodeFocusFloor1->setMaterialFlag(video::EMF_FOG_ENABLE, false);         // Node is affected by FOG?
    nodeFocusFloor1->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);  // Render both sides?
    /// nodeFocusFloor1->setMaterialType(video::EMT_REFLECTION_2_LAYER);    // EMT_REFLECTION_2_LAYER
    nodeFocusFloor1->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);  // Increase view distance quality (similar to sharpness)
    nodeFocusFloor1->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeFocusFloor1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); /// Transparent material (for the focus PNG lights)

	for(u32 i=0; i<nodeFocusFloor1->getMaterialCount(); i++){
	/// nodeFocusFloor1->getMaterial(i).setTexture(0, driver->getTexture("focus.jpg")); // Apply texture to my specified material
	nodeFocusFloor1->getMaterial(i).Lighting = true;
    // nodeFocusFloor1->getMaterial(i).Shininess = 50.0f; // Set size of specular highlights (20.0f == Metallic)
	// nodeFocusFloor1->getMaterial(i).SpecularColor.set(20,255,255,255);
	// nodeFocusFloor1->getMaterial(i).DiffuseColor.set(255,255,255,255);
	// nodeFocusFloor1->getMaterial(i).EmissiveColor.set(255,255,255,255);
	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeFocusFloor1->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeFocusFloor1->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	};

    scene::ISceneNodeAnimator* animff = smgr->createRotationAnimator(core::vector3df(0,2.0f,0));
    nodeFocusFloor1->addAnimator(animff);
    animff->drop();

    /// Create (CLONE Object) FOCUS FLOOR:
    scene::ISceneNode* nodeFocusFloor2 = nodeFocusFloor1->clone();
	nodeFocusFloor2->setPosition(vector3df(-200,300,-495));

    /// Create (CLONE Object) FOCUS FLOOR:
    scene::ISceneNode* nodeFocusFloor3 = nodeFocusFloor1->clone();
	nodeFocusFloor3->setPosition(vector3df(200,300,-495));


    /// Loading Bar: #5
    LoadBar->setPercent(40); // 0 to 100
    LoadBar->draw();
    // LoadBar->setVisible(false);



	/// Create (Loads Object) TV BACK:
	// scene::ISceneNode* nodeTVback1 = smgr->addSceneNode(smgr->getMesh("tv.obj"));
	// scene::IAnimatedMesh* nodeTVback1 = smgr->getMesh("tv.obj");
	scene::IMeshSceneNode* nodeTVback1 = smgr->addMeshSceneNode(smgr->getMesh("tv_back.obj")->getMesh(0));
	scene::IMeshBuffer* bufferTV1 = smgr->getMesh("tv_back.obj")->getMesh(0)->getMeshBuffer(0);
    int vc2 = bufferTV1->getVertexCount();
    std::cout << "TV-Back1 vertex count: " << vc2 << std::endl;
    nodeTVback1->setName("TV-Back");
	nodeTVback1->setPosition(vector3df(0, 100, 0.0));
	nodeTVback1->setRotation(vector3df(0, 0, 0));
	nodeTVback1->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeTVback1->setMaterialFlag(video::EMF_LIGHTING, true);            // Node is affected by LIGHT?
    nodeTVback1->setMaterialFlag(video::EMF_FOG_ENABLE, false);         // Node is affected by FOG?
    nodeTVback1->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);  // Render both sides?
    nodeTVback1->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);  // Increase view distance quality (similar to sharpness)
    nodeTVback1->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    // nodeTVback1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); /// EMT_TRANSPARENT_ALPHA_CHANNEL_REF

    /// Static texture:
    /// video::ITexture* textureTVbackStopped = driver->getTexture("datas/imp-tv/0.jpg");

    /// Animated textures for TV 1:
    if(nodeTVback1){
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Load TV1 images:" << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    // Load textures for animation:
    core::array<video::ITexture*> texturesTVbackPlaying;
    for(s32 n=1; n<50; ++n){ // Total images - 1!
    core::stringc tmp = "";
    tmp = "television_N1_"; tmp += n; tmp += ".jpg"; // Path to images: included in the "world.zip" file
    video::ITexture* tx = driver->getTexture(tmp.c_str());
    if(tx!=NULL){ texturesTVbackPlaying.push_back(tx); }; // image found?... Add it!
    };

    scene::ISceneNodeAnimator* animatorTVback = smgr->createTextureAnimator(texturesTVbackPlaying, 1500, true); // Time between TV images (textures)
    nodeTVback1->addAnimator(animatorTVback);
    animatorTVback->drop();

 	for(u32 i=0; i<nodeTVback1->getMaterialCount(); i++){

	/// Start Colors:
	/// Set size of specular highlights
	nodeTVback1->getMaterial(i).Lighting = true;
	nodeTVback1->getMaterial(i).Shininess = 125; /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
    /// Affect the colour of the reflected light:
    nodeTVback1->getMaterial(i).SpecularColor = video::SColor(200,150,150,150); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeTVback1->getMaterial(i).AmbientColor  = video::SColor(200,150,150,150); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    nodeTVback1->getMaterial(i).DiffuseColor  = video::SColor(200,150,150,150); /// (a (0 = transparent | 255 = opaque), r, g, b)
    nodeTVback1->getMaterial(i).EmissiveColor = video::SColor(200,255,255,255); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeTVback1->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeTVback1->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	};
	};





    // nodeTVback->getMaterial(0).setTexture(0, textureTVbackStopped); // Apply texture to my specified material

    /// Create (CLONE Object) TV BACK:
    scene::ISceneNode* nodeTVback2 = nodeTVback1->clone();
	nodeTVback2->setPosition(vector3df(-258, 120, -390.0));
	nodeTVback2->setRotation(vector3df(0, -67, 0));
	nodeTVback2->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeTVback2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	/// Animated textures for TV 2:
    if(nodeTVback2){
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Load TV2 images:" << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    // Load textures for animation:
    core::array<video::ITexture*> texturesTVbackPlaying;
    for(s32 n=1; n<10; ++n){ // Total images - 1!
    core::stringc tmp = "";
    tmp = "television_N2_"; tmp += n; tmp += ".jpg"; // Path to images: included in the "world.zip" file
    video::ITexture* tx = driver->getTexture(tmp.c_str());
    if(tx!=NULL){ texturesTVbackPlaying.push_back(tx); }; // image found?... Add it!
    };
    scene::ISceneNodeAnimator* animatorTVback = smgr->createTextureAnimator(texturesTVbackPlaying, 5000, true); // Time between TV images (textures)
    nodeTVback2->addAnimator(animatorTVback);
    animatorTVback->drop();
    };

    /// Create (CLONE Object) TV BACK:
    scene::ISceneNode* nodeTVback3 = nodeTVback1->clone();
	nodeTVback3->setPosition(vector3df(269, 120, -390.0));
	nodeTVback3->setRotation(vector3df(0, 67, 0));
	nodeTVback3->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeTVback3->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	/// Animated textures for TV 3:
    if(nodeTVback3){
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Load TV3 images:" << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    // Load textures for animation:
    core::array<video::ITexture*> texturesTVbackPlaying;
    for(s32 n=1; n<10; ++n){ // Total images - 1!
    core::stringc tmp = "";
    tmp = "television_N3_"; tmp += n; tmp += ".jpg"; // Path to images: included in the "world.zip" file
    video::ITexture* tx = driver->getTexture(tmp.c_str());
    if(tx!=NULL){ texturesTVbackPlaying.push_back(tx); }; // image found?... Add it!
    };
    scene::ISceneNodeAnimator* animatorTVback = smgr->createTextureAnimator(texturesTVbackPlaying, 5000, true); // Time between TV images (textures)
    nodeTVback3->addAnimator(animatorTVback);
    animatorTVback->drop();
    };





	/// Create (Loads Object) TV FRONT: (The TV Hardware: buttons, leds, logo, etc)
	// scene::ISceneNode* nodeTVfront1 = smgr->addSceneNode(smgr->getMesh("tv.obj"));
	// scene::IAnimatedMesh* nodeTVfront1 = smgr->getMesh("tv.obj");
	scene::IMeshSceneNode* nodeTVfront1 = smgr->addMeshSceneNode(smgr->getMesh("tv_front.obj")->getMesh(0));
	scene::IMeshBuffer* buffer2f = smgr->getMesh("tv_front.obj")->getMesh(0)->getMeshBuffer(0);
    int vc2f = buffer2f->getVertexCount();
    std::cout << "TV-Front vertex count: " << vc2f << std::endl;
    nodeTVfront1->setName("TV-Front");
	nodeTVfront1->setPosition(vector3df(0, 100, 0.2));
	nodeTVfront1->setRotation(vector3df(0, 0, 0));
	nodeTVfront1->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeTVfront1->setMaterialFlag(video::EMF_LIGHTING, true);            // Node is affected by LIGHT?
    nodeTVfront1->setMaterialFlag(video::EMF_FOG_ENABLE, false);         // Node is affected by FOG?
    nodeTVfront1->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);  // Render both sides?
    nodeTVfront1->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);  // Increase view distance quality (similar to sharpness)
    nodeTVfront1->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeTVfront1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); /// EMT_TRANSPARENT_ALPHA_CHANNEL_REF

    // Create texture animator
    if(nodeTVfront1){
    // Load textures for animation:
    core::array<video::ITexture*> texturesTVfront;
    for(s32 n=1; n<2; ++n){ // Total images (minus) - 1 !
    core::stringc tmp = "";
    tmp = "tv_"; tmp += n; tmp += ".png"; // Path to images: "tv" directory, included in the "world.zip" file
    video::ITexture* tx = driver->getTexture(tmp.c_str());
    texturesTVfront.push_back(tx);
    };
    scene::ISceneNodeAnimator* animatorTVfront = smgr->createTextureAnimator(texturesTVfront, 300, true); // Time between TV images (textures)
    nodeTVfront1->addAnimator(animatorTVfront);
    animatorTVfront->drop();
    };

	for(u32 i=0; i<nodeTVfront1->getMaterialCount(); i++){
	nodeTVfront1->getMaterial(i).setTexture(0, driver->getTexture("tv_1.png")); // Apply texture to my specified material

	/// Start Colors:
	/// Set size of specular highlights
	nodeTVfront1->getMaterial(i).Lighting = true;
	nodeTVfront1->getMaterial(i).Shininess = 5.0f; /// Reflex type: 20.0f == Metallic
    /// Affect the colour of the reflected light:
    nodeTVfront1->getMaterial(i).SpecularColor = video::SColor(150,10,10,10); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeTVfront1->getMaterial(i).AmbientColor  = video::SColor(50,10,10,10); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    nodeTVfront1->getMaterial(i).DiffuseColor  = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    nodeTVfront1->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeTVfront1->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeTVfront1->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	};


	/// Create (CLONE Object) TV FRONT:
    scene::ISceneNode* nodeTVfront2 = nodeTVfront1->clone();
	nodeTVfront2->setPosition(vector3df(-258.2, 120, -390.0));
	nodeTVfront2->setRotation(vector3df(0, -67, 0));
	nodeTVfront2->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeTVfront2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

	/// Create (CLONE Object) TV FRONT:
    scene::ISceneNode* nodeTVfront3 = nodeTVfront1->clone();
	nodeTVfront3->setPosition(vector3df(269.2, 120, -390.0));
	nodeTVfront3->setRotation(vector3df(0, 67, 0));
	nodeTVfront3->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeTVfront3->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);


    std::cout << "Get other meshes... Ok" << std::endl;



    /// Loading Bar: #6
    LoadBar->setPercent(50); // 0 to 100
    LoadBar->draw();
    // LoadBar->setVisible(false);




    /// ############# CREATE LIGHTS ################################################################


    /// Create main LIGHT:
    /// smgr->setAmbientLight(video::SColorf(200,200,255,0)); // Ambient Light. SColorf == r,g,b,a
    smgr->setAmbientLight(video::SColorf(0.50,0.50,0.50,0.0)); // Ambient Light. SColorf == r,g,b,a

    /// Create FOG: (E_FOG_TYPE ---> EFT_FOG_EXP or EFT_FOG_LINEAR or EFT_FOG_EXP2)
    // SYNTAX: driver->setFog(SColor, E_FOG_TYPE, f32, f32, f32, bool, bool);
    driver->setFog(video::SColor(0,0.0,0.0,0.0), video::EFT_FOG_LINEAR, 250, 1000, .003f, true, false);
    // node->setMaterialFlag(video::EMF_FOG_ENABLE, true);


	/// Create DECAL manager:
	/*
    // Create decal manager
    DecalManager* decalManager = new DecalManager(smgr);
    // Set the target node that will have decals:
    decalManager->addMesh(nodeStageFloor);
    // nodeBand->setPosition(vector3df(600, 96, -520));  /// Out of stage
    // Create a decal
    irr::core::vector3df position = irr::core::vector3df(0,110,-510); // Position to place the decal
    irr::core::vector3df dimension = irr::core::vector3df(20,20,20);  // Dimension of decal
    irr::core::vector3df normal = irr::core::vector3df(0,1,0);        // Orientation of the decal
    irr::f32 textureRotation = 0;                                     // Rotation in degrees
    irr::scene::ISceneNode* parent = 0;                               // Parent
    irr::f32 lifeTime = 160;                                          // Time to life (in seconds)
    irr::f32 distance = 1000;                                         // Max viewing distance

    decalManager->addDecal("decal_01.jpg", position, dimension, normal, textureRotation, parent, lifeTime, distance);
    */


    /// Create Spot LIGHT:
    /*
    scene::ISceneNode* node_SP1 = 0;
    scene::IBillboardSceneNode* billboard_SP1 = smgr->addBillboardSceneNode(node_SP1);
    billboard_SP1->setPosition(core::vector3df(0,200,-350));
    billboard_SP1->setMaterialTexture(0, driver->getTexture("dummy.png")); /// Invisible: "dummy.png" - Visible: "estrela.png"
    billboard_SP1->setMaterialFlag(video::EMF_LIGHTING, false);
    billboard_SP1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
    billboard_SP1->setScale(core::vector3df(450,450,450));
    */
    light_SP1 = smgr->addLightSceneNode(0, core::vector3df(0,200,-350), video::SColorf(1.0,1.0,1.0,0.0), 1000); // SColorf: 0.0f to 1.0f full color. SColorf = r,g,b,a
    // light_SP1->setPosition(vector3df(600, 96, -520));  /// Out of stage
	light_SP1->setRotation(vector3df(160.0,0.0,0.0)); // X90 = Middle Stage & Y90 = left, Y-90 = right
	light_SP1->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    data_SP1.Type = ELT_SPOT; // ELT_POINT | ELT_SPOT | ELT_DIRECTIONAL
    data_SP1.InnerCone = 20.0f;
    data_SP1.OuterCone = 30.0f;
    data_SP1.Falloff   =  5.0f;
    // data_SP1.Direction     = core::vector3df(0.0, 0.0, -1.0);
    data_SP1.AmbientColor  = video::SColorf(1.0, 0.0, 0.0, 0.0);
    data_SP1.DiffuseColor  = video::SColorf(1.0, 0.0, 0.0, 0.0);
    data_SP1.SpecularColor = video::SColorf(1.0, 0.0, 0.0, 0.0);
    light_SP1->setLightData(data_SP1); // Apply data to the light
    light_SP1->enableCastShadow(true);
    light_SP1->setVisible(true);



    /// Create Main Colored LIGHT, does cast shadows:
    scene::ISceneNode* node_ML1 = 0;
    scene::IBillboardSceneNode* billboard_ML1 = smgr->addBillboardSceneNode(node_ML1);
    billboard_ML1->setPosition(core::vector3df(0,190,-420));
    billboard_ML1->setMaterialTexture(0, driver->getTexture("dummy.png")); /// Invisible: "dummy.png" - Visible: "estrela.png"
    billboard_ML1->setMaterialFlag(video::EMF_LIGHTING, false);
    billboard_ML1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
    billboard_ML1->setScale(core::vector3df(450,450,450));

    // Add animator:
    scene::ISceneNodeAnimator* anim_ML1 = smgr->createFlyCircleAnimator(core::vector3df(0,190,-420), 3.0f, 0.0030f);
    billboard_ML1->addAnimator(anim_ML1);
    anim_ML1->drop();

    /// Create the light: (This is the main light responsible for all the stage colors)
    light_ML1 = smgr->addLightSceneNode(billboard_ML1, vector3df(0,0,0), video::SColorf(1.0,1.0,1.0,0.0), 3000); // SColorf: 0.0f to 1.0f full color. SColorf = r,g,b,a
    light_ML1->enableCastShadow(true);
    light_ML1->setVisible(true);
    data_ML1.Type = video::ELT_POINT; // ELT_POINT | ELT_SPOT | ELT_DIRECTIONAL
    // data_ML1.Direction = vector3df(0,-1,0);
    data_ML1.AmbientColor  = video::SColorf(0,0,0,0); // Will change in main loop
    data_ML1.SpecularColor = video::SColorf(0,0,0,0); // Will change in main loop
    data_ML1.DiffuseColor  = video::SColorf(0,0,0,0); // Will change in main loop
    light_ML1->setLightData(data_ML1); // Apply data to the light (Will change in main loop)




    /// Create White Flash Strobe LIGHT:
    scene::ISceneNode* node_FL1 = 0;
    scene::IBillboardSceneNode* billboard_FL1 = smgr->addBillboardSceneNode(node_FL1);
    billboard_FL1->setPosition(vector3df(0,180,-420));
    billboard_FL1->setMaterialTexture(0, driver->getTexture("dummy.png")); /// Invisible: "dummy.png" - Visible: "estrela.png"
    billboard_FL1->setMaterialFlag(video::EMF_LIGHTING, false);
    billboard_FL1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
    billboard_FL1->setScale(core::vector3df(450,450,450));
    // Add animator:
    scene::ISceneNodeAnimator* anim_FL1 = smgr->createFlyCircleAnimator(core::vector3df(0,180,-420), 5.0f, 0.0030f);
    billboard_FL1->addAnimator(anim_FL1);
    anim_FL1->drop();

    /// Create the light: (This is the main light responsible for all the stage colors)
    light_FL1 = smgr->addLightSceneNode(billboard_FL1, vector3df(0,0,0), video::SColorf(1.0,1.0,1.0,0.0), 3000); // SColorf: 0.0f to 1.0f full color. SColorf = r,g,b,a
    light_FL1->enableCastShadow(true);
    light_FL1->setVisible(false);
    data_FL1.Type = video::ELT_POINT; // ELT_POINT | ELT_SPOT | ELT_DIRECTIONAL
    // data_FL1.Direction = vector3df(0,45,0);
    data_FL1.AmbientColor  = video::SColorf(0.9,0.9,0.9,1.0); // SColorf = r,g,b,a
    data_FL1.SpecularColor = video::SColorf(0.9,0.9,0.9,1.0); // SColorf = r,g,b,a
    data_FL1.DiffuseColor  = video::SColorf(0.9,0.9,0.9,1.0); // SColorf = r,g,b,a
    light_FL1->setLightData(data_FL1); // Apply data to the light




    /// Create small FAKE LIGHTS: (6 columns x 8 lights = 48 lights per column)
    /// scene::ILightSceneNode* light[48];
    scene::ISceneNode* light[48]; /// Fake Lights Array
    scene::ISceneNode* bill[48];  // Bills Array

    // Lights coordinates arrays, per column:
    irr::f32 aX[48] = { // Lights X axis:
     258, 258, 258, 258, 258, 258,   // Column 1
     210, 210, 210, 210, 210, 210,   // Column 2
     158, 158, 158, 158, 158, 158,   // Column 3
     110, 110, 110, 110, 110, 110,   // Column 4
    -112,-112,-112,-112,-112,-112,   // Column 5
    -161,-161,-161,-161,-161,-161,   // Column 6
    -213,-213,-213,-213,-213,-213,   // Column 7
    -261,-261,-261,-261,-261,-261 }; // Column 8

    irr::f32 aY[48] = { // Lights Y axis:
    243, 225, 207, 189, 171, 153,    // Column 1
    243, 225, 207, 189, 171, 153,    // Column 2
    243, 225, 207, 189, 171, 153,    // Column 3
    243, 225, 207, 189, 171, 153,    // Column 4
    243, 225, 207, 189, 171, 153,    // Column 5
    243, 225, 207, 189, 171, 153,    // Column 6
    243, 225, 207, 189, 171, 153,    // Column 7
    243, 225, 207, 189, 171, 153 };  // Column 8

    irr::f32 aZ[48] = { // Lights Z axis:
    -593,-592,-590,-589,-588,-587,   // Column 1
    -593,-592,-590,-589,-588,-587,   // Column 2
    -593,-592,-590,-589,-588,-587,   // Column 3
    -593,-592,-590,-589,-588,-587,   // Column 4
    -593,-592,-590,-589,-588,-587,   // Column 5
    -593,-592,-590,-589,-588,-587,   // Column 6
    -593,-592,-590,-589,-588,-587,   // Column 7
    -593,-592,-590,-589,-588,-587 }; // Column 8


    /// Add 6 FAKE lights per column, loop:
    for(int i=0; i<48; i++){ // Loop trought lights (6 Lights X 8 columns = 48 Lights)
	light[i] = smgr->addEmptySceneNode(); /// Fake Lights
	light[i]->setPosition(core::vector3df(aX[i],aY[i],aZ[i]));
	// bill[column][i] = smgr->addBillboardSceneNode(light[column][i], core::dimension2d<f32>(60,30)); // Attach billboard to light.
	bill[i] = smgr->addBillboardSceneNode(light[i], core::dimension2d<f32>(85,300)); // Attach billboard to light.
	bill[i]->setMaterialFlag(video::EMF_LIGHTING, false);   // Bill Node is affected by LIGHT?
	bill[i]->setMaterialFlag(video::EMF_FOG_ENABLE, false); // Bill Node is affected by FOG?
	bill[i]->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	bill[i]->setMaterialTexture(0, driver->getTexture("estrela_vertical.png"));
    /*
	/// Start Colors:
	/// Set size of specular highlights
	bill[i]->getMaterial(0).Lighting = true;
	bill[i]->getMaterial(0).Shininess = 50.0f; /// Reflex type: 20.0f == Metallic
    /// Affect the colour of the reflected light:
    bill[i]->getMaterial(0).SpecularColor = video::SColor(255,255,255,255); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    bill[i]->getMaterial(0).AmbientColor  = video::SColor(255,255,255,255); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    bill[i]->getMaterial(0).DiffuseColor  = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    bill[i]->getMaterial(0).EmissiveColor = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors
    */
    };

    std::cout << "Lights... Ok" << std::endl;



    /// Loading Bar: #7
    LoadBar->setPercent(60); // 0 to 100
    LoadBar->draw();
    // LoadBar->setVisible(false);




    /// Create LASER LIGHTS:
    // Get images:
    core::array<irr::video::ITexture*> texturesLaser;
    for(s32 n=0; n<13; n++){
    core::stringc tmp; tmp = "laser_light_"; tmp += n; tmp += ".jpg";
    video::ITexture* tx = driver->getTexture(tmp.c_str());
    texturesLaser.push_back(tx);
    };
    /// Create LASER LIGHTS group for left:
    // Light nodes:
    for(int i=0; i<8; i++){
    nodeLaserA[i] = smgr->addVolumeLightSceneNode(0, -1, 4, 4, video::SColor(0, 255, 255, 255), video::SColor(0, 0, 0, 0)); // Set: | ? | ? | Subdivisions on U axis | Subdivisions on V axis | foot color | tail color
    // nodeLaserA[i]->enableCastShadow(false);
    nodeLaserA[i]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    nodeLaserA[i]->setScale(core::vector3df(0.35f,900.0f,0.1f));
    nodeLaserA[i]->setPosition(core::vector3df(150,384.1f,-583)); /// Position Y = 384.1f is perfect!
    // Create texture animator
    scene::ISceneNodeAnimator* glow = smgr->createTextureAnimator(texturesLaser, 250, true); // Time between images (textures)
    // Add the animator: G/// Create LASER LIGHTS at the left:low
    nodeLaserA[i]->addAnimator(glow);
    // Drop the animator because it was created with a create() function
    glow->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserA[0]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(0.1f,0,1.0f));
    nodeLaserA[0]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserA[1]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(0.1f,0,-1.0f));
    nodeLaserA[1]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserA[2]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(-0.1f,0,0.2f));
    nodeLaserA[2]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserA[3]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(-0.1f,0,-0.2f));
    nodeLaserA[3]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserA[4]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(0.1f,0,0.7f));
    nodeLaserA[4]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserA[5]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(0.1f,0,-0.7f));
    nodeLaserA[5]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserA[6]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(-0.1f,0,1.7f));
    nodeLaserA[6]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserA[7]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(-0.1f,0,-1.7f));
    nodeLaserA[7]->addAnimator(animLaser);
    animLaser->drop();
    };
    /// Hide lasers:
    for(int i=0; i<8; i++){ if(nodeLaserA[i]){ nodeLaserA[i]->setVisible(false); }; };

    /// Create LASER LIGHTS group for right:
    for(int i=0; i<8; i++){
    nodeLaserB[i] = smgr->addVolumeLightSceneNode(0, -1, 4, 4, video::SColor(0, 255, 255, 255), video::SColor(0, 0, 0, 0)); // Set: | ? | ? | Subdivisions on U axis | Subdivisions on V axis | foot color | tail color
    // nodeLaserB[i]->enableCastShadow(false);
    nodeLaserB[i]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    nodeLaserB[i]->setScale(core::vector3df(0.35f,900.0f,0.1f));
    nodeLaserB[i]->setPosition(core::vector3df(-150,384.1f,-583)); /// Position Y = 384.1f is perfect!
    // Create texture animator
    scene::ISceneNodeAnimator* glow = smgr->createTextureAnimator(texturesLaser, 250, true); // Time between images (textures)
    // Add the animator: Glow
    nodeLaserB[i]->addAnimator(glow);
    // Drop the animator because it was created with a create() function
    glow->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserB[0]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(0.1f,0,1.0f));
    nodeLaserB[0]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserB[1]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(0.1f,0,-1.0f));
    nodeLaserB[1]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserB[2]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(-0.1f,0,0.2f));
    nodeLaserB[2]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserB[3]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(-0.1f,0,-0.2f));
    nodeLaserB[3]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserB[4]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(0.1f,0,0.7f));
    nodeLaserB[4]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserB[5]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(0.1f,0,-0.7f));
    nodeLaserB[5]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserB[6]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(-0.1f,0,1.7f));
    nodeLaserB[6]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserB[7]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(-0.1f,0,-1.7f));
    nodeLaserB[7]->addAnimator(animLaser);
    animLaser->drop();
    };
    /// Hide lasers:
    for(int i=0; i<8; i++){ if(nodeLaserB[i]){ nodeLaserB[i]->setVisible(false); }; };

    std::cout << "Lasers... Ok" << std::endl;




    /// ##########################################################################################
    /// ############# PARTICLES ##################################################################
    /// ##########################################################################################

    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Creating particles system..." << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;

    /// Create particle system: (DISCO BALL N1 - reflexed lights)
	scene::IParticleSystemSceneNode* ps60 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em60 = ps60->createSphereEmitter( /// SPHERE Emitter (Same geometry as the mesh is)
	core::vector3df(0.0f,0.0f,0.0f),  // emitter position
    0.282f,                           // f32 radius
	core::vector3df(0.0f,0.0f,0.0f),  // initial direction & speed (all set to 0.0f == particles stopped)
	0,0,                              // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),      // darkest color
	video::SColor(0,0,0,0),           // darkest color
	video::SColor(255,255,255,255),   // brightest color
	90,200,0,                         // min age, max age, angle
	core::dimension2df( 5.0f, 5.0f),  // min size
	core::dimension2df(25.0f,25.0f)); // max size

	ps60->setEmitter(em60); // Grabs the emitter
	em60->drop(); // We can drop it here, without deleting it

	ps60->setParent(nodeDiscoBall1); /// Attach these reflexed lights to the Disco Ball N1 (Match to the same position & rotation)
	// ps60->setPosition(core::vector3df(-550,100,-500)); // x, y, z: profundidade
	// ps60->setScale(core::vector3df(100,100,100));
	ps60->setScale(core::vector3df(40,40,40));
	ps60->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps60->setMaterialTexture(0, driver->getTexture("estrela_squared.png"));
	// ps60->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps60->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	ps60->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps60->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps60);
	// ps60->getMaterial(0).Lighting = false;
    // ps60->getMaterial(0).Shininess = 100.0f; // Set size of specular highlights
	// ps60->getMaterial(0).SpecularColor.set(255,255,255,255);
	// ps60->getMaterial(0).DiffuseColor.set(255,255,255,255);
	// ps60->getMaterial(0).EmissiveColor.set(255,255,255,255);
	/// Disabled:
    em60->setMinParticlesPerSecond(0);
	em60->setMaxParticlesPerSecond(0);


    /// Create particle system: (DISCO BALL N2 - reflexed lights)
	scene::IParticleSystemSceneNode* ps61 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em61 = ps61->createSphereEmitter( /// SPHERE Emitter (Same geometry as the mesh is)
	core::vector3df(0.0f,0.0f,0.0f),  // emitter position
    0.282f,                           // f32 radius
	core::vector3df(0.0f,0.0f,0.0f),  // initial direction & speed (all set to 0.0f == particles stopped)
	0,0,                              // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),      // darkest color
	video::SColor(0,0,0,0),           // darkest color
	video::SColor(255,255,255,255),   // brightest color
	90,200,0,                         // min age, max age, angle
	core::dimension2df( 5.0f, 5.0f),  // min size
	core::dimension2df(25.0f,25.0f)); // max size

	ps61->setEmitter(em61); // Grabs the emitter
	em61->drop(); // We can drop it here, without deleting it

	ps61->setParent(nodeDiscoBall2); /// Attach these reflexed lights to the Disco Ball N2 (Match to the same position & rotation)
	// ps61->setPosition(core::vector3df(-550,100,-500)); // x, y, z: profundidade
	// ps61->setScale(core::vector3df(100,100,100));
	ps61->setScale(core::vector3df(40,40,40));
	ps61->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps61->setMaterialTexture(0, driver->getTexture("estrela_squared.png"));
	// ps61->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps61->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	ps61->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps61->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps61);
	// ps61->getMaterial(0).Lighting = false;
    // ps61->getMaterial(0).Shininess = 100.0f; // Set size of specular highlights
	// ps61->getMaterial(0).SpecularColor.set(255,255,255,255);
	// ps61->getMaterial(0).DiffuseColor.set(255,255,255,255);
	// ps61->getMaterial(0).EmissiveColor.set(255,255,255,255);
	/// Disabled:
    em61->setMinParticlesPerSecond(0);
	em61->setMaxParticlesPerSecond(0);


    /// Create particle system: (DISCO BALL N3 - reflexed lights)
	scene::IParticleSystemSceneNode* ps62 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em62 = ps62->createSphereEmitter( /// SPHERE Emitter (Same geometry as the mesh is)
	core::vector3df(0.0f,0.0f,0.0f),  // emitter position
    0.282f,                           // f32 radius
	core::vector3df(0.0f,0.0f,0.0f),  // initial direction & speed (all set to 0.0f == particles stopped)
	0,0,                              // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),      // darkest color
	video::SColor(0,0,0,0),           // darkest color
	video::SColor(255,255,255,255),   // brightest color
	90,200,0,                         // min age, max age, angle
	core::dimension2df( 5.0f, 5.0f),  // min size
	core::dimension2df(25.0f,25.0f)); // max size

	ps62->setEmitter(em62); // Grabs the emitter
	em62->drop(); // We can drop it here, without deleting it

	ps62->setParent(nodeDiscoBall3); /// Attach these reflexed lights to the Disco Ball N3 (Match to the same position & rotation)
	// ps62->setPosition(core::vector3df(-550,100,-500)); // x, y, z: profundidade
	// ps62->setScale(core::vector3df(100,100,100));
	ps62->setScale(core::vector3df(40,40,40));
	ps62->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps62->setMaterialTexture(0, driver->getTexture("estrela_squared.png"));
	// ps62->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps62->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	ps62->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps62->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps62);
	// ps62->getMaterial(0).Lighting = false;
    // ps62->getMaterial(0).Shininess = 100.0f; // Set size of specular highlights
	// ps62->getMaterial(0).SpecularColor.set(255,255,255,255);
	// ps62->getMaterial(0).DiffuseColor.set(255,255,255,255);
	// ps62->getMaterial(0).EmissiveColor.set(255,255,255,255);
	/// Disabled:
    em62->setMinParticlesPerSecond(0);
	em62->setMaxParticlesPerSecond(0);


    /// Create particle system: (DISCO BALL N4 - reflexed lights)
	scene::IParticleSystemSceneNode* ps63 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em63 = ps63->createSphereEmitter( /// SPHERE Emitter (Same geometry as the mesh is)
	core::vector3df(0.0f,0.0f,0.0f),  // emitter position
    0.282f,                           // f32 radius
	core::vector3df(0.0f,0.0f,0.0f),  // initial direction & speed (all set to 0.0f == particles stopped)
	0,0,                              // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),      // darkest color
	video::SColor(0,0,0,0),           // darkest color
	video::SColor(255,255,255,255),   // brightest color
	90,200,0,                         // min age, max age, angle
	core::dimension2df( 5.0f, 5.0f),  // min size
	core::dimension2df(25.0f,25.0f)); // max size

	ps63->setEmitter(em63); // Grabs the emitter
	em63->drop(); // We can drop it here, without deleting it

	ps63->setParent(nodeDiscoBall4); /// Attach these reflexed lights to the Disco Ball N4 (Match to the same position & rotation)
	// ps63->setPosition(core::vector3df(-550,100,-500)); // x, y, z: profundidade
	// ps63->setScale(core::vector3df(100,100,100));
	ps63->setScale(core::vector3df(40,40,40));
	ps63->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps63->setMaterialTexture(0, driver->getTexture("estrela_squared.png"));
	// ps63->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps63->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	ps63->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps63->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps63);
	// ps63->getMaterial(0).Lighting = false;
    // ps63->getMaterial(0).Shininess = 100.0f; // Set size of specular highlights
	// ps63->getMaterial(0).SpecularColor.set(255,255,255,255);
	// ps63->getMaterial(0).DiffuseColor.set(255,255,255,255);
	// ps63->getMaterial(0).EmissiveColor.set(255,255,255,255);
	/// Disabled:
    em63->setMinParticlesPerSecond(0);
	em63->setMaxParticlesPerSecond(0);


    /// Create particle system: (Falling STARS)
	scene::IParticleSystemSceneNode* ps2 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em2 = ps2->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(0,0,0,30,30,8),  // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                         // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,-0.02f,0.0f),   // initial direction. Y = speed
	0,0,                                 // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),         // darkest color
	video::SColor(0,0,0,0),              // darkest color
	video::SColor(255,255,255,255),      // brightest color
	16000,30000,40,                      // min age, max age, angle
	core::dimension2df(0.5f,0.5f),       // min size
	core::dimension2df(6.0f,6.0f));      // max size

	ps2->setEmitter(em2); // Grabs the emitter
	em2->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf2 = ps2->createRotationAffector(core::vector3df(3,3,3), core::vector3df(0,0,0)); /// Rotates our particles (speed, pivot point)
	ps2->addAffector(paf2); // Same goes for the affector
	paf2->drop();

	ps2->setPosition(core::vector3df(-550,100,-500)); // x, y, z: profundidade
	// ps2->setScale(core::vector3df(100,100,100));
	ps2->setScale(core::vector3df(40,40,40));
	ps2->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps2->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps2->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps2->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	ps2->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps2);
	// ps2->getMaterial(0).Lighting = false;
    // ps2->getMaterial(0).Shininess = 100.0f; // Set size of specular highlights
	// ps2->getMaterial(0).SpecularColor.set(255,255,255,255);
	// ps2->getMaterial(0).DiffuseColor.set(255,255,255,255);
	// ps2->getMaterial(0).EmissiveColor.set(255,255,255,255);
	/// Disabled:
    em2->setMinParticlesPerSecond(0);
	em2->setMaxParticlesPerSecond(0);


    /// Create particle system: (RIGHT FIRE)
	scene::IParticleSystemSceneNode* ps3 = smgr->addParticleSystemSceneNode(false);
    // ps3->setParent(nodeBand[0]);

	scene::IParticleEmitter* em3 = ps3->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(-7,0,-7,7,1,7),   // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                          // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,0.2f,0.0f),      // initial direction. Y = speed
	0,0,                                  // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),          // darkest color
	video::SColor(0,255,255,255),         // darkest color
	video::SColor(0,255,255,255),         // brightest color
	400,1000,0,                           // min age, max age, angle
	core::dimension2df(3.f,3.f),          // min size
	core::dimension2df(17.f,17.f));       // max size

	ps3->setEmitter(em3); // Grabs the emitter
	em3->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf3 = ps3->createFadeOutParticleAffector(); // Fade out particles
	ps3->addAffector(paf3); // Same goes for the affector
	paf3->drop();

	ps3->setPosition(core::vector3df(-297,170,-598)); // x, y, z: profundidade (+25)
	// ps3->setScale(core::vector3df(100,100,100));
	ps3->setScale(core::vector3df(1.0f,1.0f,1.0f));
	ps3->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps3->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps3->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps3->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps3->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps3->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps3->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps3);
	/// Disabled:
    em3->setMinParticlesPerSecond(0);
	em3->setMaxParticlesPerSecond(0);


    /// Create particle system: (LEFT FIRE)
	scene::IParticleSystemSceneNode* ps4 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em4 = ps4->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(-7,0,-7,7,1,7),   // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                          // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,0.2f,0.0f),      // initial direction. Y = speed
	0,0,                                  // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),          // darkest color
	video::SColor(0,255,255,255),         // darkest color
	video::SColor(0,255,255,255),         // brightest color
	400,1000,0,                           // min age, max age, angle
	core::dimension2df(3.f,3.f),          // min size
	core::dimension2df(17.f,17.f));       // max size

	ps4->setEmitter(em4); // Grabs the emitter
	em4->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf4 = ps4->createFadeOutParticleAffector(); // Fade out particles
	ps4->addAffector(paf4); // Same goes for the affector
	paf4->drop();

	ps4->setPosition(core::vector3df(292,170,-598)); // x, y, z: profundidade
	// ps4->setScale(core::vector3df(100,100,100));
	ps4->setScale(core::vector3df(1.0f,1.0f,1.0f));
	ps4->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps4->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps4->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps4->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps4->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps4->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps4->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps4);
	/// Disabled:
    em4->setMinParticlesPerSecond(0);
	em4->setMaxParticlesPerSecond(0);
	// TODO stop emiters, if no song playing:
	// ps->getEmitter()->setMaxParticlesPerSecond(10);
    // ps->getEmitter()->setMinParticlesPerSecond(10);
    // ps->setEmitter(0);


	/// Create particle system: (CENTERED FIREWORKS) - Only visible when music stops
	scene::IParticleSystemSceneNode* ps5 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em5 = ps5->createRingEmitter( /// RING Emitter
    vector3df(0.0f,0.0f,0.0f),          // center
    1,10,                               // radius / ring thickness
    vector3df(0.0f,0.1f,0.0f),          // direction
    0,0,                                // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
    SColor(255,255,255,255),            // darkest color
    SColor(0,0,0,0),                    // brightest color
    8000,10000,180,                     // lifeTimeMin / lifeTimeMax / maxAngleDegrees
    core::dimension2df(1.0f, 1.0f),     // minStartSize
    core::dimension2df(5.0f, 5.0f));    // maxStartSize

	ps5->setEmitter(em5); // Grabs the emitter
	em5->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf5 = ps5->createFadeOutParticleAffector(); // Fade out particles
	ps5->addAffector(paf5); // Same goes for the affector
	paf5->drop();

	ps5->setPosition(core::vector3df(0,380,-390)); // x, y, z: profundidade
	// ps5->setScale(core::vector3df(100,100,100));
	// ps5->setScale(core::vector3df(10,10,10));
	ps5->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps5->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps5->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps5->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps5->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps5->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps5->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps5);
	/// Disabled:
    em5->setMinParticlesPerSecond(0);
	em5->setMaxParticlesPerSecond(0);

	// TODO stop emiters, if no song playing:
	// ps->getEmitter()->setMaxParticlesPerSecond(10);
    // ps->getEmitter()->setMinParticlesPerSecond(10);
    // ps5->setEmitter();

    /// Create particle system: (falling flames RIGHT)
	scene::IParticleSystemSceneNode* ps7 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em7 = ps7->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(0,0,0,0.4,0.4,0.4), // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                            // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,-0.03f,0.0f),      // initial direction. Y = speed
	0,0,                                    // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	video::SColor(0,0,0,0),                 // darkest color
	video::SColor(255,255,255,255),         // brightest color
	500,1000,1,                             // min age, max age, angle
	core::dimension2df(1.0f,1.0f),          // min size
	core::dimension2df(7.0f,7.0f));         // max size

	ps7->setEmitter(em7); // Grabs the emitter
	em7->drop(); // We can drop it here, without deleting it

	// scene::IParticleAffector* paf7 = ps7->createFadeOutParticleAffector(); // Fade out particles
	// ps7->addAffector(paf7); // Same goes for the affector
	// paf7->drop();

	ps7->setPosition(core::vector3df(-306,160,-608)); // x, y, z: profundidade
	// ps7->setScale(core::vector3df(100,100,100));
	ps7->setScale(core::vector3df(40,40,40));
	ps7->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps7->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps7->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps7->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	ps7->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps7->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps7);
	// ps7->getMaterial(0).Lighting = false;
    // ps7->getMaterial(0).Shininess = 100.0f; // Set size of specular highlights
	// ps7->getMaterial(0).SpecularColor.set(255,255,255,255);
	// ps7->getMaterial(0).DiffuseColor.set(255,255,255,255);
	// ps7->getMaterial(0).EmissiveColor.set(255,255,255,255);
	/// Disabled:
    em7->setMinParticlesPerSecond(0);
	em7->setMaxParticlesPerSecond(0);


	/// Create particle system: (falling flames LEFT)
	scene::IParticleSystemSceneNode* ps8 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em8 = ps8->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(0,0,0,0.4,0.4,0.4), // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                            // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,-0.03f,0.0f),      // initial direction. Y = speed
	0,0,                                    // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	video::SColor(0,0,0,0),                 // darkest color
	video::SColor(255,255,255,255),         // brightest color
	500,1000,1,                             // min age, max age, angle
	core::dimension2df(1.0f,1.0f),          // min size
	core::dimension2df(7.0f,7.0f));         // max size

	ps8->setEmitter(em8); // Grabs the emitter
	em8->drop(); // We can drop it here, without deleting it

	// scene::IParticleAffector* paf8 = ps8->createFadeOutParticleAffector(); // Fade out particles
	// ps8->addAffector(paf8); // Same goes for the affector
	// paf8->drop();

	ps8->setPosition(core::vector3df(286,160,-608)); // x, y, z: profundidade
	// ps8->setScale(core::vector3df(100,100,100));
	ps8->setScale(core::vector3df(40,40,40));
	ps8->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps8->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps8->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps8->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	ps8->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps8->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps8);
	// ps8->getMaterial(0).Lighting = false;
    // ps8->getMaterial(0).Shininess = 100.0f; // Set size of specular highlights
	// ps8->getMaterial(0).SpecularColor.set(255,255,255,255);
	// ps8->getMaterial(0).DiffuseColor.set(255,255,255,255);
	// ps8->getMaterial(0).EmissiveColor.set(255,255,255,255);
	/// Disabled:
    em8->setMinParticlesPerSecond(0);
	em8->setMaxParticlesPerSecond(0);


    /// Create particle system: (STAGE SMALL UP FIRE N1)
	scene::IParticleSystemSceneNode* ps30 = smgr->addParticleSystemSceneNode(false);
    // ps30->setParent(nodeBand[0]);

	scene::IParticleEmitter* em30 = ps30->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(-1,0,-1,1,1,1),   // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                          // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,0.15f,0.0f),     // initial direction. Y = speed
	0,0,                                  // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),          // darkest color
	video::SColor(0,0,0,0),               // darkest color
	video::SColor(0,255,255,255),         // brightest color
	100,1000,0,                           // min age, max age, angle
	core::dimension2df(1.0f,1.0f),        // min size
	core::dimension2df(13.f,13.f));       // max size

	ps30->setEmitter(em30); // Grabs the emitter
	em30->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf30a = ps30->createFadeOutParticleAffector(); // Affector: Fade out particles
	ps30->addAffector(paf30a);
	paf30a->drop();

	ps30->setPosition(core::vector3df(147,105,-428)); // x, y, z: profundidade (+25)
	// ps30->setRotation(core::vector3df(-2.5,0,-2.5)); /// Rotation needed, its to compensate emitter angle
	// ps30->setScale(core::vector3df(100,100,100));
	ps30->setScale(core::vector3df(1,1,1));
	ps30->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps30->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps30->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps30->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps30->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps30->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps30->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps30);
	/// Disabled:
    em30->setMinParticlesPerSecond(0);
	em30->setMaxParticlesPerSecond(0);


    /// Create particle system: (STAGE SMALL UP FIRE N2)
	scene::IParticleSystemSceneNode* ps31 = smgr->addParticleSystemSceneNode(false);
    // ps30->setParent(nodeBand[0]);

	scene::IParticleEmitter* em31 = ps31->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(-1,0,-1,1,1,1),   // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                          // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,0.15f,0.0f),     // initial direction. Y = speed
	0,0,                                  // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),          // darkest color
	video::SColor(0,0,0,0),               // darkest color
	video::SColor(0,255,255,255),         // brightest color
	100,1000,0,                           // min age, max age, angle
	core::dimension2df(1.0f,1.0f),        // min size
	core::dimension2df(13.f,13.f));       // max size

	ps31->setEmitter(em31); // Grabs the emitter
	em31->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf31a = ps31->createFadeOutParticleAffector(); // Affector: Fade out particles
	ps31->addAffector(paf31a);
	paf31a->drop();

	ps31->setPosition(core::vector3df(49,105,-428)); // x, y, z: profundidade (+25)
	// ps31->setRotation(core::vector3df(-2.5,0,-2.5)); /// Rotation needed, its to compensate emitter angle
	// ps31->setScale(core::vector3df(100,100,100));
	ps31->setScale(core::vector3df(1,1,1));
	ps31->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps31->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps31->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps31->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps31->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps31->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps31->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps31);
	/// Disabled:
    em31->setMinParticlesPerSecond(0);
	em31->setMaxParticlesPerSecond(0);


    /// Create particle system: (STAGE SMALL UP FIRE N3)
	scene::IParticleSystemSceneNode* ps32 = smgr->addParticleSystemSceneNode(false);
    // ps32->setParent(nodeBand[0]);

	scene::IParticleEmitter* em32 = ps32->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(-1,0,-1,1,1,1),   // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                          // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,0.15f,0.0f),     // initial direction. Y = speed
	0,0,                                  // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),          // darkest color
	video::SColor(0,0,0,0),               // darkest color
	video::SColor(0,255,255,255),         // brightest color
	100,1000,0,                           // min age, max age, angle
	core::dimension2df(1.0f,1.0f),        // min size
	core::dimension2df(13.f,13.f));       // max size

	ps32->setEmitter(em32); // Grabs the emitter
	em32->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf32a = ps32->createFadeOutParticleAffector(); // Affector: Fade out particles
	ps32->addAffector(paf32a);
	paf32a->drop();

	ps32->setPosition(core::vector3df(-49,105,-428)); // x, y, z: profundidade (+25)
	// ps32->setRotation(core::vector3df(-2.5,0,-2.5)); /// Rotation needed, its to compensate emitter angle
	// ps32->setScale(core::vector3df(100,100,100));
	ps32->setScale(core::vector3df(1,1,1));
	ps32->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps32->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps32->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps32->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps32->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps32->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps32->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps31);
	/// Disabled:
    em32->setMinParticlesPerSecond(0);
	em32->setMaxParticlesPerSecond(0);


    /// Create particle system: (STAGE SMALL UP FIRE N4)
	scene::IParticleSystemSceneNode* ps33 = smgr->addParticleSystemSceneNode(false);
    // ps33->setParent(nodeBand[0]);

	scene::IParticleEmitter* em33 = ps33->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(-1,0,-1,1,1,1),   // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                          // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,0.15f,0.0f),     // initial direction. Y = speed
	0,0,                                  // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),          // darkest color
	video::SColor(0,0,0,0),               // darkest color
	video::SColor(0,255,255,255),         // brightest color
	100,1000,0,                           // min age, max age, angle
	core::dimension2df(1.0f,1.0f),        // min size
	core::dimension2df(13.f,13.f));       // max size

	ps33->setEmitter(em33); // Grabs the emitter
	em33->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf33a = ps33->createFadeOutParticleAffector(); // Affector: Fade out particles
	ps33->addAffector(paf33a);
	paf33a->drop();

	ps33->setPosition(core::vector3df(-147,105,-428)); // x, y, z: profundidade (+25)
	// ps33->setRotation(core::vector3df(-2.5,0,-2.5)); /// Rotation needed, its to compensate emitter angle
	// ps33->setScale(core::vector3df(100,100,100));
	ps33->setScale(core::vector3df(1,1,1));
	ps33->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps33->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps33->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps33->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps33->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps33->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps33->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps33);
	/// Disabled:
    em33->setMinParticlesPerSecond(0);
	em33->setMaxParticlesPerSecond(0);


    /// Create particle system: (STAGE SMOKE MACHINE N1 - LEFT)
	scene::IParticleSystemSceneNode* ps50 = smgr->addParticleSystemSceneNode(false);
    // ps50->setParent(nodeBand[0]);

	scene::IParticleEmitter* em50 = ps50->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(-1,0,-1,1,1,1),     // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                            // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(-0.035f,0.010f,0.010f), // initial direction & speed
	0,0,                                    // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	video::SColor(0,0,0,0),                 // darkest color
	video::SColor(0,255,255,255),           // brightest color
	10000,30000,30,                         // min age, max age, angle
	core::dimension2df(5.0f,5.0f),          // min size
	core::dimension2df(20.f,20.f));         // max size

	ps50->setEmitter(em50); // Grabs the emitter
	em50->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf50a = ps50->createFadeOutParticleAffector(); /// Affector: Fade out particles
	ps50->addAffector(paf50a);
	paf50a->drop();

	scene::IParticleAffector* paf50b = ps50->createGravityAffector(core::vector3df(0.020f,  0.030f, 0.0f), 70000); /// Affector: Apply gravity to particles (when at age of 5000)
	ps50->addAffector(paf50b);
	paf50b->drop();

	ps50->setPosition(core::vector3df(357,125,-650)); // x, y, z
	// ps50->setRotation(core::vector3df(0,0,74));
	// ps50->setScale(core::vector3df(100,100,100));
	ps50->setScale(core::vector3df(1,1,1));
	ps50->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps50->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps50->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps50->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps50->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps50->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps50->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps50);
	/// Disabled:
    em50->setMinParticlesPerSecond(0);
	em50->setMaxParticlesPerSecond(0);


    /// Create particle system: (STAGE SMOKE MACHINE N2 - RIGHT)
	scene::IParticleSystemSceneNode* ps51 = smgr->addParticleSystemSceneNode(false);
    // ps51->setParent(nodeBand[0]);

	scene::IParticleEmitter* em51 = ps51->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(-1,0,-1,1,1,1),      // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                             // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.035f,-0.010f,-0.010f), // initial direction & speed
	0,0,                                     // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),             // darkest color
	video::SColor(0,0,0,0),                  // darkest color
	video::SColor(0,255,255,255),            // brightest color
	10000,30000,20,                          // min age, max age, angle
	core::dimension2df(5.0f,5.0f),           // min size
	core::dimension2df(20.f,20.f));          // max size

	ps51->setEmitter(em51); // Grabs the emitter
	em51->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf51a = ps51->createFadeOutParticleAffector(); /// Affector: Fade out particles
	ps51->addAffector(paf51a);
	paf51a->drop();

	scene::IParticleAffector* paf51b = ps51->createGravityAffector(core::vector3df(-0.020f,  0.030f, 0.0f), 70000); /// Affector: Apply gravity to particles (when at age of 5000)
	ps51->addAffector(paf51b);
	paf51b->drop();

	ps51->setPosition(core::vector3df(-357,125,-650)); // x, y, z
	// ps51->setRotation(core::vector3df(0,0,-74));
	// ps51->setScale(core::vector3df(100,100,100));
	ps51->setScale(core::vector3df(1,1,1));
	ps51->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps51->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps51->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps51->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps51->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps51->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps51->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps51);
	/// Disabled:
    em51->setMinParticlesPerSecond(0);
	em51->setMaxParticlesPerSecond(0);


	std::cout << "All Particles... Ok" << std::endl;




    /// ##########################################################################################
    /// ############# PARTICLES - END ############################################################
    /// ##########################################################################################



    /// Loading Bar: #8
    LoadBar->setPercent(70); // 0 to 100
    LoadBar->draw();
    // LoadBar->setVisible(false);



    /// ############# CREATE CAMERAS ############################################################

    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Creating Cameras..." << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;

    /// Cameras:
    ICameraSceneNode* camera[30];
    f32 SPEED_CAMERA = 0.0017f; // Default: 0.0010f;

    // Cameras target is: (Selected band position)
    cameraTarget = vector3df(0,0,0); /// core::vector3df(nodeBand[BAND_SELECTED]->getPosition().X, nodeBand[BAND_SELECTED]->getPosition().Y+50, nodeBand[BAND_SELECTED]->getPosition().Z);

    // Camera 1:
    camera[1] = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    camera[1]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[1]->setPosition(core::vector3df(0,0,0));         // Set Camera Position
    camera[1]->setTarget(cameraTarget);                     // Set Camera Target (band)
    camera[1]->setInputReceiverEnabled(false);
    camera[1]->setFarValue(3000.0f); /// this increases the shadows visible range
    camera[1]->setFOV(0.55f); /// Default, will change in main loop at start (Just for a cool zoom effect)
    // Add fly circle animator to this camera:
    scene::ISceneNodeAnimator* animc0 = smgr->createFlyCircleAnimator(core::vector3df(0,110,-100), 2.0f, SPEED_CAMERA);
    camera[1]->addAnimator(animc0);
    animc0->drop();

    // Camera 2:
    camera[2] = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    camera[2]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[2]->setPosition(core::vector3df(0,0,0));         // Set Camera Position
    camera[2]->setTarget(cameraTarget);                     // Set Camera Target (band)
    camera[2]->setInputReceiverEnabled(false);
    camera[2]->setFarValue(3000.0f); /// this increases the shadows visible range
    // Add fly circle animator to this camera:
    scene::ISceneNodeAnimator* animc1 = smgr->createFlyCircleAnimator(core::vector3df(0,350,-150), 130.0f, SPEED_CAMERA);
    camera[2]->addAnimator(animc1);
    animc1->drop();

    // Camera 3:
    camera[3] = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    camera[3]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[3]->setPosition(core::vector3df(-300,110,-100));         // Set Camera Position
    camera[3]->setTarget(cameraTarget);                     // Set Camera Target (band)
    camera[3]->setInputReceiverEnabled(false);
    camera[3]->setFarValue(3000.0f); /// this increases the shadows visible range
    // Add fly circle animator to this camera:
    scene::ISceneNodeAnimator* animc2 = smgr->createFlyCircleAnimator(core::vector3df(-300,120,-100), 5.0f, SPEED_CAMERA);
    camera[3]->addAnimator(animc2);
    animc2->drop();

    // Camera 4:
    camera[4] = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    camera[4]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[4]->setPosition(core::vector3df(300,110,-100));         // Set Camera Position
    camera[4]->setTarget(cameraTarget);                     // Set Camera Target (band)
    camera[4]->setInputReceiverEnabled(false);
    camera[4]->setFarValue(3000.0f); /// this increases the shadows visible range
    // Add fly circle animator to this camera:
    scene::ISceneNodeAnimator* animc3 = smgr->createFlyCircleAnimator(core::vector3df(300,120,-100), 5.0f, SPEED_CAMERA);
    camera[4]->addAnimator(animc3);
    animc3->drop();

   // Camera 5:
    camera[5] = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    camera[5]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[5]->setPosition(core::vector3df(0,0,0));         // Set Camera Position
    camera[5]->setTarget(cameraTarget);                     // Set Camera Target (band)
    camera[5]->setInputReceiverEnabled(false);
    camera[5]->setFarValue(3000.0f); /// this increases the shadows visible range
    // Add fly circle animator to this camera:
    scene::ISceneNodeAnimator* animc4 = smgr->createFlyCircleAnimator(core::vector3df(0,260,-40), 5.0f, SPEED_CAMERA);
    camera[5]->addAnimator(animc4);
    animc4->drop();

    // Camera 6:
    camera[6] = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    camera[6]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[6]->setPosition(core::vector3df(0,0,0));         // Set Camera Position
    camera[6]->setTarget(cameraTarget);                     // Set Camera Target (band)
    camera[6]->setInputReceiverEnabled(false);
    camera[6]->setFarValue(3000.0f); /// this increases the shadows visible range
    // Add fly circle animator to this camera:
    scene::ISceneNodeAnimator* animc5 = smgr->createFlyCircleAnimator(core::vector3df(0,150,-200), 170.0f, SPEED_CAMERA);
    camera[6]->addAnimator(animc5);
    animc5->drop();

    // Camera 7:
    camera[7] = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    camera[7]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[7]->setPosition(core::vector3df(0,240,-650));    // Set Camera Position
    camera[7]->setTarget(core::vector3df(0,-20,50));        /// Set Camera Target (spectators)
    camera[7]->setInputReceiverEnabled(false);
    camera[7]->setFarValue(3000.0f); /// this increases the shadows visible range
    // Add fly circle animator to this camera:
    scene::ISceneNodeAnimator* animc7 = smgr->createFlyCircleAnimator(core::vector3df(0,240,-650), 3.0f, SPEED_CAMERA);
    camera[7]->addAnimator(animc7);
    animc7->drop();

    // Camera 8:
    camera[8] = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    camera[8]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[8]->setPosition(core::vector3df(300,150,-250));  // Set Camera Position
    camera[8]->setTarget(cameraTarget);                     // Set Camera Target (band)
    camera[8]->setInputReceiverEnabled(false);
    camera[8]->setFarValue(3000.0f); /// this increases the shadows visible range
    // Add fly circle animator to this camera:
    scene::ISceneNodeAnimator* animc8 = smgr->createFlyCircleAnimator(core::vector3df(300,150,-250), 5.0f, SPEED_CAMERA);
    camera[8]->addAnimator(animc8);
    animc8->drop();

    // Camera 9:
    camera[9] = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    camera[9]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[9]->setPosition(core::vector3df(-300,150,-250)); // Set Camera Position
    camera[9]->setTarget(cameraTarget);                     // Set Camera Target (band)
    camera[9]->setInputReceiverEnabled(false);
    camera[9]->setFarValue(3000.0f); /// this increases the shadows visible range
    // Add fly circle animator to this camera:
    scene::ISceneNodeAnimator* animc9 = smgr->createFlyCircleAnimator(core::vector3df(-300,150,-250), 5.0f, SPEED_CAMERA);
    camera[9]->addAnimator(animc9);
    animc9->drop();

    // Camera 10:
    camera[10] = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    camera[10]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[10]->setPosition(core::vector3df(300,105,-50));   // Set Camera Position
    camera[10]->setTarget(cameraTarget);                     // Set Camera Target (band)
    camera[10]->setInputReceiverEnabled(false);
    camera[10]->setFarValue(3000.0f); /// this increases the shadows visible range

    // Initialize follow spiline to this camera (needs two dummy nodes):
    scene::ISceneNode* CameraPositionNode = smgr->addEmptySceneNode();

    // Camera -> createFollowSplineAnimator():
    irr::core::array<core::vector3df> camPositions;
    // Fill position array:
    camPositions.push_back(core::vector3df( 300, 105,  -50)); // Corner
    camPositions.push_back(core::vector3df(   0, 445,  -40)); // Note: 445 max height
    camPositions.push_back(core::vector3df(-300, 105,  -50)); // Corner
    camPositions.push_back(core::vector3df(-300, 400, -300)); // Corner
    camPositions.push_back(core::vector3df(   0, 102, -270));
    camPositions.push_back(core::vector3df( 300, 400, -300)); // Corner

    // Add follow positions spline animator to this node:
    scene::ISceneNodeAnimator* animCam = smgr->createFollowSplineAnimator(0, camPositions, 0.2f, 0.2f, true, true); // CreateFollowSplineAnimator(startTime,points,speed,tightness,loop,pingpong);
    CameraPositionNode->addAnimator(animCam);
    animCam->drop();


    /// Camera 20: FPS (for DEBUG only)
    /// Set FPS camera keys:
    SKeyMap keyMap[10];
    keyMap[0].Action  = EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = KEY_UP;
    keyMap[1].Action  = EKA_MOVE_BACKWARD;
    keyMap[1].KeyCode = KEY_DOWN;
    keyMap[2].Action  = EKA_STRAFE_LEFT;
    keyMap[2].KeyCode = KEY_LEFT;
    keyMap[3].Action  = EKA_STRAFE_RIGHT;
    keyMap[3].KeyCode = KEY_RIGHT;

    camera[20] = smgr->addCameraSceneNodeFPS(0, 100, 0.1, -1, keyMap, 2);
    camera[20]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[20]->setPosition(core::vector3df(0,350,-50)); // Set Camera Position
    camera[20]->setTarget(cameraTarget);                     // Set Camera Target (band)
    camera[20]->setInputReceiverEnabled(true);
    camera[20]->setFarValue(10000.0f); /// this increase a shadow visible range


    std::cout << "Cameras... Ok" << std::endl;




    /// Loading Bar: #9
    LoadBar->setPercent(80); // 0 to 100
    LoadBar->draw();
    // LoadBar->setVisible(false);



    /// ############# SPECIAL EFFECTS ##############################################################


    /// EFFECTS (1/2): ( From: "libraries\PostProcessor\..." ) -- HELP at --> http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=34830
    // Starting with effects (Post processing):
    ppRenderer = new CRendererPostProc(smgr, dimension2du(1024,1024), true, true, SColor(255,100,101,140));
    // When setting up the effect, the parameters are: Input, size of output, effect ID (see CEffectPostProc.h for full list), effect parameters (in this case, blur size):
    /// All effects at "libraries\PostProcessor\CEffectPostProc.cpp". (eg: PP_BLUR, PP_BLURDOFFAR, PP_BLURDOFNEAR, PP_BLURDOF, PP_BLOOM, PP_GLOOM,...)


    ppEffects[0] = new CEffectPostProc(ppRenderer, dimension2du(1024,1024), PP_GREYSCALE, 1.0f);   ppEffectName[0]="Greyscale";
    ppEffects[1] = new CEffectPostProc(ppRenderer, dimension2du(1024,1024), PP_BLOOM, 1.0f);       ppEffectName[1]="Bloom";
    ppEffects[2] = new CEffectPostProc(ppRenderer, dimension2du(1024,1024), PP_GLOOM, 10.0f);      ppEffectName[2]="Gloom";
    ppEffects[3] = new CEffectPostProc(ppRenderer, dimension2du(1024,1024), PP_BLUR, 0.01f );      ppEffectName[3]="Blur";         /// Ugly!...
    ppEffects[4] = new CEffectPostProc(ppRenderer, dimension2du(1024,1024), PP_RADIALBLUR, 1.0f);  ppEffectName[4]="Radial Blur";  /// Ugly!...
    ppEffects[5] = new CEffectPostProc(ppRenderer, dimension2du(1024,1024), PP_MONITOR, 2.0f );    ppEffectName[5]="Monitor";      /// Ugly!...
    ppEffects[6] = new CEffectPostProc(ppRenderer, dimension2du(1024,1024), PP_WATERCOLOR, 1.5f ); ppEffectName[6]="Water Color";  /// Ugly!...
    ppEffects[7] = new CEffectPostProc(ppRenderer, dimension2du(1024,1024), PP_SATURATE, 1.0f);    ppEffectName[7]="Saturate";     /// Ugly!...

    for(int i=0; i<ppeCount; i++){ ppEffects[i]->setQuality(PPQ_GOOD); }; // Change to a better quality effects - not all shaders will respect these, but they can be used to hint the rendering standard required.

    bool ppeffectsEnable = false;           // Post prosessing effects
    int ppEffectsRand = rand()%ppeCount;    // Post prosessing (default) random selected effect

    std::cout << "Effects... Ok" << std::endl;




    /// ##########################################################################################
    /// ############# USER INTERFACE (GUI) #######################################################
    /// ##########################################################################################

    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Creating GUI..." << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;

    /// Initialize GUI stuff:
    skin  = guienv->getSkin();
    fontMain = guienv->getFont("font1.bmp"); // Get Texts Font
    // IGUIFont* font2 = guienv->getFont("font2.bmp"); // Get Texts Font


    /// Fonts for lyrics:
    font_lyrics[1] = guienv->getFont("font_lyrics_14.bmp");
    font_lyrics[2] = guienv->getFont("font_lyrics_18.bmp");
    font_lyrics[3] = guienv->getFont("font_lyrics_22.bmp");
    font_lyrics[4] = guienv->getFont("font_lyrics_26.bmp");
    font_lyrics[5] = guienv->getFont("font_lyrics_36.bmp");
    font_lyrics[6] = guienv->getFont("font_lyrics_68.bmp");


    /// Apply font to GUI:
    if(fontMain){ skin->setFont(fontMain); };
    skin->setColor(gui::EGDC_BUTTON_TEXT, video::SColor(255,210,235,255)); // Set Texts Font Color
    // IGUIStaticText->setTextAlignment - EGUIA_CENTER

    // SCREEN_WIDTH  = driver->getScreenSize().Width;
    // SCREEN_HEIGHT = driver->getScreenSize().Height;
    // s32 pos = ((IGUIScrollBar*)GUIEvent.Caller)->getPos();

    /// Set GUI Colors: (transparencies, etc...)
    std::cout << "Total GUI Elements: " << EGDC_COUNT << std::endl; // Write log
    /// --> Paint some GUI elements...
    for(s32 i=0; i<8; i++){ guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, SColor(200,25,25,40)); }; /// --> Paint some GUI elements... (opacity, red, green, blue)
    /// ID:0 - Windows right,bottom borders (shadows) + cursors
    /// ID:1 - Windows right,bottom borders + cursors
    /// ID:2 - Cursors + window fills
    /// ID:3 - Windows left,top borders + cursors
    /// ID:4 - Input text right,bottom borders
    /// ID:5 - Windows Toolbar
    /// ID:6 - Unknown...
    /// ID:7 - Unknown...
    /// ID:8 - GUI Texts

    /// --> ...Override this... (Painted in the above loop)
    guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)2,  SColor(200,15,15,25));    /// --> ID:2  - Cursors + window fills (opacity, red, green, blue)

    /// --> ...Then, these ones...
    guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)9,  SColor(150,255,255,255)); /// --> ID:9  - Text Fields Colors (opacity, red, green, blue)
    guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)10, SColor(200,50,50,100));   /// --> ID:10 - Selectors Background Color (opacity, red, green, blue)
    // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)11, SColor(255,255,0,0));  /// --> ID:11 - Unknown... (opacity, red, green, blue)
    // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)12, SColor(255,255,0,0));  /// --> ID:12 - Unknown... (opacity, red, green, blue)
    // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)13, SColor(255,255,0,0));  /// --> ID:13 - Unknown... (opacity, red, green, blue)
    // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)14, SColor(255,255,0,0));  /// --> ID:14 - Unknown... (opacity, red, green, blue)
    // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)15, SColor(255,255,0,0));  /// --> ID:15 - Unknown... (opacity, red, green, blue)
    guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)16, SColor(100,200,200,250)); /// --> ID:16 - Scrollbars (opacity, red, green, blue)
    // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)17, SColor(255,255,0,0));  /// --> ID:17 - Unknown... (opacity, red, green, blue)
    guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)18, SColor(200,200,200,200)); /// --> ID:18 - Cursors and Window Close button (opacity, red, green, blue)
    guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)19, SColor(200,200,200,200)); /// --> ID:19 - Directories Icons Colors (opacity, red, green, blue)
    // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)20, SColor(255,255,0,0));  /// --> ID:20 - Unknown... (opacity, red, green, blue)
    // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)21, SColor(255,255,0,0));  /// --> ID:21 - Unknown... (opacity, red, green, blue)
    guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)22, SColor(80,0,0,0));        /// --> ID:22 - Text Path Background Color (opacity, red, green, blue)


    /// --> ...Finally...
    for(s32 i=23; i<gui::EGDC_COUNT; i++){ guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, SColor(255,255,255,255)); }; /// --> Paint any other GUI unknown elements... (opacity, red, green, blue)



    /// GUI Colors:
    /// http://irrlicht.convextech.ca/IrrlichtDocument/html/namespaceirr_1_1gui.html#abd15860fde29833c48daff5f95d5467a
    // guienv->getSkin()->setColor(EGDC_ICON_HIGH_LIGHT, SColor(255, 255,0,0));
    // guienv->getSkin()->setColor(EGDC_3D_HIGH_LIGHT, SColor(255, 255,0,0));
    // guienv->getSkin()->setColor(EGDC_3D_DARK_SHADOW, SColor(255, 255,0,0));
    // guienv->getSkin()->setColor(EGDC_3D_SHADOW, SColor(255, 255,0,0));


    /// Create a window: (elements container)
    win1 = guienv->addWindow(core::rect<s32>(5,5,180,SCREEN_HEIGHT-5), false, L"Win1");
    // win1->setRelativePosition(core::rect<s32>(5,5,180,SCREEN_HEIGHT-5));
    win1->getCloseButton()->setVisible(false); // Hides the irrlicht window´s default close (X) button
    win1->setDraggable(true);
    win1->setDrawTitlebar(false);
    win1->setVisible(true);
    int win1Width  = 180;
    int win1Height = SCREEN_HEIGHT-5;

    /// Create a window: (elements container)
    win2 = guienv->addWindow(core::rect<s32>(185,5,400,65), false, L"Win2");
    // win2->setRelativePosition(core::rect<s32>(5,5,180,SCREEN_HEIGHT-5));
    win2->getCloseButton()->setVisible(false); // Hides the irrlicht window´s default close (X) button
    win2->setDraggable(true);
    win2->setDrawTitlebar(false);
    win2->setVisible(true);
    int win2Width  = 400;
    int win2Height = 65;

    /// Add an icon to that window:
    video::ITexture* img = driver->getTexture("drag.png");
    // IGUIImage* drag = guienv->addImage(img, core::position2d<s32>(347,38), true, win2, -1, 0);
    guienv->addImage(img, core::position2d<s32>(347,38), true, win2, -1, 0);





    /// Disable window (is visible, but cant be used)
    // guienv->getSkin()->setColor(EGDC_WINDOW, SColor(255, 255,0,0));
    // IGUIInOutFader* fader;
    // fader = device->getGUIEnvironment()->addInOutFader();
    // fader->setColor(video::SColor(0,255,255,255));
    // fader->fadeIn(900);

    /// Window N1 - Add texts:
    // IGUIStaticText* texts[50]={};
    int sy = 10; // Texts starting y position

    sy=10;
    // texts[16] = guienv->addStaticText(L"IMP - Irrlicht Music Player", core::rect<s32>(10,sy,200,SCREEN_HEIGHT), false, false, win1); /// border? - text wrap? - parent name
    // texts[16]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=0;
    texts[0] = guienv->addStaticText(L"Show/Hide Menus: M", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[0]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=14;
    texts[15] = guienv->addStaticText(L"Display Infos: None (I)", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[15]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=14;
    texts[11] = guienv->addStaticText(L"Spectators Sounds: 1, 2", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[11]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=14;
    texts[1] = guienv->addStaticText(L"Camera: Auto (C)", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[1]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    /*
    sy+=14;
    texts[10] = guienv->addStaticText(L"Zoom: Z, X", core::rect<s32>(10,sy,200,SCREEN_HEIGHT), false, false, win1); // border? wordwrap?
    texts[10]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue
    */


    /// External settings: --->

    sy+=28;
    texts[14] = guienv->addStaticText(L"Band: Man (B)", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[14]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=14;
    texts[22] = guienv->addStaticText(L"Show: Normal (S)", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[22]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=14;
    texts[19] = guienv->addStaticText(L"Lyrics Color: White (L)", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[19]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=14;
    texts[18] = guienv->addStaticText(L"Lyrics Size: 3 (K)", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[18]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=14;
    texts[23] = guienv->addStaticText(L"Sounds: On (O)", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[23]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=14;
    texts[24] = guienv->addStaticText(L"Clock: Off (H)", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[24]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    /// ---------------------->

    sy+=28;
    texts[8] = guienv->addStaticText(L"Play Mode: Single (P)", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[8]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    // sy+=14;
    // texts[21] = guienv->addStaticText(L"Pitch: Normal (7, 8)", core::rect<s32>(10,sy,200,SCREEN_HEIGHT), false, false, win); // border? wordwrap?
    // texts[21]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    // sy+=14;
    // volume 	0.0 = silence, 1.0 = maximum volume (default)
    // stringw volt = L""; volt += L"Volume: "; for(f32 i=0.0; i<GLOBAL_VOLUME; i+=0.1){ volt += L"|"; }; volt += L" (9, 0)";
    // texts[20] = guienv->addStaticText(volt.c_str(), core::rect<s32>(10,sy,200,SCREEN_HEIGHT), false, false, win); // border? wordwrap?
    // texts[20]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=14;
    texts[13] = guienv->addStaticText(L"Play/Pause: Space", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[13]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    /// ListBox texts:
    sy+=55;
    texts[2] = guienv->addStaticText(L"\nSelect Directory: D\nSelect: Click or Arrow Keys", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[2]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=14; /// Empty - Not used by now.
    texts[17] = guienv->addStaticText(L"", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[17]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue



    /// Other texts:
    texts[3] = guienv->addStaticText(L"Loading Music...", core::rect<s32>(SCREEN_WIDTH/2-50,SCREEN_HEIGHT/2,SCREEN_WIDTH,SCREEN_HEIGHT), false, false, 0); /// border? - text wrap? - parent name
    texts[3]->setOverrideColor(video::SColor(255,255,255,255)); // opacity, red, green, blue
    texts[3]->setVisible(false);



    /// Window N2 - Add texts:
    texts[4] = guienv->addStaticText(L"Position:\nVolume:\nPitch:", core::rect<s32>(10,10,50,50), false, true, win2); /// border, wrap, parent
    texts[4]->setOverrideColor(video::SColor(255,255,255,255)); // opacity, red, green, blue
    texts[4]->setTextAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT); // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE

    /// Timmers:
    texts[5] = guienv->addStaticText(L"00:00:00 - 00:00:00", core::rect<s32>(220,10,win2Width,win2Height), false, false, win2); /// border? - text wrap? - parent name
    texts[5]->setOverrideColor(video::SColor(200,150,150,200)); // opacity, red, green, blue
    texts[5]->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_UPPERLEFT); // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE
    /// Volume:
    stringw volumet = L""; for(float i=0.0; i<GLOBAL_VOLUME; i+=0.05){ volumet += L"|"; }; volumet += L" 50%";
    texts[26] = guienv->addStaticText(volumet.c_str(), core::rect<s32>(220,23,win2Width,win2Height), false, false, win2); /// border? - text wrap? - parent name
    texts[26]->setOverrideColor(video::SColor(200,150,150,200)); // opacity, red, green, blue
    texts[26]->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_UPPERLEFT); // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE
    /// Pitch:
    stringw pitcht = L""; for(float i=0.0; i<MUSIC_PITCH; i+=0.1){ pitcht += L"|"; }; pitcht += L" 1.0 Normal";
    texts[27] = guienv->addStaticText(pitcht.c_str(), core::rect<s32>(220,37,win2Width,win2Height), false, false, win2); /// border? - text wrap? - parent name
    texts[27]->setOverrideColor(video::SColor(200,150,150,200)); // opacity, red, green, blue
    texts[27]->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_UPPERLEFT); // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE
    /// Music has lyrics?
    texts[28] = guienv->addStaticText(L"Lyrics: No", core::rect<s32>(315,10,win2Width-12,win2Height), false, false, win2); /// border? - text wrap? - parent name
    texts[28]->setOverrideColor(video::SColor(200,150,150,200)); // opacity, red, green, blue
    texts[28]->setTextAlignment(EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT); // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE



    /// Display LOADING text: (Please standby, the Show will continue in a moment)
    texts[20] = guienv->addStaticText(L"", core::rect<s32>(0,0,SCREEN_WIDTH,SCREEN_HEIGHT), false, true, 0); /// border? - text wrap? - parent name
    texts[20]->setOverrideColor(video::SColor(255,255,255,255)); // opacity, red, green, blue
    texts[20]->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER); // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE

    /// Display PAUSED text: (show intermission)
    texts[12] = guienv->addStaticText(L"", core::rect<s32>(0,0,SCREEN_WIDTH,SCREEN_HEIGHT), false, true, 0); /// border? - text wrap? - parent name
    texts[12]->setOverrideColor(video::SColor(255,255,255,255)); // opacity, red, green, blue
    texts[12]->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER); // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE

    /// Display: Infos: Music Tags, Help, About, Nothing.
    texts[6] = guienv->addStaticText(L"", core::rect<s32>(190,5,SCREEN_WIDTH,SCREEN_HEIGHT), false, false, 0); /// border? - text wrap? - parent name
    // texts[6]->setOverrideColor(video::SColor(200,180,180,180)); // opacity, red, green, blue
    texts[6]->setOverrideColor(video::SColor(200,150,150,200)); // opacity, red, green, blue

    /// Display LYRICS:
    texts[7] = guienv->addStaticText(L"", core::rect<s32>(50,450,SCREEN_WIDTH-50,SCREEN_HEIGHT), false, true, 0); /// border? - text wrap? - parent name
    // texts[7]->setTextAlignment(EGUIA_CENTER, EGUIA_UPPERLEFT); // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE
    texts[7]->setOverrideColor(video::SColor(255,255,255,255)); // opacity, red, green, blue
    texts[7]->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER); // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE
    /// texts[7]->setTextRestrainedInside(true);
    /// texts[7]->getParent()->bringToFront(texts[7]); // ---> Brings text to front (placed in main loop), but when over ListBox some elements dont accept clicks!


    /// The Loading Bars texts: (Loading...)
    texts[25] = guienv->addStaticText(L"", core::rect<s32>(50,SCREEN_HEIGHT-130,SCREEN_WIDTH,SCREEN_HEIGHT), false, false, 0); /// border? - text wrap? - parent name
    texts[25]->setOverrideColor(video::SColor(255,150,150,200));
    texts[25]->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER); // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE



    std::cout << "GUI texts... Ok" << std::endl;


    /*
    /// Add buttons, one for each found song: (loop)
    int mc = musicList.size()-1;  // Get total songs count
    int sPos = 185;                // Buttons starting Y position
    IGUIButton* button[10]={};
    for(int i=0; i<=mc; i++){
    stringw songPath = musicList[i].c_str(); // eg: stringw text("Hello");
    stringw songNameOnly = musicListNamesOnly[i].c_str();
    stringw songExtensionOnly = musicExtensionsOnly[i].c_str();
    stringw allWords = ""; allWords += "["; allWords += songExtensionOnly; allWords += "] "; allWords += songNameOnly;
    button[i] = guienv->addButton(rect<s32>(position2di(10,sPos), dimension2di(155,20)), win, -1, allWords.c_str());
    // button[i] = guienv->addButton(rect<s32>(position2di(10,sPos), dimension2di(155,20)), win, 50, songPath.c_str());
    button[i]->setID(i+100); // set IDs, IDs counting from 100, in this case
    sPos += 20;
    }
    */


    /// Create GUI ListBox:
    /// Customize --> http://irrlicht.convextech.ca/IrrlichtDocument/html/namespaceirr_1_1gui.html#a7da705f0a0b4aa5385e6842adf409cb6
    /// gui::IGUIListBox* musicListBox;
    musicListBox = guienv->addListBox(core::rect<s32>(10,300,165,SCREEN_HEIGHT-20), win1, -1, true);
    musicListBox->setAutoScrollEnabled(true); // Auto scroll to selected item?
    musicListBox->setDrawBackground(false);
    musicListBox->setItemHeight(19);
    /*
    musicListBox->setItemOverrideColor(musicID, video::SColor(255,210,235,255));
    musicListBox->setItemOverrideColor(musicID, EGUI_LBC_TEXT, video::SColor(255,210,235,255)); // Color for selected item
    musicListBox->setItemOverrideColor(musicID, EGUI_LBC_TEXT_HIGHLIGHT, video::SColor(255,210,235,255)); // Color for selected item
    musicListBox->setItemOverrideColor(musicID, EGUI_LBC_ICON, video::SColor(255,210,235,255)); // Color for selected item
    musicListBox->setItemOverrideColor(musicID, EGUI_LBC_ICON_HIGHLIGHT, video::SColor(255,210,235,255)); // Color for selected item
    */

    /// Fill ListBox:
    if(musicListBox){ fillListBox(); };

    /*
    // Customize ListBox: (TODO)
    for (s32 i=0; i<musicList.size(); i++){ // for (s32 i=0; i<EGDC_COUNT; i++){
    SColor col = guienv->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
    col.setAlpha(30); // Alpha: 0 (transparent) to 255 (opaque)
    musicListBox->setItemOverrideColor((EGUI_LISTBOX_COLOR)i, col);
    }
    */

    /// Create GUI FileOpenDialog:
    /// http://irrlicht.sourceforge.net/docu/classirr_1_1gui_1_1_i_g_u_i_environment.html
    /// gui::IGUIFileOpenDialog* musicFileDialog;
    // IGUIFileOpenDialog * addFileOpenDialog (const wchar_t *title=0, bool modal=true, IGUIElement *parent=0, s32 id=-1, bool restoreCWD=false, io::path::char_type *startDir=0)=0
    /// musicFileDialog = guienv->addFileOpenDialog(L"SELECT THE NEW TARGET MUSIC FOLDER", true, 0, -1, false, 0);
    // musicFileDialog->setVisible(false); /// Starts hidden
    // musicFileDialog->getFileName();
    // musicFileDialog->getDirectoryName();

    /// Create GUI music scrollbar: (Position)
    // IGUIScrollBar (IGUIEnvironment *environment, IGUIElement *parent, s32 id, core::rect< s32 > rectangle)
    // musicPositionScrollbar = guienv->addScrollBar(true, rect<s32>(58, 12, 410, 22), win2, ID_SCROLLBAR_POSITION);
    musicPositionScrollbar = guienv->addScrollBar(true, rect<s32>(58, 12, 210, 22), win2, ID_SCROLLBAR_POSITION);
	musicPositionScrollbar->setMin(0);
	musicPositionScrollbar->setPos(0);
	musicPositionScrollbar->setMax(0);
    musicPositionScrollbar->setSmallStep(0);
    musicPositionScrollbar->setLargeStep(0);

    /// Create GUI music scrollbar: (Volume)
    musicVolumeScrollbar = guienv->addScrollBar(true, rect<s32>(58, 26, 210, 36), win2, ID_SCROLLBAR_VOLUME);
	musicVolumeScrollbar->setMin(0);
	musicVolumeScrollbar->setPos(10);
	musicVolumeScrollbar->setMax(20);
    musicVolumeScrollbar->setSmallStep(1);
    musicVolumeScrollbar->setLargeStep(20); /// setLargeStep(20) == setMax(20)

    /// Create GUI music scrollbar: (Pitch)
    musicPitchScrollbar = guienv->addScrollBar(true, rect<s32>(58, 40, 210, 50), win2, ID_SCROLLBAR_PITCH);
	musicPitchScrollbar->setMin(0);
	musicPitchScrollbar->setPos(10);
	musicPitchScrollbar->setMax(20);
    musicPitchScrollbar->setSmallStep(1);
    musicPitchScrollbar->setLargeStep(20); /// setLargeStep(20) == setMax(20)

	// ((IGUIListBox*)guienv->getRootGUIElement()->getElementFromId(100, true))->getSelected();
	// ((IGUIScrollBar*)guienv->getRootGUIElement()->getElementFromId(100, true))->getPos(); // setColor((EGUI_DEFAULT_COLOR)9, SColor(150,255,255,255));


	/// Create 3D CLOCK:
    scene::ISceneNode* nBody   = NULL;
    scene::ISceneNode* nHour   = NULL;
    scene::ISceneNode* nMinute = NULL;
    scene::ISceneNode* nSecond = NULL;
    CClockSceneNode* nodeClock = NULL;

    /// Create 3D CLOCK:
	nBody = smgr->addMeshSceneNode(smgr->getMesh("clock_body.obj"));
	nBody->setPosition(vector3df(0, 193, -691.5));
	for(u32 i=0; i<nBody->getMaterialCount(); i++){
    // nBody->getMaterial(i).setTexture(0, driver->getTexture(""));
    // nBody->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nBody->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nBody->getMaterial(i).getTextureMatrix(1).setTextureScale(0.1f,0.1f); /// Repeat (tile) the texture
	nBody->getMaterial(i).Lighting = true;
	nBody->getMaterial(i).Shininess = 0.01f; // Set size of specular highlights (20.0f == Metallic)
	nBody->getMaterial(i).SpecularColor.set(3,3,3,3); /// The lights reflexes
	// nBody->getMaterial(i).DiffuseColor.set(0,0,0,0);
	// nBody->getMaterial(i).EmissiveColor.set(255,255,255,255);
	};
	nHour = smgr->addMeshSceneNode(smgr->getMesh("clock_hours.obj"));
	nHour->setPosition(vector3df(0, 193, -691.5));
	for(u32 i=0; i<nHour->getMaterialCount(); i++){
    // nHour->getMaterial(i).setTexture(0, driver->getTexture(""));
    // nHour->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nHour->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nHour->getMaterial(i).getTextureMatrix(1).setTextureScale(0.1f,0.1f); /// Repeat (tile) the texture
	nHour->getMaterial(i).Lighting = true;
	nHour->getMaterial(i).Shininess = 0.01f; // Set size of specular highlights (20.0f == Metallic)
	nHour->getMaterial(i).SpecularColor.set(3,3,3,3); /// The lights reflexes
	// nHour->getMaterial(i).DiffuseColor.set(0,0,0,0);
	// nHour->getMaterial(i).EmissiveColor.set(255,255,255,255);
	};
	nMinute = smgr->addMeshSceneNode(smgr->getMesh("clock_minutes.obj"));
	nMinute->setPosition(vector3df(0, 193, -691.5));
	for(u32 i=0; i<nMinute->getMaterialCount(); i++){
	// nMinute->getMaterial(i).setTexture(0, driver->getTexture(""));
    // nMinute->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nMinute->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nMinute->getMaterial(i).getTextureMatrix(1).setTextureScale(0.1f,0.1f); /// Repeat (tile) the texture
	nMinute->getMaterial(i).Lighting = true;
	nMinute->getMaterial(i).Shininess = 0.01f; // Set size of specular highlights (20.0f == Metallic)
	nMinute->getMaterial(i).SpecularColor.set(3,3,3,3); /// The lights reflexes
	// nMinute->getMaterial(i).DiffuseColor.set(0,0,0,0);
	// nMinute->getMaterial(i).EmissiveColor.set(255,255,255,255);
	};
	nSecond = smgr->addMeshSceneNode(smgr->getMesh("clock_seconds.obj"));
	nSecond->setPosition(vector3df(0, 193, -691.5));
	for(u32 i=0; i<nSecond->getMaterialCount(); i++){
	// nSecond->getMaterial(i).setTexture(0, driver->getTexture(""));
    // nSecond->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nSecond->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nSecond->getMaterial(i).getTextureMatrix(1).setTextureScale(0.1f,0.1f); /// Repeat (tile) the texture
	nSecond->getMaterial(i).Lighting = true;
	nSecond->getMaterial(i).Shininess = 0.01f; // Set size of specular highlights (20.0f == Metallic)
	nSecond->getMaterial(i).SpecularColor.set(3,3,3,3); /// The lights reflexes
	// nSecond->getMaterial(i).DiffuseColor.set(0,0,0,0);
	// nSecond->getMaterial(i).EmissiveColor.set(255,255,255,255);
	};
	nodeClock = new CClockSceneNode(device, smgr, nHour, nMinute, nSecond);
	nodeClock->drop();
	ITimer *real_time = device->getTimer();
	int curTimeH = real_time->getRealTimeAndDate().Hour;
	int curTimeM = real_time->getRealTimeAndDate().Minute;
	int curTimeS = real_time->getRealTimeAndDate().Second;
	nodeClock->setTime(curTimeH, curTimeM, curTimeS);
	nodeClock->setUpdatesPerSecond(20);
	nodeClock->setAutoUpdate(true);
	// nodeClock->setShowSeconds(true);
	nodeClock->setUseRealTime(true);

	nBody->setVisible(false);        // Clock body
	nodeClock->setVisibility(false); // Clock pointers



	/// Add GUI images:
    IGUIImage* image[10]={};
	image[0] = guienv->addImage(driver->getTexture("logotipo_1.png"), position2d<int>(SCREEN_WIDTH-145,10));
	// image[1] = guienv->addImage(driver->getTexture("logotipo_2.png"), position2d<int>(SCREEN_WIDTH-145,10));
	// image[2] = guienv->addImage(driver->getTexture("logotipo_3.png"), position2d<int>(SCREEN_WIDTH-145,10));
	// guienv->addGUIElement("myButton", IGUIButton(core::rect< s32> (0,0,200,50), 0, 101, wchar_t "mybutton", wchar_t "clickhere")  )



	std::cout << "GUI images... Ok" << std::endl;

	/// ##########################################################################################
    /// ############# USER INTERFACE (GUI) - END #################################################
    /// ##########################################################################################










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




            /// EVENTS: ( From: "libraries\MainLibs\myEvents.h" )

            // A Keyboard event: (exit)
            if(receiver.IsKeyDown(irr::KEY_ESCAPE)){ quit = true; };


            // A Keyboard event: (pause music + pause device)
            if(receiver.IsKeyDown(irr::KEY_SPACE) && canClickAgain[0] == 1){
            canClickAgain[0] = 0;
            for(int i=0; i<5; i++){ doingCamTransOnce[i]=true; };

            if(INTERNALS_SOUNDS){ soundClick->play(); }; // Audiere
            // sound.Play(); // SFML
            if(music && music->isPlaying()){ /// Confirm if music exists (else crashes!)
            if(INTERNALS_SOUNDS){ soundClaps->stop(); soundClaps->reset(); soundOoo->play(); };  // Booooo

            music->stop(); // Pause music
            paused = true;
            texts[12]->setText(L"Intermission\nThe show will continue in a few moments..."); // PAUSED == break, intermission
            // timer->stop(); // Pause irr device
            } else if(music){  /// Confirm if music exists (else crashes!)
            if(INTERNALS_SOUNDS){ soundClaps->play(); soundOoo->stop(); soundOoo->reset(); };  // Claps
            music->play();  // Start music
            paused = false;
            texts[12]->setText(L"");
            // timer->start(); // Start irr device
            };  // Audiere
            // if(music.GetStatus() == sf::Sound::Playing){ music.Stop(); } else { music.Play(); }  // SFML
            };
            if(receiver.IsKeyUp(irr::KEY_SPACE)){ canClickAgain[0] = 1; };
            // if(music && music->isPlaying()){ texts[12]->setText(L"");  }; /// BUG: To avoid the fails at "play loop mode"










            // A Keyboard event: (play mode music)
            if(receiver.IsKeyDown(irr::KEY_KEY_P) && canClickAgain[2] == 1){
            canClickAgain[2] = 0;
            if(INTERNALS_SOUNDS){ soundClick->play(); }; // Audiere
            // music.Play(); // SFML
            playMode += 1; if(playMode>4){ playMode = 1; }; // Play Modes: 1 Single, 2 Single Loop, 3 List loop, 4 List Random
            stringw textt = "";
            if(playMode == 1){ textt = "Play Mode: Single (P)";      };
            if(playMode == 2){ textt = "Play Mode: Single Loop (P)"; };
            if(playMode == 3){ textt = "Play Mode: List Loop (P)";   };
            if(playMode == 4){ textt = "Play Mode: List Random (P)"; };
            texts[8]->setText(textt.c_str());
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_P)){ canClickAgain[2] = 1; };






            /// GUI ListBox events ------------------------------------------------------------------------------------------ START


            // A GUI ListBox event: (select a music)
            if(ItemSelected_GUIListBox){

            // if(texts[20]!=NULL){ texts[20]->setText(L"Please Standby\nThe show will continue in a moment..."); };

            // if(!music){ std::cout << "music = NULL" << std::endl; };
            // std::cout << "musicListBox->getItemCount() = " << musicListBox->getItemCount() << std::endl; /// Empty returns 0
            // std::cout << "musicListBox->getSelected() = " << musicListBox->getSelected() << std::endl;   /// Empty returns -1
            // std::cout << "musicList.size() = " << musicList.size() << std::endl;                         /// Empty returns 0

            if(musicListBox->getItemCount()>0 && musicListBox->getSelected()>-1 && musicList.size()>0){ // void crashes if musicListBox has no items
            if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            int s = musicListBox->getSelected();
            musicID = s;
            /// -----------------------------------------------------------------------------------
            /// Reset to default camera:
            // CAMERA_SELECTED = 1;
            /// The call to "TEXT TO TEXTURE" function is here:
            /// -----------------------------------------------------------------------------------
            /// And write some text to texture too:
            /// doTextToTexture(nodeFlyingPlackard, "Irrlicht Music Player");
            doTextToTexture(nodeFlyingPlackard, musicListNamesOnly[musicID].c_str());
            /// -----------------------------------------------------------------------------------
            /// Coloring ListBox:
            int c = musicListBox->getItemCount();
            for(int i=0; i<c; i++){ musicListBox->setItemOverrideColor(i, video::SColor(200,150,150,200)); }; // Colors to default
            musicListBox->setItemOverrideColor(musicID, video::SColor(255,210,235,255));
            /// -----------------------------------------------------------------------------------
            // musicListBox->setItemHeight(10);
            // musicListBox->setItemOverrideColor(s, video::SColor(255,255,0,0)); // Add selected item a new color
            ItemSelected_GUIListBox = false;
            playNextSong = true;
            };

            // if(texts[20]!=NULL){ texts[20]->setText(L""); };

            };



            if(DEBUG_MODE==0){

             // A Keyboard event: (Get ListBox previous item)
            if(receiver.IsKeyDown(irr::KEY_UP) && canClickAgain[20] == 1){
            canClickAgain[20] = 0;
            int totbox = musicListBox->getItemCount();
            if(totbox>0){ /// Void crashes if musicListBox is empty!
            if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            int s = musicListBox->getSelected(); s-=1;
            if(s <= totbox && s <= (int)musicList.size()-1){
            musicListBox->setSelected(s); ItemSelected_GUIListBox = true; /// Select until last list item - and - Trigger: Load new music + lyric
            } else {
            musicListBox->setSelected(totbox-1); ItemSelected_GUIListBox = true; /// Goes to last list item - and - Trigger: Load new music + lyric
            };
            };
            };
            if(receiver.IsKeyUp(irr::KEY_UP)){ canClickAgain[20] = 1; };

            // A Keyboard event: (Get ListBox next item)
            if(receiver.IsKeyDown(irr::KEY_DOWN) && canClickAgain[21] == 1){
            canClickAgain[21] = 0;
            int totbox = musicListBox->getItemCount();
            if(totbox>0){ /// Void crashes if musicListBox is empty!
            if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            int s = musicListBox->getSelected(); s+=1;
            if(s <= totbox && s <= (int)musicList.size()-1){
            musicListBox->setSelected(s); ItemSelected_GUIListBox = true; /// Select until last list item - and - Trigger: Load new music + lyric
            } else {
            musicListBox->setSelected(0); ItemSelected_GUIListBox = true; /// Goes to the first list item - and - Trigger: Load new music + lyric
            };
            };
            };
            if(receiver.IsKeyUp(irr::KEY_DOWN)){ canClickAgain[21] = 1; };

            };


            // A Keyboard event: (Get ListBox current item)
            if(receiver.IsKeyDown(irr::KEY_RETURN) && canClickAgain[26] == 1){
            canClickAgain[26] = 0;
            int totbox = musicListBox->getItemCount();
            if(totbox>0){ /// Void crashes if musicListBox is empty!
            if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            signed int s = musicListBox->getSelected();
            if(s <= totbox && s <= (int)musicList.size()-1){ musicListBox->setSelected(s); ItemSelected_GUIListBox = true; }; /// Triggers: Load new music + lyrics
            };
            };
            if(receiver.IsKeyUp(irr::KEY_RETURN)){ canClickAgain[26] = 1; };







            /// GUI ListBox events ------------------------------------------------------------------------------------------ END



            // A Keyboard event: (Show FileOpenDialog)
            if(receiver.IsKeyDown(irr::KEY_KEY_D) && canClickAgain[30] == 1){
            canClickAgain[30] = 0;
            if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            if(canCreateNewFOD == true){ /// <-- To allow only 1 dialog instance!
            canCreateNewFOD = false;
            /// addFileOpenDialog --> http://irrlicht.sourceforge.net/docu/classirr_1_1gui_1_1_i_g_u_i_environment.html#aaac7c786a19c5cac51a550ce77cd972a
            musicFileDialog = guienv->addFileOpenDialog(L"", false, 0, -1, false, 0);
            };
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_D)){ canClickAgain[30] = 1; };

            // FileOpenDialog is done!...
            if(DirSelected_GUIFileOpenDialog == true){
            /// Fill ListBox:
            if(musicListBox){ fillListBox(); };
            // Reset:
            DirSelected_GUIFileOpenDialog = false;
            canCreateNewFOD = true;
            };



            // if(music && !music->isPlaying()){ } //  && playOneSong
            // if(!playSongNow && (music && !music->isPlaying()) && (playMode == 3 || playMode == 4)){ playSongNow = true; }
            /// TO-DO work still on progres...

            /// Play Mode: 1 - Single
            if(playMode == 1 && playNextSong && paused == false){
            if(musicID<0 || musicID>(int)musicList.size()-1){ musicID = 0; }; // Just in case...
            // music.Play(); // SFML
            if(music){ music->setRepeat(false); }; // Audiere
            playNextSong = false;
            musicListBox->setSelected(musicID);    // Update ListBox

            std::string mm = musicList[musicID];            // Get song name (full path) + GUI Buttons array starts at 100
            std::string nn = musicLyricsListPath[musicID];  // Get lyrics (full path) + GUI Buttons array starts at 100
            load_MUSIC(mm);
            load_LYRICS(nn);
            if(music){ music->play(); }; // Audiere
            if(music && INTERNALS_SOUNDS){ soundClaps->play(); };
            };



            /// Play Mode: 2 - Single Loop
            /*
            if(playMode == 2 && playNextSong && paused == false && music){
            if(musicID<0 || musicID>musicList.size()-1){ musicID = 0; }; // Just in case...
            // music.Play(); // SFML
            if(music){ music->setRepeat(true); }; // Audiere
            playNextSong = false;
            musicListBox->setSelected(musicID);   // Update ListBox

            std::string mm = musicList[musicID];            // Get song name (full path) + GUI Buttons array starts at 100
            std::string nn = musicLyricsListPath[musicID];  // Get lyrics (full path) + GUI Buttons array starts at 100
            loadMyMusic(mm); load_LYRICS(nn); if(music){ music->play(); }; // Audiere
            if(music && INTERNALS_SOUNDS){ soundClaps->play(); };
            };
            */

            /// Play Mode: 2 - Single Loop
            if(playMode == 2 && playNextSong && paused == false){ /// music is selected
            if(musicID<0 || musicID>(int)musicList.size()-1){ musicID = 0; }; // Just in case...
            // music.Play(); // SFML
            if(music){ music->setRepeat(true); }; // Audiere
            playNextSong = false;
            musicListBox->setSelected(musicID);   // Update ListBox

            // std::string mm = musicList[musicID];            // Get song name (full path) + GUI Buttons array starts at 100
            // std::string nn = musicLyricsListPath[musicID];  // Get lyrics (full path) + GUI Buttons array starts at 100
            // loadMyMusic(mm); load_LYRICS(nn); if(music){ music->play(); }; // Audiere
            // if(music && INTERNALS_SOUNDS){ soundClaps->play(); };
            };


            /// Play Mode: 3 - List Loop
            if(playMode == 3 && playNextSong && paused == false){
            // musicID += 1;
            // if(ClickedGUIButton == false){ musicID += 1; }
            if(musicID<0 || musicID>(int)musicList.size()-1){ musicID = 0; }; // Just in case...
            // music.Play(); // SFML
            if(music){ music->setRepeat(false); }; // Audiere
            playNextSong = false;
            musicListBox->setSelected(musicID);    // Update ListBox

            std::string mm = musicList[musicID];            // Get song name (full path) + GUI Buttons array starts at 100
            std::string nn = musicLyricsListPath[musicID];  // Get lyrics (full path) + GUI Buttons array starts at 100
            load_MUSIC(mm); load_LYRICS(nn); if(music){ music->play(); }; // Audiere
            if(music && INTERNALS_SOUNDS){ soundClaps->play(); };
            };


            /// Play Mode: 4 - List Random
            if(playMode == 4 && playNextSong && paused == false){
            int oldMusicID = musicID;
            musicID = rand()%musicList.size();
            if(musicID == oldMusicID){ musicID += 1; };
            if(musicID<0 || musicID>(int)musicList.size()-1){ musicID = 0; }; // Just in case...
            // music.Play(); // SFML
            if(music){ music->setRepeat(false); }; // Audiere
            playNextSong = false;
            musicListBox->setSelected(musicID);    // Update ListBox

            std::string mm = musicList[musicID];            // Get song name (full path) + GUI Buttons array starts at 100
            std::string nn = musicLyricsListPath[musicID];  // Get lyrics (full path) + GUI Buttons array starts at 100
            load_MUSIC(mm); load_LYRICS(nn); if(music){ music->play(); }; // Audiere
            if(music && INTERNALS_SOUNDS){ soundClaps->play(); };
            };



            // if((music.GetStatus() == sf::Sound::Playing) && (playMode == 3 || playMode == 4)){ // SFML
            if((music && !music->isPlaying()) && (playMode == 3 || playMode == 4) && paused == false){ // Audiere
            if(musicList.size()>0){
            playNextSong = true;
            musicID += 1;
            if(musicID<0 || musicID>(int)musicList.size()-1){ musicID = 0; }; // Just in case...
            /// -----------------------------------------------------------------------------------
            /// Reset to default camera:
            // CAMERA_SELECTED = 1;
            /// The call to "TEXT TO TEXTURE" function is here:
            /// -----------------------------------------------------------------------------------
            /// And write some text to texture too:
            /// doTextToTexture(nodeFlyingPlackard, "Irrlicht Music Player");
            doTextToTexture(nodeFlyingPlackard, musicListNamesOnly[musicID].c_str());
            /// -----------------------------------------------------------------------------------
            /// Coloring ListBox:
            int c = musicListBox->getItemCount();
            for(int i=0; i<c; i++){ musicListBox->setItemOverrideColor(i, video::SColor(200,150,150,200)); }; // Colors to default
            musicListBox->setItemOverrideColor(musicID, video::SColor(255,210,235,255));
            /// -----------------------------------------------------------------------------------
            };
            };



            /// 3D music volume: (todo)
            /*
            if(nodeBand && music){
            f32 px = camera[CAMERA_SELECTED]->getPosition().X;
            f32 pz = camera[CAMERA_SELECTED]->getPosition().Z;
            f32 sx = nodeBand->getPosition().X;
            f32 sz = nodeBand->getPosition().Z;
            // f32 distance = sqrt(pow(std::abs(px-sx),2) + pow(std::abs(pz-sz),2));
            // irr::f32 distance = sqrt(pow(abs(px-sx),2) + pow(abs(pz-sz),2));
            f32 distance = sqrt(pow(std::abs((int)px-(int)sx),2));
            music->setVolume(abs((int)distance-1000)/1000.0);
            };
            */

            /// Render to textute for nodeStageFloor:
            /*
            // set render target texture
            driver->setRenderTarget(rttr, true, true, video::SColor(0,0,0,255));
            smgr->setActiveCamera(camera[2]);
            // draw whole scene into render buffer
            smgr->drawAll();
            // set back old render target (The buffer might have been distorted, so clear it)
            driver->setRenderTarget(0, true, true, 0);
            smgr->setActiveCamera(camera[CAMERA_SELECTED]);
            */




            /// Animate all scene? (if music is playing)
            doAnimations = false;
            if(music && music->isPlaying()){ doAnimations = true; }; // Audiere




            // A Keyboard or Mouse event: (Show/Hide Menu)
            if(receiver.IsKeyDown(irr::KEY_KEY_M) && canClickAgain[3] == 1){
            if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            canClickAgain[3] = 0;
            static int wintohide = 1;
            bool doOnce = true;

            if(wintohide == 1 && doOnce == true){
            win1->setVisible(true);         // Left window visibility: visible
            musicListBox->setVisible(true); // ListBox visibility: visible
            texts[6]->setVisible(true);
            win2->setVisible(false);
            texts[4]->setVisible(false);
            texts[5]->setVisible(false);
            texts[26]->setVisible(false);
            texts[27]->setVisible(false);
            texts[28]->setVisible(false);
            musicPositionScrollbar->setVisible(false);
            musicVolumeScrollbar->setVisible(false);
            musicPitchScrollbar->setVisible(false);
            // Finished:
            wintohide = 2;
            doOnce = false;
            };

            if(wintohide == 2 && doOnce == true){
            win1->setVisible(false);         // Left window visibility: hidden
            musicListBox->setVisible(false); // ListBox visibility: visible
            texts[6]->setVisible(false);
            win2->setVisible(true);
            texts[4]->setVisible(true);
            texts[5]->setVisible(true);
            texts[26]->setVisible(true);
            texts[27]->setVisible(true);
            texts[28]->setVisible(true);
            musicPositionScrollbar->setVisible(true);
            musicVolumeScrollbar->setVisible(true);
            musicPitchScrollbar->setVisible(true);
            // Finished:
            wintohide = 3;
            doOnce = false;
            };

            if(wintohide == 3 && doOnce == true){
            win1->setVisible(false);         // Left window visibility: visible
            musicListBox->setVisible(false); // ListBox visibility: visible
            texts[6]->setVisible(false);
            win2->setVisible(false);
            texts[4]->setVisible(false);
            texts[5]->setVisible(false);
            texts[26]->setVisible(false);
            texts[27]->setVisible(false);
            texts[28]->setVisible(false);
            musicPositionScrollbar->setVisible(false);
            musicVolumeScrollbar->setVisible(false);
            musicPitchScrollbar->setVisible(false);
            // Finished:
            wintohide = 4;
            doOnce = false;
            };

            if(wintohide == 4 && doOnce == true){
            win1->setVisible(true);         // Left window visibility: visible
            musicListBox->setVisible(true); // ListBox visibility: visible
            texts[6]->setVisible(true);
            win2->setVisible(true);
            texts[4]->setVisible(true);
            texts[5]->setVisible(true);
            texts[26]->setVisible(true);
            texts[27]->setVisible(true);
            texts[28]->setVisible(true);
            musicPositionScrollbar->setVisible(true);
            musicVolumeScrollbar->setVisible(true);
            musicPitchScrollbar->setVisible(true);
            // Finished:
            wintohide = 1;
            doOnce = false;
            };

            };
            if(receiver.IsKeyUp(irr::KEY_KEY_M)){ canClickAgain[3] = 1; };



            // A Keyboard or Mouse event: (Show/Hide Infos)
            if(receiver.IsKeyDown(irr::KEY_KEY_I) && canClickAgain[14] == 1){
            if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            canClickAgain[14] = 0;
            infos+=1; // 0 = Music Infos, 1= Lyrics file content, 2 = Help, 3 = About, 4 = Nothing
            if(infos<0 || infos>5){ infos = 0; };
            if(infos==0){ texts[15]->setText(L"Display Infos: System (I)"); };
            if(infos==1){ texts[15]->setText(L"Display Infos: Lyrics (I)"); };
            if(infos==2){ texts[15]->setText(L"Display Infos: Help (I)");   };
            if(infos==3){ texts[15]->setText(L"Display Infos: Keys (I)");   };
            if(infos==4){ texts[15]->setText(L"Display Infos: About (I)");  };
            if(infos==5){ texts[15]->setText(L"Display Infos: None (I)");   };
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_I)){ canClickAgain[14] = 1; };



            // A Keyboard or Mouse event: (Select the Band on stage)
            if(receiver.IsKeyDown(irr::KEY_KEY_B) && canClickAgain[4] == 1){
            if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            canClickAgain[4] = 0;
            texts[14]->setOverrideColor(video::SColor(255,210,235,255));
            // cout << "Band: " << BAND_STYLE << endl;
            // changingBands = false; /// NOT launch moving loop
            BAND_SELECTED += 1;
            if(BAND_SELECTED<=0){ BAND_SELECTED = 0; }; // Just in case
            if(BAND_SELECTED >7){ BAND_SELECTED = 0; }; // No more bands, restart.
            load_BANDMESH(); /// Loading Band...
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_B)){ canClickAgain[4] = 1; };





            /// Moving Smooth Band entering on stage:
            if(!nodeBand){
            BAND_SELECTED = 0;
            load_BANDMESH(); /// Loading Band...
            };

            if(nodeBand){
            f32 bX = nodeBand->getPosition().X;
            f32 bY = nodeBand->getPosition().Y;
            f32 bZ = nodeBand->getPosition().Z;
            if(bX > 0){
            if(STENCIL_SHADOWS==true && texts[20]!=NULL){ texts[20]->setText(L"Generating shadows..."); };
            nodeBand->setPosition(vector3df(bX -= 0.70*frameDeltaTime, bY, bZ));
            };
            if(bX < 0){
            bX = 0.0f;
            if(STENCIL_SHADOWS==true && texts[20]!=NULL){ texts[20]->setText(L"Generating shadows..."); };
            nodeBand->setPosition(vector3df(bX, bY, bZ));
            nodeBand->addShadowVolumeSceneNode(); /// Activates STENCIL_SHADOWS only on this node (BAND) and only AFTER placed in its right position!
            };
            if(bX == 0){
            if(STENCIL_SHADOWS==true && texts[20]!=NULL){ texts[20]->setText(L""); };
            };
            };



             // A Keyboard event: (Change Lyrics Font Size)
            if(receiver.IsKeyDown(irr::KEY_KEY_K) && canClickAgain[16] == 1){
            canClickAgain[16] = 0;
            texts[18]->setOverrideColor(video::SColor(255,210,235,255));
            if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            FONT_SIZE_SELECTED += 1;
            if(FONT_SIZE_SELECTED<1 || FONT_SIZE_SELECTED>6){ FONT_SIZE_SELECTED = 1; };
            if(FONT_SIZE_SELECTED == 1){ FONT_SIZE = L"1"; };
            if(FONT_SIZE_SELECTED == 2){ FONT_SIZE = L"2"; };
            if(FONT_SIZE_SELECTED == 3){ FONT_SIZE = L"3"; };
            if(FONT_SIZE_SELECTED == 4){ FONT_SIZE = L"4"; };
            if(FONT_SIZE_SELECTED == 5){ FONT_SIZE = L"5"; };
            if(FONT_SIZE_SELECTED == 6){ FONT_SIZE = L"6"; };
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_K)){ canClickAgain[16] = 1; };

            stringw fontls = L"Lyrics Size: "; fontls+=FONT_SIZE.c_str(); fontls+=L" (K)";
            texts[18]->setText(fontls.c_str()); /// Set Lyrics Font Size




            // A Keyboard event: (Change Lyrics Colors)
            if(receiver.IsKeyDown(irr::KEY_KEY_L) && canClickAgain[19] == 1){
            canClickAgain[19] = 0;
            texts[19]->setOverrideColor(video::SColor(255,210,235,255));
            if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            FONT_COLOR_SELECTED += 1;
            if(FONT_COLOR_SELECTED<0 || FONT_COLOR_SELECTED>5){ FONT_COLOR_SELECTED = 0; };
            if(FONT_COLOR_SELECTED == 0){ FONT_COLOR = L"White";  };
            if(FONT_COLOR_SELECTED == 1){ FONT_COLOR = L"Red";    };
            if(FONT_COLOR_SELECTED == 2){ FONT_COLOR = L"Green";  };
            if(FONT_COLOR_SELECTED == 3){ FONT_COLOR = L"Blue";   };
            if(FONT_COLOR_SELECTED == 4){ FONT_COLOR = L"Yellow"; };
            if(FONT_COLOR_SELECTED == 5){ FONT_COLOR = L"Purple"; };
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_L)){ canClickAgain[19] = 1; };

            stringw fontlc = L"Lyrics Color: "; fontlc+=FONT_COLOR.c_str(); fontlc+=L" (L)";
            texts[19]->setText(fontlc.c_str()); /// Set Lyrics Font Color

            /// Lyrics Font final calculations:
            if(FONT_COLOR_SELECTED == 0){ FONT_COLOR = L"White";  FONT_COLOR_RED = 255; FONT_COLOR_GREEN = 255; FONT_COLOR_BLUE = 255; };
            if(FONT_COLOR_SELECTED == 1){ FONT_COLOR = L"Red";    FONT_COLOR_RED = 255; FONT_COLOR_GREEN = 0;   FONT_COLOR_BLUE = 0;   };
            if(FONT_COLOR_SELECTED == 2){ FONT_COLOR = L"Green";  FONT_COLOR_RED = 0;   FONT_COLOR_GREEN = 255; FONT_COLOR_BLUE = 0;   };
            if(FONT_COLOR_SELECTED == 3){ FONT_COLOR = L"Blue";   FONT_COLOR_RED = 0;   FONT_COLOR_GREEN = 0;   FONT_COLOR_BLUE = 255; };
            if(FONT_COLOR_SELECTED == 4){ FONT_COLOR = L"Yellow"; FONT_COLOR_RED = 255; FONT_COLOR_GREEN = 255; FONT_COLOR_BLUE = 0;   };
            if(FONT_COLOR_SELECTED == 5){ FONT_COLOR = L"Purple"; FONT_COLOR_RED = 200; FONT_COLOR_GREEN = 100; FONT_COLOR_BLUE = 255; };

            /// Apply lyrics font styles:
            texts[7]->setOverrideFont(font_lyrics[FONT_SIZE_SELECTED]); // Set the new lyrics font size
            /// texts[7]->setOverrideColor(video::SColor(255, FONT_COLOR_RED, FONT_COLOR_GREEN, FONT_COLOR_BLUE)); // Set the new lyrics font color





            /// Pitch Keys:
            /// ------------------------------------------->>>

            // A Keyboard event: (Pitch - reduce)
            if(receiver.IsKeyDown(irr::KEY_KEY_7) && canClickAgain[24] == 1){
            canClickAgain[24] = 0;
            userDragging_pitchScroolbar = true;
            /// if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            f32 sp = musicPitchScrollbar->getPos();
            sp -= 1;
            if(sp<=0 ){ sp = 0;  };
            if(sp>=20){ sp = 20; };
            musicPitchScrollbar->setPos(sp);
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_7)){ canClickAgain[24] = 1; };

            // A Keyboard event: (Pitch - increase)
            if(receiver.IsKeyDown(irr::KEY_KEY_8) && canClickAgain[25] == 1){
            canClickAgain[25] = 0;
            userDragging_pitchScroolbar = true;
            /// if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            f32 sp = musicPitchScrollbar->getPos();
            sp += 1;
            if(sp<=0 ){ sp = 0;  };
            if(sp>=20){ sp = 20; };
            musicPitchScrollbar->setPos(sp);
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_8)){ canClickAgain[25] = 1; };


            /// Pitch Scrollbar Controller:
            /// ------------------------------------------->>>

            if(userDragging_pitchScroolbar == true){
            f32 sp = musicPitchScrollbar->getPos();
            // musicVolumeScrollbar->setPos(sp);
            MUSIC_PITCH = sp/10;
            if(MUSIC_PITCH <= 0.1){ MUSIC_PITCH = 0.1; };
            if(MUSIC_PITCH >= 2.0){ MUSIC_PITCH = 2.0; };
            if(music){ music->setPitchShift(MUSIC_PITCH); }; /// Apply to the music

            stringw pitchtxt = L""; for(float i=0.0; i<MUSIC_PITCH; i+=0.1){ pitchtxt += L"|"; };
            /// Decimal places restriction:
            std::stringstream pssin; pssin.precision(1); pssin << std::fixed << MUSIC_PITCH; std::string pssout = pssin.str();
            /// Display texts:
            if(pssout=="1.0"){ pitchtxt += L" 1.0 Normal"; } else { pitchtxt += L" "; pitchtxt += pssout.c_str(); };
            texts[27]->setText(pitchtxt.c_str());
            userDragging_pitchScroolbar = false;
            };

            /// ------------------------------------------->>>






            /// Volume Keys:
            /// ------------------------------------------->>>

            /// Compares mouse position and ListBox and FileDialog positions: (ELSE: mouse wheel event works at Volume Bar and on that GUI elements simultaneously!)
            core::position2di mousepos = device->getCursorControl()->getPosition();

            if(musicListBox && musicListBox->isVisible()){
            core::rect<int> listboxpos = musicListBox->getRelativePosition();
            if((mousepos.X<listboxpos.UpperLeftCorner.X || mousepos.X>listboxpos.LowerRightCorner.X) || (mousepos.Y<listboxpos.UpperLeftCorner.Y || mousepos.Y>listboxpos.LowerRightCorner.Y)){
            mouseIsOutsideThisArea = true;   // Mouse is outside ListBox area!
            } else {
            mouseIsOutsideThisArea = false;  // Mouse is inside ListBox area!
            userMouseWheel = "";             // Reset this one too!
            };
            };
            if(musicListBox && !musicListBox->isVisible()){
            mouseIsOutsideThisArea = true;   /// ListBox is hidden, so mouse is outside the forbidden area!
            };

            if(musicFileDialog && canCreateNewFOD == false){
            core::rect<int> fdialogpos = musicFileDialog->getRelativePosition();
            if((mousepos.X<fdialogpos.UpperLeftCorner.X || mousepos.X>fdialogpos.LowerRightCorner.X) || (mousepos.Y<fdialogpos.UpperLeftCorner.Y || mousepos.Y>fdialogpos.LowerRightCorner.Y)){
            mouseIsOutsideThisArea = true;   // Mouse is outside FileDialog area!
            } else {
            mouseIsOutsideThisArea = false;  // Mouse is inside FileDialog area!
            userMouseWheel = "";            // Reset this one too!
            };
            };
            if(musicFileDialog && canCreateNewFOD == true){
            mouseIsOutsideThisArea = true;   /// FileDialog is hidden, so mouse is outside the forbidden area!
            };


            // cout << "musicListBox: " << win->isVisible() << endl;



            // A Keyboard event: (Volume - reduce)
            if((receiver.IsKeyDown(irr::KEY_KEY_9) || (userMouseWheel == "down" && mouseIsOutsideThisArea)) && canClickAgain[22] == 1){
            canClickAgain[22] = 0;
            userDragging_volumeScroolbar = 1;
            userMouseWheel = "";
            /// if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            f32 sp = musicVolumeScrollbar->getPos();
            sp -= 1;
            if(sp<=0 ){ sp = 0;  };
            if(sp>=20){ sp = 20; };
            musicVolumeScrollbar->setPos(sp);
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_9)){ canClickAgain[22] = 1; };

            // A Keyboard event: (Volume - increase)
            if((receiver.IsKeyDown(irr::KEY_KEY_0) || (userMouseWheel == "up" && mouseIsOutsideThisArea)) && canClickAgain[23] == 1){
            canClickAgain[23] = 0;
            userDragging_volumeScroolbar = 1;
            userMouseWheel = "";
            /// if(INTERNALS_SOUNDS){ soundClick->play(); };  // Audiere
            f32 sp = musicVolumeScrollbar->getPos();
            sp += 1;
            if(sp<=0 ){ sp = 0;  };
            if(sp>=20){ sp = 20; };
            musicVolumeScrollbar->setPos(sp);
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_0)){ canClickAgain[23] = 1; };

            /// Volume Scrollbar Controller:
            /// ------------------------------------------->>>

            if(userDragging_volumeScroolbar == 1){
            f32 sp = musicVolumeScrollbar->getPos();
            // musicVolumeScrollbar->setPos(sp);
            GLOBAL_VOLUME = sp/20;
            if(GLOBAL_VOLUME<=0.0){ GLOBAL_VOLUME = 0.0; };
            if(GLOBAL_VOLUME>=1.0){ GLOBAL_VOLUME = 1.0; };
            if(music     ){ music->setVolume(GLOBAL_VOLUME);        }; /// Apply to the music
            if(soundClick){ soundClick->setVolume(GLOBAL_VOLUME/2); }; /// Apply to audio
            if(soundIntro){ soundIntro->setVolume(GLOBAL_VOLUME/2); }; /// Apply to audio
            if(soundOoo  ){ soundOoo->setVolume(GLOBAL_VOLUME/3);   }; /// Apply to audio
            if(soundClaps){ soundClaps->setVolume(GLOBAL_VOLUME/2); }; /// Apply to audio
            stringw volt = L""; for(float i=0.0; i<GLOBAL_VOLUME; i+=0.05){ volt += L"|"; };
            /// Decimal places restriction:
            std::stringstream vssin; vssin.precision(2); vssin << std::fixed << GLOBAL_VOLUME; std::string vssout = vssin.str();
            /// Display texts:
            if(vssout=="0.00"){ volt += L"Mute";  } else { volt += L""; };
            if(vssout=="0.05"){ volt += L" 5%";   } else { volt += L""; };
            if(vssout=="0.10"){ volt += L" 10%";  } else { volt += L""; };
            if(vssout=="0.15"){ volt += L" 15%";  } else { volt += L""; };
            if(vssout=="0.20"){ volt += L" 20%";  } else { volt += L""; };
            if(vssout=="0.25"){ volt += L" 25%";  } else { volt += L""; };
            if(vssout=="0.30"){ volt += L" 30%";  } else { volt += L""; };
            if(vssout=="0.35"){ volt += L" 35%";  } else { volt += L""; };
            if(vssout=="0.40"){ volt += L" 40%";  } else { volt += L""; };
            if(vssout=="0.45"){ volt += L" 45%";  } else { volt += L""; };
            if(vssout=="0.50"){ volt += L" 50%";  } else { volt += L""; };
            if(vssout=="0.55"){ volt += L" 55%";  } else { volt += L""; };
            if(vssout=="0.60"){ volt += L" 60%";  } else { volt += L""; };
            if(vssout=="0.65"){ volt += L" 65%";  } else { volt += L""; };
            if(vssout=="0.70"){ volt += L" 70%";  } else { volt += L""; };
            if(vssout=="0.75"){ volt += L" 75%";  } else { volt += L""; };
            if(vssout=="0.80"){ volt += L" 80%";  } else { volt += L""; };
            if(vssout=="0.85"){ volt += L" 85%";  } else { volt += L""; };
            if(vssout=="0.90"){ volt += L" 90%";  } else { volt += L""; };
            if(vssout=="0.95"){ volt += L" 95%";  } else { volt += L""; };
            if(vssout=="1.00"){ volt += L" 100%"; } else { volt += L""; };
            texts[26]->setText(volt.c_str());
            userDragging_volumeScroolbar = 0;
            };

            /// ------------------------------------------->>>










            // A Keyboard event: (choose view camera)
            if(receiver.IsKeyDown(irr::KEY_KEY_C) && canClickAgain[5] == 1){
            canClickAgain[5] = 0;
            if(INTERNALS_SOUNDS){ soundClick->play(); }  // Audiere
            CAMERA_SELECTED += 1;
            if(CAMERA_SELECTED <1 || CAMERA_SELECTED >10){ CAMERA_SELECTED = 1; };
            stringw txtcam = L"";
            if(CAMERA_SELECTED==1){ randomCamera = true;  txtcam += L"Camera: Auto (C)"; };
            if(CAMERA_SELECTED >1){ randomCamera = false; txtcam += L"Camera: "; txtcam += CAMERA_SELECTED; txtcam += L" (C)"; };
            texts[1]->setText(txtcam.c_str()); /// Set Camera
            doCameraTransition = true;
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_C)){ canClickAgain[5]  = 1; };



            // A Keyboard event: (choose your sound)
            // texts[11] = guienv->addStaticText(L"Audios: 1, 2, 3", core::rect<s32>(10,sy,200,SCREEN_HEIGHT), false, false, win); // border? wordwrap?

            if(receiver.IsKeyDown(irr::KEY_KEY_1) && canClickAgain[12] == 1){
            canClickAgain[12] = 0;
            if(INTERNALS_SOUNDS){ soundClaps->play(); soundOoo->stop(); soundOoo->reset(); };  // Claps
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_1)){ canClickAgain[12] = 1; };

            if(receiver.IsKeyDown(irr::KEY_KEY_2) && canClickAgain[13] == 1){
            canClickAgain[13] = 0;
            if(INTERNALS_SOUNDS){ soundClaps->stop(); soundClaps->reset(); soundOoo->play(); };  // Booooo
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_2)){ canClickAgain[13] = 1; };



            stringw spectxt = L"Sounds: ";
            if(receiver.IsKeyDown(irr::KEY_KEY_O) && canClickAgain[29] == 1){
            canClickAgain[29] = 0;
            texts[23]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue
            if(INTERNALS_SOUNDS){ soundClick->play(); }  // Audiere
            if(INTERNALS_SOUNDS){ INTERNALS_SOUNDS = false; } else { INTERNALS_SOUNDS = true; };
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_O)){ canClickAgain[29] = 1; };
            // Apply text:
            if(INTERNALS_SOUNDS == false){ spectxt += "Off (O)"; } else { spectxt += "On (O)"; };
            texts[23]->setText(spectxt.c_str());



            stringw txtclock = L"Clock: ";
            if(receiver.IsKeyDown(irr::KEY_KEY_H) && canClickAgain[31] == 1){
            canClickAgain[31] = 0;
            texts[24]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue
            if(INTERNALS_SOUNDS){ soundClick->play(); }  // Audiere
            if(CLOCK_VISIBLE){ CLOCK_VISIBLE = false; } else { CLOCK_VISIBLE = true; };
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_H)){ canClickAgain[31] = 1; };
            // Apply text:
            if(CLOCK_VISIBLE == false){ txtclock += "Off (H)"; } else { txtclock += "On (H)"; };
            texts[24]->setText(txtclock.c_str());








            /*
            // A Keyboard event: (check for updates)
            if(receiver.IsKeyDown(irr::KEY_F1) && canClickAgain[15] == 1){
            canClickAgain[15] = 0;
            if(INTERNALS_SOUNDS){ soundClick->play(); }  // Audiere
            system("cmd /c start http://sourceforge.net/projects/irrlichtmp");
            };
            if(receiver.IsKeyUp(irr::KEY_F1)){ canClickAgain[15] = 1; };

            // A Keyboard event: (visit author webpage)
            if(receiver.IsKeyDown(irr::KEY_F2) && canClickAgain[27] == 1){
            canClickAgain[27] = 0;
            if(INTERNALS_SOUNDS){ soundClick->play(); }  // Audiere
            system("cmd /c start http://sites.google.com/site/jorgerosaportfolio");
            };
            if(receiver.IsKeyUp(irr::KEY_F2)){ canClickAgain[27] = 1; };
            */


            // A Keyboard event: (Show type)
            if(receiver.IsKeyDown(irr::KEY_KEY_S) && canClickAgain[28] == 1){
            canClickAgain[28] = 0;
            texts[22]->setOverrideColor(video::SColor(255,210,235,255));
            if(INTERNALS_SOUNDS){ soundClick->play(); }  // Audiere
            SHOW_TYPE += 1;
            if(SHOW_TYPE<0 || SHOW_TYPE>2){ SHOW_TYPE = 0; };
            };
            if(receiver.IsKeyUp(irr::KEY_KEY_S)){ canClickAgain[28] = 1; };

            stringw txt = L"Show: ";
            if(SHOW_TYPE == 0){ txt += L"Slow (S)";   };
            if(SHOW_TYPE == 1){ txt += L"Normal (S)"; };
            if(SHOW_TYPE == 2){ txt += L"Fast (S)";   };
            texts[22]->setText(txt.c_str());




            // A Keyboard event: (cameras ZOOM)
            /*
            if(receiver.IsKeyDown(irr::KEY_KEY_Z)){ // Zoom out
            f32 newFOV1 = camera[CAMERA_SELECTED]->getFOV();
            // newFOV1 = core::min_(newFOV1 + DEGTORAD, core::PI * .5f);
            newFOV1 = core::min_(newFOV1 + DEGTORAD, core::PI * 1.0f);
            if(newFOV1 >=1.5f){ newFOV1 = 1.5f; };
            camera[CAMERA_SELECTED]->setFOV(newFOV1);
            };
            if(receiver.IsKeyDown(irr::KEY_KEY_X)){ // Zoom in
            f32 newFOV1 = camera[CAMERA_SELECTED]->getFOV();
            // newFOV1 = core::max_(newFOV1 - DEGTORAD, core::PI * .0125f);
            newFOV1 = core::max_(newFOV1 - DEGTORAD, core::PI * 0.0250f);
            if(newFOV1 <=0.3f){ newFOV1 = 0.3f; };
            camera[CAMERA_SELECTED]->setFOV(newFOV1);
            };
            */




            static int previous_cam = 999999999;
            static int rcam = 999999999;
            static int keepcam = 0; // Keep random camera a bit, before change it.
            static int keepcam_duration = 12000; // For some cameras stay longer time than others
            if(SHOW_TYPE==0){ keepcam_duration = 12000; };
            if(SHOW_TYPE==1){ keepcam_duration =  8000; };
            if(SHOW_TYPE==2){ keepcam_duration =  5000; };
            keepcam += 1*frameDeltaTime; // keep random camera an minimum ammount of time

            if(doAnimations && randomCamera && keepcam>=keepcam_duration){
            // Reset cameras out of random loop:
            for(int i=0; i<5; i++){ doingCamTransOnce[i]=true; };
            // Random camera:
            rcam = (rand()%10)+1; // 1 to 11
            if(previous_cam == rcam) { /// Avoid new same camera again
            keepcam = 999999999;
            } else {
            previous_cam = rcam;
            CAMERA_SELECTED = 1;
            doCameraTransition = true;
            if(rcam == 1 ){ CAMERA_SELECTED = 1;  keepcam = 0; doCameraTransition = true; };
            if(rcam == 2 && SHOW_TYPE != 0){ CAMERA_SELECTED = 2; keepcam = 0; doCameraTransition = true; }; /// Disabled in Show:Slow
            if(rcam == 3 ){ CAMERA_SELECTED = 3;  keepcam = 0; doCameraTransition = true; };
            if(rcam == 4 ){ CAMERA_SELECTED = 4;  keepcam = 0; doCameraTransition = true; };
            if(rcam == 5 ){ CAMERA_SELECTED = 5;  keepcam = 0; doCameraTransition = true; };
            if(rcam == 6 && SHOW_TYPE != 0){ CAMERA_SELECTED = 6; doCameraTransition = true; }; /// Disabled in Show:Slow
            if(rcam == 7 && SHOW_TYPE == 0){ CAMERA_SELECTED = 7; keepcam = +9000; doCameraTransition = true; }; /// The camera from behind bands (less time)
            if(rcam == 7 && SHOW_TYPE == 1){ CAMERA_SELECTED = 7; keepcam = +5000; doCameraTransition = true; }; /// The camera from behind bands (less time)
            if(rcam == 7 && SHOW_TYPE == 2){ CAMERA_SELECTED = 7; keepcam = +2000; doCameraTransition = true; }; /// The camera from behind bands (less time)
            if(rcam == 8 ){ CAMERA_SELECTED = 8;  keepcam = 0; doCameraTransition = true; };
            if(rcam == 9 ){ CAMERA_SELECTED = 9;  keepcam = 0; doCameraTransition = true; };
            if(rcam == 10){ CAMERA_SELECTED = 10; keepcam = -3000; doCameraTransition = true; }; /// The follow path camera (more time)
            // if(rcam >= 10 && rcam < 16){ CAMERA_SELECTED = 1;  keepcam = 0; keepcam_duration = 2000; }; /// More chances for camera 1
            // if(rcam >= 16 && rcam < 21){ CAMERA_SELECTED = 5;  keepcam = 0; keepcam_duration = 2000; }; /// More chances for camera 5
            // if(rcam >= 21 && rcam < 25){ CAMERA_SELECTED = 10; keepcam = 0; keepcam_duration = 6000; }; /// More chances for camera 10
            newFOV1 = (double)((rand())/0.90f);
            if(newFOV1 >= 0.90f){ newFOV1 = 0.90f; };
            if(newFOV1 <= 0.60f){ newFOV1 = 0.60f; };
            camera[CAMERA_SELECTED]->setFOV(newFOV1);
            };
            };





            /// If music exists, show is not slow mode, and music >5 sec to end, do EFFECTS:
            /// Cameras effects: (postrender effects)
            if(doAnimations && SHOW_TYPE != 0 && songCurrentTime<(songFullTime-5)){
            /// Random Camera effects ---> TODO: Sync this stuff, with song beats
            if(doEFFrand){
            int df = 0;
            if(SHOW_TYPE==1){ df = rand()%1000; }; // 1500; // 800
            if(SHOW_TYPE==2){ df = rand()%800;  }; // 1500; // 800
            if(df == 100){ waitEFF = 0; doEFF = true; } else { doEFF = false; };
            };
            waitEFF += 0.1*frameDeltaTime;
            // if(doEFF && waitEFF <100){
            if(doEFF && waitEFF == 1){ /// Random effect
            doEFF = true;
            doEFFrand = false;
            ppeffectsEnable = true;
            ppEffectsRand = rand()%ppeCount;
            };
            if(doEFF && waitEFF > 1 && waitEFF < 70){ /// Keep the effect for a while
            doEFF = true;
            doEFFrand = false;
            ppeffectsEnable = true;
            // ppEffectsRand = rand()%ppeCount;
            };
            if(doEFF && waitEFF >= 70){ /// Remove effects
            waitEFF = 0;
            doEFF = false;
            doEFFrand = true;
            ppeffectsEnable = false;
            // ppEffectsRand = rand()%ppeCount;
            };
            } else { /// Remove effects
            waitEFF = 0;
            doEFF = false;
            doEFFrand = true;
            ppeffectsEnable = false;
            // ppEffectsRand = rand()%ppeCount;
            };


            /// If MUTED, always do grey effect:
            if(GLOBAL_VOLUME<=0.0){
            doEFF = true;           /// For effect
            doEFFrand = false;      /// For effect
            ppeffectsEnable = true; /// For effect
            ppEffectsRand = 0;      /// 0 == Do only grey effect now
            };





            /// Cameras FAST ZOOMS in/out: Waiting...
            zoomwait += 1 * frameDeltaTime;

            if(zoomwait >= zoomwaited){ zoomwaited = rand()%20000+1000; zoomwait = 0.0; dozoomrand = true; };
            /// Cameras FAST ZOOMS in/out: Do it!
            if(doAnimations){

            if(dozoomrand){
            int direction = rand()%2; // 0 to 1
            if(direction == 0){ doZoomingIn = true;  };
            if(direction == 1){ doZoomingIn = false; };
            dozoomrand = false;
            };
            if(doZoomingIn && zoomwait<800){
            f32 fovShowSpeed = 0.00030f;
            if(SHOW_TYPE ==0){ fovShowSpeed = 0.00001f; };
            if(SHOW_TYPE ==1){ fovShowSpeed = 0.00030f; };
            if(SHOW_TYPE ==2){ fovShowSpeed = 0.00090f; };
            /// Random Camera zooms ---> TO-DO: Sync this stuff, with song beats
            camRatio1 = ((4.0-0.0)*((irr::f32)rand()/RAND_MAX))+1.0; // from (0.0 to 4.0)
            camRatio2 = ((3.0-0.0)*((irr::f32)rand()/RAND_MAX))+1.0; // from (0.0 to 3.0)
            newFOV1 -= fovShowSpeed * frameDeltaTime;
            if(newFOV1 >= 1.25f){ newFOV1 = 1.25f; doZoomingIn = true;  };
            if(newFOV1 <= 0.55f){ newFOV1 = 0.55f; doZoomingIn = false; };
            camera[CAMERA_SELECTED]->setFOV(newFOV1);
            camera[CAMERA_SELECTED]->setAspectRatio(camRatio1/camRatio2); // Sets the aspect ratio (irr camera default ratio is: 4.0f/3.0f)
            };
            if(!doZoomingIn && zoomwait<800){
            f32 fovShowSpeed = 0.00030f;
            if(SHOW_TYPE ==0){ fovShowSpeed = 0.00001f; };
            if(SHOW_TYPE ==1){ fovShowSpeed = 0.00030f; };
            if(SHOW_TYPE ==2){ fovShowSpeed = 0.00090f; };
            camRatio1 = ((4.0-0.0)*((irr::f32)rand()/RAND_MAX))+1.0; // from (0.0 to 4.0)
            camRatio2 = ((3.0-0.0)*((irr::f32)rand()/RAND_MAX))+1.0; // from (0.0 to 3.0)
            newFOV1 += fovShowSpeed * frameDeltaTime;
            if(newFOV1 >= 1.25f){ newFOV1 = 1.25f; doZoomingIn = true;  };
            if(newFOV1 <= 0.55f){ newFOV1 = 0.55f; doZoomingIn = false; };
            camera[CAMERA_SELECTED]->setFOV(newFOV1);
            camera[CAMERA_SELECTED]->setAspectRatio(camRatio1/camRatio2); // Sets the aspect ratio (irr camera default ratio is: 4.0f/3.0f)
            };

            /// Cameras CONSTANT SLOW ZOOMS in/out:
            if(doZoomingIn2){
            f32 fovShowSpeed = 0.00012f;
            int fovmultiplier = 2;
            if(SHOW_TYPE ==0){ fovShowSpeed = 0.00001f; fovmultiplier = 10; };
            if(SHOW_TYPE ==1){ fovShowSpeed = 0.00012f; fovmultiplier = 1;  };
            if(SHOW_TYPE ==2){ fovShowSpeed = 0.00015f; fovmultiplier = 2;  };
            /// Random Camera zooms ---> TO-DO: Sync this stuff, with song beats
            camRatio1 = ((4.0-0.0)*((irr::f32)rand()/RAND_MAX))+1.0; // from (0.0 to 4.0)
            camRatio2 = ((3.0-0.0)*((irr::f32)rand()/RAND_MAX))+1.0; // from (0.0 to 3.0)
            newFOV2 = (fovShowSpeed*frameDeltaTime);
            newFOV1 -= newFOV2*fovmultiplier;
            if(newFOV1 >= 1.25f){ newFOV1 = 1.25f; doZoomingIn2 = true;  };
            if(newFOV1 <= 0.55f){ newFOV1 = 0.55f; doZoomingIn2 = false; };
            camera[CAMERA_SELECTED]->setFOV(newFOV1);
            camera[CAMERA_SELECTED]->setAspectRatio(camRatio1/camRatio2); // Sets the aspect ratio (irr camera default ratio is: 4.0f/3.0f)
            // cout << "Zooming In: " << newFOV1 << endl;
            };
            if(!doZoomingIn2){
            f32 fovShowSpeed = 0.00012f;
            int fovmultiplier = 2;
            if(SHOW_TYPE ==0){ fovShowSpeed = 0.00001f; fovmultiplier = 10; };
            if(SHOW_TYPE ==1){ fovShowSpeed = 0.00012f; fovmultiplier = 1;  };
            if(SHOW_TYPE ==2){ fovShowSpeed = 0.00015f; fovmultiplier = 2;  };
            camRatio1 = ((4.0-0.0)*((irr::f32)rand()/RAND_MAX))+1.0; // from (0.0 to 4.0)
            camRatio2 = ((3.0-0.0)*((irr::f32)rand()/RAND_MAX))+1.0; // from (0.0 to 3.0)
            newFOV2 = (fovShowSpeed*frameDeltaTime);
            newFOV1 += newFOV2*fovmultiplier;
            if(newFOV1 >= 1.25f){ newFOV1 = 1.25f; doZoomingIn2 = true;  };
            if(newFOV1 <= 0.55f){ newFOV1 = 0.55f; doZoomingIn2 = false; };
            camera[CAMERA_SELECTED]->setFOV(newFOV1);
            camera[CAMERA_SELECTED]->setAspectRatio(camRatio1/camRatio2); // Sets the aspect ratio (irr camera default ratio is: 4.0f/3.0f)
            // cout << "Zooming Out: " << newFOV1 << endl;
            };

            }; // END: if(doAnimations)

            /// No music, then set FOV (zoom) to it default value: (0.80f)
            if(!doAnimations && newFOV1!=0.80f){
            f32 fovShowSpeed = 0.00004f;
            int fovmultiplier = 2;
            camRatio1 = ((4.0-0.0)*((irr::f32)rand()/RAND_MAX))+1.0; // from (0.0 to 4.0)
            camRatio2 = ((3.0-0.0)*((irr::f32)rand()/RAND_MAX))+1.0; // from (0.0 to 3.0)
            newFOV2 = (fovShowSpeed*frameDeltaTime);
            if(newFOV1 > 0.80f){ newFOV1 -= (newFOV2*fovmultiplier); };
            if(newFOV1 < 0.80f){ newFOV1 += (newFOV2*fovmultiplier); };
            if(newFOV1 >= 1.25f){ newFOV1 = 1.25f; };
            if(newFOV1 <= 0.55f){ newFOV1 = 0.55f; };
            camera[CAMERA_SELECTED]->setFOV(newFOV1);
            camera[CAMERA_SELECTED]->setAspectRatio(camRatio1/camRatio2); // Sets the aspect ratio (irr camera default ratio is: 4.0f/3.0f)
            };


            /*
            /// Animate textures (reflexes.jpg)
            static f32 luzinhas_rotY = 0.0f;
            nodeStage->getMaterial(0).getTextureMatrix(1).setTextureRotationCenter( luzinhas_rotY );
            // nodeStage->getMaterial(0).getTextureMatrix(0).setRotationDegrees(irr::core::vector3d<f32>(luzinhas_rotY,luzinhas_rotY,luzinhas_rotY));
            luzinhas_rotY += 0.05 * frameDeltaTime;
            */




            if(doAnimations){
            /// Animate textures --> http://www.irrlicht3d.org/wiki/index.php?n=Main.HowToUseTexturesAndMaterials
            // 1) Animated "irrlich words": (Scrolling Texture)
            // int mc = nodeStagePlackard->getMaterialCount();
            // std::cout << "Material count: " << mc << std::endl;
            // for(int i=0; i<5; i++){
            f32 fspeed = 0;
            if(SHOW_TYPE == 0){ fspeed = 0.00002; };
            if(SHOW_TYPE == 1){ fspeed = 0.00003; };
            if(SHOW_TYPE == 2){ fspeed = 0.00006; };
            nodeStagePlackard->getMaterial(0).getTextureMatrix(0).setTextureTranslate(uu*1,vv);  /// Irrlicht characters ---> Range is clipped between 0 and 1 with the default initial value of 0.5.
            nodeFlyingPlackard->getMaterial(1).getTextureMatrix(0).setTextureTranslate(uu*3,vv); /// Plackard ---> Range is clipped between 0 and 1 with the default initial value of 0.5.
            uu += fspeed * frameDeltaTime; // vv += 0.0005;
            if(uu>1.0 || vv>1.0){ uu = 0.0; vv = 0.0; };
            // };

            // 2) Animated "disco spot lights ball": (Scrolling Texture all over the scene)
            int db = rand()%800; if(db==100){ doBall = true; } else { doBall = false; };
            if(doBall){ ballDirection = rand()%2; };

            if(ballDirection==0){
            for(u32 i=0; i<nodeStagePlackard->getMaterialCount(); i++){
            nodeStagePlackard->getMaterial(i).getTextureMatrix(1).setTextureTranslate(uuu,vvv); // Moving ball textures
            // nodeStagePlackard->getMaterial(i).getTextureMatrix(1).setTextureScaleCenter(0.001,0.001);
            };

            uuu += 0.0001 * frameDeltaTime;
            vvv += 0.0001 * frameDeltaTime;
            if(uuu>1.0 || vvv>1.0){ uuu = 0.0; vvv = 0.0; };
            };
            if(ballDirection==1){
            for(u32 i=0; i<nodeStagePlackard->getMaterialCount(); i++){
            nodeStagePlackard->getMaterial(i).getTextureMatrix(1).setTextureRotationCenter(radians); // Rotating ball textures
            };
            radians += 0.003; if(radians>360){ radians=0; };
            };


            // 3A) Animated TV: (Scaling Texture && Rotation)
            // rot += 0.01;
            static bool scale_decrease = true;
            if(scale_decrease==true){
            if(SHOW_TYPE == 0){ zx += 0.00003 * frameDeltaTime; zy += 0.00003 * frameDeltaTime; };
            if(SHOW_TYPE == 1){ zx += 0.00050 * frameDeltaTime; zy += 0.00050 * frameDeltaTime; };
            if(SHOW_TYPE == 2){ zx += 0.00100 * frameDeltaTime; zy += 0.00100 * frameDeltaTime; };
            };
            if(scale_decrease==false){
            if(SHOW_TYPE == 0){ zx -= 0.00003 * frameDeltaTime; zy -= 0.00003 * frameDeltaTime; };
            if(SHOW_TYPE == 1){ zx -= 0.00050 * frameDeltaTime; zy -= 0.00050 * frameDeltaTime; };
            if(SHOW_TYPE == 2){ zx -= 0.00100 * frameDeltaTime; zy -= 0.00100 * frameDeltaTime; };
            };
            if(zx>1.0 || zy>1.0){ zx = 1.0; zy = 1.0; scale_decrease = false; };
            if(zx<0.8 || zy<0.8){ zx = 0.8; zy = 0.8; scale_decrease = true;  };
            for(u32 i=0; i < nodeTVback1->getMaterialCount(); i++){
            nodeTVback1->getMaterial(i).getTextureMatrix(0).setTextureScaleCenter(zx,zx); // from 0 to 1 you're zooming with 0.5 means to 2x;
            // nodeTVback1->getMaterial(i).getTextureMatrix(0).setTextureRotationCenter(rot); // radians
            };

            /// The TVS images:
            nodeTVback1->setVisible(true);
            nodeTVback2->setVisible(true);
            nodeTVback3->setVisible(true);

            } else {

            // 3B) Animated TV: (Scaling Texture && Rotation) = ZERO
            // for(u32 i=0; i<nodeTVback1->getMaterialCount(); i++){
            // nodeTVback1->getMaterial(i).getTextureMatrix(0).setTextureScaleCenter(1,1); // from 0 to 1 you're zooming with 0.5 means to 2x;
            /// nodeTVback1->getMaterial(i).getTextureMatrix(0).setTextureRotationCenter(rot); // radians
            // };

            /// Plackard with texts centered:
            uu = 0.0;
            nodeFlyingPlackard->getMaterial(1).getTextureMatrix(0).setTextureTranslate(uu,vv);

            /// The TV images:
            nodeTVback1->setVisible(false);
            nodeTVback2->setVisible(false);
            nodeTVback3->setVisible(false);

            };




            if(CLOCK_VISIBLE == true){
            /// Show Clock:
            nBody->setVisible(true);        // Clock body
            nodeClock->setVisibility(true); // Clock pointers
            nodeTVback1->setVisible(false); // TV images
            nodeTVback2->setVisible(false); // TV images
            nodeTVback3->setVisible(false); // TV images
            } else {
            /// Hide Clock:
            nBody->setVisible(false);        // Clock body
            nodeClock->setVisibility(false); // Clock pointers
            nodeTVback1->setVisible(true);   // TV images
            nodeTVback2->setVisible(true);   // TV images
            nodeTVback3->setVisible(true);   // TV images
            };



            if(!doAnimations){
            /// Hide TV and Clock:
            nBody->setVisible(false);        // Clock body
            nodeClock->setVisibility(false); // Clock pointers
            nodeTVback1->setVisible(false);  // TV images
            nodeTVback2->setVisible(false);  // TV images
            nodeTVback3->setVisible(false);  // TV images
            };










            /// Constant camera rotation on Y axis:
            rotcamera_X = camera[CAMERA_SELECTED]->getRotation().X;
            // rotcamera_Y = camera[CAMERA_SELECTED]->getRotation().Y;
            // rotcamera_Z = camera[CAMERA_SELECTED]->getRotation().Z;

            // rotcamera_X = 0.2;
            rotcamera_Y = 0.2;
            rotcamera_Z = 0.2;

            f32 rotspeed = 0.000010;
            if(SHOW_TYPE == 0){ rotspeed = 0.000005; };
            if(SHOW_TYPE == 1){ rotspeed = 0.000010; };
            if(SHOW_TYPE == 2){ rotspeed = 0.000010; };


            if(rotcameratoleft == true ){ rotcamera_X -= rotspeed * frameDeltaTime; };
            if(rotcameratoleft == false){ rotcamera_X += rotspeed * frameDeltaTime; };
            if(rotcamera_X < -0.2){ rotcamera_X = -0.2; rotcameratoleft = false;    };
            if(rotcamera_X >  0.2){ rotcamera_X =  0.2; rotcameratoleft = true;     };
            vector3df calc1 = vector3df(rotcamera_X, rotcamera_Y, rotcamera_Z);
            camera[CAMERA_SELECTED]->setRotation(vector3df(calc1));
            /*
            if(rotcameratoleft == true ){ rotcamera_Y -= rotspeed * frameDeltaTime; };
            if(rotcameratoleft == false){ rotcamera_Y += rotspeed * frameDeltaTime; };
            if(rotcamera_Y < -0.2){ rotcamera_Y = -0.2; rotcameratoleft = false;    };
            if(rotcamera_Y >  0.2){ rotcamera_Y =  0.2; rotcameratoleft = true;     };
            vector3df calc2 = vector3df(rotcamera_X, rotcamera_Y, rotcamera_Z);
            camera[CAMERA_SELECTED]->setRotation(vector3df(calc2));

            if(rotcameratoleft == true ){ rotcamera_Z -= rotspeed * frameDeltaTime; };
            if(rotcameratoleft == false){ rotcamera_Z += rotspeed * frameDeltaTime; };
            if(rotcamera_Z < -0.2){ rotcamera_Z = -0.2; rotcameratoleft = false;    };
            if(rotcamera_Z >  0.2){ rotcamera_Z =  0.2; rotcameratoleft = true;     };
            vector3df calc3 = vector3df(rotcamera_X, rotcamera_Y, rotcamera_Z);
            camera[CAMERA_SELECTED]->setRotation(vector3df(calc3));
            */

            /// HELP -> http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=4680
            // Get transformation matrix of node:
            irr::core::matrix4 mx;

            mx.setRotationDegrees(vector3df(rotcamera_X, rotcamera_Y, rotcamera_Z));
            // Transform forward vector of camera:
            vector3df frv = vector3df(cameraTarget);
            mx.transformVect(frv);
            // Transform upvector of camera:
            /// vector3df upv = vector3df(0.0, 1.00, 0.25); /// Set the limits in each AXIs

            static irr::f32 Zdegrees = 0.00;
            static irr::f32 Zmax     = 0.20;
            static irr::f32 Zspeed   = 0.00003;
            static bool Zincrease    = false;

            if(SHOW_TYPE==0){ Zmax = 0.20; Zspeed = ((0.00006-0.00003)*((irr::f32)rand()/RAND_MAX))+0.00003; }; // Random speed float - Between 0.00003 & 0.00006;
            if(SHOW_TYPE==1){ Zmax = 0.25; Zspeed = ((0.00012-0.00006)*((irr::f32)rand()/RAND_MAX))+0.00006; }; // Random speed float - Between 0.00006 & 0.00012
            if(SHOW_TYPE==2){ Zmax = 0.35; Zspeed = ((0.00042-0.00012)*((irr::f32)rand()/RAND_MAX))+0.00012; }; // Random speed float - Between 0.00012 & 0.00042

            if(Zincrease==true ){ Zdegrees += Zspeed * frameDeltaTime; };
            if(Zincrease==false){ Zdegrees -= Zspeed * frameDeltaTime; };
            if(Zdegrees>=  Zmax){ Zdegrees=  Zmax; Zincrease=false; };
            if(Zdegrees<= -Zmax){ Zdegrees= -Zmax; Zincrease=true;  };

            vector3df upv = vector3df(0.0, 1.00, Zdegrees); /// Set the max angle limits for each AXIs
            mx.transformVect(upv);
            // set camera:
            camera[CAMERA_SELECTED]->setUpVector(upv); // Set up vector of camera
            camera[CAMERA_SELECTED]->setTarget(cameraTarget+frv); // Set target of camera (look at point) (The "offset" corrects this)
            camera[CAMERA_SELECTED]->updateAbsolutePosition();








            /// texts[28]->setText(L"");
            stringw taa = L"";
            // Display texts song info:
            if(music){ // (Audiere) Var: doAnimations, cant be here!!!
            // if(music.GetStatus() == sf::Sound::Playing){ // SMFL
            // L"Music: "; selectedMusicInfos += musicListNamesOnly[musicID].c_str(); // Get song name (full path) + GUI Buttons array starts at 100
            stringw lyricstxt = L"";
            if(thisMusicHasLyrics){ lyricstxt += L"Lyrics: Yes"; } else { lyricstxt += L"Lyrics: No"; };
            /// Apply all music infos texts:
            texts[28]->setText(lyricstxt.c_str());



            /// Changing from frames, to seconds:
            songFullTime = music->getLength()/frate;   // (current samples/rate) Retrieves number in seconds, ex: 1756785
            songCurrentTime = music->getPosition()/frate; // (last samples/rate) Retrieves number in seconds, ex: 1756785


            /// Position Scrollbar Controller:
            /// ------------------------------------------->>>

            if(userDragging_positionScroolbar == 1){
            int sp = musicPositionScrollbar->getPos();
            musicPositionScrollbar->setPos(sp);
            music->setPosition(sp);
            userDragging_positionScroolbar = 0;
            lyricsFadeout = true;
            for(int i=0; i<5; i++){ doingCamTransOnce[i]=true; };
            doEFF = true;           /// For effect
            doEFFrand = false;      /// For effect
            ppeffectsEnable = true; /// For effect
            ppEffectsRand = 0;      /// 0 == Do only grey effect now
            };
            musicPositionScrollbar->setPos(music->getPosition());

            /// ------------------------------------------->>>



            int mil1 = songFullTime-(songFullTime-songCurrentTime); /// Gets the time is seconds (Not in milliseconds!)
            int mil2 = songFullTime;                   /// Gets the time is seconds (Not in milliseconds!)

            // int milliseconds1 =  mil1;
            int seconds1 =  mil1%60;
            int minutes1 = (mil1/60)%60;
            int hours1   = (mil1/60/60)%24;
            /// int day1     =  mil1/60/60/24;

            int seconds2 =  mil2%60;
            int minutes2 = (mil2/60)%60;
            int hours2   = (mil2/60/60)%24;
            /// int day2     =  mil2/60/60/24;


            /*

            int mil11 = songFullTime-(songFullTime-songCurrentTime); /// Gets the time is seconds (Not in milliseconds!)
            int mil22 = songFullTime;                   /// Gets the time is seconds (Not in milliseconds!)

            int seconds11 =  mil11%600;
            int minutes11 = (mil11/60)%60;
            int hours11   = (mil11/60/60)%24;
            int day11     =  mil11/60/60/24;

            int seconds22 =  mil22%60;
            int minutes22 = (mil22/60)%60;
            int hours22   = (mil22/60/60)%24;
            int day22     =  mil22/60/60/24;
            */



            /// Changing from frames, to seconds:
            // songFullTime = music->getLength()/frate;   // Retrieves number in seconds, ex: 1756785
            // songCurrentTime = music->getPosition()/frate; // Retrieves number in seconds, ex: 1756785

            // 1000 / (sample rate / number of samples) = milliseconds
            // Retrieves number in seconds, ex: 1756785

            /// https://www.audio-forums.com/threads/samples-to-milliseconds-conversion.9552/


            // float a1 = ((mil1)/frate)*1000;

            /// For instance 128 samples in seconds = 128/44100 = 0,0029 seconds = 2,9ms (That's assuming your samplerate is 44100)
            /// Frame length (in ms) = (samples per frame / sample rate (in hz)) * 1000

            // int tot = samples*a1;

            // float a1 = (music->getPosition()/frate)/1000;
            // double b1 = 1000/a1;
            // cout <<  a1 << endl;





            // Prepare to display (first, we need to convert INT to string):
            std::string h1 = convertIntToString(hours1);
            std::string m1 = convertIntToString(minutes1);
            std::string s1 = convertIntToString(seconds1);
            if(h1.size()<=1){h1="0"+h1;}; if(m1.size()<=1){m1="0"+m1;}; if(s1.size()<=1){s1="0"+s1;};
            taa = h1.c_str(); taa+=":"; taa+=m1.c_str(); taa+=":"; taa+=s1.c_str(); // Ready formated time first line.

            // Prepare to display (first, we need to convert INT to string):
            stringw tbb;
            std::string h2 = convertIntToString(hours2);
            std::string m2 = convertIntToString(minutes2);
            std::string s2 = convertIntToString(seconds2);
            if(h2.size()<=1){h2="0"+h2;}; if(m2.size()<=1){m2="0"+m2;}; if(s2.size()<=1){s2="0"+s2;};
            tbb = h2.c_str(); tbb+=":"; tbb+=m2.c_str(); tbb+=":"; tbb+=s2.c_str(); // Ready formated time second line.

            /// Diplay song times: (add the 2 time lines created above)
            // stringw tss = L"Position: "; tss+=taa; tss+=L" - "; tss+=tbb;
            stringw tss = L""; tss+=taa; tss+=L" - "; tss+=tbb;
            texts[5]->setText(tss.c_str());



            /// Always select camera N1, until 3 seconds of the START of the song:
            if(songCurrentTime<=5 && randomCamera==true){
            CAMERA_SELECTED = 1;
            keepcam = 0;
            if(doingCamTransOnce[1]==true){ doCameraTransition=true; doingCamTransOnce[1]=false; };
            };

            /// Always select camera N5, between 10 a 5 seconds BEFORE the song ENDS:
            if(songCurrentTime>=(songFullTime-15) && randomCamera==true){
            CAMERA_SELECTED = 5;
            keepcam = 0;
            if(doingCamTransOnce[5]==true){ doCameraTransition=true; doingCamTransOnce[5]=false; };
            };

            /// Always select camera N1, 5 seconds BEFORE the song ENDS:
            if(songCurrentTime>=(songFullTime-5) && randomCamera==true){
            CAMERA_SELECTED = 1;
            keepcam = 0;
            if(doingCamTransOnce[1]==true){ doCameraTransition=true; doingCamTransOnce[1]=false; };
            };





            /// Do clapping, at 1 second BEFORE the song ENDS:
            if(INTERNALS_SOUNDS && (songCurrentTime>=(songFullTime-1))){ // At 1 seconds BEFORE the song ENDS, do applause.
            soundClaps->play();
            };

            }; /// Ends: if(music)



            // Get current time, from the above code.
            wstring currentTime = taa.c_str();

            /// Diplay lyrics:

            for(int r=0; r<linesCounter; ++r){
            wstring starTime = arrayLYRICS[r][0];
            wstring endTime  = arrayLYRICS[r][1];

            if(currentTime >= starTime && currentTime < endTime){ /// TIME MATCH! - Comparison between 2 wstring (MUST be without wstring.c_str())
            fulllines = arrayLYRICS[r][2];
            lyricsFadeout = false;
            };

            if(currentTime >= endTime){  /// Comparison between 2 wstring (MUST be without wstring.c_str())
            lyricsFadeout = true;
            };

            };

            /// Display lyrics string:
            texts[7]->setOverrideColor(video::SColor(255, FONT_COLOR_RED, FONT_COLOR_GREEN, FONT_COLOR_BLUE)); // Set the new lyrics font color
            texts[7]->setText(fulllines.c_str());

            if(lyricsFadeout){
            lirFadeOut -= 0.25*frameDeltaTime;
            texts[7]->setOverrideColor(video::SColor(lirFadeOut, FONT_COLOR_RED, FONT_COLOR_GREEN, FONT_COLOR_BLUE));
            if(lirFadeOut<=10){
            lyricsFadeout = false;
            lirFadeOut = 255;
            fulllines = L"";
            };
            };







            if(!doAnimations){
            lyricsFadeout = true;
            };





            /// TEXTS infos are: 0 = Music Infos, 1= Lyrics file content, 2 = Help, 3 = About, 4 = Nothing
            stringw stags;

            if(infos == 0){
            // stags += L"\n\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"SYSTEM\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"Irrlicht v."; stags += device->getVersion(); stags += L"\n";
            stags += L"Video Driver: "; stags += driver->getName(); stags += L"\n";
            stags += L"Audiere v."; stags += audiere::GetVersion(); stags += L"\n"; // Get audiere version
            stags += L"Audio Device: "; if(audiereAudioDevice){ stags += audiereAudioDevice->getName(); }; stags += L"\n\n";   // Get audiere device
            // stags += L"Midi Device: ";  if(audiereMidiDevice) { stags += audiereMidiDevice->getName();  }; stags += L"\n\n"; // Get audiere device
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"3D SCENE\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            FRAMES_PER_SECOND = driver->getFPS();
            stags += L"FPS: "; stags += FRAMES_PER_SECOND; stags += L" (";
            stringw vsync;
            if(VSYNC==true){ vsync = L"On"; } else { vsync = L"Off"; };
            stags += L"Vertical Sync: "; stags += vsync; stags += L")\n";
            stags += L"Stage Lights: "; stags += stagelights; stags += L"\n";
            // int dlc = driver->getDynamicLightCount();
            // stags += L"Dynamic Lights: "; stags += dlc; stags += L"\n";
            stringw effect;
            if(ppeffectsEnable == true){ effect = ppEffectName[ppEffectsRand].c_str(); } else { effect = L"None"; };
            stags += L"Post Effect: "; stags += effect.c_str(); stags += L" \n";
            stags += L"Camera: "; stags += CAMERA_SELECTED; stags += L" \n";
            /// Decimal places restriction:
            irr::f32 float1 = camera[CAMERA_SELECTED]->getFOV();
            std::stringstream ssin1; ssin1.precision(2); ssin1 << std::fixed << float1; std::string ssout1 = ssin1.str(); /// Float into String
            stags += L"Camera FOV: "; stags += ssout1.c_str(); stags += L" \n";
            irr::f32 float2 = Zdegrees;
            std::stringstream ssin2; ssin2.precision(2); ssin2 << std::fixed << float2; std::string ssout2 = ssin2.str();   /// Float into String
            stags += L"Camera Z Degrees: "; stags += ssout2.c_str(); stags += L" \n\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"MUSIC INFO\n"; // stags += ta; stags += "---"; stags += tb; stags += " \n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"Channels: "; stags += fchannels; stags += L"\n";
            stags += L"Rate: "; stags += frate; stags += L" Hz\n"; // Get audiere version
            stags += L"Format: "; stags += musicExtensionsOnly[musicID].c_str(); stags += L"\n\n";
            for(int i=0; i<(int)tags.size(); i++){ stags += tags[i].c_str(); }; // Get all tags
            };
            if(infos == 1){
            // stags += L"\n\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"LYRICS FILE\n"; // Nothing (Hidden)
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += fullLoadedTexts.c_str(); // Full text from .lyrics file
            };
            if(infos == 2){
            // stags += L"\n\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"LYRICS AND SETTINGS HELP\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"(1) Musics and lyrics:\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"The lyrics file is a simply text file and must have the same name of the music file.\n";
            stags += L"The \".TXT\" or \".SRT\" and \".MP3\" (or any other type of music file) must be inside the same directory.\n\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"(2) Defining user settings in the lyrics file:\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"- \"Band:\"             -- options are -->  \"Man\", \"Woman\", \"Pop\", \"Traditional\", \"Rock\", \"Dj\", \"Drummers\", \"Santa\".\n";
            stags += L"- \"Show:\"            -- options are -->  \"Slow\", \"Normal\", \"Fast\".\n";
            stags += L"- \"LyricsColor:\"   -- options are -->  \"White\", \"Red\", \"Green\", \"Blue\", \"Yellow\", \"Purple\".\n";
            stags += L"- \"LyricsSize:\"    -- options are -->  \"1\" to \"6\".\n";
            stags += L"- \"Sounds:\"         -- options are -->  \"On\", \"Off\".\n";
            stags += L"- \"Clock:\"             -- options are -->  \"On\", \"Off\".\n\n";
            stags += L"Examples:\n\n";
            stags += L"<Band:Pop>\n";
            stags += L"<Show:Fast>\n";
            stags += L"<LyricsColor:Yellow>\n";
            stags += L"<LyricsSize:2>\n";
            stags += L"<Clock:On>\n";
            stags += L"<Sounds:On>\n";
            };
            if(infos == 3){
            // stags += L"\n\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"KEYS (RESUME)\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"Music:\n";
            stags += L"Play/Select Music: Enter, Arrow Up, Arrow Down\n";
            stags += L"Play/Pause Music: Space Bar\n";
            stags += L"Music Volume: 9, 0, Mouse Wheel\n";
            stags += L"Music Pitch: 7, 8\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"Lyrics:\n";
            stags += L"Lyrics Color: L\n";
            stags += L"Lyrics Size: K\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            };
            if(infos == 4){
            // stags += L"\n\n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"ABOUT\n"; // stags += ta; stags += "---"; stags += tb; stags += " \n";
            stags += L"------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
            stags += L"IMP - Irrlicht Music Player\n";
            stags += L"Developer: Jorge Rosa - Portugal\n";
            stags += L"Email: jorge.bigarte@gmail.com\n";
            stags += L"Portfolio: http://sites.google.com/site/jorgerosaportfolio\n";
            stags += L"Social: http://www.facebook.com/jorge.bigarte\n";
            stags += L"Project: https://sourceforge.net/projects/irrlichtmp\n";
            stags += L"Developed to ASCR with love!\n\n";
            };
            if(infos == 5){
            stags += L""; // Nothing (Hidden)
            };
            texts[6]->setText(stags.c_str());






            // TEXTS - Band:
            if(BAND_SELECTED == 0){ BAND_STYLE = L"Man";         };
            if(BAND_SELECTED == 1){ BAND_STYLE = L"Woman";       };
            if(BAND_SELECTED == 2){ BAND_STYLE = L"Pop";         };
            if(BAND_SELECTED == 3){ BAND_STYLE = L"Traditional"; };
            if(BAND_SELECTED == 4){ BAND_STYLE = L"Rock";        };
            if(BAND_SELECTED == 5){ BAND_STYLE = L"Dj";          };
            if(BAND_SELECTED == 6){ BAND_STYLE = L"Drummers";    };
            if(BAND_SELECTED == 7){ BAND_STYLE = L"Santa";       };

            stringw btxt;
            btxt += L"Band: "; btxt += BAND_STYLE.c_str(); btxt += L" (B)";
            texts[14]->setText(btxt.c_str()); // border? wordwrap?

            // int i = musicID;
            // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, video::SColor(255, time%255, time%255, 255));

            // 1 Single, 2 Single Loop, 3 List loop, 4 List Random
            // playMode+=1; if(playMode>4){ playMode = 1; }




            // If music exists, animate the SINGERS:
            if(doAnimations || (music && music->isPlaying())){
            /// Animate singers:
            if(nodeBand){ animation_BP(nodeBand); };
            /// Animate staff:
            animation_SEC(nodeStaff1);
            animation_SEC(nodeStaff2);
            } else {
            if(nodeBand){ animation_BS(nodeBand); };
            };





            // if(INTERNALS_SOUNDS){ soundClaps->play(); }  // Audiere
            // if(doAnimations){
            for(int x=0; x<48; x++){

            /// Anim - Spectators clapping:
            if(soundClaps->isPlaying()){
            animation5(nodeSpectators[x]);
            };

            /// Anim - Spectators dancing:
            if(music){
            if(!soundClaps->isPlaying() && music->isPlaying() && doAnimations && !soundOoo->isPlaying()){
            animation4(nodeSpectators[x]);
            // animation_BALLOONS_UP(nodeBalloons[x]);
            };
            };

            /// Anim - Spectators standby:
            if(!soundClaps->isPlaying() && !doAnimations && !soundOoo->isPlaying()){
            animation6(nodeSpectators[x]);
            // animation_BALLOONS_RESET(nodeBalloons[x]);
            };

            /// Update Spectators to always face BAND:
            if(nodeBand){
            vector3df nodePos = nodeSpectators[x]->getPosition();
            vector3df targetPos = nodeBand->getPosition();
            vector3df diff = -(targetPos - nodePos); // Normal calculation is: vector3df diff = targetPos - nodePos;
            nodeSpectators[x]->setRotation(diff.getHorizontalAngle());
            nodeSpectators[x]->updateAbsolutePosition();
            };
            };





            /// Anim - Free a BALLOON:
            if(doAnimations){
            if(doingOneBalloon == false){ ballon_id = rand()%48; };
            if(nodeBalloons[ballon_id]->getPosition().Y <= 650){
            doingOneBalloon = true;
            animation_BALLOONS_UP(nodeBalloons[ballon_id]);
            };
            if(nodeBalloons[ballon_id]->getPosition().Y > 650){
            doingOneBalloon = false;
            for(int x=0; x<48; x++){ animation_BALLOONS_RESET(nodeBalloons[x]); };
            };
            } else {
            doingOneBalloon = false;
            for(int x=0; x<48; x++){ animation_BALLOONS_RESET(nodeBalloons[x]); };
            };



            /// COURTAINS do...
            open_or_close_COURTAINS();




            /// #1) Calculations for the next EMITTERS quantities:
            static int cqt_90  = 0;
            static int cqt_100 = 0;
            static int cqt_150 = 0;
            static int cqt_200 = 0;
            if(doAnimations){
            cqt_90 +=0.150*frameDeltaTime; if(cqt_90 >=90 ){ cqt_90  = 90;  }; /// <-- Force a smooth gradually start
            cqt_100+=0.150*frameDeltaTime; if(cqt_100>=100){ cqt_100 = 100; }; /// <-- Force a smooth gradually start
            cqt_150+=0.150*frameDeltaTime; if(cqt_150>=150){ cqt_150 = 150; }; /// <-- Force a smooth gradually start
            cqt_200+=0.150*frameDeltaTime; if(cqt_200>=200){ cqt_200 = 200; }; /// <-- Force a smooth gradually start
            } else {
            cqt_90  = 0;
            cqt_100 = 0;
            cqt_150 = 0;
            cqt_200 = 0;
            };
            /// #2) Do constant EMITTERS:
            if(doAnimations){
            em2->setMinParticlesPerSecond(cqt_100);  em2->setMaxParticlesPerSecond(cqt_200);  /// falling stars
            em3->setMinParticlesPerSecond(cqt_90 );  em3->setMaxParticlesPerSecond(cqt_150);  /// right fire
            em4->setMinParticlesPerSecond(cqt_90 );  em4->setMaxParticlesPerSecond(cqt_150);  /// left fire
            em7->setMinParticlesPerSecond(cqt_100);  em7->setMaxParticlesPerSecond(cqt_200);  /// falling flames right
            em8->setMinParticlesPerSecond(cqt_100);  em8->setMaxParticlesPerSecond(cqt_200);  /// falling flames left
            em60->setMinParticlesPerSecond(cqt_90); em60->setMaxParticlesPerSecond(cqt_100); /// disco ball n1 - lights reflexes
            em61->setMinParticlesPerSecond(cqt_90); em61->setMaxParticlesPerSecond(cqt_100); /// disco ball n2 - lights reflexes
            em62->setMinParticlesPerSecond(cqt_90); em62->setMaxParticlesPerSecond(cqt_100); /// disco ball n3 - lights reflexes
            em63->setMinParticlesPerSecond(cqt_90); em63->setMaxParticlesPerSecond(cqt_100); /// disco ball n4 - lights reflexes
            } else {
            em2->setMinParticlesPerSecond(0);   em2->setMaxParticlesPerSecond(0);  /// falling stars
            em3->setMinParticlesPerSecond(0);   em3->setMaxParticlesPerSecond(0);  /// right fire
            em4->setMinParticlesPerSecond(0);   em4->setMaxParticlesPerSecond(0);  /// left fire
            em7->setMinParticlesPerSecond(0);   em7->setMaxParticlesPerSecond(0);  /// falling flames right
            em8->setMinParticlesPerSecond(0);   em8->setMaxParticlesPerSecond(0);  /// falling flames left
            em60->setMinParticlesPerSecond(0);	em60->setMaxParticlesPerSecond(0); /// disco ball n1 - lights reflexes
            em61->setMinParticlesPerSecond(0);	em61->setMaxParticlesPerSecond(0); /// disco ball n2 - lights reflexes
            em62->setMinParticlesPerSecond(0);	em62->setMaxParticlesPerSecond(0); /// disco ball n3 - lights reflexes
            em63->setMinParticlesPerSecond(0);	em63->setMaxParticlesPerSecond(0); /// disco ball n4 - lights reflexes
            };




            /// #1) Calculations for the next EMITTERS quantities:
            static int fqt_100   = 0;
            static int fqt_150   = 0;
            static int emitThis  = 0;
            static int emit_wait = 0;
            static bool finalflames = true;

            /// #2) Do random EMITTERS:
            if(doAnimations){
            fqt_100+=0.150*frameDeltaTime; if(fqt_100>=100){ fqt_100 = 100; }; /// <-- Force a smooth gradually start
            fqt_150+=0.150*frameDeltaTime; if(fqt_150>=150){ fqt_150 = 150; }; /// <-- Force a smooth gradually start
            emit_wait += (0.1*frameDeltaTime);
            /// Emitter duration finished:
            if(emit_wait>200 && emit_wait<=900){
            em30->setMinParticlesPerSecond(0); em30->setMaxParticlesPerSecond(0);
            em31->setMinParticlesPerSecond(0); em31->setMaxParticlesPerSecond(0);
            em32->setMinParticlesPerSecond(0); em32->setMaxParticlesPerSecond(0);
            em33->setMinParticlesPerSecond(0); em33->setMaxParticlesPerSecond(0);
            emitThis = 0; /// Reset
            };
            if(emit_wait>900){
            emitThis = (rand()%8)+1; // from (1 to 7)
            if(emitThis==1){ emit_wait=0; fqt_100=0; fqt_150=0; };
            if(emitThis==2){ emit_wait=0; fqt_100=0; fqt_150=0; };
            if(emitThis==3){ emit_wait=0; fqt_100=0; fqt_150=0; };
            if(emitThis==4){ emit_wait=0; fqt_100=0; fqt_150=0; };
            if(emitThis==5){ emit_wait=0; fqt_100=0; fqt_150=0; };
            if(emitThis==6){ emit_wait=0; fqt_100=0; fqt_150=0; };
            if(emitThis==7){ emit_wait=0; fqt_100=0; fqt_150=0; };
            };
             /// Do small stage flames, at 4 seconds BEFORE the song ENDS:
            if(songCurrentTime>=(songFullTime-4) && finalflames == true){ emitThis=7; emit_wait=0; fqt_100=0; fqt_150=0; finalflames=false; };
            };

            if(emitThis == 1){
            em30->setMinParticlesPerSecond(fqt_100); em30->setMaxParticlesPerSecond(fqt_150); /// small stage flames N1 going up
            };
            if(emitThis == 2){
            em31->setMinParticlesPerSecond(fqt_100); em31->setMaxParticlesPerSecond(fqt_150); /// small stage flames N2 going up
            };
            if(emitThis == 3){
            em32->setMinParticlesPerSecond(fqt_100); em32->setMaxParticlesPerSecond(fqt_150); /// small stage flames N3 going up
            };
            if(emitThis == 4){
            em33->setMinParticlesPerSecond(fqt_100); em33->setMaxParticlesPerSecond(fqt_150); /// small stage flames N4 going up
            };
            if(emitThis == 5){
            em30->setMinParticlesPerSecond(fqt_100); em30->setMaxParticlesPerSecond(fqt_150); /// small stage flames N1 going up
            em33->setMinParticlesPerSecond(fqt_100); em33->setMaxParticlesPerSecond(fqt_150);
            };
            if(emitThis == 6){
            em31->setMinParticlesPerSecond(fqt_100); em31->setMaxParticlesPerSecond(fqt_150); /// small stage flames N2 going up
            em32->setMinParticlesPerSecond(fqt_100); em32->setMaxParticlesPerSecond(fqt_150); /// small stage flames N3 going up
            };
            if(emitThis == 7){
            em30->setMinParticlesPerSecond(fqt_100); em30->setMaxParticlesPerSecond(fqt_150); /// small stage flames N1 going up
            em31->setMinParticlesPerSecond(fqt_100); em31->setMaxParticlesPerSecond(fqt_150); /// small stage flames N2 going up
            em32->setMinParticlesPerSecond(fqt_100); em32->setMaxParticlesPerSecond(fqt_150); /// small stage flames N3 going up
            em33->setMinParticlesPerSecond(fqt_100); em33->setMaxParticlesPerSecond(fqt_150); /// small stage flames N4 going up
            };

            if(!doAnimations){ // doAnimations = false
            em30->setMinParticlesPerSecond(0); em30->setMaxParticlesPerSecond(0); /// small stage flames N1 going up
            em31->setMinParticlesPerSecond(0); em31->setMaxParticlesPerSecond(0); /// small stage flames N2 going up
            em32->setMinParticlesPerSecond(0); em32->setMaxParticlesPerSecond(0); /// small stage flames N3 going up
            em33->setMinParticlesPerSecond(0); em33->setMaxParticlesPerSecond(0); /// small stage flames N4 going up
            emitThis  = 0; /// Reset
            emit_wait = 0; /// Reset
            finalflames = true; /// Reset
            };








            /// #1) Calculations for the next EMITTERS quantities:
            static int smqt_150 = 0;
            static int smqt_250 = 0;
            /// If music exists, and song > 40 seconds, and <= 40 seconds BEFORE the song ENDS:
            if(doAnimations && songFullTime>=000040 && (songCurrentTime>=(songFullTime-40))){
            smqt_150+=0.150*frameDeltaTime; if(smqt_150>=150){ smqt_150 = 150; }; /// <-- Force a smooth gradually start
            smqt_250+=0.150*frameDeltaTime; if(smqt_250>=250){ smqt_250 = 250; }; /// <-- Force a smooth gradually start
            } else {
            smqt_150-=0.001*frameDeltaTime; if(smqt_150<=0){ smqt_150 = 0; };     /// <-- Force a smooth gradually ending
            smqt_250-=0.001*frameDeltaTime; if(smqt_250<=0){ smqt_250 = 0; };     /// <-- Force a smooth gradually ending
            };
            /// #2) Do Smoke Machines EMITTERS:
            em50->setMinParticlesPerSecond(smqt_150); em50->setMaxParticlesPerSecond(smqt_250); /// smoke machine N1 - left
            em51->setMinParticlesPerSecond(smqt_150); em51->setMaxParticlesPerSecond(smqt_250); /// smoke machine N2 - right



            /// Do fireworks, at 2 seconds BEFORE the song ENDS:
            if(doAnimations && songCurrentTime>=(songFullTime-2)){ // At 1 seconds BEFORE the song ENDS, do applause.
            em5->setMinParticlesPerSecond(5000); em5->setMaxParticlesPerSecond(5000); // centered fireworks
            } else {
            em5->setMinParticlesPerSecond(0); em5->setMaxParticlesPerSecond(0);    // centered fireworks
            };











            /// If music exists, and music >5 sec to end, do STROBE LIGHT: (Main Flash White Light)
            static irr::f32 strobeRepeat = 0.0;
            if(doAnimations && (SHOW_TYPE == 1 || SHOW_TYPE == 2) && songCurrentTime<(songFullTime-6)){
            static irr::f32 wait = 0.0;
            int k = 0;
            if(SHOW_TYPE == 1){ k = rand()%1500; }; // from (0 to 1999) - More frequency
            if(SHOW_TYPE == 2){ k = rand()%3000; }; // from (0 to 3999) - Less frequency
            if(k==1000 && strobeRepeat==0.0){ strobeRepeat = 200.0; };
            light_ML1->setVisible(true); /// Turn ON main colored light
            light_FL1->setVisible(false);
            if(strobeRepeat>0.0) {
            if(wait< 5.0){ light_ML1->setVisible(false); light_FL1->setVisible(false); };
            if(wait>=5.0){ light_ML1->setVisible(false); light_FL1->setVisible(true);  };
            wait += (0.15*frameDeltaTime); if(wait>=10.0){ wait = 0.0; };
            strobeRepeat -= (0.20*frameDeltaTime); if(strobeRepeat<=0.0){ strobeRepeat = 0.0; };
            };
            } else {
            strobeRepeat = 0.0;
            light_ML1->setVisible(true);
            light_FL1->setVisible(false);
            };


            /// If NOT music exists, hide these lights:
            if(!doAnimations){
            light_ML1->setVisible(false);
            light_FL1->setVisible(false);
            };




            /// 1) Do spot light rotations: (left - right - left ... Y axis)
            /*
            static float rotX = 160.0;
            static float rotY = 0.0;
            static bool rotYIncrease = false;
            if(rotYIncrease==false){ rotY-=(0.015*frameDeltaTime); };
            if(rotYIncrease==true ){ rotY+=(0.015*frameDeltaTime); };
            if(rotY<= -10){ rotY= -10; rotYIncrease = true;  };
            if(rotY>=  10){ rotY=  10; rotYIncrease = false; };
            // rotX+=(0.150*frameDeltaTime);
            // std::cout << "rotY: " << rotY << std::endl;
            /// 2) Do spot light rotations: (up - down ... X axis)
            if(doAnimations){
            rotX+=(0.150*frameDeltaTime);
            } else {
            rotX = 160.0;
            };
            /// 3) Apply to the light both rotations:
            light_SP1->setRotation(vector3df(rotX,rotY,0.0)); // X90 = Middle Stage & Y90 = left, Y-90 = right
            */




            /// Show lights?...
            if(doAnimations){
            nodeFocusFloor1->setVisible(true);
            nodeFocusFloor2->setVisible(true);
            nodeFocusFloor3->setVisible(true);
            nodeFocusSmokeLight1->setVisible(true);
            nodeFocusSmokeLight2->setVisible(true);
            nodeFocusSmokeLight3->setVisible(true);
            light_SP1->setVisible(false);
            smgr->setAmbientLight(video::SColorf(0.50,0.50,0.50,0.0)); // Ambient Light. SColorf == r,g,b,a
            } else {
            nodeFocusFloor1->setVisible(false);
            nodeFocusFloor2->setVisible(false);
            nodeFocusFloor3->setVisible(false);
            nodeFocusSmokeLight1->setVisible(false);
            nodeFocusSmokeLight2->setVisible(false);
            nodeFocusSmokeLight3->setVisible(false);
            light_SP1->setVisible(true);
            smgr->setAmbientLight(video::SColorf(0.50,0.50,0.50,0.0)); // Ambient Light. SColorf == r,g,b,a
            };


            /// If music exists, and music >5 sec to end, do LASER LIGHTS:
            if(doAnimations && songCurrentTime<(songFullTime-6)){
             /// Show/Hide left LASER LIGHTS:
            if(SHOW_TYPE == 0){ for(int i=0; i<8; i++){ if(nodeLaserA[i]){ nodeLaserA[i]->setVisible(false); }; }; };
            if(SHOW_TYPE == 1){ for(int i=0; i<8; i++){ if(nodeLaserA[i]){ nodeLaserA[i]->setVisible(true);  }; }; };
            if(SHOW_TYPE == 2){ for(int i=0; i<8; i++){ if(nodeLaserA[i]){ nodeLaserA[i]->setVisible(true);  }; }; };
            /// Show/Hide right LASER LIGHTS:
            if(SHOW_TYPE == 0){ for(int i=0; i<8; i++){ if(nodeLaserB[i]){ nodeLaserB[i]->setVisible(false); }; }; };
            if(SHOW_TYPE == 1){ for(int i=0; i<8; i++){ if(nodeLaserB[i]){ nodeLaserB[i]->setVisible(true);  }; }; };
            if(SHOW_TYPE == 2){ for(int i=0; i<8; i++){ if(nodeLaserB[i]){ nodeLaserB[i]->setVisible(true);  }; }; };
            } else {
            /// Hide all LASER LIGHTS:
            for(int i=0; i<8; i++){ if(nodeLaserA[i]){ nodeLaserA[i]->setVisible(false); }; };
            for(int i=0; i<8; i++){ if(nodeLaserB[i]){ nodeLaserB[i]->setVisible(false); }; };
            };



            /// If music exists, do COLUMNS FAKE LIGHTS:
            if(doAnimations){
            irr::f32 wait = 40;
            if(SHOW_TYPE==0){ wait = 200; };
            if(SHOW_TYPE==1){ wait = 60;  };
            if(SHOW_TYPE==2){ wait = 40;  };
			if(columns_light_speed > wait){
            /// Flashing lights:
			// Reset all lights to hidden:
			for(int i=0; i<48; i++){ light[i]->setVisible(false); };  // Hides all fake lights

            // Choose random column to visible:
            int k = rand()%20; // from (0 to 19)

            if(k == 0){ /// 1st column lights on:
			for ( int i=0; i<6; i++   ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k == 1){ /// 2nd column lights on:
			for ( int i=6; i<12; i++  ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k == 2){ /// 3rd column lights on:
            for ( int i=12; i<18; i++ ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k == 3){ /// 4th column lights on:
            for ( int i=18; i<24; i++ ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k == 4){ /// 5th column lights on:
            for ( int i=24; i<30; i++ ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k == 5){ /// 6th column lights on:
            for ( int i=30; i<36; i++ ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k == 6){ /// 7th column lights on:
            for ( int i=36; i<42; i++ ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k == 7){ /// 8th column lights on:
            for ( int i=42; i<48; i++ ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k == 8){ /// all left columns lights on:
            for ( int i=0; i<24; i++  ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k == 9){ /// all right columns lights on:
            for ( int i=24; i<48; i++ ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k == 10){ /// all left and right columns lights on:
            for ( int i=0; i<48; i++  ){ stagelights = i+1; light[i]->setVisible(true); }; };
            if(k >= 11){ /// only eight random lights at a time, are on:
            int l1 = rand()%48; // from (0 to 47)
            int l2 = rand()%48; // from (0 to 47)
            int l3 = rand()%48; // from (0 to 47)
            int l4 = rand()%48; // from (0 to 47)
            int l5 = rand()%48; // from (0 to 47)
            int l6 = rand()%48; // from (0 to 47)
            int l7 = rand()%48; // from (0 to 47)
            int l8 = rand()%48; // from (0 to 47)
            light[l1]->setVisible(true);
            light[l2]->setVisible(true);
            light[l3]->setVisible(true);
            light[l4]->setVisible(true);
            light[l5]->setVisible(true);
            light[l6]->setVisible(true);
            light[l7]->setVisible(true);
            light[l8]->setVisible(true);
            stagelights = 8;
            };
			columns_light_speed = 0;
			};
			/// The speed of the lights:
            columns_light_speed += (SPEED_LIGHTS*frameDeltaTime);
            } else {
            /// All lights are off:
            for(int i=0; i<48; i++){ light[i]->setVisible(false); }; // Hides all the fake lights
            stagelights = 0;
            };






            /// If music exists, do MAIN COLORED LIGHT:
            if(doAnimations){
            irr::f32 wait = 50.0f;
            if(SHOW_TYPE == 0){ wait = 100.0f; };
            if(SHOW_TYPE == 1){ wait = 60.0f;  };
            if(SHOW_TYPE == 2){ wait = 50.0f;  };

            /// Main Colored light:
            if(delay > wait){
            delay = 0.0f;
            /// Animated light: ( Change to a random color... )
            irr::f32 red   = ((1.0-0.0)*((float)rand()/RAND_MAX))+0.0; // from (0.0 to 1.0)
            irr::f32 green = ((1.0-0.0)*((float)rand()/RAND_MAX))+0.0; // from (0.0 to 1.0)
            irr::f32 blue  = ((1.0-0.0)*((float)rand()/RAND_MAX))+0.0; // from (0.0 to 1.0)
            data_ML1.AmbientColor  = video::SColorf(red,green,blue,0.0);
            data_ML1.SpecularColor = video::SColorf(red,green,blue,0.0);
            data_ML1.DiffuseColor  = video::SColorf(red,green,blue,0.0);
            light_ML1->setLightData(data_ML1); // Apply data to the light
            /// Transform SColorf to SColor and apply it to the shadows: (Great, but... NOT great visually)
            // video::SColor scolor = data_ML1.AmbientColor.toSColor();
            // smgr->setShadowColor(video::SColor(30,scolor.getRed(),scolor.getGreen(),scolor.getBlue())); // a (0 = transparent | 255 = opaque), r, g, b
            /// Animated light: ( ... OR... Change to a pre-defined color... )
            static bool do_my_color = false;
            irr::f32 pred   = 0.0; // from (0.0 to 1.0)
            irr::f32 pgreen = 0.0; // from (0.0 to 1.0)
            irr::f32 pblue  = 0.0; // from (0.0 to 1.0)
            int fixed_color = rand()%50; // from (0 to 49)
            if(fixed_color== 5){ pred = 1.0; pgreen = 0.0; pblue = 0.0; do_my_color = true; }; // red
            if(fixed_color==10){ pred = 0.0; pgreen = 1.0; pblue = 0.0; do_my_color = true; }; // green
            if(fixed_color==15){ pred = 0.0; pgreen = 0.0; pblue = 1.0; do_my_color = true; }; // blue
            if(fixed_color==20){ pred = 1.0; pgreen = 1.0; pblue = 0.0; do_my_color = true; }; // yellow
            if(fixed_color==25){ pred = 1.0; pgreen = 0.0; pblue = 1.0; do_my_color = true; }; // pink
            if(fixed_color==30){ pred = 1.0; pgreen = 0.5; pblue = 0.0; do_my_color = true; }; // orange
            if(fixed_color==35){ pred = 0.0; pgreen = 1.0; pblue = 1.0; do_my_color = true; }; // cyan
            if(fixed_color==40){ pred = 1.0; pgreen = 1.0; pblue = 1.0; do_my_color = true; }; // white
            if(do_my_color == true){
            do_my_color = false;
            data_ML1.AmbientColor  = video::SColorf(pred,pgreen,pblue,0.0);
            data_ML1.SpecularColor = video::SColorf(pred,pgreen,pblue,0.0);
            data_ML1.DiffuseColor  = video::SColorf(pred,pgreen,pblue,0.0);
            light_ML1->setLightData(data_ML1); // Apply data to the light
            };
            };
            delay += (0.1*frameDeltaTime);
            } else { // doAnimations = false (set defaults)
            delay = 0.0f;
            };





            /// Set ambient lights colors, 5 seconds BEFORE the song ENDS:
            if(songCurrentTime>=(songFullTime-5)){
            /// All fake lights are on, between 5 and 2 seconds BEFORE the song ENDS:
            for(int i=0; i<48; i++){ light[i]->setVisible(true); }; stagelights = 48;
            /// Animated light: ( Change color to white )
            data_ML1.AmbientColor  = video::SColorf(1.0,1.0,1.0,0.0);
            data_ML1.SpecularColor = video::SColorf(1.0,1.0,1.0,0.0);
            data_ML1.DiffuseColor  = video::SColorf(1.0,1.0,1.0,0.0);
            light_ML1->setLightData(data_ML1); // Apply data to the light
            /// Ambient light:
            smgr->setAmbientLight(video::SColorf(1.0,1.0,1.0,0.0)); // Ambient Light. SColorf == r,g,b,a
            };







            /*
            /// Add Colors to the bill lights:
            for(int i=0; i<48; i++){ // Loop trought lights (6 Lights X 8 columns = 48 Lights)
            light[i]->setVisible(true);
            int mc = light[i]->getMaterialCount();
            for(int x=0; i<mc; x++){
            light[i]->getMaterial(x).Shininess = 0.0f; // Set size of specular highlights
            light[i]->getMaterial(x).ColorMaterial = 0;
            light[i]->getMaterial(x).ColorMask = 0;
            light[i]->getMaterial(x).AmbientColor  = video::SColor(0,0,0,0); // video::SColor(0,0.0,0.0,0.0)
            light[i]->getMaterial(x).SpecularColor = video::SColor(0,0,0,0); // video::SColor(0,0.0,0.0,0.0)
            light[i]->getMaterial(x).DiffuseColor  = video::SColor(0,0,0,0);
            light[i]->getMaterial(x).EmissiveColor = video::SColor(150,255,255,150);
            };
            };
            */








            /// If music exists, do PHOTO FLASHES:
            flashes_light_speed += 1.5*frameDeltaTime;

            flashwait += 1.5*frameDeltaTime;
            if(flashwait>=40){
            for(int i=0; i<48; i++){ flash[i]->setVisible(false); }; // Hides all photo flashes
            flashwait = 0;
            };

            static irr::f32 flwait = 0;
            if(doAnimations){
            if(SHOW_TYPE == 0){ flwait = 3000.0; };
            if(SHOW_TYPE == 1){ flwait = 1000.0; };
            if(SHOW_TYPE == 2){ flwait =  500.0; };
            if(flashes_light_speed > flwait){
            flashes_light_speed = 0;
            randomflash = rand()%48; // from (0 to 47)
            flash[randomflash]->setVisible(true); // flash visible
            };
            };

            /// This consumes LOTS of CPU, so its disabled...
            /// if(flash[randomflash]->isVisible()){ animation8(nodeSpectators[randomflash]); };








            /// Animate the logotipo...
            /*
            static int logoChange = 0;
            logoChange += 0.1*frameDeltaTime;
            if(logoChange>=100 ){ image[0] };
            if(logoChange>=200 ){ image[1] };
            if(logoChange>=300 ){ image[2] };
            if(logoChange>=1000){ logoChange = 0; };
            std::cout << "logoChange: " << logoChange << std::endl;
            */



            /// OnResize window, do...
            int sw = driver->getScreenSize().Width;
            int sh = driver->getScreenSize().Height;
            driver->OnResize(core::dimension2d<irr::u32>(sw,sh));

            /// Update globals:
            SCREEN_WIDTH  = sw;
            SCREEN_HEIGHT = sh;

            /// OnResize main widow, update camera ratio:
            camera[CAMERA_SELECTED]->setAspectRatio((float)sw/(float)sh);

            /// OnResize main widow, update listbox position: (Songs list)
            musicListBox->setRelativePosition(core::rect<s32>(10,300,165,sh-20));
            /// Keep constant focus REMOVE on Listbox: (So its own ListBox keys events dont mess with other IMP keys!)
            guienv->removeFocus(musicListBox); // Opposite is: guienv->setFocus(musicListBox);

            /// OnResize main widow, update 2D Graphics positions: (Logotipo)
            image[0]->setRelativePosition(core::position2d<int>(sw-145,10));
            // image[1]->setRelativePosition(core::position2d<int>(sw-145,10));
            // image[2]->setRelativePosition(core::position2d<int>(sw-145,10));

            /// OnResize main widow, update Display Infos: Music Tags, Help, About, Nothing.
            rect<int> post6 = texts[6]->getRelativePosition();
            texts[6]->setRelativePosition(core::rect<s32>(post6.UpperLeftCorner.X, post6.UpperLeftCorner.Y, sw, sh));

            /// OnResize main widow, update text infos to new height:
            // rect<int> post14 = texts[14]->getRelativePosition();
            // texts[14]->setRelativePosition(core::rect<s32>(post14.UpperLeftCorner.X, post14.UpperLeftCorner.Y, sw, sh));


            /// OnResize main widow, update text position: (Lyrics texts)
            int fts = texts[7]->getTextHeight(); /// Get current lyrics font size (height)
            texts[7]->setRelativePosition(core::rect<s32>(50,(sh-fts)-15,sw-50,sh-10));

            /// OnResize main widow, update text position: (PAUSED texts: Show intermission)
            texts[12]->setRelativePosition(core::rect<s32>(0,0,sw,sh));

            /// OnResize main widow, update text position: (Loading texts: Please standby, Show will continue in moments)
            texts[20]->setRelativePosition(core::rect<s32>(0,0,sw,sh));

            /// OnResize main widow, update text position: (Loading texts: Loading music, Loading lyrics)
            texts[25]->setRelativePosition(core::rect<s32>(0,sh-130,sw,sh));


            /// OnResize main widow, keep win2 always in the visible screen area:
            core::rect<s32> posW2 = win2->getRelativePosition();
            // if(posW2.UpperLeftCorner.X  < 185){ posW2.UpperLeftCorner.X = 185; }; <-- Only if we want it to collide with the win1
            if(posW2.UpperLeftCorner.X  < 5){ posW2.UpperLeftCorner.X = 5; };
            if(posW2.UpperLeftCorner.Y  < 5){ posW2.UpperLeftCorner.Y = 5; };
            if(posW2.UpperLeftCorner.X  > (int)SCREEN_WIDTH-400-5 ){ posW2.UpperLeftCorner.X = SCREEN_WIDTH-400-5; };
            if(posW2.UpperLeftCorner.Y  > (int)SCREEN_HEIGHT-65-5 ){ posW2.UpperLeftCorner.Y = SCREEN_HEIGHT-65-5; };
            win2->setRelativePosition(core::rect<s32>(posW2.UpperLeftCorner.X, posW2.UpperLeftCorner.Y, posW2.UpperLeftCorner.X+400, posW2.UpperLeftCorner.Y+65));
            win2->getParent()->bringToFront(win2);

            /// OnResize main widow, update internal window:
            win1->setRelativePosition(core::rect<s32>(5,5,180,sh-5));
            win1->getParent()->bringToFront(win1); /// <-- This way its ListBox child, is clickable (else may fail clicking on the last item in the list)


            /// OnResize main widow, update text position: (Music timmings texts)
            /// int mts = texts[4]->getTextWidth(); /// Get current lyrics font size (height) - Ok
            /// rect<int> post4 = texts[4]->getRelativePosition(); // First we get the "father" text... - Ok
            /// texts[5]->setRelativePosition(core::rect<s32>(post4.UpperLeftCorner.X + mts + 152, post4.UpperLeftCorner.Y,sw-50,sh-10)); // New position (00:00:00)  - Ok
            /// musicPositionScrollbar->setRelativePosition(position2di(post4.UpperLeftCorner.X + mts + 0, post4.UpperLeftCorner.Y + 2));         // New position (Scrollbar) - Ok
            // texts[5]->setRelativePosition(core::rect<s32>(660,15,sw-50,sh-10)); // New position (00:00:00)
            // musicPositionScrollbar->setRelativePosition(position2di(240, 17));  // New position (Scrollbar)



















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


