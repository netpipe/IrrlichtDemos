// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "music_manager.h"

#include "sound.h"
#include "main.h"
#include "random.h"
#include "config.h"
#include "logging.h"
#include "irrlicht_manager.h"
#include <assert.h>
#include <iostream>

using namespace irr;

MusicManager::MusicManager()
: mSound(0)
{
}

MusicManager::~MusicManager()
{
}

void MusicManager::Init(ISound* sound_)
{
    mSound = sound_;

    assert(APP.GetConfig());
    AddMusicToPlayList(mGuiMusic, APP.GetConfig()->GetPathMusicMenu().c_str(), "ogg");
    AddMusicToPlayList(mLevelMusic, APP.GetConfig()->GetPathMusicGame().c_str(), "ogg");
}

void MusicManager::PlayMenuMusic()
{
    if ( !mSound )
        return;

    PlayRandomMusicFromPlayList(mGuiMusic);
}

void MusicManager::PlayLevelMusic()
{
    if ( !mSound )
        return;

    PlayRandomMusicFromPlayList(mLevelMusic);
}

void MusicManager::AddMusicToPlayList(Playlist & playList_, const char * path_, const char * ending_)
{
    if ( !path_ || !ending_ )
        return;

    irr::io::IFileSystem * fs = APP.GetIrrlichtManager()->GetIrrlichtDevice()->getFileSystem();
    assert(fs);

	const irr::io::IFileList * fileList = NULL;
#ifdef _IRR_ANDROID_PLATFORM_
	for ( u32 i=0; i<fs->getFileArchiveCount (); ++i )
	{
		io::IFileArchive * archive = fs->getFileArchive (i);
		if ( archive->getType() == io::EFAT_ANDROID_ASSET )
		{
			fileList = archive->getFileList();
			fileList->grab();
			break;
		}
	}
	if ( !fileList )
	{
		LOG.LogLn(LP_WARN, "MusicManager::AddMusicToPlayList - did not find the EFAT_ANDROID_ASSET archive.");
	}
#else
	irr::io::path oldDir = fs->getWorkingDirectory();
	fs->changeWorkingDirectoryTo(path_);
    fileList = fs->createFileList();
    fs->changeWorkingDirectoryTo(oldDir);
#endif

    if ( fileList )
    {
        for ( u32 i=0; i < fileList->getFileCount(); ++i )
        {
			if ( fileList->isDirectory(i) )
				continue;

			const io::path & fullName = fileList->getFullFileName(i);
            if ( core::hasFileExtension(fullName, io::path(ending_)) )
            {
#ifdef _IRR_ANDROID_PLATFORM_
				if ( core::isInSameDirectory (io::path(path_), fullName) < 0 )
					continue;
#endif
				LOG.LogLn(LP_DEBUG, "Adding to playlist: ", fullName.c_str());
				PlayListEntry entry(fullName.c_str());
				playList_.push_back(entry);
            }
        }
        fileList->drop();
    }
}

void MusicManager::PlayRandomMusicFromPlayList(Playlist & playList_)
{
    RandomGenerator * rnd = APP.GetRandomGenerator();
    assert(rnd);

    if ( !playList_.size() || !mSound || !mSound->IsAvailable() )
        return;

    int rndIndex = rnd->GetNumberInRange(0, playList_.size()-1);
    int selectedIndex = rndIndex;
    for ( unsigned int i=0; i < playList_.size(); ++i )
    {
        if ( playList_[(i+rndIndex) % playList_.size()].mCounter < playList_[selectedIndex].mCounter )
            selectedIndex = (i+rndIndex) % playList_.size();
    }

    LOG.LogLn(LP_DEBUG, "Now playing: ", playList_[selectedIndex].mFilename.c_str() );
    assert(mSound);
    mSound->PlayMusic(playList_[selectedIndex].mFilename.c_str());
    ++playList_[selectedIndex].mCounter;
}
