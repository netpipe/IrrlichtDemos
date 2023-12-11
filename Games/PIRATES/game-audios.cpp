
/// Audio (Audiere)
#include <audiere.h> // For Audio. ALSO must add "libaudiere.a" in the "Project"->"Build Options"->"Linker" !!!
using namespace audiere;


/// Define Audiere Global Variables:
audiere::AudioDevicePtr audiereDevice = NULL;  // Must be globals! (else audiere will remove it).
// audiere::AudioDevicePtr audiereDevice(OpenDevice());  // Must be globals! (else audiere will remove it).
// audiere::MIDIDevice *audiereMIDIDevice = NULL;

audiere::OutputStreamPtr music_01 = NULL;      // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr music_02 = NULL;      // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr music_03 = NULL;      // Must be globals! (else audiere will remove it).

audiere::OutputStreamPtr fx_01    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_02    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_03    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_04    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_05    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_06    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_07    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_08    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_09    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_10    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_11    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_12    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_13    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_14    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_15    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_16    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_17    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_18    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_19    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_20    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_21    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_22    = NULL;       // Must be globals! (else audiere will remove it).
/// audiere::OutputStreamPtr fx_23    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_24    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_25    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_26    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_27    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_28    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_29    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_30    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_31    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_32    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_33    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_34    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_35    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_36    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_37    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_38    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_39    = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr fx_40    = NULL;       // Must be globals! (else audiere will remove it).


audiere::OutputStreamPtr talk_01  = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr talk_02  = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr talk_03  = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr talk_04  = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr talk_05  = NULL;       // Must be globals! (else audiere will remove it).
audiere::OutputStreamPtr talk_06  = NULL;       // Must be globals! (else audiere will remove it).


audiere::OutputStreamPtr button_01 = NULL;      // Must be globals! (else audiere will remove it).



bool AUDIOS_INITIALIZED = false; /// Global, else will mess
float MUSIC1_VOLUME = 0.100; // Maximum is 1.0
float MUSIC2_VOLUME = 0.025; // Maximum is 1.0
float MUSIC3_VOLUME = 0.015; // Maximum is 1.0
float BUTTON_VOLUME = 1.000; // Maximum is 1.0
float FX_VOLUME     = 0.600, MIN_FX_VOLUME = 0.030, MAX_FX_VOLUME = 0.600, PAN = 0.0; // -1 is Left, 0 is Centered, 1 is Right




class Audios : public Game {

    public:

    Audios();  // Constructor
    ~Audios(); // Destructor

    void Initialize();

    void PlayFx(int in, vector3df pos);
    void StopFx(int in);

    void PlayTalk(int in, vector3df pos);

    void PlayMusic(int in);
    void StopMusic(int in);

    void PlayButton(int in);
    void Make3DAudio(vector3df pos);
    void Loop();
    void Exit();

    private:

};

Audios::Audios(){};  // Constructor (Can accept arguments, but this one does not)
Audios::~Audios(){}; // Destructor (do not accept arguments)


