// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "recorder.h"
#include "logging.h"
#include "streaming.h"
#include <cassert>
#include <algorithm>
#include "tinyxml/tinyxml.h"

using namespace irr;

const int RECORD_MAGIC_DATA[4] = { 51723, 81272, 201006, 4711 };
const int RECORD_VERSION = 3;

// --------------------------------------------------------
bool operator<(int id_, const Record &obj_)
{
    return id_ < obj_.mId;
}

bool operator<(IRecordable * ptr_, const RecordingObject &obj_)
{
    return ptr_ < obj_.mRecordable;
}

bool operator<(IRecordable * ptr_, const PlayingObject &obj_)
{
    return ptr_ < obj_.mRecordable;
}

bool operator<(u32 time_, const RecordTimeEntry& entry_)
{
    return time_ < entry_.mTime;
}

// --------------------------------------------------------
Record::Record()
    : IReferenceCounted()
    , mStreamArchive(0)
    , mSerializeArchive(0)
{
    mStreamArchive = new MemArchive();
    mSerializeArchive = new MemArchive();
}

Record::~Record()
{
    delete mStreamArchive;
    delete mSerializeArchive;
}

Record::Record(const Record &other_)
: IReferenceCounted()
, mStreamArchive(NULL)
, mSerializeArchive(NULL)
{
    mStreamArchive = new MemArchive();
    mSerializeArchive = new MemArchive();
    *this = other_;
}

Record& Record::operator=(const Record &other_)
{
//    mId = other_.mId; // that must be handled in recorder class
    mTimeTable = other_.mTimeTable;
    *mStreamArchive = *(other_.mStreamArchive);
    *mSerializeArchive = *(other_.mSerializeArchive);

    return *this;
}

void Record::Clear()
{
    mStreamArchive->Clear();
    mSerializeArchive->Clear();
    mTimeTable.clear();
}

bool Record::Save(const std::string &filename_) const
{
    if ( !mStreamArchive)
        return false;

    unsigned long aSize = mStreamArchive->GetSize();
    if ( !aSize )
        return false;

    unsigned int ttSize = mTimeTable.size();
    if ( !ttSize )
        return false;

    std::string filenameArchive(filename_);
    FileArchive fileArchive;
    if ( !fileArchive.Open(filenameArchive, "wb") )
        return false;

    // write magic string and version
    fileArchive.Write(RECORD_MAGIC_DATA[0]);
    fileArchive.Write(RECORD_MAGIC_DATA[1]);
    fileArchive.Write(RECORD_MAGIC_DATA[2]);
    fileArchive.Write(RECORD_MAGIC_DATA[3]);
    fileArchive.Write(RECORD_VERSION);

    // write serialize data
    char * data = NULL;
    unsigned long oldReadPos = mSerializeArchive->GetReadPos();
    mSerializeArchive->SetReadPos(0);
    mSerializeArchive->ReadRaw(mSerializeArchive->GetSize(), (void**)&data);
    mSerializeArchive->SetReadPos(oldReadPos);
    fileArchive.WriteData(mSerializeArchive->GetSize(), data);
    delete[] data;

    fileArchive.Write(ttSize);
    for ( unsigned int i=0; i<ttSize; ++i)
    {
        fileArchive.Write(mTimeTable[i].mTime);
        fileArchive.Write((uint64_t)mTimeTable[i].mStreamPos);
    }

    // write streaming data
    oldReadPos = mStreamArchive->GetReadPos();
    mStreamArchive->SetReadPos(0);
    mStreamArchive->ReadRaw(aSize, (void**)&data);
    mStreamArchive->SetReadPos(oldReadPos);
    fileArchive.WriteData(mStreamArchive->GetSize(), data);
    delete[] data;

    return true;
}

bool Record::Load(const std::string &filename_, bool onlySerializeArchive_)
{
    if ( !mStreamArchive)
        return false;

    std::string filenameArchive(filename_);
    FileArchive fileArchive;
    if ( !fileArchive.Open(filenameArchive, "rb") )
        return false;

    // get version
    int version = 0;
    if ( fileArchive.GetSize() >= 5*sizeof(int) )
    {
        // check for magic string
        bool hasMagic = true;
        int magic[4];
        for ( int i=0; i < 4; ++i )
        {
            fileArchive.Read(magic[i]);
            if ( magic[i] != RECORD_MAGIC_DATA[i] )
            {
                hasMagic = false;
                break;
            }
        }
        if ( hasMagic )
        {
            fileArchive.Read(version);
        }
        else    // version 0 didn't have the magic
        {
            fileArchive.SetReadPos(0);
        }
    }

    if ( version < 3 )
    {
        return false;   // no longer supported because of bug in streaming class
    }

    mStreamArchive->Clear();
    mSerializeArchive->Clear();
    mTimeTable.clear();

    unsigned long aSize=0;
    char * data = NULL;
    aSize = fileArchive.ReadData((void**)&data);

    mSerializeArchive->WriteRaw(aSize, data);
    mSerializeArchive->SetReadPos(0);
    delete[] data;

    if ( onlySerializeArchive_ )
    {
        return true;
    }

    unsigned int ttSize = 0;
    fileArchive.Read(ttSize);

    for ( unsigned int i=0; i<ttSize; ++i)
    {
        u32 time;
        unsigned long streamPos;
        fileArchive.Read(time);
        uint64_t spos;
        fileArchive.Read(spos);
        streamPos = spos;

        mTimeTable.push_back(RecordTimeEntry(time, streamPos));
    }

    aSize = fileArchive.ReadData((void**)&data);

    mStreamArchive->WriteRaw(aSize, data);
    mStreamArchive->SetReadPos(0);
    delete[] data;

    return true;
}

