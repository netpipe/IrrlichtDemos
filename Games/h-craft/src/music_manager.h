// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include "compile_config.h"

#include <string>
#include <vector>

class ISound;

struct PlayListEntry
{
    PlayListEntry(const char * filename_) : mFilename(filename_), mCounter(0) {}
    std::string mFilename;
    unsigned int mCounter;
};
typedef std::vector<PlayListEntry> Playlist;

class MusicManager
{
public:
    MusicManager();
    ~MusicManager();

    void Init(ISound* sound_);
    void PlayMenuMusic();
    void PlayLevelMusic();

protected:
    void AddMusicToPlayList(Playlist & playList_, const char * path_, const char * ending_);
    void PlayRandomMusicFromPlayList(Playlist & playList_);

private:
    ISound *    mSound;
    Playlist    mGuiMusic;
    Playlist    mLevelMusic;
};

#endif // MUSIC_MANAGER_H