void Audios::Initialize(){


    if(!AUDIOS_INITIALIZED){
    AUDIOS_INITIALIZED = true;

    /// -------------------------------------------------------------------------------------------------------

    std::cout << "1st) Load audio files from system to memory..." << std::endl;

    /// -------------------------------------------------------------------------------------------------------

    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileM01 = device->getFileSystem()->createAndOpenFile("music_01.mp3");
    FilePtr targetFileM01;
    if(readFileM01){
    void* bufferM01 = malloc(readFileM01->getSize());
    readFileM01->read(bufferM01, readFileM01->getSize());
    // Create memory file:
    targetFileM01 = audiere::CreateMemoryFile(bufferM01, readFileM01->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferM01);
    readFileM01->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };

    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileM02 = device->getFileSystem()->createAndOpenFile("music_02.wav");
    FilePtr targetFileM02;
    if(readFileM02){
    void* bufferM02 = malloc(readFileM02->getSize());
    readFileM02->read(bufferM02, readFileM02->getSize());
    // Create memory file:
    targetFileM02 = audiere::CreateMemoryFile(bufferM02, readFileM02->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferM02);
    readFileM02->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };

    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileM03 = device->getFileSystem()->createAndOpenFile("music_03.wav");
    FilePtr targetFileM03;
    if(readFileM03){
    void* bufferM03 = malloc(readFileM03->getSize());
    readFileM03->read(bufferM03, readFileM03->getSize());
    // Create memory file:
    targetFileM03 = audiere::CreateMemoryFile(bufferM03, readFileM03->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferM03);
    readFileM03->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };

    /// -------------------------------------------------------------------------------------------------------

    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS01 = device->getFileSystem()->createAndOpenFile("fx_beep.wav");
    FilePtr targetFileS01;
    if(readFileS01){
    void* bufferS01 = malloc(readFileS01->getSize());
    readFileS01->read(bufferS01, readFileS01->getSize());
    // Create memory file:
    targetFileS01 = audiere::CreateMemoryFile(bufferS01, readFileS01->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS01);
    readFileS01->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };

    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS02 = device->getFileSystem()->createAndOpenFile("fx_hit1.wav");
    FilePtr targetFileS02;
    if(readFileS02){
    void* bufferS02 = malloc(readFileS02->getSize());
    readFileS02->read(bufferS02, readFileS02->getSize());
    // Create memory file:
    targetFileS02 = audiere::CreateMemoryFile(bufferS02, readFileS02->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS02);
    readFileS02->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };

    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS03 = device->getFileSystem()->createAndOpenFile("fx_cannon.wav");
    FilePtr targetFileS03;
    if(readFileS03){
    void* bufferS03 = malloc(readFileS03->getSize());
    readFileS03->read(bufferS03, readFileS03->getSize());
    // Create memory file:
    targetFileS03 = audiere::CreateMemoryFile(bufferS03, readFileS03->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS03);
    readFileS03->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };

    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS04 = device->getFileSystem()->createAndOpenFile("fx_metallic1.wav");
    FilePtr targetFileS04;
    if(readFileS04){
    void* bufferS04 = malloc(readFileS04->getSize());
    readFileS04->read(bufferS04, readFileS04->getSize());
    // Create memory file:
    targetFileS04 = audiere::CreateMemoryFile(bufferS04, readFileS04->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS04);
    readFileS04->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };

    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS05 = device->getFileSystem()->createAndOpenFile("fx_hit2.wav");
    FilePtr targetFileS05;
    if(readFileS05){
    void* bufferS05 = malloc(readFileS05->getSize());
    readFileS05->read(bufferS05, readFileS05->getSize());
    // Create memory file:
    targetFileS05 = audiere::CreateMemoryFile(bufferS05, readFileS05->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS05);
    readFileS05->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };

    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS06 = device->getFileSystem()->createAndOpenFile("fx_zoom.wav");
    FilePtr targetFileS06;
    if(readFileS06){
    void* bufferS06 = malloc(readFileS06->getSize());
    readFileS06->read(bufferS06, readFileS06->getSize());
    // Create memory file:
    targetFileS06 = audiere::CreateMemoryFile(bufferS06, readFileS06->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS06);
    readFileS06->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };

    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS07 = device->getFileSystem()->createAndOpenFile("fx_wood1.wav");
    FilePtr targetFileS07;
    if(readFileS07){
    void* bufferS07 = malloc(readFileS07->getSize());
    readFileS07->read(bufferS07, readFileS07->getSize());
    // Create memory file:
    targetFileS07 = audiere::CreateMemoryFile(bufferS07, readFileS07->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS07);
    readFileS07->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };

    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS08 = device->getFileSystem()->createAndOpenFile("fx_water.wav");
    FilePtr targetFileS08;
    if(readFileS08){
    void* bufferS08 = malloc(readFileS08->getSize());
    readFileS08->read(bufferS08, readFileS08->getSize());
    // Create memory file:
    targetFileS08 = audiere::CreateMemoryFile(bufferS08, readFileS08->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS08);
    readFileS08->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };

    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS09 = device->getFileSystem()->createAndOpenFile("fx_spring.wav");
    FilePtr targetFileS09;
    if(readFileS09){
    void* bufferS09 = malloc(readFileS09->getSize());
    readFileS09->read(bufferS09, readFileS09->getSize());
    // Create memory file:
    targetFileS09 = audiere::CreateMemoryFile(bufferS09, readFileS09->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS09);
    readFileS09->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };

    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS10 = device->getFileSystem()->createAndOpenFile("fx_metallic2.wav");
    FilePtr targetFileS10;
    if(readFileS10){
    void* bufferS10 = malloc(readFileS10->getSize());
    readFileS10->read(bufferS10, readFileS10->getSize());
    // Create memory file:
    targetFileS10 = audiere::CreateMemoryFile(bufferS10, readFileS10->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS10);
    readFileS10->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS11 = device->getFileSystem()->createAndOpenFile("fx_seagull.wav");
    FilePtr targetFileS11;
    if(readFileS11){
    void* bufferS11 = malloc(readFileS11->getSize());
    readFileS11->read(bufferS11, readFileS11->getSize());
    // Create memory file:
    targetFileS11 = audiere::CreateMemoryFile(bufferS11, readFileS11->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS11);
    readFileS11->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS12 = device->getFileSystem()->createAndOpenFile("fx_free.wav");
    FilePtr targetFileS12;
    if(readFileS12){
    void* bufferS12 = malloc(readFileS12->getSize());
    readFileS12->read(bufferS12, readFileS12->getSize());
    // Create memory file:
    targetFileS12 = audiere::CreateMemoryFile(bufferS12, readFileS12->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS12);
    readFileS12->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS13 = device->getFileSystem()->createAndOpenFile("fx_wood2.wav");
    FilePtr targetFileS13;
    if(readFileS13){
    void* bufferS13 = malloc(readFileS13->getSize());
    readFileS13->read(bufferS13, readFileS13->getSize());
    // Create memory file:
    targetFileS13 = audiere::CreateMemoryFile(bufferS13, readFileS13->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS13);
    readFileS13->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS14 = device->getFileSystem()->createAndOpenFile("fx_chicken.wav");
    FilePtr targetFileS14;
    if(readFileS14){
    void* bufferS14 = malloc(readFileS14->getSize());
    readFileS14->read(bufferS14, readFileS14->getSize());
    // Create memory file:
    targetFileS14 = audiere::CreateMemoryFile(bufferS14, readFileS14->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS14);
    readFileS14->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS15 = device->getFileSystem()->createAndOpenFile("fx_ball.wav");
    FilePtr targetFileS15;
    if(readFileS15){
    void* bufferS15 = malloc(readFileS15->getSize());
    readFileS15->read(bufferS15, readFileS15->getSize());
    // Create memory file:
    targetFileS15 = audiere::CreateMemoryFile(bufferS15, readFileS15->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS15);
    readFileS15->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS16 = device->getFileSystem()->createAndOpenFile("fx_bubbles.wav");
    FilePtr targetFileS16;
    if(readFileS16){
    void* bufferS16 = malloc(readFileS16->getSize());
    readFileS16->read(bufferS16, readFileS16->getSize());
    // Create memory file:
    targetFileS16 = audiere::CreateMemoryFile(bufferS16, readFileS16->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS16);
    readFileS16->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS17 = device->getFileSystem()->createAndOpenFile("fx_disappear.wav");
    FilePtr targetFileS17;
    if(readFileS17){
    void* bufferS17 = malloc(readFileS17->getSize());
    readFileS17->read(bufferS17, readFileS17->getSize());
    // Create memory file:
    targetFileS17 = audiere::CreateMemoryFile(bufferS17, readFileS17->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS17);
    readFileS17->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS18 = device->getFileSystem()->createAndOpenFile("fx_bell.wav");
    FilePtr targetFileS18;
    if(readFileS18){
    void* bufferS18 = malloc(readFileS18->getSize());
    readFileS18->read(bufferS18, readFileS18->getSize());
    // Create memory file:
    targetFileS18 = audiere::CreateMemoryFile(bufferS18, readFileS18->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS18);
    readFileS18->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS19 = device->getFileSystem()->createAndOpenFile("fx_winch.wav");
    FilePtr targetFileS19;
    if(readFileS19){
    void* bufferS19 = malloc(readFileS19->getSize());
    readFileS19->read(bufferS19, readFileS19->getSize());
    // Create memory file:
    targetFileS19 = audiere::CreateMemoryFile(bufferS19, readFileS19->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS19);
    readFileS19->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS20 = device->getFileSystem()->createAndOpenFile("fx_ball2.wav");
    FilePtr targetFileS20;
    if(readFileS20){
    void* bufferS20 = malloc(readFileS20->getSize());
    readFileS20->read(bufferS20, readFileS20->getSize());
    // Create memory file:
    targetFileS20 = audiere::CreateMemoryFile(bufferS20, readFileS20->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS20);
    readFileS20->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS21 = device->getFileSystem()->createAndOpenFile("fx_horse.wav");
    FilePtr targetFileS21;
    if(readFileS21){
    void* bufferS21 = malloc(readFileS21->getSize());
    readFileS21->read(bufferS21, readFileS21->getSize());
    // Create memory file:
    targetFileS21 = audiere::CreateMemoryFile(bufferS21, readFileS21->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS21);
    readFileS21->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS22 = device->getFileSystem()->createAndOpenFile("fx_cock.wav");
    FilePtr targetFileS22;
    if(readFileS22){
    void* bufferS22 = malloc(readFileS22->getSize());
    readFileS22->read(bufferS22, readFileS22->getSize());
    // Create memory file:
    targetFileS22 = audiere::CreateMemoryFile(bufferS22, readFileS22->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS22);
    readFileS22->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    /*
    irr::io::IReadFile* readFileS23 = device->getFileSystem()->createAndOpenFile("fx_king.wav");
    FilePtr targetFileS23;
    if(readFileS23){
    void* bufferS23 = malloc(readFileS23->getSize());
    readFileS23->read(bufferS23, readFileS23->getSize());
    // Create memory file:
    targetFileS23 = audiere::CreateMemoryFile(bufferS23, readFileS23->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS23);
    readFileS23->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };
    */


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS24 = device->getFileSystem()->createAndOpenFile("fx_yell.wav");
    FilePtr targetFileS24;
    if(readFileS24){
    void* bufferS24 = malloc(readFileS24->getSize());
    readFileS24->read(bufferS24, readFileS24->getSize());
    // Create memory file:
    targetFileS24 = audiere::CreateMemoryFile(bufferS24, readFileS24->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS24);
    readFileS24->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS25 = device->getFileSystem()->createAndOpenFile("fx_drink.wav");
    FilePtr targetFileS25;
    if(readFileS25){
    void* bufferS25 = malloc(readFileS25->getSize());
    readFileS25->read(bufferS25, readFileS25->getSize());
    // Create memory file:
    targetFileS25 = audiere::CreateMemoryFile(bufferS25, readFileS25->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS25);
    readFileS25->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS26 = device->getFileSystem()->createAndOpenFile("fx_cannon2.wav");
    FilePtr targetFileS26;
    if(readFileS26){
    void* bufferS26 = malloc(readFileS26->getSize());
    readFileS26->read(bufferS26, readFileS26->getSize());
    // Create memory file:
    targetFileS26 = audiere::CreateMemoryFile(bufferS26, readFileS26->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS26);
    readFileS26->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS27 = device->getFileSystem()->createAndOpenFile("fx_bull.wav");
    FilePtr targetFileS27;
    if(readFileS27){
    void* bufferS27 = malloc(readFileS27->getSize());
    readFileS27->read(bufferS27, readFileS27->getSize());
    // Create memory file:
    targetFileS27 = audiere::CreateMemoryFile(bufferS27, readFileS27->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS27);
    readFileS27->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS28 = device->getFileSystem()->createAndOpenFile("fx_hammer.wav");
    FilePtr targetFileS28;
    if(readFileS28){
    void* bufferS28 = malloc(readFileS28->getSize());
    readFileS28->read(bufferS28, readFileS28->getSize());
    // Create memory file:
    targetFileS28 = audiere::CreateMemoryFile(bufferS28, readFileS28->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS28);
    readFileS28->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS29 = device->getFileSystem()->createAndOpenFile("fx_door.wav");
    FilePtr targetFileS29;
    if(readFileS29){
    void* bufferS29 = malloc(readFileS29->getSize());
    readFileS29->read(bufferS29, readFileS29->getSize());
    // Create memory file:
    targetFileS29 = audiere::CreateMemoryFile(bufferS29, readFileS29->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS29);
    readFileS29->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS30 = device->getFileSystem()->createAndOpenFile("fx_suspense.wav");
    FilePtr targetFileS30;
    if(readFileS30){
    void* bufferS30 = malloc(readFileS30->getSize());
    readFileS30->read(bufferS30, readFileS30->getSize());
    // Create memory file:
    targetFileS30 = audiere::CreateMemoryFile(bufferS30, readFileS30->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS30);
    readFileS30->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS31 = device->getFileSystem()->createAndOpenFile("fx_escaping.wav");
    FilePtr targetFileS31;
    if(readFileS31){
    void* bufferS31 = malloc(readFileS31->getSize());
    readFileS31->read(bufferS31, readFileS31->getSize());
    // Create memory file:
    targetFileS31 = audiere::CreateMemoryFile(bufferS31, readFileS31->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS31);
    readFileS31->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS32 = device->getFileSystem()->createAndOpenFile("fx_hit3.wav");
    FilePtr targetFileS32;
    if(readFileS32){
    void* bufferS32 = malloc(readFileS32->getSize());
    readFileS32->read(bufferS32, readFileS32->getSize());
    // Create memory file:
    targetFileS32 = audiere::CreateMemoryFile(bufferS32, readFileS32->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS32);
    readFileS32->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS33 = device->getFileSystem()->createAndOpenFile("fx_jump.wav");
    FilePtr targetFileS33;
    if(readFileS33){
    void* bufferS33 = malloc(readFileS33->getSize());
    readFileS33->read(bufferS33, readFileS33->getSize());
    // Create memory file:
    targetFileS33 = audiere::CreateMemoryFile(bufferS33, readFileS33->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS33);
    readFileS33->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS34 = device->getFileSystem()->createAndOpenFile("fx_laser.wav");
    FilePtr targetFileS34;
    if(readFileS34){
    void* bufferS34 = malloc(readFileS34->getSize());
    readFileS34->read(bufferS34, readFileS34->getSize());
    // Create memory file:
    targetFileS34 = audiere::CreateMemoryFile(bufferS34, readFileS34->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS34);
    readFileS34->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS35 = device->getFileSystem()->createAndOpenFile("fx_zop.wav");
    FilePtr targetFileS35;
    if(readFileS35){
    void* bufferS35 = malloc(readFileS35->getSize());
    readFileS35->read(bufferS35, readFileS35->getSize());
    // Create memory file:
    targetFileS35 = audiere::CreateMemoryFile(bufferS35, readFileS35->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS35);
    readFileS35->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS36 = device->getFileSystem()->createAndOpenFile("fx_fountain.wav");
    FilePtr targetFileS36;
    if(readFileS36){
    void* bufferS36 = malloc(readFileS36->getSize());
    readFileS36->read(bufferS36, readFileS36->getSize());
    // Create memory file:
    targetFileS36 = audiere::CreateMemoryFile(bufferS36, readFileS36->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS36);
    readFileS36->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS37 = device->getFileSystem()->createAndOpenFile("fx_game_intro.wav");
    FilePtr targetFileS37;
    if(readFileS37){
    void* bufferS37 = malloc(readFileS37->getSize());
    readFileS37->read(bufferS37, readFileS37->getSize());
    // Create memory file:
    targetFileS37 = audiere::CreateMemoryFile(bufferS37, readFileS37->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS37);
    readFileS37->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS38 = device->getFileSystem()->createAndOpenFile("fx_game_end.wav");
    FilePtr targetFileS38;
    if(readFileS38){
    void* bufferS38 = malloc(readFileS38->getSize());
    readFileS38->read(bufferS38, readFileS38->getSize());
    // Create memory file:
    targetFileS38 = audiere::CreateMemoryFile(bufferS38, readFileS38->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS38);
    readFileS38->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };



    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS39 = device->getFileSystem()->createAndOpenFile("fx_gamestart.wav");
    FilePtr targetFileS39;
    if(readFileS39){
    void* bufferS39 = malloc(readFileS39->getSize());
    readFileS39->read(bufferS39, readFileS39->getSize());
    // Create memory file:
    targetFileS39 = audiere::CreateMemoryFile(bufferS39, readFileS39->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS39);
    readFileS39->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


        /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileS40 = device->getFileSystem()->createAndOpenFile("fx_gameover.wav");
    FilePtr targetFileS40;
    if(readFileS40){
    void* bufferS40 = malloc(readFileS40->getSize());
    readFileS40->read(bufferS40, readFileS40->getSize());
    // Create memory file:
    targetFileS40 = audiere::CreateMemoryFile(bufferS40, readFileS40->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferS40);
    readFileS40->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// -------------------------------------------------------------------------------------------------------
    /// -------------------------------------------------------------------------------------------------------



    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileT01 = device->getFileSystem()->createAndOpenFile("talk_help.wav");
    FilePtr targetFileT01;
    if(readFileT01){
    void* bufferT01 = malloc(readFileT01->getSize());
    readFileT01->read(bufferT01, readFileT01->getSize());
    // Create memory file:
    targetFileT01 = audiere::CreateMemoryFile(bufferT01, readFileT01->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferT01);
    readFileT01->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileT02 = device->getFileSystem()->createAndOpenFile("talk_yeah.wav");
    FilePtr targetFileT02;
    if(readFileT02){
    void* bufferT02 = malloc(readFileT02->getSize());
    readFileT02->read(bufferT02, readFileT02->getSize());
    // Create memory file:
    targetFileT02 = audiere::CreateMemoryFile(bufferT02, readFileT02->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferT02);
    readFileT02->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileT03 = device->getFileSystem()->createAndOpenFile("talk_king.wav");
    FilePtr targetFileT03;
    if(readFileT03){
    void* bufferT03 = malloc(readFileT03->getSize());
    readFileT03->read(bufferT03, readFileT03->getSize());
    // Create memory file:
    targetFileT03 = audiere::CreateMemoryFile(bufferT03, readFileT03->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferT03);
    readFileT03->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileT04 = device->getFileSystem()->createAndOpenFile("talk_king2.wav");
    FilePtr targetFileT04;
    if(readFileT04){
    void* bufferT04 = malloc(readFileT04->getSize());
    readFileT04->read(bufferT04, readFileT04->getSize());
    // Create memory file:
    targetFileT04 = audiere::CreateMemoryFile(bufferT04, readFileT04->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferT04);
    readFileT04->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileT05 = device->getFileSystem()->createAndOpenFile("talk_drunk.wav");
    FilePtr targetFileT05;
    if(readFileT05){
    void* bufferT05 = malloc(readFileT05->getSize());
    readFileT05->read(bufferT05, readFileT05->getSize());
    // Create memory file:
    targetFileT05 = audiere::CreateMemoryFile(bufferT05, readFileT05->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferT05);
    readFileT05->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileT06 = device->getFileSystem()->createAndOpenFile("talk_clear.wav");
    FilePtr targetFileT06;
    if(readFileT06){
    void* bufferT06 = malloc(readFileT06->getSize());
    readFileT06->read(bufferT06, readFileT06->getSize());
    // Create memory file:
    targetFileT06 = audiere::CreateMemoryFile(bufferT06, readFileT06->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferT06);
    readFileT06->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// -------------------------------------------------------------------------------------------------------
    /// -------------------------------------------------------------------------------------------------------


    /// Load sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFileB01 = device->getFileSystem()->createAndOpenFile("fx_button.wav");
    FilePtr targetFileB01;
    if(readFileB01){
    void* bufferB01 = malloc(readFileB01->getSize());
    readFileB01->read(bufferB01, readFileB01->getSize());
    // Create memory file:
    targetFileB01 = audiere::CreateMemoryFile(bufferB01, readFileB01->getSize()); // targetFile11 is the file (path) that we want !!!
    free(bufferB01);
    readFileB01->drop();
    } else { cout << "ERROR: Audio file not found!" << endl; };


    /// -------------------------------------------------------------------------------------------------------
    /// -------------------------------------------------------------------------------------------------------


    /// Remove:
    if(audiereDevice){ audiereDevice->clearCallbacks(); audiereDevice = NULL; };
    /// Create:
    audiereDevice = OpenDevice();
    if(!audiereDevice){ std::cout << "ERROR: Audiere: Cant open audio device! \n" << std::endl; };
    /// OK! Now loads sounds to AUDIERE:
    std::cout << "2nd) Load audio files from memory to Audiere..." << std::endl;

    /// -------------------------------------------------------------------------------------------------------

    /// Create:
    if(targetFileM01){
    music_01 = audiere::OpenSound(audiereDevice.get(), targetFileM01, true); // Load music into memory? true / false
    music_01->setVolume(MUSIC1_VOLUME); /// MUSIC: Main Theme
    music_01->setPan(0);    // -1 Left, 1 Right
    music_01->setRepeat(1); // 1 loop, 0 don't loop
    // music_01->play();
    };

    /// Create:
    if(targetFileM02){
    music_02 = audiere::OpenSound(audiereDevice.get(), targetFileM02, true); // Load music into memory? true / false
    music_02->setVolume(MUSIC2_VOLUME); /// MUSIC: OCEAN
    music_02->setPan(0);    // -1 Left, 1 Right
    music_02->setRepeat(1); // 1 loop, 0 don't loop
    // music_02->play();
    };

    /// Create:
    if(targetFileM03){
    music_03 = audiere::OpenSound(audiereDevice.get(), targetFileM03, true); // Load music into memory? true / false
    music_03->setVolume(MUSIC3_VOLUME); /// MUSIC: PEOPLE TALKING VERY FAR AWAY
    music_03->setPan(0);    // -1 Left, 1 Right
    music_03->setRepeat(1); // 1 loop, 0 don't loop
    // music_03->play();
    std::cout << "PLAYING music_03!..." << std::endl;
    };

    /// -------------------------------------------------------------------------------------------------------

    /// Create:
    if(targetFileS01){
    fx_01 = audiere::OpenSound(audiereDevice.get(), targetFileS01, true); // Load music into memory? true / false
    fx_01->setVolume(FX_VOLUME);
    fx_01->setPan(PAN);    // -1 Left, 1 Right
    fx_01->setRepeat(0); // 1 loop, 0 don't loop
    // fx_01->play();
    };

    /// Create:
    if(targetFileS02){
    fx_02 = audiere::OpenSound(audiereDevice.get(), targetFileS02, true); // Load music into memory? true / false
    fx_02->setVolume(FX_VOLUME);
    fx_02->setPan(PAN);    // -1 Left, 1 Right
    fx_02->setRepeat(0); // 1 loop, 0 don't loop
    // fx_02->play();
    };

    /// Create:
    if(targetFileS03){
    fx_03 = audiere::OpenSound(audiereDevice.get(), targetFileS03, true); // Load music into memory? true / false
    fx_03->setVolume(FX_VOLUME/1.3);
    fx_03->setPan(PAN); // -1 Left, 1 Right
    fx_03->setRepeat(0); // 1 loop, 0 don't loop
    // fx_03->play();
    };

    /// Create:
    if(targetFileS04){
    fx_04 = audiere::OpenSound(audiereDevice.get(), targetFileS04, true); // Load music into memory? true / false
    fx_04->setVolume(FX_VOLUME);
    fx_04->setPan(PAN);    // -1 Left, 1 Right
    fx_04->setRepeat(0); // 1 loop, 0 don't loop
    // fx_04->play();
    };

    /// Create:
    if(targetFileS05){
    fx_05 = audiere::OpenSound(audiereDevice.get(), targetFileS05, true); // Load music into memory? true / false
    fx_05->setVolume(FX_VOLUME);
    fx_05->setPan(PAN);    // -1 Left, 1 Right
    fx_05->setRepeat(0); // 1 loop, 0 don't loop
    // fx_05->play();
    };

    /// Create:
    if(targetFileS06){
    fx_06 = audiere::OpenSound(audiereDevice.get(), targetFileS06, true); // Load music into memory? true / false
    fx_06->setVolume(FX_VOLUME);
    fx_06->setPan(PAN);    // -1 Left, 1 Right
    fx_06->setRepeat(0); // 1 loop, 0 don't loop
    // fx_06->play();
    };

    /// Create:
    if(targetFileS07){
    fx_07 = audiere::OpenSound(audiereDevice.get(), targetFileS07, true); // Load music into memory? true / false
    fx_07->setVolume(FX_VOLUME);
    fx_07->setPan(PAN);    // -1 Left, 1 Right
    fx_07->setRepeat(0); // 1 loop, 0 don't loop
    // fx_07->play();
    };

    /// Create:
    if(targetFileS08){
    fx_08 = audiere::OpenSound(audiereDevice.get(), targetFileS08, true); // Load music into memory? true / false
    fx_08->setVolume(FX_VOLUME/2);
    fx_08->setPan(PAN);    // -1 Left, 1 Right
    fx_08->setRepeat(0); // 1 loop, 0 don't loop
    // fx_08->play();
    };

    /// Create:
    if(targetFileS09){
    fx_09 = audiere::OpenSound(audiereDevice.get(), targetFileS09, true); // Load music into memory? true / false
    fx_09->setVolume(FX_VOLUME/3.0);
    fx_09->setPan(PAN);    // -1 Left, 1 Right
    fx_09->setRepeat(0); // 1 loop, 0 don't loop
    // fx_09->play();
    };

    /// Create:
    if(targetFileS10){
    fx_10 = audiere::OpenSound(audiereDevice.get(), targetFileS10, true); // Load music into memory? true / false
    fx_10->setVolume(FX_VOLUME/5.0);
    fx_10->setPan(PAN);    // -1 Left, 1 Right
    fx_10->setRepeat(0); // 1 loop, 0 don't loop
    // fx_10->play();
    };

    /// Create:
    if(targetFileS11){
    fx_11 = audiere::OpenSound(audiereDevice.get(), targetFileS11, true); // Load music into memory? true / false
    fx_11->setVolume(FX_VOLUME);
    fx_11->setPan(PAN);    // -1 Left, 1 Right
    fx_11->setRepeat(0); // 1 loop, 0 don't loop
    // fx_11->play();
    };

    /// Create:
    if(targetFileS12){
    fx_12 = audiere::OpenSound(audiereDevice.get(), targetFileS12, true); // Load music into memory? true / false
    fx_12->setVolume(FX_VOLUME);
    fx_12->setPan(PAN);    // -1 Left, 1 Right
    fx_12->setRepeat(0); // 1 loop, 0 don't loop
    // fx_12->play();
    };

    /// Create:
    if(targetFileS13){
    fx_13 = audiere::OpenSound(audiereDevice.get(), targetFileS13, true); // Load music into memory? true / false
    fx_13->setVolume(FX_VOLUME);
    fx_13->setPan(PAN);    // -1 Left, 1 Right
    fx_13->setRepeat(0); // 1 loop, 0 don't loop
    // fx_13->play();
    };

    /// Create:
    if(targetFileS14){
    fx_14 = audiere::OpenSound(audiereDevice.get(), targetFileS14, true); // Load music into memory? true / false
    fx_14->setVolume(FX_VOLUME);
    fx_14->setPan(PAN);    // -1 Left, 1 Right
    fx_14->setRepeat(0); // 1 loop, 0 don't loop
    // fx_14->play();
    };

    /// Create:
    if(targetFileS15){
    fx_15 = audiere::OpenSound(audiereDevice.get(), targetFileS15, true); // Load music into memory? true / false
    fx_15->setVolume(FX_VOLUME);
    fx_15->setPan(PAN);  // -1 Left, 1 Right
    fx_15->setRepeat(0); // 1 loop, 0 don't loop
    // fx_15->play();
    };

    /// Create:
    if(targetFileS16){
    fx_16 = audiere::OpenSound(audiereDevice.get(), targetFileS16, true); // Load music into memory? true / false
    fx_16->setVolume(FX_VOLUME);
    fx_16->setPan(PAN);  // -1 Left, 1 Right
    fx_16->setRepeat(0); // 1 loop, 0 don't loop
    // fx_16->play();
    };

    /// Create:
    if(targetFileS17){
    fx_17 = audiere::OpenSound(audiereDevice.get(), targetFileS17, true); // Load music into memory? true / false
    fx_17->setVolume(FX_VOLUME);
    fx_17->setPan(PAN);  // -1 Left, 1 Right
    fx_17->setRepeat(0); // 1 loop, 0 don't loop
    // fx_17->play();
    };

    /// Create:
    if(targetFileS18){
    fx_18 = audiere::OpenSound(audiereDevice.get(), targetFileS18, true); // Load music into memory? true / false
    fx_18->setVolume(FX_VOLUME);
    fx_18->setPan(PAN);  // -1 Left, 1 Right
    fx_18->setRepeat(0); // 1 loop, 0 don't loop
    // fx_18->play();
    };

    /// Create:
    if(targetFileS19){
    fx_19 = audiere::OpenSound(audiereDevice.get(), targetFileS19, true); // Load music into memory? true / false
    fx_19->setVolume(FX_VOLUME);
    fx_19->setPan(PAN);  // -1 Left, 1 Right
    fx_19->setRepeat(0); // 1 loop, 0 don't loop
    // fx_19->play();
    };

    /// Create:
    if(targetFileS20){
    fx_20 = audiere::OpenSound(audiereDevice.get(), targetFileS20, true); // Load music into memory? true / false
    fx_20->setVolume(FX_VOLUME);
    fx_20->setPan(PAN);  // -1 Left, 1 Right
    fx_20->setRepeat(0); // 1 loop, 0 don't loop
    // fx_20->play();
    };

    /// Create:
    if(targetFileS21){
    fx_21 = audiere::OpenSound(audiereDevice.get(), targetFileS21, true); // Load music into memory? true / false
    fx_21->setVolume(FX_VOLUME);
    fx_21->setPan(PAN);  // -1 Left, 1 Right
    fx_21->setRepeat(0); // 1 loop, 0 don't loop
    // fx_21->play();
    };

    /// Create:
    if(targetFileS22){
    fx_22 = audiere::OpenSound(audiereDevice.get(), targetFileS22, true); // Load music into memory? true / false
    fx_22->setVolume(FX_VOLUME);
    fx_22->setPan(PAN);  // -1 Left, 1 Right
    fx_22->setRepeat(0); // 1 loop, 0 don't loop
    // fx_22->play();
    };

    /// Create:
    /*
    if(targetFileS23){
    fx_23 = audiere::OpenSound(audiereDevice.get(), targetFileS23, true); // Load music into memory? true / false
    fx_23->setVolume(FX_VOLUME);
    fx_23->setPan(PAN);  // -1 Left, 1 Right
    fx_23->setRepeat(0); // 1 loop, 0 don't loop
    // fx_23->play();
    };
    */

    /// Create:
    if(targetFileS24){
    fx_24 = audiere::OpenSound(audiereDevice.get(), targetFileS24, true); // Load music into memory? true / false
    fx_24->setVolume(FX_VOLUME);
    fx_24->setPan(PAN);  // -1 Left, 1 Right
    fx_24->setRepeat(0); // 1 loop, 0 don't loop
    // fx_24->play();
    };

    /// Create:
    if(targetFileS25){
    fx_25 = audiere::OpenSound(audiereDevice.get(), targetFileS25, true); // Load music into memory? true / false
    fx_25->setVolume(FX_VOLUME);
    fx_25->setPan(PAN);  // -1 Left, 1 Right
    fx_25->setRepeat(0); // 1 loop, 0 don't loop
    // fx_25->play();
    };

    /// Create:
    if(targetFileS26){
    fx_26 = audiere::OpenSound(audiereDevice.get(), targetFileS26, true); // Load music into memory? true / false
    fx_26->setVolume(FX_VOLUME);
    fx_26->setPan(PAN);  // -1 Left, 1 Right
    fx_26->setRepeat(0); // 1 loop, 0 don't loop
    // fx_26->play();
    };

    /// Create:
    if(targetFileS27){
    fx_27 = audiere::OpenSound(audiereDevice.get(), targetFileS27, true); // Load music into memory? true / false
    fx_27->setVolume(FX_VOLUME);
    fx_27->setPan(PAN);  // -1 Left, 1 Right
    fx_27->setRepeat(0); // 1 loop, 0 don't loop
    // fx_27->play();
    };

    /// Create:
    if(targetFileS28){
    fx_28 = audiere::OpenSound(audiereDevice.get(), targetFileS28, true); // Load music into memory? true / false
    fx_28->setVolume(FX_VOLUME);
    fx_28->setPan(PAN);  // -1 Left, 1 Right
    fx_28->setRepeat(0); // 1 loop, 0 don't loop
    // fx_28->play();
    };

    /// Create:
    if(targetFileS29){
    fx_29 = audiere::OpenSound(audiereDevice.get(), targetFileS29, true); // Load music into memory? true / false
    fx_29->setVolume(FX_VOLUME);
    fx_29->setPan(PAN);  // -1 Left, 1 Right
    fx_29->setRepeat(0); // 1 loop, 0 don't loop
    // fx_29->play();
    };

    /// Create:
    if(targetFileS30){
    fx_30 = audiere::OpenSound(audiereDevice.get(), targetFileS30, true); // Load music into memory? true / false
    fx_30->setVolume(FX_VOLUME);
    fx_30->setPan(PAN);  // -1 Left, 1 Right
    fx_30->setRepeat(0); // 1 loop, 0 don't loop
    // fx_30->play();
    };

    /// Create:
    if(targetFileS31){
    fx_31 = audiere::OpenSound(audiereDevice.get(), targetFileS31, true); // Load music into memory? true / false
    fx_31->setVolume(FX_VOLUME);
    fx_31->setPan(PAN);  // -1 Left, 1 Right
    fx_31->setRepeat(0); // 1 loop, 0 don't loop
    // fx_31->play();
    };

    /// Create:
    if(targetFileS32){
    fx_32 = audiere::OpenSound(audiereDevice.get(), targetFileS32, true); // Load music into memory? true / false
    fx_32->setVolume(FX_VOLUME);
    fx_32->setPan(PAN);  // -1 Left, 1 Right
    fx_32->setRepeat(0); // 1 loop, 0 don't loop
    // fx_32->play();
    };

    /// Create:
    if(targetFileS33){
    fx_33 = audiere::OpenSound(audiereDevice.get(), targetFileS33, true); // Load music into memory? true / false
    fx_33->setVolume(FX_VOLUME);
    fx_33->setPan(PAN);  // -1 Left, 1 Right
    fx_33->setRepeat(0); // 1 loop, 0 don't loop
    // fx_33->play();
    };

    /// Create:
    if(targetFileS34){
    fx_34 = audiere::OpenSound(audiereDevice.get(), targetFileS34, true); // Load music into memory? true / false
    fx_34->setVolume(FX_VOLUME);
    fx_34->setPan(PAN);  // -1 Left, 1 Right
    fx_34->setRepeat(0); // 1 loop, 0 don't loop
    // fx_34->play();
    };

    /// Create:
    if(targetFileS35){
    fx_35 = audiere::OpenSound(audiereDevice.get(), targetFileS35, true); // Load music into memory? true / false
    fx_35->setVolume(FX_VOLUME);
    fx_35->setPan(PAN);  // -1 Left, 1 Right
    fx_35->setRepeat(0); // 1 loop, 0 don't loop
    // fx_35->play();
    };

    /// Create:
    if(targetFileS36){
    fx_36 = audiere::OpenSound(audiereDevice.get(), targetFileS36, true); // Load music into memory? true / false
    fx_36->setVolume(FX_VOLUME);
    fx_36->setPan(PAN);  // -1 Left, 1 Right
    fx_36->setRepeat(0); // 1 loop, 0 don't loop
    // fx_36->play();
    };

    /// Create:
    if(targetFileS37){
    fx_37 = audiere::OpenSound(audiereDevice.get(), targetFileS37, true); // Load music into memory? true / false
    fx_37->setVolume(FX_VOLUME);
    fx_37->setPan(PAN);  // -1 Left, 1 Right
    fx_37->setRepeat(0); // 1 loop, 0 don't loop
    // fx_37->play();
    };

    /// Create:
    if(targetFileS38){
    fx_38 = audiere::OpenSound(audiereDevice.get(), targetFileS38, true); // Load music into memory? true / false
    fx_38->setVolume(FX_VOLUME);
    fx_38->setPan(PAN);  // -1 Left, 1 Right
    fx_38->setRepeat(0); // 1 loop, 0 don't loop
    // fx_38->play();
    };

    /// Create:
    if(targetFileS39){
    fx_39 = audiere::OpenSound(audiereDevice.get(), targetFileS39, true); // Load music into memory? true / false
    fx_39->setVolume(FX_VOLUME);
    fx_39->setPan(PAN);  // -1 Left, 1 Right
    fx_39->setRepeat(0); // 1 loop, 0 don't loop
    // fx_39->play();
    };

    /// Create:
    if(targetFileS40){
    fx_40 = audiere::OpenSound(audiereDevice.get(), targetFileS40, true); // Load music into memory? true / false
    fx_40->setVolume(FX_VOLUME);
    fx_40->setPan(PAN);  // -1 Left, 1 Right
    fx_40->setRepeat(0); // 1 loop, 0 don't loop
    // fx_40->play();
    };


    /// -------------------------------------------------------------------------------------------------------


    /// Create:
    if(targetFileT01){
    talk_01 = audiere::OpenSound(audiereDevice.get(), targetFileT01, true); // Load music into memory? true / false
    talk_01->setVolume(FX_VOLUME);
    talk_01->setPan(PAN);  // -1 Left, 1 Right
    talk_01->setRepeat(0); // 1 loop, 0 don't loop
    // talk_01->play();
    };

    /// Create:
    if(targetFileT02){
    talk_02 = audiere::OpenSound(audiereDevice.get(), targetFileT02, true); // Load music into memory? true / false
    talk_02->setVolume(FX_VOLUME);
    talk_02->setPan(PAN);  // -1 Left, 1 Right
    talk_02->setRepeat(0); // 1 loop, 0 don't loop
    // talk_02->play();
    };

    /// Create:
    if(targetFileT03){
    talk_03 = audiere::OpenSound(audiereDevice.get(), targetFileT03, true); // Load music into memory? true / false
    talk_03->setVolume(FX_VOLUME);
    talk_03->setPan(PAN);  // -1 Left, 1 Right
    talk_03->setRepeat(0); // 1 loop, 0 don't loop
    // talk_03->play();
    };

    /// Create:
    if(targetFileT04){
    talk_04 = audiere::OpenSound(audiereDevice.get(), targetFileT04, true); // Load music into memory? true / false
    talk_04->setVolume(FX_VOLUME);
    talk_04->setPan(PAN);  // -1 Left, 1 Right
    talk_04->setRepeat(0); // 1 loop, 0 don't loop
    // talk_04->play();
    };

    /// Create:
    if(targetFileT05){
    talk_05 = audiere::OpenSound(audiereDevice.get(), targetFileT05, true); // Load music into memory? true / false
    talk_05->setVolume(FX_VOLUME);
    talk_05->setPan(PAN);  // -1 Left, 1 Right
    talk_05->setRepeat(0); // 1 loop, 0 don't loop
    // talk_05->play();
    };

    /// Create:
    if(targetFileT06){
    talk_06 = audiere::OpenSound(audiereDevice.get(), targetFileT06, true); // Load music into memory? true / false
    talk_06->setVolume(FX_VOLUME);
    talk_06->setPan(PAN);  // -1 Left, 1 Right
    talk_06->setRepeat(0); // 1 loop, 0 don't loop
    // talk_06->play();
    };


    /// -------------------------------------------------------------------------------------------------------

    /// Create:
    if(targetFileB01){
    button_01 = audiere::OpenSound(audiereDevice.get(), targetFileB01, true); // Load music into memory? true / false
    button_01->setVolume(BUTTON_VOLUME/2.0);
    button_01->setPan(0.0);  // -1 Left, 1 Right
    button_01->setRepeat(0); // 1 loop, 0 don't loop
    // button_01->play();
    };



    }; /// audiere_initialized = false;

    /// OK:
    std::cout << "Audios Created!" << std::endl;
};





void Audios::PlayFx(int in, vector3df pos) {

/// ----------------------------------------------------------------------------------------------------------
/// All audios are converted in "WAV - 16Bit PCM" format (Lossless audio)
/// CRASHES (!!!) if audios are too short and are in constant loop !!! (for example: "fx_winch.wav")
/// ----------------------------------------------------------------------------------------------------------

    /// 3D Audio:
    Make3DAudio(pos);
    /// Play Audio:
    /// if(in!=19){ cout << "Play audio: fx_" << in << endl; };
    if(in== 1 && fx_01){ fx_01->setVolume(FX_VOLUME); fx_01->setPan(PAN); fx_01->play(); };
    if(in== 2 && fx_02){ fx_02->setVolume(FX_VOLUME); fx_02->setPan(PAN); fx_02->play(); };
    if(in== 3 && fx_03){ fx_03->setVolume(MAX_FX_VOLUME); fx_03->setPan(-0.4); fx_03->play(); }; /// Cannon fire
    if(in== 4 && fx_04){ fx_04->setVolume(FX_VOLUME); fx_04->setPan(PAN); fx_04->play(); };
    if(in== 5 && fx_05){ fx_05->setVolume(FX_VOLUME); fx_05->setPan(PAN); fx_05->play(); };
    if(in== 6 && fx_06){ fx_06->setVolume(MAX_FX_VOLUME/2.0); fx_06->setPan(0.0); fx_06->play(); }; /// Zoom
    if(in== 7 && fx_07){ fx_07->setVolume(FX_VOLUME); fx_07->setPan(PAN); fx_07->play(); };
    if(in== 8 && fx_08){ fx_08->setVolume(FX_VOLUME); fx_08->setPan(PAN); fx_08->play(); };
    if(in== 9 && fx_09){ fx_09->setVolume(FX_VOLUME/4.0); fx_09->setPan(PAN); fx_09->play(); }; /// Jump
    if(in==10 && fx_10){ fx_10->setVolume(FX_VOLUME); fx_10->setPan(PAN); fx_10->play(); };
    if(in==11 && fx_11){ fx_11->setVolume(FX_VOLUME); fx_11->setPan(PAN); fx_11->play(); };
    if(in==12 && fx_12){ fx_12->setVolume(MAX_FX_VOLUME/3.0); fx_12->setPan(0.0); fx_12->play(); }; /// Prisoner is free
    if(in==13 && fx_13){ fx_13->setVolume(FX_VOLUME); fx_13->setPan(PAN); fx_13->play(); }; /// wood2
    if(in==14 && fx_14){ fx_14->setVolume(FX_VOLUME); fx_14->setPan(PAN); fx_14->play(); }; /// chicken
    if(in==15 && fx_15){ fx_15->setVolume(FX_VOLUME); fx_15->setPan(PAN); fx_15->play(); }; /// ball bouncing
    if(in==16 && fx_16){ fx_16->setVolume(FX_VOLUME); fx_16->setPan(PAN); fx_16->play(); }; /// bubbles
    if(in==17 && fx_17){ fx_17->setVolume(FX_VOLUME); fx_17->setPan(PAN); fx_17->play(); }; /// disappear
    if(in==18 && fx_18){ fx_18->setVolume(FX_VOLUME); fx_18->setPan(0.2); fx_18->play(); }; /// bell
    if(in==19 && fx_19){ fx_19->setVolume(FX_VOLUME/35.0); fx_19->setPan(PAN); fx_19->play(); }; /// winch
    if(in==20 && fx_20){ fx_20->setVolume(FX_VOLUME); fx_20->setPan(PAN); fx_20->play(); }; /// ball2
    if(in==21 && fx_21){ fx_21->setVolume(FX_VOLUME/8.0); fx_21->setPan(PAN); fx_21->play(); }; /// RANDOM AUDIO: horse
    if(in==22 && fx_22){ fx_22->setVolume(FX_VOLUME/8.0); fx_22->setPan(PAN); fx_22->play(); }; /// RANDOM AUDIO: cock
    /// if(in==23 && fx_23){ fx_23->setVolume(FX_VOLUME); fx_23->setPan(PAN); fx_23->play(); }; /// talk king
    if(in==24 && fx_24){ fx_24->setVolume(FX_VOLUME/10.0); fx_24->setPan(PAN); fx_24->play(); }; /// RANDOM AUDIO: yell
    if(in==25 && fx_25){ fx_25->setVolume(FX_VOLUME/5.0); fx_25->setPan(PAN); fx_25->play(); }; /// drink
    if(in==26 && fx_26){ fx_26->setVolume(MAX_FX_VOLUME/10.0); fx_26->setPan(-0.4); fx_26->play(); }; /// Cannon targeting
    if(in==27 && fx_27){ fx_27->setVolume(FX_VOLUME/8.0); fx_27->setPan(PAN); fx_27->play(); }; /// RANDOM AUDIO: bull
    if(in==28 && fx_28){ fx_28->setVolume(FX_VOLUME/8.0); fx_28->setPan(PAN); fx_28->play(); }; /// RANDOM AUDIO: hammer
    if(in==29 && fx_29){ fx_29->setVolume(FX_VOLUME/8.0); fx_29->setPan(PAN); fx_29->play(); }; /// RANDOM AUDIO: door
    if(in==30 && fx_30){ fx_30->setVolume(FX_VOLUME/2.0); fx_30->setPan(PAN); fx_30->play(); }; /// Shark - suspense
    if(in==31 && fx_31){ fx_31->setVolume(FX_VOLUME/1.2); fx_31->setPan(PAN); fx_31->play(); }; /// Prisoner - Escaping
    if(in==32 && fx_32){ fx_32->setVolume(FX_VOLUME/1.0); fx_32->setPan(PAN); fx_32->play(); }; /// Hit3 (Targets)
    if(in==33 && fx_33){ fx_33->setVolume(FX_VOLUME/1.0); fx_33->setPan(PAN); fx_33->play(); }; /// Jump (spring)
    if(in==34 && fx_34){ fx_34->setVolume(FX_VOLUME/2.0); fx_34->setPan(PAN); fx_34->play(); }; /// Laser
    if(in==35 && fx_35){ fx_35->setVolume(FX_VOLUME/1.0); fx_35->setPan(PAN); fx_35->play(); }; /// zop
    if(in==36 && fx_36){ fx_36->setVolume(FX_VOLUME/10.0); fx_36->setPan(PAN); fx_36->play(); }; /// fountain
    if(in==37 && fx_37){ fx_37->setVolume(FX_VOLUME/8.0); fx_37->setPan(PAN); fx_37->play(); }; /// GAME intro
    if(in==38 && fx_38){ fx_38->setVolume(FX_VOLUME/8.0); fx_38->setPan(PAN); fx_38->play(); }; /// GAME end
    if(in==39 && fx_39){ fx_39->setVolume(FX_VOLUME/1.5); fx_39->setPan(PAN); fx_39->play(); }; /// GAME start
    if(in==40 && fx_40){ fx_40->setVolume(FX_VOLUME/1.5); fx_40->setPan(PAN); fx_40->play(); }; /// GAME over
};


void Audios::StopFx(int in) {
    if(in==30 && music_01){ fx_30->stop(); }; /// Shark - suspense
};



void Audios::PlayTalk(int in, vector3df pos) {
    /// 3D Audio:
    Make3DAudio(pos);
    /// Play Audio:
    /// cout << "Play audio: talk_01" << endl;
    if(in==1 && talk_01){ talk_01->setVolume(FX_VOLUME/2.0); talk_01->setPan(PAN); talk_01->play(); }; /// Help
    if(in==2 && talk_02){ talk_02->setVolume(FX_VOLUME/2.0); talk_02->setPan(PAN); talk_02->play(); }; /// Yeah
    if(in==3 && talk_03){ talk_03->setVolume(FX_VOLUME/2.0); talk_03->setPan(PAN); talk_03->play(); }; /// King order talk
    if(in==4 && talk_04){ talk_04->setVolume(FX_VOLUME/2.0); talk_04->setPan(PAN); talk_04->play(); }; /// King random balloon talk
    if(in==5 && talk_05){ talk_05->setVolume(FX_VOLUME/2.0); talk_05->setPan(PAN); talk_05->play(); }; /// Priest Drunk
    if(in==6 && talk_06){ talk_06->setVolume(FX_VOLUME/2.0); talk_06->setPan(PAN); talk_06->play(); }; /// Soldiers talk
};


void Audios::PlayMusic(int in) {
    if(in==1 && music_01){ music_01->setPosition(0); music_01->play(); }; /// Music: Main Theme
    if(in==2 && music_02){ music_02->setPosition(0); music_02->play(); }; /// Music: Ocean
    if(in==3 && music_03){ music_03->setPosition(0); music_03->play(); }; /// Music: People talking
};


void Audios::StopMusic(int in) {
    if(in==1 && music_01){ music_01->stop(); }; /// Music: Main Theme
    if(in==2 && music_02){ music_02->stop(); }; /// Music: Ocean
    if(in==3 && music_03){ music_03->stop(); }; /// Music: People talking
};


void Audios::PlayButton(int in) {
    /// Play Audio:
    cout << "Play audio: button_01" << endl;
    if(in==1 && button_01){ button_01->play(); };
};



void Audios::Make3DAudio(vector3df pos){
    if(cameras[0]){
    /// Example: vector3df pos = nodePrisoner[0]->getPosition();
    vector3df campos = cameras[0]->getPosition();
    /// GLOBAL PAN calculations:
    PAN = ((pos.X-campos.X)/1000);
    if(PAN<-1.0){ PAN = -1.0; };
    if(PAN> 1.0){ PAN =  1.0; };
    /// FX_VOLUME calculations:
    FX_VOLUME = fabsf((pos.X)/(pos.X-campos.X))/8; /// fabsf == always positive float number
    if(FX_VOLUME<MIN_FX_VOLUME){ FX_VOLUME = MIN_FX_VOLUME; };
    if(FX_VOLUME>MAX_FX_VOLUME){ FX_VOLUME = MAX_FX_VOLUME; };
    };
};





void Audios::Loop() {
    /// Pan for ship sounds = constant loop:
    /// Make3DAudioForPirateShip();
};


void Audios::Exit() {
    /// Free and close Audiere: TO-DO !!!....
    if(audiereDevice){
    audiereDevice->clearCallbacks();
    audiereDevice = NULL;
    // delete audiereDevice;
    };
};