void Record::UpdateRecording(u32 time_, IRecordable * recordable_)
{
    mTimeTable.push_back(RecordTimeEntry(time_, mStreamArchive->GetSize()));
    recordable_->Recording(mStreamArchive);
}

void Record::UpdatePlaying(u32 time_, IRecordable * recordable_)
{
    if ( !mTimeTable.size() )
        return;

    std::vector<RecordTimeEntry>::iterator itTime = std::lower_bound( mTimeTable.begin(), mTimeTable.end()-1, time_);

    if ( itTime < mTimeTable.end() )
    {
        mStreamArchive->SetReadPos(itTime->mStreamPos);
        recordable_->Playing(mStreamArchive);
    }
    recordable_->PlayingInterpolated(*this, time_);
}

void Record::UpdatePlaying(f32 time_, IRecordable * recordable_)
{
    if ( !mTimeTable.size() )
        return;

    recordable_->PlayingInterpolated(*this, time_);
}

// --------------------------------------------------------
Recorder::Recorder()
: mHighestRecordId(0)
{
}

Recorder::~Recorder()
{
    RemoveAllRecords();
}

int Recorder::AddRecord()
{
    Record * record = new Record();
    record->mId = mHighestRecordId;

    mRecords.push_back(record);
    ++mHighestRecordId;

    LOG.LogLn(LP_DEBUG, "Recorder::AddRecord added id ", mRecords[mRecords.size()-1]->mId );

    return record->mId;
}

void Recorder::RemoveRecord(int recordId_)
{
    for ( unsigned int i=0; i<mRecords.size(); ++i)
    {
        if ( mRecords[i]->mId == recordId_ )
        {
            mRecords[i]->drop();
            mRecords.erase(mRecords.begin() + i );
            return;
        }
    }
}

void Recorder::RemoveAllRecords()
{
    for ( unsigned int i=0; i<mRecords.size(); ++i)
    {
        mRecords[i]->drop();
    }
    mRecords.clear();
}

void Recorder::RemoveAllUnusedRecords()
{
    for ( int i= (int)mRecords.size()-1; i>=0; --i)
    {
        bool used = false;
        for ( unsigned int r=0; r<mRecordingObjects.size(); ++r )
        {
            if ( mRecordingObjects[r].mRecordId == mRecords[i]->mId )
            {
                used = true;
                break;
            }
        }
        for ( unsigned int p=0; p<mPlayingObjects.size(); ++p)
        {
            if ( mPlayingObjects[p].mRecordId == mRecords[i]->mId )
            {
                used = true;
                break;
            }
        }

        if ( !used )
        {
            mRecords[i]->drop();
            mRecords.erase(mRecords.begin() + i );
        }
    }
}

void Recorder::AddRecordingObject(IRecordable *recordable_, int recordId_)
{
    if ( !recordable_ )
    {
        assert(!"recordable_ was NULL in Recorder::AddRecordingObject");
        return;
    }

    RecordingObject ro;
    ro.mRecordable = recordable_;
    ro.mRecordId = recordId_;

    mRecordingObjects.push_back(ro);
}

void Recorder::RemoveRecordingObject(IRecordable *recordable_)
{
    std::pair<std::vector<RecordingObject>::iterator, std::vector<RecordingObject>::iterator> result = std::equal_range(mRecordingObjects.begin(), mRecordingObjects.end(), recordable_);
    if ( result.first != mRecordingObjects.end() && result.first->mRecordable == recordable_ )
    {
        mRecordingObjects.erase(result.first);
    }
}

void Recorder::RemoveAllRecordingObjects()
{
    mRecordingObjects.clear();
}

void Recorder::AddPlayingObject(IRecordable *recordable_, int recordId_)
{
    if ( !recordable_ )
    {
        assert(!"recordable_ was NULL in Recorder::AddPlayingObject");
        return;
    }

    PlayingObject po;
    po.mRecordable = recordable_;
    po.mRecordId = recordId_;

    mPlayingObjects.push_back(po);
}

void Recorder::RemovePlayingObject(IRecordable *recordable_)
{
    std::pair<std::vector<PlayingObject>::iterator, std::vector<PlayingObject>::iterator> result = std::equal_range(mPlayingObjects.begin(), mPlayingObjects.end(), recordable_);
    if ( result.first != mPlayingObjects.end() && result.first->mRecordable == recordable_ )
    {
        mPlayingObjects.erase(result.first);
    }
}

void Recorder::RemoveAllPlayingObjects()
{
    mPlayingObjects.clear();
}

Record* Recorder::GetRecord(int recordId_) const
{
    for ( unsigned int i = 0; i < mRecords.size(); ++i )
    {
        if ( mRecords[i]->mId == recordId_ )
            return mRecords[i];
    }
    return NULL;
}

void Recorder::UpdateRecording(u32 time_)
{
   for ( unsigned int i=0; i<mRecordingObjects.size(); ++i)
   {
        Record * record = GetRecord(mRecordingObjects[i].mRecordId);
        if ( record )
        {
            record->UpdateRecording(time_, mRecordingObjects[i].mRecordable);
        }
    }
}

void Recorder::UpdatePlaying(u32 time_)
{
   for ( unsigned int i=0; i< mPlayingObjects.size(); ++i)
   {
        Record * record = GetRecord(mPlayingObjects[i].mRecordId);
        if ( record )
        {
            record->UpdatePlaying(time_, mPlayingObjects[i].mRecordable);
        }
   }
}

void Recorder::UpdatePlaying(f32 time_)
{
   for ( unsigned int i=0; i< mPlayingObjects.size(); ++i)
   {
        Record * record = GetRecord(mPlayingObjects[i].mRecordId);
        if ( record )
        {
            record->UpdatePlaying(time_, mPlayingObjects[i].mRecordable);
        }
   }
}
